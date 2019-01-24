/*           reglevg.h       */
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

#ifndef REGLEVG_H
#define REGLEVG_H

#include <QObject>
#include <QRegExp>

class RegleVG : public QObject
{
    Q_OBJECT

    private:
        QRegExp _a;
        QString _b;
        bool    _ci;
    public:
        RegleVG(QString l);
        QRegExp a();
        QString b();
        bool    ci();
        QString doc();
        QString transf(QString s);
};

# endif
