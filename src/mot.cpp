#include "mot.h"

Mot::Mot(QString forme, int rang, QObject *parent)
{
//    qDebug() << forme;
    _forme = forme;
    _rang = rang;
    _lemmatiseur = qobject_cast<Lemmat *>(parent);
    _probas.clear();
    _tagEncl = "";
    if (forme.isEmpty())
    {
        _tags << "snt";
        _probas["snt"] = 1;
    }
    else
    {
        _mapLem = _lemmatiseur->lemmatiseM(forme, rang == 0);
        QString enclitique = "";
        // échecs
        if (_mapLem.empty())
        {
            // Je ne sais pas encore quoi faire.
        }
        else foreach (Lemme *l, _mapLem.keys())
        {
            QString lem = l->humain(true, _lemmatiseur->cible(), true);
            int nb = l->nbOcc();
            foreach (SLem m, _mapLem.value(l))
            {
                QString t = _lemmatiseur->tag(l, m.morpho);
                long fr = nb * _lemmatiseur->fraction(t);
                _lemmes.append(lem);
                _tags.append(t);
                _nbOcc.append(fr);
                _probas[t] += fr;
//                qDebug() << forme << lem << nb << t << fr;
                if (m.sufq.isEmpty())
                    _morphos.append(m.grq + " " + m.morpho);
                else
                {
                    _morphos.append(m.grq + " + " + m.sufq + " " + m.morpho);
                    enclitique = m.sufq;
                }
            }
        }
        // J'ai construit les listes de lemmes, morphos, tags et nombres d'occurrences.
        // J'ai aussi une QMap qui associe les tags aux probas, que je dois normaliser.
        long total = 0;
        foreach (QString t, _probas.keys()) total += _probas[t];
        if (total == 0)
        {
            total = 1;
            qDebug() << forme << " : toutes les probas sont nulles !";
        }
        foreach (QString t, _probas.keys())
        {
            long pr = _probas[t] * 1024 /total;
            if (pr == 0) pr++;
            _probas[t] = pr;
        }

        if ((enclitique == "quĕ") || (enclitique == "vĕ")) _tagEncl = "ce ";
        else if (enclitique == "nĕ") _tagEncl = "de ";
        else if (enclitique == "st") _tagEncl = "v11";
        if (forme.endsWith("cum"))
        {
            bool encl = (forme == "mecum") || (forme == "tecum") || (forme == "secum");
            encl = encl || (forme == "nobiscum") || (forme == "vobiscum") || (forme == "quibuscum");
            encl = encl || (forme == "quacum") || (forme == "quocum") || (forme == "quicum");
            if (encl)
            {
//                qDebug() << forme << " avec enclitique";
                _tagEncl = "re ";
                if (_tags.isEmpty())
                {
                    _tags.append("p61");
                    _probas.insert("p61",1024);
                    _lemmes.append(forme);
                    _morphos.append("...");
                    _nbOcc.append(1);
                }
                else if (_probas.size() == 1)
                {
                    if (!_probas.keys().contains("p61"))
                    {
                        _tags[0]="p61";
                        _probas.clear();
                        _probas.insert("p61",1024);
                    }
                }
                else qDebug() << "Erreur sur " << forme << " : " << _tags;
            }
        }
    }
//    qDebug() << forme;
}

QString Mot::choisir(QString t, bool tout)
{
    QString choix = "";
    int valeur = -1;
    for (int i=0; i < _tags.size(); i++)
        if ((_tags[i] == t) && (valeur < _nbOcc[i]))
        {
            choix = _lemmes[i] + " — " + _morphos[i];
            valeur = _nbOcc[i];
        }
    if (!choix.isEmpty())
    {
        choix.prepend("<br/>—&gt;&nbsp;<span style='color:black'>");
        choix.append("</span>\n");
    }
    if (tout || choix.isEmpty())
    {
        choix.append("<span style='color:#777777'><ul>");
        for (int i=0; i < _tags.size(); i++)
            choix.append("<li>" + _lemmes[i] + " — " + _morphos[i] + "</li>\n");
        choix.append("</ul></span>\n");
    }
    choix.prepend("<li><strong>" + _forme + "</strong> " + t);
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
