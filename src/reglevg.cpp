/*           reglevg.cpp       */
/*
 *  This file is part of PRAELECTOR
 *
 *  PRAELECTOR is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  PRAELECTOR is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PRAELECTOR; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * © Yves Ouvrard, 2009 - 2019
 */

#include "reglevg.h"
#include <QDebug>


RegleVG::RegleVG(QString l)
{
    QChar sep = ';';
    if (l.contains('>')) sep = '>';
    _a = QRegExp(l.section(sep,0,0));
    _b = l.section(sep,1,1);
    _ci = sep == '>';
}

/*
 * \fn QRegExp RegleVG::a()
 * \brief expression régulière permettant de détecter
 *        les chaînes à transformer dans le radical ou
 *        la désinence.
 */
QRegExp RegleVG::a()
{
    return _a;
}

/*
 * \fn QString RegleVG::b()
 * \brief renvoie la chaîne transformée par _a
 */
QString RegleVG::b()
{
    return _b;
}

/*
	\fn bool RegleVG::ci()
	\brief renvoie true si la règle est une règle aval (séparateur '>') 
 */
bool RegleVG::ci()
{
    return _ci;
}

QString RegleVG::doc()
{
    return QString("%1 -> %2")
        .arg (_a.pattern())
        .arg (_b);
}

/*
 * \fn QString RegleVG::transf(QString s)
 * \brief transformation de s selon la règle graphique
 */
QString RegleVG::transf(QString s)
{
    return s.replace(_a, _b);
}
