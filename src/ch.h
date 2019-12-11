/*       ch.h     */
/*  This file is part of COLLATINUS.
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
 * © Yves Ouvrard, 2009 - 2019
 */

#ifndef CH_H
#define CH_H

#include <QRegExp>
#include <QString>
#include <QStringList>

namespace Ch
{
	QString         accentue(QString l);
	QStringList     ajoute(QString mot, QStringList liste);
	QString         ajoutSuff(QString fq, QString suffixe, QString l_etym, int accent);
	void            allonge(QString *f);
	QString         atone(const QString a, bool bdc = false);
	QString         chemin(QString f, char t);
	QString         communes(QString g);
	QString         deAccent(QString c);
	void            deQuant(QString *c);
	QChar           der(QString s);
	QString const   consonnes = "bcdfgjklmnpqrstvxz";
	QString         deramise(QString r);
	void            elide(QString *mp);
	bool            inv_sort_i(const QString &a, const QString &b);
	void            genStrNum(const QString s, QString *ch, int *n);
	const QRegExp   reAlphas("(\\w+)");
	const QRegExp   reEspace("\\s+");
	const QRegExp   reLettres("\\w");
	const QRegExp   rePonct("([\\.?!;:])");
	QChar const     separSyll = 0x00B7;
	bool            sort_i(const QString &a, const QString &b);
	QString         transforme(QString k);
	QString         versPC(QString k);
	QString         versPedeCerto(QString k);
	QString const   voyelles = "āăēĕīĭōŏūŭȳўĀĂĒĔĪĬŌŎŪŬȲЎ";
}
#endif
