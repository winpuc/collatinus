#ifndef REGLESYNT_H
#define REGLESYNT_H

#include <QtCore>
#include <QString>
#include "lemme.h"

class ElS : public QObject
{
    Q_OBJECT

    private:
//        RegleS     *_regle;
//        QString     _idSub; // L'élément doit avoir un sub via une règle d'id _idSub.
        QStringList _lemmes;
        QStringList _lemmesExclus;
        QStringList _pos;
        QStringList _morphos;
        bool _transitif;
//        bool        _sup;

    public:
//        ElS(QString lin, RegleS *parent, bool sp);
        ElS(QString lin, QObject *parent);
//        void        debog();
//        QString     idSub();
        bool        okLem(QString l);
        bool        okPos(QString p);
        bool        okMorpho(QString m);
        bool transitif();
//        bool        sup();
        QStringList pos();
};


class RegleSynt : public QObject
{
    Q_OBJECT
public:
    explicit RegleSynt(QStringList lignes, LemCore *parent = 0);
    QString      accord();
//    bool         annule(RegleS *r);
    bool         bloquant();
    bool         contigu();
    QString      debloc();
    QString      doc();
    bool         estSub(Lemme *l, QString morpho, bool ante);
    bool         estSuper(Lemme *l, QString morpho, bool ante);
    bool        extra();
//    bool         estUn(RegleS *r);
//    QString      idSub();
    QString      f();
//    QString      fonction(Mot *super = 0, Mot *sub = 0);
    QString      id();
    QString      idPere();
    QString      idFam(); // Retourne l'idPere si non vide, l'id sinon.
    bool         multiple();
    int          rangTr();
    bool         superSub();
    QString      sens();
    ElS    *     super();
    ElS    *     sub();
    QString      synt();
    void    herite(RegleSynt *rp);
    // Pour ne pas devoir inclure le parent de cette classe,
    // l'héritage se fait après.
    bool accepteFils(SLem sl, QString morpho, QString tag);
    bool acceptePere(SLem sl, QString morpho, QString tag);
    QString supEstSup();
    QString subEstSup();
    QString supEstSub();
    QString subEstSub();
    QStringList annule();

private:
    QStringList const cles = QStringList() << "id"      // 0
                                     << "doc"     // 1
                                     << "pere"    // 2
                                     << "super"   // 3
                                     << "sub"     // 4
                                     << "sens"    // 5
                                     << "accord"  // 6
                                     << "tr"      // 7
                                     << "f"       // 8
                                     << "synt"    // 9
                                     << "nul"     // 10
                                     << "supsup"  // 11
                                     << "supsub"  // 12
                                     << "subsup"  // 13
                                     << "subsub"  // 14
                                     << "trSub"   // 15
                                     << "trSup"   // 16
                                     << "rangTr"  // 17
                                     << "debloc"//; // 18
                                     << "extra";  // 19

    QString     _accord;
    QStringList _annule;
    QString     _debloc;
    QString     _doc;
    bool        _extra;
    QString     _f;
    QString     _id;
    QString     _idPere;
    RegleSynt *_pere;
    int         _rangTr;
    bool        _superSub;
    QString     _sens;
    ElS        *_super;
    ElS        *_sub;
    QString     _supsup;
    QString     _supsub;
    QString     _subsup;
    QString     _subsub;
    QString     _synt;
    QString     _tr;
    QString     _trSub; // traduction imposée du sub
    QString     _trSup; // traduction imposée du super

    LemCore     *_lemCore;

signals:

public slots:
};

#endif // REGLESYNT_H
