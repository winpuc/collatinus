/*           syntaxe.h        */

/*
   Module d'analyse syntaxique.

- TODO Une règle a deux membres : père et fils
  Une expression peut avoir trois membres ou plus.
  Exemple :
    nominatif + datif + sum
    On résout en posant une règle supplémentaire
    sumDatif:
    - fils datif
    - sum père d'un sujet au nominatif.

- Trouver le moyen d'exprimer la projectivité dans la hiérarchisation des liens
  . si le mot suivant n'est pas lié à motCour
    . chercher s'il est lié au mot précédent.
	  . si oui, continuer
	  . si non chercher s'il est lié au mot suivant.
	    . si oui, on peut continuer à condition que la morpho
		  du mot suivant lui permette d'être lié à motCour.
		  . si non, le groupe est terminé.

 . Exemple de structure sur laquelle travailler : 
   "incidit de uxoribus mentio."  On en vient à parler des épouses.
      | |   ||    |       |
      | +---++----+       |
      +-------------------+

 . Il faudrait que certains mots+lien soient bloquants.  Une
   préposition bloque les expansions à droite du verbe
   jusqu'à ce que son régime soit lu. 
   bloquant = lien prioritaire ? Soit la phrase
   De uxoribus incidit mentio.
   /uxoribus/ pourrait être le sub de /incidit/, mais
   comme il peut être aussi le sub de /de/, on peut
   dire que le lien de -> uxoribus est prioritaire
   sur le lien uxoribus <- incidit. 

 . TODO : rejeter les liens grisés en fin d'affichage.

 . Hypothèse : lorsque un mot a trouvé son super, les
     mots suivants ne peuvent être sub de ce mot.

 .  Solution 1: Analyser toute la phrase avant d'afficher les
    liens du mot cliqué ?
*/

#ifndef SYNTAXE_H
#define SYNTAXE_H

#include <QString>
#include "lemmatiseur.h"
#include "flexfr.h"

class RegleS;

class ElS: public QObject
{
	Q_OBJECT

	private:
		RegleS        *_regle;
		QStringList    _lemmes;
		QStringList    _pos;
		QStringList    _morphos;
	public:
		ElS(QString lin, RegleS *parent);
		bool		   okLem(QString l);
		bool           okPos(QString p);
		bool           okMorpho(QString m);
		QStringList    pos();
};

class Mot;

class RegleS: public QObject
{
	Q_OBJECT

	private:
		QString        _accord;
		QString        _doc;
		QString        _f;
		QString        _id;
		QString        _idPere;
		RegleS        *_pere;
		QString        _sens;
		ElS           *_super;
		ElS           *_sub;
        QString        _synt;
		QString        _tr;
	public:
		RegleS (QStringList lignes);
		QString         accord();
		QString         doc();
		QString         id();
		QString         fonction(Mot *super=0, Mot *sub=0);
		bool            estSub(Lemme *l, QString morpho, bool ante);
		bool            estSuper(Lemme *l, QString morpho);
        bool            bloquant();
		QString         tr();
};

class Super: public QObject
{
	Q_OBJECT

	private:
		RegleS       *_regle;
		Lemme        *_lemme;
		QStringList   _morpho;
		Mot          *_mot;
	public:
		Super(RegleS *r, Lemme *l, QStringList m, Mot *parent);
		bool          estSub(Lemme *l, QString morpho, bool ante);
		Lemme*        lemme();
		QStringList   morpho();
		Mot*          mot();
		RegleS*       regle();
};

class Mot: public QObject
{

	Q_OBJECT

	private:
		QString          _gr;
		MapLem           _morphos;
		QChar            _ponctD;
		QChar            _ponctG;
		QList<RegleS*>   _rSub;
		QList<Super*>    _super;
	public:
		Mot (QString g);
		void          addRSub(RegleS *r);
		void          addSuper(RegleS *r, Lemme *l, QStringList m);
		QString       gr();
		QString       humain();
		MapLem        morphos();   
		QChar         ponctD();
		QChar         ponctG();
		void          setMorphos(MapLem m);
		void          setPonctD(QChar c);
		void          setPonctG(QChar c);
		void          setRSub(QList<RegleS*>);
		void          setRSuper(QList<RegleS*>);
		QList<Super*> super();
};

class Syntaxe: public QObject
{

	Q_OBJECT

	private:
		bool            accord(QString ma, QString mb, QString cgn);
		Lemmat        *_lemmatiseur;
		QList<RegleS*> _regles;
		QString        _texte;
		// variables motCour
		Mot           *_motCour; // mot courant
		QList<Mot*>    _motsP;   // mots précédents
		QList<Mot*>    _motsS;   // mots suivants
        Pronom        *_pronom;
	public:
		Syntaxe (QString t, Lemmat *parent);
		QString analyse(QString t, int p);
		QString motSous(int p);
		void    setText(QString t);
		QString tr(RegleS *r, Lemme *sup, QString msup, Lemme *sub, QString msub);
		QString trLemme (Lemme *l, QString m);
};

#endif
