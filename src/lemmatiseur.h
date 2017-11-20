/*      lemmatiseur.h
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

#ifndef LEMMATISEUR_H
#define LEMMATISEUR_H

#include <QObject>
#include <QHash>

#include <QDebug>

#include "ch.h"
#include "lemCore.h"

class Lemmatiseur : public QObject
{
//    Q_OBJECT
public:
    Lemmatiseur(QObject *parent = 0, LemCore *l=0, QString resDir="");
    // Créateur de la classe
    QStringList frequences(QString txt);
    QStringList lemmatiseF(QString f, bool deb);
    QString lemmatiseFichier(QString f, bool alpha = false,
                             bool cumVocibus = false, bool cumMorpho = false,
                             bool nreconnu = true);
    // lemmatiseT lemmatise un texte
    QString lemmatiseT(QString &t);
    QString lemmatiseT(QString &t, bool alpha, bool cumVocibus = false,
                       bool cumMorpho = false, bool nreconnu = false);

    void verbaOut(QString fichier); // Connaître l'usage des mots connus
    void verbaCognita(QString fichier, bool vb=false); // Coloriser le texte avec les mots connus

private:
    LemCore * _lemCore;
    QString _resDir;
    QHash<QString,int> _hLem;
    QStringList _couleurs;
};

#endif // LEMMATISEUR_H
