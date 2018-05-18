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

/**
 * @brief Tagueur::Tagueur :
 * \if French
 * Créateur de la classe Tagueur pour désambiguïser des textes.
 * \else
 * Constructs a Tagueur to tag sentences.
 * \endif
 * @param parent :
 * \if French
 * Un pointeur vers l'objet qui crée cette classe.
 * \else
 * The owner of this class.
 * \endif
 * @param l :
 * \if French
 * Un pointeur vers un moteur de lemmatisation (LemCore).
 * \else
 * A pointeur to the lemmatization core (LemCore).
 * \endif
 * @param cible :
 * \if French
 * La langue cible pour les traductions et les analyses.
 * \else
 * The language for the translations and analyses.
 * \endif
 * @param resDir :
 * \if French
 * Le chemin complet du dossier contenant les fichiers de donnée.
 * \else
 * The absolute path for the resources directory.
 * \endif
 *
 * \if French
 * La classe Tagueur est conçue pour proposer une fonction d'étiquetage
 * basée sur un modèle de Markov caché (HMM) au 2e ordre.
 * Initialement intégrée à Collatinus, elle en utilise le moteur de lemmatisation (LemCore).
 * Si le moteur est déjà créé par ailleurs,
 * il suffit de passer le pointeur en question.
 * Si le pointeur n'est pas donné,
 * le moteur (LemCore) sera créé ici.
 * Si l'application envisagée utilise plusieurs modules
 * intermédiaires (tagueur, scandeur...),
 * il vaut mieux créer un seul moteur commun.
 *
 * Lors de la création de cette classe, on peut choisir
 * la langue cible dans laquelle seront données les traductions et analyses.
 * Il s'agit d'une chaine de caractères contenant au moins deux lettres
 * (fr : français ; en : anglais ; etc...) et au plus huit
 * (jusqu'à trois groupes de deux lettres séparés par un espace).
 * Le fait de spécifier plusieurs langues précise l'ordre dans lequel
 * seront cherchées les traductions si on ne dispose pas de la traduction
 * dans la première langue choisie. Ainsi "it fr en" conduire le
 * programme a cherché la traduction en italien. Si la traduction
 * n'existe pas en italien, elle sera d'abord cherchée dans le lexique
 * français et en désespoir de cause en anglais.
 * Par défaut, elle vaut "fr en es", valeur définie dans LemCore.
 *
 * Le paramètre optionnel resDir donne
 * le chemin complet du dossier contenant les fichiers de donnée.
 * Par défaut, il s'agit du fichier "data" placé à côté de l'exécutable.
 * \else
 * The Tagueur class is meant to desambiguate with tags
 * using a second order Hidden Markov Model (HMM).
 * As part of Collatinus, it uses its lemmatization core (LemCore).
 * If this core has been created elsewhere, a pointer is given to this creator.
 * If the pointer l is empty, then the lemmatization core is created here.
 * When the developped application uses different modules as this one
 * (e.g. Lemmatiseur, Scandeur), it is recommended to create a shared core.
 *
 * One can change the language used for the translations and the analyses
 * with the QString cible. It should contain at least two characters
 * (en : English; fr : French) and at most eight (three groups of 2,
 * separated by a space). For instance "de en fr" will give the translations
 * in German, if possible. If the German translation is not found,
 * the English one will be searched for. French would be the last trial.
 * The default value is "fr en es" as defined in LemCore.
 *
 * The optional parameter resDir gives the absolute path to the
 * resources directory. If empty, the resources are assumed to be
 * in a folder "data" placed in the same dir as the exe.
 * \endif
 */
Tagueur::Tagueur(QObject *parent, LemCore *l, QString cible, QString resDir) : QObject(parent)
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
    if (cible != "") _lemCore->setCible(cible);
    defMask();
}

/**
 * @brief Tagueur::tagTexte
 * \if French
 * Pour désambiguïser une phrase (ou un texte) par étiquetage (HMM).
 * \else
 * To disambiguate a sentence (or a text) using a Hidden Markov Model.
 * \endif
 * @param t :
 * \if French
 * Le texte.
 * \else
 * The text.
 * \endif
 * @param p :
 * \if French
 * Une position dans le texte en nombre de caractères.
 * \else
 * A position in the text as a character count.
 * \endif
 * @param affTout :
 * \if French
 * Option pour afficher toutes les possibilités d'analyse
 * plutôt que la seule choisie.
 * \else
 * Option to show all the possible analyses.
 * \endif
 * @param majPert :
 * \if French
 * Option pour tenir compte des majuscules initiales.
 * \else
 * Option to take into account the initial uppercase characters.
 * \endif
 * @return
 * \if French
 * Une chaine de caractères contenant la meilleure solution en HTML.
 * \else
 * A QString with the best solution encoded in HTML.
 * \endif
 *
 * \if French
 * Cette fonction cherche la meilleure analyse des mots d'une phrase
 * en tenant compte du contexte à l'aide d'un modèle de Markov caché (HMM).
 * Pour cela, elle attribue des étiquettes aux mots et calcule une
 * probabilité pour chaque séquence d'étiquettes possible.
 * Pour ce faire, elle s'appuie sur les données statistiques
 * tirées des textes lemmatisés du LASLA.
 *
 * La phrase traitée est celle autour de la position p dans le texte t.
 * Si p<0, tout le texte sera traité en séparant chaque phrase.
 * Pour p≥0, le programme va, en partant de la position p, remonter et descendre
 * dans le texte jusqu'à trouver une ponctuation forte (. ! ? : ;).
 *
 * Le format de sortie est principalement destiné à l'affichage HTML.
 * J'ai ajouté quelques ancres pour faciliter la navigation,
 * mais je n'ai pas mis de liens pour y aller :
 * elles servent de repères.
 * Chaque phrase est entre des balises <div id='Sentence_x'> et </div>
 * où x est le numéro de la phrase. La numérotation commence à 0.
 * Ça n'a de sens que si l'ensemble du texte est tagué.
 * Si on ne tague qu'une seule phrase, ce numéro vaut toujours 0.
 *
 * Ces <div> commencent par donner la phrase, puis la meilleure séquence de tags
 * et la probabilité associées. Vient ensuite le second choix qui ne sera pas explicité.
 * La meilleure séquence de tags est explicitée sous la forme d'une
 * <ul id='sent_x'> où chaque mot est un <li id='S_x_W_y'>
 * avec x le numéro de la phrase et y le numéro du mot dans celle-ci.
 * Chaque <li> commence avec la forme du texte suivie du tag qui
 * lui est attribué par la meilleure séquence.
 * Le tag le plus probable hors contexte est donné entre parenthèses
 * lorsqu'il diffère de celui choisi par le tagueur.
 *
 * Si l'option affTout est valisée (affTout = true), toutes les analyses possibles
 * sont ajoutées, dans le <li> précédent, sous la forme d'une <ul> sans ancre.
 * À la fin de chaque <li> de cette seconde liste,
 * sont donnés le tag et la probabilité associée.
 * S'il y a des homonymes qui ont des analyses avec le même tag,
 * ces probabilités sont les mêmes (elles sont associées au tag).
 * Le choix se fait alors en fonction du nombre d'occurrences
 * relevé pour le lemme. Ce nombre est donné aussi (lorsqu'il n'est pas nul).
 * Il est entre des balises <small> et </small> et entre parenthèses.
 * \else
 * This function looks for the best analysis of the words in the sentence
 * taking into account the context with tags and a Hidden Markov Model (HMM).
 * The statistical data used in the HMM are taken from the texts
 * lemmatized by the LASLA.
 *
 * The treated sentence in around the position p in the text.
 * If p<0, all the text is treated, sentence by sentence.
 * For p≥0, the program looks forward and backward for the next
 * strong punctuation mark (. ! ? : ;).
 *
 * The output format is mainly intended for the display.
 * I have added a few anchors to facilitate the browsing,
 * but I do not add any link to them.
 * Each sentence is enclosed in tags <div id='Sentence_x'> and </div>
 * where x is the rank of the sentence, starting from 0.
 * It makes sense only if all the text is treated.
 * If a single sentence is treated, this numbre is always 0.
 *
 * These <div> start with the sentence, and then give
 * the best sequence of tags together with its probability.
 * Then comes the second choice and the associated probability.
 * The best choice is made explicit as a <ul id='sent_x'> where
 * each word is an item <li id='S_x_W_y'>, x and y being
 * the rank of respectively the sentence and the word.
 * Each item begins with the form of the text and the associated tag.
 * The most probable tag without taking into account the context
 * is given between parenthesis if different.
 *
 * With affTout=true, all the possible analyses of the form are given
 * in the previous item as a <ul> without any anchor.
 * At the end of each item of this second list, the tag and
 * the associated probability are given.
 * \endif
 */
QString Tagueur::tagTexte(QString t, int p, bool affTout, bool majPert)
{
    // éliminer les chiffres et les espaces surnuméraires
    t.remove(QRegExp("\\d"));
//    t = t.simplified(); // Rmq : perd les retours de ligne !
    int tl = t.length() - 1;
    const QString pp = ".;!?";
    int numPhr = 0; // Un numéro pour les phrases (utile seulement pous le texte complet)
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
            // J'ai maintenant une phrase complète.
            // Je la conserve pour la construction d'arbres.
            _phrase = phr;

//            QList<Mot*> mots;
            // Je dois commencer par effacer les _mots précédents
            effacer();

            // lemmatisation pour chaque mot
            for (int i = 1; i < lm.length(); i += 2)
            {
                bool debVers = !majPert || lm[i-1].contains("\n");
                if (Ch::abrev.contains(lm[i]))
                {
                    // J'ai bêtement découpé les mots : si j'avais une abréviation,
                    // je dois déplacer le point.
                    if (lm[i+1].startsWith("'"))
                    {
                        lm[i].append("'.");
                        if (lm[i+1].startsWith("'."))
                            lm[i+1] = lm[i+1].mid(2);
                        else lm[i+1] = lm[i+1].mid(1);
                    }
                    else if (lm[i+1].startsWith("."))
                    {
                        lm[i].append(".");
                        lm[i+1] = lm[i+1].mid(1);
                    }
                }
                Mot * mot = new Mot(lm[i],(i-1)/2, debVers, _lemCore);
                // TODO : Vérifier si on a des vers avec majuscule...
                _mots.append(mot);
            }  // fin de boucle de lemmatisation pour chaque mot
            Mot * mot = new Mot("",_mots.size(),true, _lemCore); // Fin de phrase
            _mots.append(mot); // J'ajoute un mot virtuel en fin de phrase avec le tag "snt".

            QStringList sequences;
            QList<double> probabilites;
            sequences.append("snt");
            probabilites.append(1.0);
            double branches = 1.0; // Pour savoir combien de branches a l'arbre.
            // Je suis en début de phrase : je n'ai que le tag "snt" et une proba de 1.
            for (int i = 0; i < _mots.size(); i++)
            {
                Mot *mot = _mots[i];
                QStringList lTags = mot->tags(); // La liste des tags possibles pour le mot
                QStringList nvlSeq; // Nouvelle liste des séquences possibles
                QList<double> nvlProba; // Nouvelle liste des probas.
                // Je dois ajouter tous les tags possibles à toutes les sequences
                // et calculer les nouvelles probas.
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
                        // Si j'avais gardé toutes les séquences,
                        // ce serait une vraie probabilité (normalisée à 1)
                    }
                }
                // Ajouter les enclitiques.
                if (!mot->tagEncl().isEmpty())
                {
                    QString ajout = " " + mot->tagEncl();
                    for (int j = 0; j < nvlSeq.size(); j++) nvlSeq[j].append(ajout);
                    // Comme toutes les formes à tag unique,
                    // l'enclitique ne change pas les probabilités.
                }
                // J'ai toutes les sequences et leur proba.
                // Pour chaque bigramme terminal, je ne dois garder que la séquence la plus probable.
                // En faisant ce tri, je fais une sélection sur le tag i-2

                // Si je veux garder une info sur l'ordre des tags du mot i-2, c'est maintenant !
                if (i > 0) // Si i==0, rien à faire.
                {
                    int debut = nvlSeq[0].size() - 11;
                    // 11 = 3 * 3 + 2 = 3 tags et 2 espaces (entre les tags).
                    // Si j'ai un enclitique dans l'un des 3 derniers mots,
                    // j'ai autant de tags en plus.
                    if (!mot->tagEncl().isEmpty())
                    {
                        // Si le mot courant a un enclitique, j'ai ajouté deux tags d'un coup.
                        // Le tri sur le bigramme terminal va alors me faire perdre des tags
                        // pour les mots i-1 et i-2. Sinon, seul le mot i-2 est concerné.
                        // Le mot i-1 existe !
                        if (_mots[i-1]->tagEncl().isEmpty())
                        {
                            for (int j = 0; j < nvlSeq.size(); j++)
                            {
                                // nvlSeq[j].mid(debut, 3) est le tag du mot i-1.
                                _mots[i-1]->setBestOf(nvlSeq[j].mid(debut, 3), nvlProba[j]);
                                // Pour tous les tags du mot i-1, je garde la meilleure proba.
                                if ((i > 1) && _mots[i-2]->tagEncl().isEmpty())
                                    _mots[i-2]->setBestOf(nvlSeq[j].mid(debut-4, 3), nvlProba[j]);
                                // Si le mot i-2 portait un enclitique,
                                // ses probas ont été déterminées au coup d'avant.
                                // Sinon, nvlSeq[j].mid(debut-4, 3) est le tag du mot i-2, s'il existe.
                            }
                        }
                        else
                        {
                            // Si le mot i-1 portait aussi l'enclitique, je dois reculer d'un tag de plus.
                            debut -= 4;
                            for (int j = 0; j < nvlSeq.size(); j++)
                                _mots[i-1]->setBestOf(nvlSeq[j].mid(debut, 3), nvlProba[j]);
                            // Pour tous les tags du mot i-1, je garde la meilleure proba.
                            // Et c'est tout.
                        }
                    }
                    // J'ai traité le cas où le mot courant porte un enclitique.
                    else if (!_mots[i-1]->tagEncl().isEmpty())
                    {
                        // Si le mot i-1 portait un enclitique, il a figé
                        // les probas des deux mots qui le précédaient (i-2 et i-3).
                        // Les bigrammes terminaux commencent par l'enclitique
                        for (int j = 0; j < nvlSeq.size(); j++)
                            _mots[i-1]->setBestOf(nvlSeq[j].mid(debut, 3), nvlProba[j]);
                        // Pour tous les tags du mot i-1, je garde la meilleure proba.
                    }
                    else if ((i > 1) && _mots[i-2]->tagEncl().isEmpty())
                        for (int j = 0; j < nvlSeq.size(); j++)
                            _mots[i-2]->setBestOf(nvlSeq[j].mid(debut, 3), nvlProba[j]);
                    // Pour tous les tags du mot i-2, je garde la meilleure proba.
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
                    for (int k = j; k < nvlSeq.size(); k += sTag)
                        // Pour retrouver le bigramme terminal, il faut au moins le même dernier tag.
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
                            nvlProba[k] = -1;
                            // Pour ne pas considérer deux fois les mêmes séquences.
                        }
                    // val et seq correspondent aux proba et séquence avec le bigramme considéré
                    // qui ont la plus grande proba.
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
            if (_mots.length() > 1)
            {
                // Le mot mots.length()-2 existe !
                int debut = sequences[0].size() - 7;
                if (!_mots[_mots.length()-2]->tagEncl().isEmpty()) debut -= 4;
                // Je dois reculer d'un tag de plus.
                // Le tag du mot mots.length()-2 est sequences[j].mid(debut, 3);
                for (int j = 0; j < sequences.size(); j++)
                    _mots[_mots.length()-2]->setBestOf(sequences[j].mid(debut, 3), probabilites[j]);
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

            QString prob = "<div id='Sentence_%1'>";
            lsv.append(prob.arg(numPhr) + phr + "<br/><br/>\n");
            prob = "<br/>\n avec la proba : %1 pour %2 branches.<br/>";
            lsv.append(seq + prob.arg(val).arg(branches));
            if (val2 > 0)
            {
                prob = "Deuxième choix avec la proba : %1 <br/>\n %2<br/>";
                lsv.append(prob.arg(val2).arg(seq2));
            }
            prob = "<ul id='sent_%1'>";
            lsv.append(prob.arg(numPhr)); // Je vais commencer la liste de mots
            seq = seq.mid(4); // Je supprime le premier tag qui est "snt".
            for (int i = 0; i < _mots.size()-1; i++)
                if (!_mots[i]->inconnu()) // Les mots inconnus ne figurent pas dans la séquence (cf. plus haut)
                {
                    lsv.append(_mots[i]->choisir(seq.left(3), numPhr, affTout));
                     // Si enclitique mid(8)
                    if (_mots[i]->tagEncl().isEmpty()) seq = seq.mid(4);
                    else seq = seq.mid(5 + _mots[i]->tagEncl().size());
                }
                else lsv.append("<li id='unknown'>" + _mots[i]->forme() + " : non trouvé</li>");

            lsv.append("</ul></div>");
            if (tout)
            {
                lsv << "<br/>";
                numPhr++;
            }
            else
            {
                analyse(); // Placé ici pour les essais.

                return lsv.join("\n");
                // Je retourne le résultat de la phrase si tout est false.
            }
        }
        dph = fph + 1;
        fph++;
    } // while (fph < tl)
    // Si tout est true, je vais jusqu'au bout de texte.
    return lsv.join("\n");
}

void Tagueur::defMask()
{
    iafMult = 256; // Je réserve 8 bits pour chaque indice.
    itpMult = iafMult * iafMult;
    iapMult = itpMult * iafMult;
    irMult = iapMult * iafMult;
    itfMask = iafMult - 1;
    iafMask = itfMask * iafMult;
    itpMask = itfMask * itpMult;
    iapMask = itfMask * iapMult;
}

quint64 Tagueur::genLien(int iRegle, int iTokPere, int iAnPere, int iTokFils, int iAnFils)
{
    // Dois-je faire des tests ?
    return ((((iRegle * iafMult + iAnPere) * iafMult + iTokPere) * iafMult + iAnFils) *iafMult + iTokFils);
}

int Tagueur::iTokFils(quint64 lien)
{
    return (lien & itfMask);
}

int Tagueur::iTokPere(quint64 lien)
{
    return ((lien & itpMask)/itpMult);
}

int Tagueur::iAnFils(quint64 lien)
{
    return ((lien & iafMask)/iafMult);
}

int Tagueur::iAnPere(quint64 lien)
{
    return ((lien & iapMask)/iapMult);
}

int Tagueur::iRegle(quint64 lien)
{
    return (lien/irMult);
}

void Tagueur::effacer()
{
    if (!_mots.isEmpty())
    {
        if (_tokens.isEmpty())
            while (!_mots.isEmpty())
            {
                delete _mots.last();
                _mots.removeLast();
            }
        else
        {
            _mots.clear();
            while (!_tokens.isEmpty())
            {
                delete _tokens.last();
                _tokens.removeLast();
            }
        }
    }
    // Si j'ai construit un arbre syntaxique,
    // chaque mot a été repris comme un token,
    // mais les enclitiques ont été ajoutés.
    // Je libère tous les Mot repérés par un pointeur,
    // avant d'avoir effacer ce pointeur.
    _listLiens.clear();
    _foret.clear();
    _interrogative = false;
    _mots.clear();
    _tokens.clear();
}

void Tagueur::lireRegles()
{
    // lecture des données syntaxiques
    QStringList lignes = _lemCore->lignesFichier(_resDir + "syntaxe.la");
    QStringList slr;  // liste des lignes de la dernière règle lue.
    for (int i=0;i<lignes.count();++i)
    {
        QString l = lignes.at(i);
        if ((l.startsWith("id:")||i==lignes.count()-1) && !slr.empty())
        {
            RegleSynt *nrs = new RegleSynt(slr, this);
            _idRegle.insert(nrs->id(),_regles.size());
            _regles.append(nrs);
            slr.clear();
        }
        slr.append(l);
    }
    qDebug() << _regles.size() << _idRegle.keys().size();
}

void Tagueur::ajToken(int i)
{
    // J'introduis d'abord un token avec l'enclitique...
    // ... puis le mot, ou l'inverse.
    QString te = _mots[i]->tagEncl(); // ce de v11 re
    if (te == "v11") // -st = est ; reste après le mot.
    {
        _tokens.append(_mots[i]);
        Mot * mot = new Mot("est",i, false, _lemCore);
        _tokens.append(mot);
    }
    else if (te == "de ") // C'est un -ne
    {
        _tokens.append(_mots[i]);
//        Mot * mot = new Mot("",i, false, _lemCore);
//        _tokens.prepend(mot);
        // L'enclitique -ne introduit une question.
        // Se traduit par "Est-ce que" en début de phrase.
        _interrogative = true;
    }
    else
    {
        if (te == "ce ") // -que ou -ve
        {
            QString f = "et";
            if (_mots[i]->enclitique() == "vĕ") f = "vel";
            Mot * mot = new Mot(f,i, false, _lemCore);
            _tokens.append(mot);
        }
        else if (te == "re ") // -cum
        {
            Mot * mot = new Mot("cum",i, false, _lemCore);
            _tokens.append(mot);
        }
        _tokens.append(_mots[i]);
    }
}

bool Tagueur::accordOK(QString ma, QString mb, QString cgn)
{
    if (cgn.isEmpty()) return true;
    bool xPourMa = false;
    bool xPourMb = false;
    bool xCommun = false;
    bool mac = false;
    bool mbc = false;
    if (cgn.contains('c'))
    {
        for (int i = 0; i < 6; i++)
        {
            QString k = _lemCore->cas(i);
            mac = ma.contains(k);
            mbc = mb.contains(k);
            xPourMa = xPourMa || mac;
            xPourMb = xPourMb || mbc;
            xCommun = xCommun || (mac && mbc);
        }
        if (xPourMa && xPourMb && !xCommun)
            return false; // Les deux mots ont un cas, mais n'en ont pas en commun.
    }
    if (cgn.contains('g'))
    {
        xPourMa = false;
        xPourMb = false;
        xCommun = false;
        for (int i = 0; i < 3; i++)
        {
            QString g = _lemCore->genre(i);
            mac = ma.contains(g);
            mbc = mb.contains(g);
            xPourMa = xPourMa || mac;
            xPourMb = xPourMb || mbc;
            xCommun = xCommun || (mac && mbc);

        }
        if (xPourMa && xPourMb && !xCommun)
            return false;
    }
    if (cgn.contains('n'))
    {
        xPourMa = false;
        xPourMb = false;
        xCommun = false;
        for (int i = 0; i < 2; i++)
        {
            QString n = _lemCore->nombre(i);
            mac = ma.contains(n);
            mbc = mb.contains(n);
            xPourMa = xPourMa || mac;
            xPourMb = xPourMb || mbc;
            xCommun = xCommun || (mac && mbc);

        }
        if (xPourMa && xPourMb && !xCommun)
            return false;
    }
    if (cgn.contains('p'))
    {
        xPourMa = false;
        xPourMb = false;
        xCommun = false;
        for (int i = 1; i < 4; i++)
        {
            QString p = QString("%1").arg(i);
            mac = ma.contains(p);
            mbc = mb.contains(p);
            xPourMa = xPourMa || mac;
            xPourMb = xPourMb || mbc;
            xCommun = xCommun || (mac && mbc);

        }
        if (xPourMa && xPourMb && !xCommun)
            return false;
    }
    return true;
}

void Tagueur::trouvePere(int ir, int itf, int iaf, int itp)
{
    if ((_tokens[itp] != _mots[_tokens[itp]->rang()]) && !liensEncl(itp,ir))
        return; // Le token est un enclitique incompatible avec la règle.
    RegleSynt *rs = _regles[ir];
    for (int iap = 0; iap < _tokens[itp]->nbAnalyses(); iap++)
        if (rs->acceptePere(_tokens[itp]->sLem(iap), _tokens[itp]->morpho(iap)))
        {
            // Je dois encore vérifier les règles d'accord...
            QString mp = _tokens[itp]->morpho(iap);
            Lemme *l = _tokens[itp]->sLem(iap).lem;
            if (l->pos().contains("n")) mp.append(" "+l->genre());
            QString mf = _tokens[itf]->morpho(iaf);
            l = _tokens[itf]->sLem(iaf).lem;
            if (l->pos().contains("n")) mf.append(" "+l->genre());
            if (accordOK(mp,mf,rs->accord()))
            {
                _liensLocaux << genLien(ir,itp,iap,itf,iaf);
            }
        }
}

void Tagueur::cherchePere(int ir, int itf, int iaf)
{
    RegleSynt *rs = _regles[ir];
    // Je sais que l'analyse iaf du mot itf peut être le fils de cette règle.
    // Je cherche un père possible, avant ou après ce mot.
    if (rs->sens() != "<")
    {
        // Le père peut être avant le fils.
        // la flèche est orientée du super au sub. '>' = père avant le fils
        if (rs->synt().contains("c"))
            trouvePere(ir,itf,iaf,itf - 1);
            // Le père et le fils doivent se toucher.
        else for (int itp = 0; itp < itf; itp++)
            trouvePere(ir,itf,iaf,itp);
    }
    if (rs->sens() != ">")
    {
        // Le père peut être après le fils
        if (rs->synt().contains("c"))
            trouvePere(ir,itf,iaf,itf + 1);
            // Le père et le fils doivent se toucher.
        else for (int itp = itf + 1; itp < _tokens.size(); itp++)
            trouvePere(ir,itf,iaf,itp);
    }
}

void Tagueur::analyse()
{
//    qDebug() << _mots.size() << _tokens.size();
    if (_mots.isEmpty()) return; // Pas de phrase !
    if (_regles.isEmpty()) lireRegles(); // Premier passage.
    // Je crée une liste de tokens à partir des mots.
    // Si un mot porte un enclitique, je crée un token virtuel.
    // Pour -que, je mets un "et" avant le mot.
    // L'indice du token pourra différer du rang (du mot).
    for (int i = 0; i < _mots.size() - 1; i++) // Le dernier mot est un "snt"
        if (_mots[i]->tagEncl() == "") _tokens.append(_mots[i]);
        else ajToken(i); // S'il y a un enclitique, ça dépend...
//    qDebug() << _mots.size() << _tokens.size();

    // Détermination des liens possibles.
    for (int itf = 0; itf<_tokens.size(); itf++)
    {
        _liensLocaux.clear();
        for (int iaf = 0; iaf < _tokens[itf]->nbAnalyses(); iaf++)
            for (int ir = 0; ir < _regles.size(); ir++)
                if (_regles[ir]->accepteFils(_tokens[itf]->sLem(iaf), _tokens[itf]->morpho(iaf)))
                {
                    if (_tokens[itf] != _mots[_tokens[itf]->rang()])
                    {
                        if (liensEncl(itf,ir))
                            cherchePere(ir,itf,iaf);
                    }
                    else cherchePere(ir,itf,iaf);
                }
                // Si la règle accepte l'analyse iaf du token itf comme fils,
                // je cherche un pere qui va avec.
        // Attention aux enclitiques.
        trierLiens();
        _listLiens.append(_liensLocaux);
    }
    qDebug() << "Nombre de liens possibles :" << _listLiens.size() << "pour" << _tokens.size() << "mots.";
    /*
    for (int i=0; i < _listLiens.size(); i++)
    {
        quint64 l = _listLiens[i];
        int itp = iTokPere(l);
        int itf = iTokFils(l);
        int ir = iRegle(l);
        int iap = iAnPere(l);
        int iaf = iAnFils(l);
        qDebug() << _tokens[itp]->lemme(iap) << _tokens[itp]->morpho(iap)
                 << _regles[ir]->id() << _tokens[itf]->lemme(iaf) << _tokens[itf]->morpho(iaf);
    }
    */
    QList<int> li;
    for (int i=0; i < _tokens.size(); i++) li << i;
    Arbre arbre;
    _maxOrph = 2;
    int t = _temps.restart();
    faireCroitre(_listLiens,arbre,li,0);
    qDebug() << _foret.size() << _temps.elapsed();
    // Les arbres de la forêt sont dans l'ordre où ils ont été trouvés.
    // Il faudrait les ranger selon des critères esthétiques.
    qDebug() << sauvArbre(0);
}

void Tagueur::faireCroitre(QList<quint64> lLiens, Arbre pousse, QList<int> indices, int nbRel)
{
//    qDebug() << "Appelé avec" << lLiens.size() << pousse.size() << indices << nbRel;
    Arbre nvlPouss = pousse;
    // C'est une routine récursive.
    // Je commence par les conditions d'arrêt.
    if (_temps.elapsed() > 60000) return; // Si ça dure trop longtemps (1min), je l'arrête.
    if (indices.isEmpty())
    {
        // J'ai traité tous les tokens
        _foret.append(nvlPouss);
        return;
    }
    if (lLiens.isEmpty())
    {
        // J'ai épuisé les liens possibles
        if ((indices.size() <= _maxOrph + nbRel) && !_foret.contains(nvlPouss))
        {
            _foret.append(nvlPouss);
//            qDebug() << pousse.size() << lLiens.size() << indices.size();
//            _maxOrph = indices.size() + 1 - nbRel;
        }
        return;
    }
    // Je dois choisir quel mot je veux traiter :
    // celui sur lequel arrive au moins un lien et
    // qui en reçoit le moins possible.
    QMap<int,int> nbLiens;
    // Associe le nombre de liens qui arrivent sur un token donné.
    // L'indice du token est la clef.
    for (int i = 0; i < lLiens.size(); i++) nbLiens[iTokFils(lLiens[i])] += 1;
    QList<int> listFils = nbLiens.keys();
    if (listFils.isEmpty()) return; // Normalement, ça ne se produit pas.
    if (indices.size() - listFils.size() - nbRel > _maxOrph) return;
    // La diff de taille est le nombre d'orphelins potentiels.
    // Attention aux relatifs !
    int valMin = 1024;
    int minPos = -1;
    for (int i = 0; i < indices.size(); i++)
        if (listFils.contains(indices[i]) && (nbLiens[indices[i]] < valMin))
        {
            valMin = nbLiens[indices[i]];
            minPos = i;
        }
    // Je connais le min et son indice.
    // Si l'indice du token est dans la liste des clefs,
    // il y a au moins un lien qui y arrive.
    // Si j'arrive ici, c'est qu'il y a au moins un lien à essayer.
    int iTok = indices[minPos];
    QList<int> nvxInd = indices;
    nvxInd.removeAt(minPos);
//    qDebug() << iTok << valMin << indices.size() << nvxInd.size() << lLiens.size();
    // Si j'établis un lien qui arrive sur ce mot,
    // je le supprime de la liste.
    QList<quint64> liensIci;
    QList<quint64> ailleurs;
    int indFinFils = 0;
    for (int i = 0; i < lLiens.size(); i++)
        if (iTokFils(lLiens[i]) == iTok)
        {
            liensIci.append(lLiens[i]);
            indFinFils = i;
        }
        else ailleurs.append(lLiens[i]);
    // Je sépare la liste de liens initiale en deux :
    // ceux qui arrivent sur le token choisi et les autres.
    // indFinFils est l'indice du dernier fils dans lLiens.
    int indAvant = indFinFils - liensIci.size();
    // indAvant est l'indice du dernier liens dont le fils est avant
    // le mot que je dois traiter.
    // Indice valable autant dans lLiens que dans ailleurs.
//    qDebug() << indAvant << indFinFils << liensIci.size() << ailleurs.size();
    for (int i = 0; i < liensIci.size(); i++)
    {
        quint64 lienChoisi = liensIci[i];
        // Il faudrait vérifier ICI que le lienChoisi n'est pas incompatible
        // avec ceux déjà présents dans l'arbre.
        // En particulier, un verbe ne peut avoir qu'un seul sujet...
        QList<quint64> compat = liensComp(ailleurs, nvlPouss, lienChoisi);
        // C'est la liste des liens d'ailleurs qui sont compatibles avec lienChoisi.
        // Si le token est un pronom relatif, il peut vouloir deux liens.
        nvlPouss.append(lienChoisi);
        if (lienChoisi < 2 * irMult)
        {
            // C'est une règle de coordination que j'ai placée en tête de syntaxe.la.
            QList<quint64> liensPossibles;
            if (lienChoisi < irMult)
            {
                // C'est une coord, c'est à dire la 2e partie de l'ensemble coordonné.
                // Je dois trouver une conjCoord qui arrive sur le père de lienChoisi.
                int itp = iTokPere(lienChoisi);
                for (int j = 0; j < indAvant + 1; j++)
                    if ((iTokFils(ailleurs[j]) == itp) && (ailleurs[j] < 2 * irMult))
                    {
                        // Je dois tester l'accord entre les deux coordonnés.
                        quint64 lienCh2 = ailleurs[j];
                        // C'est à dire entre le fils de lienChoisi et le père de lienCh2.
                        if (accCoord(lienCh2,lienChoisi))
                            liensPossibles.append(lienCh2);
                    }
            }
            else
            {
//                qDebug() << "J'ai une conjCoord";
                // Je dois trouver une coord qui a pour père le fils du lien choisi.
                for (int j = indAvant + 1; j < ailleurs.size(); j++)
                    if ((ailleurs[j] < irMult) && (iTokPere(ailleurs[j]) == iTok))
                    {
                        // Je dois tester l'accord entre les deux coordonnés.
                        quint64 lienCh2 = ailleurs[j];
                        // C'est à dire entre le père de lienChoisi et le fils de lienCh2.
                        if (accCoord(lienChoisi,lienCh2))
                            liensPossibles.append(lienCh2);
                    }
//                qDebug() << "Avec" << liensPossibles.size() << "liens possibles";
            }
            if (!liensPossibles.isEmpty())
            for (int j = 0; j < liensPossibles.size(); j++)
            {
                quint64 lienCh2 = liensPossibles[j];
                QList<int> nvxxI = nvxInd;
                nvxxI.removeOne(iTokFils(lienCh2));
                QList<quint64> compat2 = liensComp(compat, nvlPouss, lienCh2);
                nvlPouss.append(lienCh2);
                faireCroitre(compat2,nvlPouss,nvxxI,nbRel);
                // Tentative de construction en ayant ajouté deux liens
                // de part et d'autre de la conjonction de coordination.
                nvlPouss.removeLast();
            }
            // J'essaie tous les liens possibles.
        }
        else if (estAntecedent(lienChoisi))
        {
            // Je dois choisir un deuxième lien parmi ceux qui suivent
            // (avant, il n'y a que des antécédents).
            for (int j = i+1; j < liensIci.size(); j++)
                if (!estAntecedent(liensIci[j]))
                {
                    quint64 lienCh2 = liensIci[j];
                    // Est-il compatible ?
                    if (iAnFils(lienCh2) == iAnFils(lienChoisi))
                    {
                        QList<quint64> compat2 = liensComp(compat, nvlPouss, lienCh2);
                        nvlPouss.append(lienCh2);
                        faireCroitre(compat2,nvlPouss,nvxInd,nbRel + 1);
                        // Tentative de construction en ajoutant deux liens pour le relatif.
                        nvlPouss.removeLast();
                    }
                }
        }
        else
        {
            faireCroitre(compat,nvlPouss,nvxInd,nbRel);
            // Tentative normale de construction.
        }
        nvlPouss.removeLast();
    } // Fin des possibilités des liens qui arrivent sur le mot choisi.
    // Il me reste la possibilité de laisser ce mot orphelin.
//    qDebug() << iTok << "orphelin !";
    faireCroitre(ailleurs,pousse,indices,nbRel);
    // Tentative de construction en laissant le mot choisi orphelin.
}

bool Tagueur::liensEncl(int it, int ir)
{
    // Le token it est un enclitique : seuls certaines règles sont possibles
    if (_tokens[it]->forme() == "et")
    {
        if (_regles[ir]->id().contains("oord"))
            return true;
        else return false;
    }
    if (_tokens[it]->forme() == "cum")
    {
        if (_regles[ir]->id() == "regimeAbl") return true;
        if (_regles[ir]->id() == "prep") return true;
        return false;
    }
    // Autres enclitiques...
    return true;
}

void Tagueur::trierLiens()
{
    QMultiMap<int,quint64> ord;
    for (int i = 0; i < _liensLocaux.size(); i++)
    {
//        qDebug() << eval(_liensLocaux[i]) << _liensLocaux[i];
        ord.insert(eval(_liensLocaux[i]),_liensLocaux[i]);
    }
    _liensLocaux = ord.values();
}

int Tagueur::eval(quint64 lien)
{
    int itf = iTokFils(lien);
    int itp = iTokPere(lien);
    int longueur = abs(itf - itp);
    if (_tokens[itf]->choix() == iAnFils(lien)) longueur -= 1;
    if (_tokens[itp]->choix() == iAnPere(lien)) longueur -= 1;
    if (_tokens[itf]->maxEC() == iAnFils(lien)) longueur -= 1;
    if (_tokens[itp]->maxEC() == iAnPere(lien)) longueur -= 1;
    if (_tokens[itf]->maxHC() == iAnFils(lien)) longueur -= 1;
    if (_tokens[itp]->maxHC() == iAnPere(lien)) longueur -= 1;
    // Si l'indice de l'analyse choisie est le meilleur (selon les 3 critères possibles),
    // je raccourcis la longueur d'une unité. Le bonus peut atteindre 6.
    if (estAntecedent(lien)) longueur -= 100;
    // Pour les relatifs, je veux que les antécédents soient traités en premier.
    return longueur;
}

bool Tagueur::estAntecedent(quint64 lien)
{
    return (_regles[iRegle(lien)]->id().startsWith("ant"));
}

QList<quint64> Tagueur::liensComp(QList<quint64> ailleurs, Arbre nvlPouss, quint64 lienChoisi)
{
    // Le but est de sélectionner les liens de la liste ailleurs
    // qui sont compatibles avec l'arbre et le nouveau lien choisi.
    QList<quint64> liste;
    int itp = iTokPere(lienChoisi);
    int itf = iTokFils(lienChoisi);
    int ir = iRegle(lienChoisi);
    int iap = iAnPere(lienChoisi);
    int iaf = iAnFils(lienChoisi);
    nvlPouss.append(lienChoisi);
    bool unic = _regles[ir]->synt().contains("u");
    QString idF = _regles[ir]->idFam();
    // Si la règle pour le nouveau lien est réputée unique,
    // je dois écarter tous liens utilisant la même règle
    // (ou un règle fille) à partir du même père.
    // Si j'ai bien travaillé, le lien que je choisis ne peut pas fermer une boucle.
    for (int i = 0; i < ailleurs.size(); i++)
    {
        quint64 lien = ailleurs[i];
        bool OK = !unic || itp != iTokPere(lien) || idF != _regles[iRegle(lien)]->idFam();
        // C'est la négation de (unic && itp==iTokPere(lien) && ir==iRegle(lien)).
        // Je tiens compte de l'héritage en comparant les idFam.
        if (OK) OK = (itf != iTokPere(lien)) || (iaf == iAnPere(lien));
        // Si les mots sont différents tout va bien.
        // Si les mots sont les mêmes, l'analyse doit être la même.
        if (OK) OK = (itp != iTokPere(lien)) || (iap == iAnPere(lien));
        if (OK) OK = (itp != iTokFils(lien)) || (iap == iAnFils(lien));
        if (OK) OK = (itf != iTokFils(lien));
        // Quand j'ajoute le 2e lien d'un groupe coordonné,
        // je dois aussi éliminer les liens qui ont le même fils.
        // Je vérifie ici que je ne créerais pas de boucle.
        if (OK) OK = pasBoucle(nvlPouss,lien);
        if (OK) liste.append(lien);
        // Si une des quantités calculées est fausse,
        // je n'en calcule pas d'autre et j'ignore le lien.
    }
    return liste;
}

bool Tagueur::pasBoucle(Arbre a, quint64 l1)
{
    // Je dois vérifier que l'ajout du lien l1 ne crée pas une boucle.
    // Pour ça, j'établis la liste des ancêtres du père de l1
    // et je vérifie que le fils de l1 n'en fait pas partie.
    return !ancetres(iTokPere(l1),a).contains(iTokFils(l1));
}

QList<int> Tagueur::ancetres(int itf, Arbre a)
{
    QList<int> res;
    for (int i = 0; i < a.size(); i++)
        if ((itf == iTokFils(a[i])) && (itf != iTokPere(a[i])))
            res << iTokPere(a[i]) << ancetres(iTokPere(a[i]),a);
    return res;
}

bool
 Tagueur::accCoord(quint64 lienCC, quint64 lienC)
{
    // lienC est le lien avec la règle coord.
    // lienCC est le lien avec la règle conjCoord.
    // Je dois vérifier l'accord entre le pere de lienCC
    // et le fils de lienC.
    // La conjonction de coordination est au milieu :
    // fils de lienCC et père de lienC.
    int itp = iTokPere(lienCC);
    int itf = iTokFils(lienC);
    int iap = iAnPere(lienCC);
    int iaf = iAnFils(lienC);
    QString pp = _tokens[itp]->sLem(iap).lem->pos();
    QString pf = _tokens[itf]->sLem(iaf).lem->pos();
    // Ce sont les POS des père et fils.
    if (pp.contains("n") || pp.contains("p"))
    {
        // Le père est un nom ou un pronom.
        if (pf.contains("n") || pf.contains("p"))
        {
            // Le fils est un nom ou un pronom.
            // Il me faut un accord en cas.
            return accordOK(_tokens[itp]->morpho(iap),_tokens[itf]->morpho(iaf),"c");
        }
        else return false;
    }
    if (pp.contains("a") && pf.contains("a"))
        return accordOK(_tokens[itp]->morpho(iap),_tokens[itf]->morpho(iaf),"cgn");
    if (pp.contains("v") && pf.contains("v")) return true;
    if (pp.contains("d") && pf.contains("d")) return true;
    return false;
}

int Tagueur::nbArbres()
{
    return _foret.size();
}

QString Tagueur::sauvArbre(int i, bool ordre)
{
    if (i >= _foret.size()) return "";
    Arbre arbre = _foret[i];
    if (arbre.isEmpty()) return "";
    QMap<int,int> am;
    for (int j = 0; j < arbre.size(); j++)
    {
        quint64 lien = arbre.at(j);
        am[iTokPere(lien)] = iAnPere(lien);
        am[iTokFils(lien)] = iAnFils(lien);
    }
    // am donne l'analyse choisie pour chaque token (dont l'indice est la clef d'am).
    QList<int> listTokens = am.keys();

    QStringList arbr;

    QString p = _phrase;
    p.append("\n");
/*    p.append(QString::number(longArbres[i]));
    if (dMin != longArbres[i])
    {
        p.append(" pour un minimum de ");
        p.append(QString::number(dMin));
    }
//    p.append(" ");
//    p.append(QString::number(foret[i]->dist2()));
*/
    QString message = "digraph arbre {\n";
    message.append("graph [ordering=\"out\"];\n");
    message.append("labelloc=\"t\";\n");
    message.append("label=\"" + p + " \";\n");
    message.append("node [shape=box];\n");
    if (ordre) message.append("rankdir=LR;\n");
    arbr << message;
    /* pour Graphviz, définition de tous les noeuds */
    QString label = "N%1 [label=\"%2\",URL=\"#N%1\n%3\n%4\"];";
    QString lem;
    QString pt;
    for (int j = 0; j < _tokens.size(); j++)
        if (_tokens[j] == _mots[_tokens[j]->rang()])
        {
            if (listTokens.contains(j))
            {
                lem = _tokens[j]->sLem(am[j]).lem->grq();
                pt = _tokens[j]->morpho(am[j]);
            }
            else
            {
                lem = "?";
                pt = "?";
            }
            // J'évite les tokens virtuels qui sont issus d'enclitique.
            arbr << label.arg(_tokens[j]->rang()).arg(_tokens[j]->forme())
                    .arg(lem).arg(pt);
        }
    // Les noeuds sont définis

    if (ordre)
    {
        // J'ajoute un lien invisible entre tous les mots pour imposer l'ordre de la phrase
        message = "\nN";
        pt = "%1->N";
        for (int j = 0; j < _mots.size() - 1; j++)
            message.append(pt.arg(j));
        message.chop(3);
        message.append(" [style=invis];\n");
        arbr << message;
    }
    // Fin des entêtes

    QString lg = "N%1->N%2 [label=\"%3\",URL=\"#L%4\"];";
    for (int j = 0; j < arbre.size(); j++)
    {
        quint64 l = arbre[j];
        arbr << lg.arg(_tokens[iTokPere(l)]->rang()).
                arg(_tokens[iTokFils(l)]->rang()).
                arg(_regles[iRegle(l)]->id()).arg(j);
    }

    arbr << "}\n";
    return arbr.join("\n");

}
