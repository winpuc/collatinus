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

typedef QList<quint64> Arbre;
// Un lien étant repéré par un entier 64 bits,
// un arbre est une liste de liens, donc de quint64.
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
    QList<quint64> _liensLocaux; // L'ensemble des liens aboutissant sur un mot
    QList<quint64> _listLiens; // L'ensemble des liens possibles
    QList<Arbre> _foret; // L'ensemble des arbre trouvés.
    void defMask();
    quint64 iafMult; // Le multiplicateur, une puissance de 2
    quint64 itfMask; // Le masque = mult - 1
    quint64 iafMask;
    quint64 itpMask;
    quint64 iapMask;
    quint64 itpMult;
    quint64 iapMult;
    quint64 irMult;
    void ajToken(int i); // Le mot i a un enclitique...
    void cherchePere(int ir, int itf, int iaf);
    void trouvePere(int ir, int itf, int iaf, int itp);
    bool liensEncl(int it, int ir);  // Pour choisir les liens pour l'enclitique.
    void trierLiens(); // Pour ajouter les liens locaux en les rangeant.
    int eval(quint64 lien); // Pour évaluer un lien.
    int _maxOrph;
    bool estAntecedent(quint64 lien);
    QTime _temps;
    quint64 genLien(int iRegle, int iTokPere, int iAnPere, int iTokFils, int iAnFils);
    // Pour combiner dans un entier 64bits, les cinq indices qui definissent un lien.
    int iRegle(quint64 lien);
    int iTokPere(quint64 lien);
    int iAnPere(quint64 lien);
    int iTokFils(quint64 lien);
    int iAnFils(quint64 lien);
    bool accordOK(QString ma, QString mb, QString cgn);
    void faireCroitre(QList<quint64> lLiens, Arbre pousse, QList<int> indices, int nbRel);
    QList<quint64> liensComp(QList<quint64> ailleurs, Arbre nvlPouss, quint64 lienChoisi);
    bool pasBoucle(Arbre a, quint64 l1);
    QList<int> ancetres(int itf, Arbre a);
    bool accCoord(quint64 lienCC, quint64 lienC);

};

#endif // TAGUEUR_H
