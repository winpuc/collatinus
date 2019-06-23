/*               mot.cpp
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

#include "mot.h"

Mot::Mot(QString forme, int rang, bool debVers, QObject *parent)
{
//    qDebug() << forme;
    _forme = forme;
    _rang = rang;
    _lemCore = qobject_cast<LemCore *>(parent);
    _probas.clear();
    _tagEncl = "";
    _enclitique = "";
    _choix = 0;
    _maxEC = 0;
    _maxHC = 0;
    if (forme.isEmpty())
    {
        _tags << "snt";
        _probas["snt"] = 1;
    }
    else
    {
        // cum se colle aux pronoms personnels ou relatifs.
        // Le LASLA le traite comme un enclitique.
        // Mais il n'est pas considéré comme un enclitique par lemCore.
        QString f = forme.toLower();
        bool encl = (f == "mecum") || (f == "tecum") || (f == "secum");
        encl = encl || (f == "quacum") || (f == "quocum") || (f == "quicum");
        bool pl = (f == "nobiscum") || (f == "vobiscum") || (f == "quibuscum");
        // Ce sont des pluriels
        if (encl || pl)
        {
            // Je dois donc le traiter "proprement"...
            _tagEncl = "re ";
            _enclitique = "cŭm";
            QString tt = "p61";
            if (pl) tt = "p62";
//            if (f == "secum") tt = "p61, p62";
            f = forme.mid(0,forme.size() - 3);
            if (f == "qui") f = "quo";
            _mapLem = _lemCore->lemmatiseM(f, (rang == 0) || debVers);
            // Je lemmatise la forme comme si de rien n'était.
            // Mais je ne dois garder que les ablatifs...
            // des pronoms personnels et du seul qui (relatif i.e. qui2).
            QList<Lemme*> listLems = _mapLem.keys();
            if (listLems.size() == 0) qDebug() << f << tt;
            Lemme * l = listLems[0];
            if (listLems.size() > 1)
                foreach (Lemme *lem, listLems)
                    if ((lem->cle() == "qui2") || (lem->cle() == "se"))
                        l = lem;
            // l est donc le bon pointeur.
            QString lem = l->humain(true, _lemCore->cible(), true);
            int nb = l->nbOcc();
            // Les ablatifs ont un numéro de morpho qui est 6, 12, 18 etc...
            foreach (SLem m, _mapLem.value(l)) if (m.morpho % 6 == 0)
            {
/*<<<<<<< HEAD
 * Pas sûr de comprendre ce conflit.
                m.sufq = _enclitique;
                if (f == "qui") m.grq = "quī";
                // Je rétablis la graphie "qui".
                // Une question ouverte est de savoir si cet ancien ablatif "qui"
                // peut être féminin. Yves dixit (3 janvier 2018) :
                // j'ai cru voir un féminin chez Virgile, En. XI, 820.
                // Si c'est vrai, il faudrait ajouter cette analyse.
                _morphos.append(_lemCore->morpho(m.morpho));
                if (_morphos.last().contains(" plur")) tt = "p62";
                // secum peut être singulier ou pluriel.
                long fr = nb * _lemCore->fraction(tt);
=======*/
                QString lt = _lemCore->tag(l, m.morpho); // Maintenant, c'est une liste de tags.
//                qDebug() << lem << lt;
                // Pour les analyses, je garde la liste de tags.
                long fr = nb * _lemCore->fraction(lt);
//>>>>>>> Medieval
                _lemmes.append(lem);
                _tags.append(tt);
                _nbOcc.append(fr);
                _sLems.append(m);
                _formes.append(m.grq + " + cŭm");
                _probas[tt] += fr;
            }
        }
        else if (forme.endsWith("."))
        {
            // C'est une abréviation : un nom à n'importe quel cas !
            _probas.clear();
            QString pseudo = "n%1";
            QString t;
            for (int i = 1; i < 7; i++)
            {
                t = pseudo.arg(i)+"1";
                _morphos.append(_lemCore->morpho(i));
                _lemmes.append("<strong>" + forme + "</strong> : abréviation.");
                _nbOcc.append(1);
    //            _sLems.append(m); // Je ne sais pas quoi mettre comme SLem
                _formes.append(forme);
                _tags.append(t);
                _probas[t] = _lemCore->tagOcc(t);
            }
        }
        else
        {
            _mapLem = _lemCore->lemmatiseM(forme, (rang == 0) || debVers);
            // échecs
            if (_mapLem.empty())
            {
                // Je ne sais pas encore quoi faire.
            }
            else foreach (Lemme *l, _mapLem.keys())
            {
                QString lem = l->humain(true, _lemCore->cible(), true);
                int nb = l->nbOcc();
                foreach (SLem m, _mapLem.value(l))
                {
                    QString lt = _lemCore->tag(l, m.morpho); // Maintenant, c'est une liste de tags.
                    // Pour les analyses, je garde la liste de tags.
                    long fr = nb * _lemCore->fraction(lt);
                    _lemmes.append(lem);
                    _tags.append(lt);
                    _nbOcc.append(fr);
                    _sLems.append(m);
//                    qDebug() << forme << lem << nb << lt << fr;
                    /*                if (m.sufq.isEmpty())
                {
                    if (m.morpho == 416) _morphos.append(m.grq);
                    else _morphos.append(m.grq + " " + _lemCore->morpho(m.morpho));
                }
                else
                {
                    if (m.morpho == 416) _morphos.append(m.grq + " + " + m.sufq);
                    else _morphos.append(m.grq + " + " + m.sufq + " " + _lemCore->morpho(m.morpho));
                    enclitique = m.sufq;
                }
                */
                    if (m.sufq.isEmpty()) _formes.append(m.grq);
                    else
                    {
                        _formes.append(m.grq + " + " + m.sufq);
                        _enclitique = m.sufq;
                    }
                    if (m.morpho == 416) _morphos.append("");
                    else _morphos.append(_lemCore->morpho(m.morpho));

                    while (lt.size() > 2)
                    {
                        QString t = lt.mid(0,3);
                        lt = lt.mid(4);
                        fr = nb * _lemCore->fraction(t);
                        _probas[t] += fr;
                    }
                }
            }
        }
//<<<<<<< HEAD
//=======
//        if (Ch::abrev.contains(forme))
        if (_lemCore->estAbr(forme))
        {
            // C'est un nom à n'importe quel cas !
            _probas.clear();
//            _tags.clear();
            QString pseudo = "n%1";
            for (int i = 1; i < 7; i++)
            {
                QString t = pseudo.arg(i)+"1";
//                _tags.append(t);
                _probas[t] = _lemCore->tagOcc(t);
            }
        }
//>>>>>>> Medieval
        // J'ai construit les listes de lemmes, morphos, tags et nombres d'occurrences.
        // J'ai aussi une QMap qui associe les tags aux probas, que je dois normaliser.
        long total = 0;
        foreach (QString t, _probas.keys()) total += _probas[t];
        if (total == 0)
        {
            total = 1;
            //qDebug() << forme << " : toutes les probas sont nulles !";
        }
        _maxProb = "";
        long prMax = -1;
        foreach (QString t, _probas.keys())
        {
            _bestOf[t] = 0.; // Je prépare une liste des tags
//            qDebug() << t << _probas[t];
            long pr = _probas[t] * 1024 /total;
            if (prMax < pr)
            {
                prMax = pr;
                _maxProb = t;
            }
            if (pr == 0) pr++;
            _probas[t] = pr;
        }

        if ((_enclitique == "quĕ") || (_enclitique == "vĕ")) _tagEncl = "ce ";
        else if (_enclitique == "nĕ") _tagEncl = "de ";
        else if (_enclitique == "st") _tagEncl = "v11";

/*        if (forme.endsWith("cum"))
        {
            bool encl = (forme == "mecum") || (forme == "tecum") || (forme == "secum");
            encl = encl || (forme == "quacum") || (forme == "quocum") || (forme == "quicum");
            bool pl = (forme == "nobiscum") || (forme == "vobiscum") || (forme == "quibuscum");
            // Ce sont des pluriels
            if (encl || pl)
            {
//                qDebug() << forme << " avec enclitique";
                _tagEncl = "re ";
                _enclitique = "cum";
                QString tt = "p61";
                if (pl) tt = "p62";
                if (_tags.isEmpty())
                {
                    _tags.append(tt);
                    _probas.insert(tt,1024);
                    _lemmes.append(forme);
                    _formes.append(forme.mid(0,-3));
                    _morphos.append("...");
                    _nbOcc.append(1);
                    _bestOf[tt] = 0.;
                }
                else if (_probas.size() == 1)
                {
                    if (!_probas.keys().contains(tt))
                    {
                        _tags[0]=tt;
                        _probas.clear();
                        _probas.insert(tt,1024);
                        _bestOf[tt] = 0.;
                    }
                }
                else qDebug() << "Erreur sur " << forme << " : " << _tags;
            }
        } */
    }
//    qDebug() << forme << _tags.size() << _tags;
}

QString Mot::choisir(QString t, int np, bool tout)
{
//    qDebug() << _forme << t << np << tout << _tags.isEmpty() << _tags.size();
    QString choix = "";
    int valeur = -1;
    long v1 = -1;
    double v2 = -1;
    for (int i=0; i < _tags.size(); i++)
    {
        if ((_tags[i].contains(t)) && (valeur < _nbOcc[i]))
        {
            // _tags peut être une liste de tags, alors que t est un tag.
            choix = _lemmes[i] + " — " + _formes[i] + " " + _morphos[i];
            valeur = _nbOcc[i];
            _choix = i;
        }
        if (v1 < (_nbOcc[i] * _probas[_tags[i]]))
        {
            v1 = (_nbOcc[i] * _probas[_tags[i]]);
            _maxHC = i;
        }
        if (v2 < (_nbOcc[i] * _bestOf[_tags[i]]))
        {
            v2 = (_nbOcc[i] * _bestOf[_tags[i]]);
            _maxEC = i;
        }
        // Je profite de l'occasion pour déterminer les 3 meilleurs indices.
    }
    if (!choix.isEmpty())
    {
        choix.prepend("<br/>\n—&gt;&nbsp;<span style='color:black'>");
        choix.append("</span>\n");
    }
    if (tout || choix.isEmpty())
    {
        choix.append("<span style='color:#777777'><ul>\n");
        double total = 0.0;
        for (int i=0; i < _tags.size(); i++)
        {
            QString lt = _tags[i];
            while (lt.size() > 2)
            {
                QString t = lt.mid(0,3);
                lt = lt.mid(4);
                total += _bestOf[t];
            }
        }
        if (total == 0.0) total = 1.0;
        for (int i=0; i < _tags.size(); i++)
        {
            QString format = "%1 : %2 HC %3 ; ";
            QString lg = "<li>" + _lemmes[i] + " — " + _formes[i] + " " + _morphos[i] + " (";
            QString lt = _tags[i];
//            qDebug() << lg << lt;
            if (lt.size() > 2)
            {
/*<<<<<<< HEAD
                QString t = lt.mid(0,3);
                lt = lt.mid(4);
                lg.append(format.arg(t).arg(_bestOf[t]/total).arg(1.0*_probas[t]/1024.0));
                // Je donne les probas en contexte (_bestOf) et hors-contexte (_probas).
=======*/
                while (lt.size() > 2)
                {
                    QString t = lt.mid(0,3);
                    lt = lt.mid(4);
                    lg.append(format.arg(t).arg(_bestOf[t]/total).arg(1.0*_probas[t]/1024.0));
//                    lg.append(format.arg(t).arg(_bestOf[t]));
                }
                lg.chop(3);
                lg.append(")</li>\n");
//>>>>>>> Medieval
            }
            else lg.append(" ? )</li>\n");
            choix.append(lg);
        }
        choix.append("</ul></span>\n");
    }
    QString ajout;
    if (t == _maxProb) ajout = t;
    else ajout = t + " (" + _maxProb + ")";
    QString debut = "<li id='S_%1_W_%2'><strong>";
    choix.prepend(debut.arg(np).arg(_rang) + _forme + "</strong> " + ajout);
    choix.append("</li>");
    return choix;
}

QString Mot::tagEncl()
{
    return _tagEncl;
}

bool Mot::inconnu()
{
    return _tags.isEmpty();
}

QStringList Mot::tags()
{
    QStringList ret = _probas.keys();
    return ret;
}

long Mot::proba(QString t)
{
    if (_probas.contains(t))
        return _probas[t];
    return 0;
}

QString Mot::forme()
{
    return _forme;
}

/**
 * @brief Mot::setBestOf retient la meilleure probabilité
 * pr associée au tag t.
 * @param t : un tag
 * @param pr : une probabilité
 *
 * Lorsque je tague la phrase, au moment de traiter le mot i,
 * j'ai encore tous les tags possibles pour le mot i-2.
 * À chaque trigramme terminal, j'ai associé une probabilité.
 * Avant d'élaguer l'arbre, j'appelle cette routine setBestOf
 * en lui passant le tag associé au mot i-2 (celui-ci) et
 * la proba associée à une séquence le contenant.
 * Parmi toutes les probas reçues, pour un tag donné,
 * je conserve la plus grande.
 */
void Mot::setBestOf(QString t, double pr)
{
//    qDebug() << t << pr;
    if (_bestOf.keys().contains(t))
    {
        if (pr > _bestOf[t]) _bestOf[t] = pr;
    }
    else qDebug() << "tag non trouvé pour" << _forme << t << pr;
        // _bestOf[t] = pr;
}

double Mot::bestOf(QString t)
{
    if (_bestOf.keys().contains(t)) return _bestOf[t];
    else return 0;
}

QString Mot::formeq(int i)
{
    return _formes[i];
}

QString Mot::lemme(int i)
{
    return _lemmes[i];
}

QString Mot::morpho(int i)
{
    return _morphos[i];
}

SLem Mot::sLem(int i)
{
    return _sLems[i];
}

QString Mot::enclitique()
{
    return _enclitique;
}

int Mot::rang()
{
    return _rang;
}

int Mot::choix()
{
    return _choix;
}

int Mot::maxEC()
{
    return _maxEC;
}

int Mot::maxHC()
{
    return _maxHC;
}

int Mot::nbAnalyses()
{
    return _sLems.size();
}
