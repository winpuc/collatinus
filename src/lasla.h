#ifndef LASLA_H
#define LASLA_H

#include "lemmatiseur.h"
#include "lemme.h"
#include "ch.h"


class Lasla
{
public:
    Lasla(Lemmat *l, QString resDir="");
    // Créateur de la classe
    QString k9(QString m);
    // Code en 9 pour le LASLA

private:
    Lemmat * _lemmatiseur;
    QString _resDir;
    QMap<QString,QString> _catLasla;
    // Les correspondances entre les modèles de Collatinus
    // et les catégories du LASLA
    void lisCat();
    // Pour la lecture du fichier.
};

#endif // LASLA_H
