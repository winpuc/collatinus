/*               tagueur.h
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

#ifndef TAGUEUR_H
#define TAGUEUR_H

#include "lemCore.h"
#include "mot.h"
#include "reglesynt.h"
#include "ch.h"

typedef QList<qint64> Arbre;
// Un lien étant repéré par un entier 64 bits,
// un arbre est une liste de liens, donc de qint64.
// Jusqu'à présent, j'utilisais des quint64 (non signés)
// mais je voudrais avoir des liens négatifs,
// pour repérer facilement ceux qui ne respectent pas la projectivité.
// Je définis un type, car j'aurai besoin de liste d'arbres.
class Tagueur : public QObject
{
public:
    Tagueur(QObject *parent = 0, LemCore *l=0, QString cible = "", QString resDir="");
    // Pour le tagger

    void analyse();
    int nbArbres();
    QString sauvArbre(int i, bool ordre = true);
    QString tagTexte(QString t, int p, bool affTout = true, bool majPert = true, bool affHTML = true);

private:
    LemCore * _lemCore;
    QString _resDir;
    QString _phrase;
    bool _interrogative;
    QList<Mot*> _mots;
    QList<Mot*> _tokens;
    QList<RegleSynt*> _regles; // La liste des règles syntaxiques.
    QMap<QString,int> _idRegle; // Pour les retrouver avec l'id.
    void lireRegles();
    void effacer();
    QList<qint64> _liensLocaux; // L'ensemble des liens aboutissant sur un mot
    QList<qint64> _listLiens; // L'ensemble des liens possibles
    QList<Arbre> _foret; // L'ensemble des arbres trouvés.
    QList<Arbre> _foret2; // L'ensemble des arbres avec plus d'un orphelin.
    void defMask();
    qint64 iafMult; // Le multiplicateur, une puissance de 2
    qint64 itfMask; // Le masque = mult - 1
    qint64 iafMask;
    qint64 itpMask;
    qint64 iapMask;
    qint64 itpMult;
    qint64 iapMult;
    qint64 irMult;
    void ajToken(int i); // Le mot i a un enclitique...
    void cherchePere(int ir, int itf, int iaf);
    void trouvePere(int ir, int itf, int iaf, int itp);
    bool liensEncl(int it, int ir);  // Pour choisir les liens pour l'enclitique.
    void trierLiens(); // Pour ajouter les liens locaux en les rangeant.
    int eval(qint64 lien); // Pour évaluer un lien.
    int _maxOrph;
    bool estAntecedent(qint64 lien);
    QTime _temps;
    qint64 genLien(int iRegle, int iTokPere, int iAnPere, int iTokFils, int iAnFils);
    // Pour combiner dans un entier 64bits, les cinq indices qui definissent un lien.
    int iRegle(qint64 lien);
    int iTokPere(qint64 lien);
    int iAnPere(qint64 lien);
    int iTokFils(qint64 lien);
    int iAnFils(qint64 lien);
    bool accordOK(QString ma, QString mb, QString cgn);
    void faireCroitre(QList<qint64> lLiens, Arbre pousse, QList<int> indices, int nbRel);
    QList<qint64> liensComp(QList<qint64> ailleurs, Arbre nvlPouss, qint64 lienChoisi, bool coord = false);
    bool pasBoucle(Arbre a, qint64 l1);
    QList<int> ancetres(int itf, Arbre a);
    bool accCoord(qint64 lienCC, qint64 lienC);

};

#endif // TAGUEUR_H
