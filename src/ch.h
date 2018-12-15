/*       ch.h     */

#ifndef CH_H
#define CH_H

#include <QRegExp>
#include <QString>
#include <QStringList>

namespace Ch
{
/* Je passe à une liste d'abréviations contenue dans un fichier.
QStringList const abrev  = QStringList()
    <<"Agr"<<"Ap"<<"A"<<"K"<<"D"<<"F"<<"C"
    <<"Cn"<<"Kal"<<"L"<<"Mam"<<"M\""<<"M"<<"N"<<"Oct"
    <<"Opet"<<"Post"<<"Pro"<<"P"<<"Q"<<"Sert"
    <<"Ser"<<"Sex"<<"S"<<"St"<<"Ti"<<"T"<<"V"
    <<"Vol"<<"Vop"<<"Pl";
// Liste des abréviations prise dans Praelector le 11/11/2016
*/
QString         accentue(QString l);
QStringList     ajoute(QString mot, QStringList liste);
QString         ajoutSuff(QString fq, QString suffixe, QString l_etym, int accent);
void            allonge(QString *f);
QString         atone(const QString a, bool bdc = false);
QString         chemin(QString f, char t);
QString         communes(QString g);
QString         deAccent(QString c);
void            deQuant(QString *c);
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
