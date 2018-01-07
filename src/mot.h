/*               mot.h
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

#ifndef MOT_H
#define MOT_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QtCore/QCoreApplication>

#include "lemCore.h"
#include "lemme.h"
#include "ch.h"

class Mot : public QObject
{
    Q_OBJECT
public:
    Mot(QString forme, int rang, bool debVers, QObject *parent = 0);
    QString choisir(QString t = "", int np = 0, bool tout = true);
    long proba(QString t);
    QStringList tags();
    QString forme();
    QString formeq(int i);
    QString lemme(int i);
    QString morpho(int i);
    QString tagEncl();
    SLem sLem(int i);
    bool inconnu();
    void setBestOf(QString t, double pr);
    double bestOf(QString t);

private:
    LemCore* _lemCore;
    QString _forme;
    int _rang;
    QString _tagEncl;
    QString _enclitique;
    MapLem _mapLem;
    QStringList _lemmes;
    QStringList _formes;
    QStringList _morphos;
    QStringList _tags;
    QList<int> _nbOcc; // Nb d'occurrences supposées de la forme ainsi analysée.
    QList<SLem> _sLems;
    // Les listes ci-dessus sont "synchronisées" :
    // tous les éléments de même indice vont ensemble.
    QMap<QString,long> _probas;
    // Il s'agit ici du nb d'occurrences du tag (qui est la clef de la QMap).
    // Que je normalise ensuite pour atteindre un total de 1024.
    QString _maxProb; // Le tag associé à la plus forte proba.
    QMap<QString,double> _bestOf;
    // Il s'agit de la meilleure proba associée au tag (clef de la QMap).
    // Elle est évaluée juste avant l'élagage qui risque de la faire disparaître.
};

#endif // MOT_H
