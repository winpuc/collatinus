/*               tagueur.cpp
 *
 *  This file is part of COLLATINUS.
 *
 *  COLLATINUS is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  COLLATINVS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with COLLATINUS; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * © Yves Ouvrard, 2009 - 2016
 */

#include "tagueur.h"

Tagueur::Tagueur(QObject *parent, LemCore *l, QString resDir) : QObject(parent)
{
    if (l==0)
    {
        _lemCore = new LemCore(this, resDir);
        // Je crée le lemmatiseur...
        _lemCore->setExtension(true);
        // ... et charge l'extension du lexique.
    }
    else _lemCore = l;
    if (resDir == "")
        _resDir = qApp->applicationDirPath() + "/data/";
    else if (resDir.endsWith("/")) _resDir = resDir;
    else _resDir = resDir + "/";

}

QString Tagueur::tagTexte(QString t, int p, bool affTout, bool majPert)
{
    // éliminer les chiffres et les espaces surnuméraires
    t.remove(QRegExp("\\d"));
//    t = t.simplified(); // Rmq : perd les retours de ligne !
    int tl = t.length() - 1;
    const QString pp = ".;!?";
    int dph = p;
    bool tout = false;
    if (p < 0)
    {
        p = 0;
        dph = 0;
        tout = true; // Pour faire tout le texte, phrase par phrase.
    }
    else
    {
        if ((dph > 0) && pp.contains(t.at(dph))) --dph;
        // Si j'ai cliqué sur une ponctuation, je traite la phrase qui précède.
        // régression au début de la phrase
        while (dph > 0 && !pp.contains(t.at(dph)) && (t.mid(dph,2) != "\n\n")) --dph;
        if (dph != 0) dph += 1; // J'élimine la ponctuation de la phrase précédente.
    }

    // conteneur pour les résultats
    QStringList lsv;
    // progression jusqu'en fin de phrase
    int fph = p;
    while (fph < tl)
    {
        while (fph < tl && !pp.contains(t.at(fph)) && (t.mid(fph,2) != "\n\n")) ++fph;
        QString phr = t.mid(dph, fph - dph).trimmed();
        // Si le texte se termine sans ponctuation, je perds le dernier caractère.
//        qDebug() << tl << fph << t.at(tl);
        if ((fph == tl) && !pp.contains(t.at(tl)) && (t.mid(tl,1) != "\n"))
            phr.append(t[tl]);
        // découpage en mots
        QStringList lm = phr.split(QRegExp("\\b"));

        if (lm.size() > 1)
        {
            // Il y a au moins un mot...
            while (Ch::abrev.contains(lm[lm.size()-2]))
            {
                // Ma phrase se terminait par une abréviation : je continue.
                fph++;
                while (fph < tl && !pp.contains(t.at(fph))) ++fph;
                phr = t.mid(dph, fph - dph).trimmed();
                if ((fph == tl) && !pp.contains(t.at(tl)) && (t.mid(tl,1) != "\n"))
                    phr.append(t[tl]);
                lm = phr.split(QRegExp("\\b"));
            }

            QList<Mot*> mots;
            // lemmatisation pour chaque mot
            for (int i = 1; i < lm.length(); i += 2)
            {
                bool debVers = !majPert || lm[i-1].contains("\n");
                Mot * mot = new Mot(lm[i],(i-1)/2, debVers, _lemCore); // TODO : Vérifier si on a des vers avec majuscule...
                mots.append(mot);
            }  // fin de boucle de lemmatisation pour chaque mot
            Mot * mot = new Mot("",mots.size(),true, _lemCore); // Fin de phrase
            mots.append(mot); // J'ajoute un mot virtuel en fin de phrase avec le tag "snt".

            QStringList sequences;
            QList<double> probabilites;
            sequences.append("snt");
            probabilites.append(1.0);
            double branches = 1.0; // Pour savoir combien de branches a l'arbre.
            // Je suis en début de phrase : je n'ai que le tag "snt" et une proba de 1.
            for (int i = 0; i < mots.size(); i++)
            {
                Mot *mot = mots[i];
                QStringList lTags = mot->tags(); // La liste des tags possibles pour le mot
                QStringList nvlSeq; // Nouvelle liste des séquences possibles
                QList<double> nvlProba; // Nouvelle liste des probas.
                // Je dois ajouter tous les tags possibles à toutes les sequences et calculer les nouvelles probas.
                int sSeq = sequences.size();
                int sTag = lTags.size();
                if (sTag == 0) continue; // J'ignore pour l'instant les mots inconnus, cf. plus bas.
                branches *= sTag;
                for (int j = 0; j < sSeq; j++)
                {
                    QString bigr = sequences[j].right(7); // Le bigramme terminal
                    long prTot = 0;
                    QList<long> pr;
                    for (int k = 0; k < sTag; k++)
                    {
                        QString seq = bigr + " " + lTags[k];
                        long p = mot->proba(lTags[k]) * (4 * _lemCore->trigram(seq) + 1);
                        pr << p;
                        prTot += p;
                    }
                    // J'ai tout ce qui dépend de k et la somme pour normaliser.
                    if (prTot == 0)
                    {
                        prTot = 1;
                        //qDebug() << mot->forme() << "proba nulle ! " << sequences[j];
                    }
                    for (int k = 0; k < sTag; k++)
                    {
                        nvlSeq.append(sequences[j] + " " + lTags[k]);
                        nvlProba.append(probabilites[j] * pr[k] / prTot);
                        // Si j'avais gardé toutes les séquences, ce serait une vraie probabilité (normalisée à 1)
                    }
                }
                // Ajouter les enclitiques.
                if (!mot->tagEncl().isEmpty())
                {
                    QString ajout = " " + mot->tagEncl();
                    for (int j = 0; j < nvlSeq.size(); j++) nvlSeq[j].append(ajout);
                    // Comme toutes les formes à tag unique, l'enclitique ne change pas les probabilités.
                }
                // J'ai toutes les sequences et leur proba.
                // Pour chaque bigramme terminal, je ne dois garder que la séquence la plus probable.
                // En faisant ce tri, je fais une sélection sur le tag i-2 (attention aux mots avec enclitique).
                // Si je veux garder une info sur l'ordre des tags du mot i-2, c'est maintenant !
                if (i > 1)
                {
                    // Le mot i-2 existe !
                    int debut = nvlSeq[0].size() - 11;
                    if (!mot->tagEncl().isEmpty()) debut -= 4; // Je dois reculer d'un tag de plus.
                    if (!mots[i-1]->tagEncl().isEmpty()) debut -= 4; // Je dois reculer d'un tag de plus.
                    if (!mots[i-2]->tagEncl().isEmpty()) debut -= 4; // Je dois reculer d'un tag de plus.
                    // Le tag du mot i-2 est nvlSeq[j].mid(debut, 3);
                    for (int j = 0; j < nvlSeq.size(); j++) mots[i-2]->setBestOf(nvlSeq[j].mid(debut, 3), nvlProba[j]);
                }
                sequences.clear();
                probabilites.clear();
//                qDebug() << mot->forme() << nvlProba << nvlSeq;
                for (int j = 0; j < nvlSeq.size(); j++) if (nvlProba[j] > 0)
                {
                    QString bigr = nvlSeq[j].right(7); // Les deux derniers tags
                    QString seq = "";
                    double val = -1;
                    QString seq2 = "";
                    double val2 = -1;
                    for (int k = j; k < nvlSeq.size(); k += sTag) // Pour retrouver le bigramme terminal, il faut au moins le même dernier tag.
                        if (bigr == nvlSeq[k].right(7))
                        {
                            if (val2 < nvlProba[k])
                            {
                                // J'y passe au moins une fois au début.
                                // La séquence mérite la 1ère ou la 2e place.
                                if (val < nvlProba[k])
                                {
                                    // 1ère place !
                                    val2 = val;
                                    seq2 = seq;
                                    val = nvlProba[k];
                                    seq = nvlSeq[k];
                                }
                                else
                                {
                                    // Seulement la 2e place
                                    val2 = nvlProba[k];
                                    seq2 = nvlSeq[k];
                                }
                            }
                            nvlProba[k] = -1; // Pour ne pas considérer deux fois les mêmes séquences.
                        }
                    // val et seq correspondent aux proba et séquence avec le bigramme considéré qui ont la plus grande proba.
                    sequences << seq;
                    probabilites << val;
                    if (val2 > 0)
                    {
                        // J'ai une deuxième séquence assez probable.
                        sequences << seq2;
                        probabilites << val2;
                    }
                }
        //        qDebug() << mot->forme() << sSeq << sTag << nvlSeq.size() << sequences.size();
                if (sequences.size() == 0) break;
            } // fin de la phrase.

            // Les probas associées aux tags du dernier vrai mot.
            if (mots.length() > 1)
            {
                // Le mot mots.length()-2 existe !
                int debut = sequences[0].size() - 7;
                if (!mots[mots.length()-2]->tagEncl().isEmpty()) debut -= 4; // Je dois reculer d'un tag de plus.
                // Le tag du mot mots.length()-2 est sequences[j].mid(debut, 3);
                for (int j = 0; j < sequences.size(); j++)
                    mots[mots.length()-2]->setBestOf(sequences[j].mid(debut, 3), probabilites[j]);
            }
            // Le tri final !
            QString seq = "";
            double val = -1;
            QString seq2 = "";
            double val2 = -1;
            for (int i = 0; i < sequences.size(); i++)
                if (val2 < probabilites[i])
                {
                    if (val < probabilites[i])
                    {
                        val2 = val;
                        seq2 = seq;
                        val = probabilites[i];
                        seq = sequences[i];
                    }
                    else
                    {
                        val2 = probabilites[i];
                        seq2 = sequences[i];
                    }
                }

            lsv.append(phr);
            lsv.append("<ul>");
            QString prob = "<br/> avec la proba : %1 pour %2 branches.<br/>";
            lsv.append(seq + prob.arg(val).arg(branches));
            if (val2 > 0)
            {
                prob = "Deuxième choix avec la proba : %1 <br/> %2<br/>";
                lsv.append(prob.arg(val2).arg(seq2));
            }

            seq = seq.mid(4); // Je supprime le premier tag qui est "snt".
            for (int i = 0; i < mots.size()-1; i++)
                if (!mots[i]->inconnu()) // Les mots inconnus ne figurent pas dans la séquence (cf. plus haut)
                {
                    lsv.append(mots[i]->choisir(seq.left(3), affTout));
                     // Si enclitique mid(8)
                    if (mots[i]->tagEncl().isEmpty()) seq = seq.mid(4);
                    else seq = seq.mid(5 + mots[i]->tagEncl().size());
                }
                else lsv.append("<li>" + mots[i]->forme() + " : non trouvé</li>");

            lsv.append("</ul>");
            if (tout) lsv << "<br/>";
            else return lsv.join("\n");
        }
        dph = fph + 1;
        fph++;
    }
    return lsv.join("\n");
}

