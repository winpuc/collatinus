#include "lasla.h"

Lasla::Lasla(Lemmat * l, QString resDir)
{
    _lemmatiseur = l;
    if (resDir == "")
        _resDir = qApp->applicationDirPath() + "/data/";
    else if (resDir.endsWith("/")) _resDir = resDir;
    else _resDir = resDir + "/";
    lisCat();
}

void Lasla::lisCat()
{
    QStringList lignes = _lemmatiseur->lignesFichier(_resDir + "CatLASLA.txt");
    foreach (QString lin, lignes)
    {
        if (lin.contains(",")) _catLasla.insert(lin.section(",",0,0),lin.section(",",1,1));
    }
}

QString Lasla::k9(QString m)
{
//    qDebug() << m;
    QStringList res;
    QString cibAct = _lemmatiseur->cible();
    _lemmatiseur->setCible("k9,fr");
    MapLem mm = _lemmatiseur->lemmatiseM(m);
    _lemmatiseur->setCible(cibAct);
    if (mm.isEmpty()) return "\n";
    // Il faut répondre quelque chose, sinon j'attends 30 secondes !
    foreach (Lemme *l, mm.keys())
    {
        QString clef = l->cle() + ", ,";
        foreach (SLem s, mm.value(l))
        {
            QString code9 = s.morpho;
            QString forme = Ch::atone(s.grq);
            if (!s.sufq.isEmpty()) forme += "<" + Ch::atone(s.sufq) +">,";
            else forme += ",";
            if (_catLasla.contains(l->modele()->gr())) code9.replace("k9",_catLasla[l->modele()->gr()]);
//            qDebug() << clef << s.morpho << code9 << _catLasla[l->modele()->gr()];
            res << forme + clef + code9;
        }
    }

    return res.join("\n");
}


