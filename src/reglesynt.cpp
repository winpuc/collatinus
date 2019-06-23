#include "reglesynt.h"

//////////////////////////////
//   Classe ElS             //
//   El(ement de )S(yntaxe) //
//////////////////////////////

/**
 * \fn ElS::ElS(QString lin, RegleS *parent)
 * \brief La classe ElS enregistre les éléments requis
 * des deux mots (super et sub) d'un lien syntaxique
 * Il est donc utilisé deux fois par la classe RegleS.
 * Son créateur s'appuie sur une ligne du fichier
 * bin/data/syntaxe.la.
 */
ElS::ElS(QString lin, QObject *parent) : QObject(parent)
{
    QStringList ecl = lin.split(':', QString::KeepEmptyParts);
/*    if (ecl.at(1) == "g")
        _idSub = ecl.at(2);
    else
    {
        _idSub.clear();
*/
    QStringList lems = ecl.at(1).split(',', QString::SkipEmptyParts);
        // répartir dans _lemmes et _lemmesExclus
        foreach (QString l, lems)
        {
            if (l.startsWith('-'))
                _lemmesExclus.append(l.remove(0,1));
            else _lemmes.append(l);
        }
        _pos = ecl.at(2).toLower().split(',', QString::SkipEmptyParts);
        _transitif = (ecl[2] == "V");
        _morphos = ecl.at(3).split(' ', QString::SkipEmptyParts);
//    }
}

bool ElS::okLem(QString lem)
{
    if (_lemmesExclus.contains(lem))
        return false;
    // les lemmes peuvent être multiples. Prendre le premier.
    // Il faudra penser à prendre le premier pour définir une RegleS.
    lem = lem.section(',',0,0);
    return _lemmes.isEmpty() || _lemmes.contains(lem);
}

bool ElS::okMorpho(QString m)
{
    if (_morphos.isEmpty()) return true;
    QStringList lm = m.split(' ');
    foreach (QString em, _morphos)
    {
        bool ok = false;
        foreach (QString elm, lm)
        {
            if (elm.startsWith(em)) ok = true;
        }
        if (!ok) return false;
    }
    return true;
}

/**
 * @brief ElS::okPos teste la compatibilité des POS.
 * @param p : une chaine donnant les POS possibles du mot à tester
 * @return true si le POS proposé est compatible avec ceux de l'élément de syntaxe.
 *
 * Cette fonction retourne true si l'élément syntaxique ne requiert pas
 * de POS spécifique (_pos vide) ou si le POS p, passé en paramètre,
 * contient l'un des pos requis. Dans tous les autres cas, elle retourne false.
 */
bool ElS::okPos(QString p)
{
    if (_pos.empty()) return true;
    bool ret = false;
    foreach (QString ep, _pos)
    {
        ret = ret || p.contains(ep);
    }
    return ret;
}

/**
 * @brief ElS::transitif
 * @return true si la règle nécessite un verbe transitif.
 *
 * Pour pouvoir établir un lien comme le COD, il faut que
 * le verbe soit transitif. Dans ce cas, le POS est donné par
 * un V majuscule dans la règle (au lieu du v minuscule habituel).
 * Si pour un élément de syntaxe cette fonction retourne true,
 * il faudra vérifier que le verbe employé n'est pas intransitif.
 */
bool ElS::transitif()
{
    return _transitif;
}

QStringList ElS::pos()
{
    return _pos;
}


RegleSynt::RegleSynt(QStringList lignes, LemCore *parent) : QObject(parent)
{
    _lemCore = parent;
    _extra = false; // A priori, une règle ne permet pas l'extraposition.
    foreach (QString lin, lignes)
    {
        QStringList ecl = lin.split(':');
        int i = cles.indexOf(ecl.at(0));
        switch (i)
        {
            case 0: // id
                _id = ecl.at(1);
                break;
            case 1: // doc
                _doc = ecl.at(1);
                break;
            case 2: // pere
                _idPere = ecl.at(1);
                break;
            case 3:  // super
                _super = new ElS(lin, this); break;
            case 4:  // sub
                _sub = new ElS(lin, this); break;
            case 5:  // sens
                _sens = ecl.at(1); break;
            case 6:  // accord
                _accord = ecl.at(1); break;
            case 7:  // traduction
                _tr = ecl.at(1); break;
            case 8:  // fonction
                _f = ecl.at(1); break;
            case 9:  // divers élément syntaxiques
                _synt = ecl.at(1); break;
            case 10: // règles annulées
                _annule = ecl.at(1).split(','); break;
            case 11: // le super doit être super par un lien d'id ecl[1]
                _supsup = ecl.at(1); break;
            case 12: // le super doit être sub ...
                _supsub = ecl.at(1); break;
            case 13: // etc.
                _subsup = ecl.at(1); break;
            case 14:
                _subsub = ecl.at(1); break;
            case 15:
                _trSub = ecl.at(1); break;
            case 16:
                _trSup = ecl.at(1); break;
            case 17:
                _rangTr = ecl.at(1).toInt(); break;
            case 18:
                _debloc = ecl.at(1); break;
        case 19:
            _extra = true; break;
            default:
                break;
        }
    }
    _superSub = (_tr.indexOf("<super>") < _tr.indexOf("<sub>"));
    _pere = 0;
    _rangTr = 0;
}

void RegleSynt::herite(RegleSynt *rp)
{
    if (rp != NULL)
    {
        // Ma règle hérite d'une autre règle après sa création.
        if (_sens == "") _sens   = rp->sens();
        // Si un des éléments héritables a été défini,
        // je ne dois pas l'écraser.
        if (_accord == "") _accord = rp->accord();
        if (_f == "") _f      = rp->f();
        if (_synt == "") _synt   = rp->synt();
        _pere   = rp;
        // annule, doc, sub et super non clonés !
    }
}

QString RegleSynt::accord()
{
    return _accord;
}

/*bool RegleSynt::annule(RegleS *r)
{
    return _annule.contains(r->id());
}
*/
bool RegleSynt::bloquant()
{
    return !_debloc.isEmpty();
}

bool RegleSynt::contigu()
{
    return _synt.contains('c');
}

QString RegleSynt::debloc()
{
    return _debloc;
}

QString RegleSynt::doc()
{
    return _doc;
}

QString RegleSynt::id()
{
    return _id;
}

/*QString RegleSynt::idSub()
{
    return _sub->idSub();
}
*/
bool RegleSynt::estSub(Lemme *l, QString morpho, bool ante)
{
    // sens
    if (ante && _sens == ">") return false;
    if (!ante && _sens == "<") return false;
    // lemme
    if (!_sub->okLem(l->gr())) return false;
    // pos
    if (!_sub->okPos(l->pos())) return false;
    // morpho
    // prénoms
    if (l->pos().contains('n') && _lemCore->estAbr(l->gr()))
        return true;
    // autres cas
    if (!_sub->okMorpho(morpho)) return false;
    return true;
}

bool RegleSynt::estSuper(Lemme *l, QString morpho, bool ante)
{
    if (ante && _sens == "<") return false;
    // lemme
    QString lgr = l->gr().section(',',0,0);
    if (!_super->okLem(lgr)) return false;
    // pos
    if (!_super->okPos(l->pos())) return false;
    // morpho
    if (l->pos().contains('n') && _lemCore->estAbr(l->gr()))
        return true;
    // prénoms
    if (!_super->okMorpho(morpho)) return false;
    return true;
}

/*bool RegleSynt::estUn(RegleS *r)
{
    if (_pere == 0) return false;
    if (_pere == r) return true;
    return _pere->estUn(r);
}
*/
QString RegleSynt::f()
{
    return _f;
}
/*
QString RegleSynt::fonction(Mot *super, Mot *sub)
{
    QString ret = _f;
    if (super != NULL) ret.replace("<super>", "<em>" + super->gr() + "</em>");
    if (sub != NULL) ret.replace("<sub>", "<em>" + sub->gr() + "</em>");
    return ret;
}
*/
QString RegleSynt::idPere()
{
    return _idPere;
}

QString RegleSynt::idFam()
{
    if (_idPere.isEmpty()) return _id;
    else return _idPere;
}

/**
 * \fn bool RegleSynt::multiple()
 * \brief vrai si le superordonné accepte plusieurs
 *     subordonnés via la même règle.
 */
bool RegleSynt::multiple()
{
    return !_synt.contains('u');
}

/**
 * \fn int RegleSynt::rangTr()
 * \brief retourne le codage de la place relative du sub dans le groupe
 *        de super.
 *   0    : le sub n'a pas de position précise. Valeur par défaut;
 *   1    : le sub est en première position
 *   100  : le sub est en dernière position
 */
int RegleSynt::rangTr()
{
    return _rangTr;
}

bool RegleSynt::superSub()
{
    return _superSub;
}

QString RegleSynt::sens()
{
    return _sens;
}

ElS* RegleSynt::sub()
{
    return _sub;
}

ElS* RegleSynt::super()
{
    return _super;
}

QString RegleSynt::synt()
{
    return _synt;
}

bool RegleSynt::accepteFils(SLem sl, QString morpho)
{
    Lemme *l = sl.lem;
    // lemme
    if (!_sub->okLem(l->gr())) return false;
    // pos
    if (!_sub->okPos(l->pos())) return false;
    // morpho
    // prénoms
    if (l->pos().contains('n') && _lemCore->estAbr(l->gr()))
        return true;
    // autres cas
    if (!_sub->okMorpho(morpho)) return false;
    return true;
}

bool RegleSynt::acceptePere(SLem sl, QString morpho)
{
    Lemme *l = sl.lem;
    // lemme
    if (!_super->okLem(l->gr())) return false;
    // pos
    if (!_super->okPos(l->pos())) return false;
    // morpho
    // prénoms
    if (l->pos().contains('n') && _lemCore->estAbr(l->gr()))
        return true;
    // autres cas
    if (!_super->okMorpho(morpho)) return false;
    return true;
}

bool RegleSynt::extra()
{
    return _extra;
}
