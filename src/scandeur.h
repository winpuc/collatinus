#ifndef SCANDEUR
#define SCANDEUR

#include "lemmatiseur.h"

class Scandeur
{
public:
    Scandeur(Lemmat *l, QString resDir="");
    // Pour scander, un texte.
    QString parPos(QString f);
    QString scandeTxt(QString texte, int accent = 0, bool stats = false, bool majAut = false);

private:
    Lemmat * _lemmatiseur;
    QString _resDir;
    QList<Reglep> _reglesp;
    void lisParPos();
    QStringList cherchePieds(int nbr, QString ligne, int i, bool pentam);
    QStringList formeq(QString forme, bool *nonTrouve, bool debPhr,
                       int accent = 0);
};

#endif // SCANDEUR

