#ifndef TAGUEUR_H
#define TAGUEUR_H

#include "lemmatiseur.h"
#include "mot.h"

class Tagueur
{
public:
    Tagueur(Lemmat *l, QString resDir="");
    // Pour le tagger
    QString tagTexte(QString t, int p, bool affTout = true, bool majPert = true);

private:
    Lemmat * _lemmatiseur;
    QString _resDir;
};

#endif // TAGUEUR_H
