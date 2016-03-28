/*           syntaxe.h        */

/*
   Module d'analyse syntaxique.

Une règle a deux membres : père et fils
Une expression peut avoir trois membres ou plus. Exemple :
nominatif + datif + sum
On résout en posant une règle supplémentaire

sumDatif:
- fils datif
- sum père d'un sujet au nominatif.

Algo : toujours envisager le mot cliqué comme sub. Recherche
vers la gauche, puis vers la droite.

- si on rencontre le super immédiatement à gauche ou à droite,
  on peut renvoyer le résultat.
- si l'élément n'est pas le super, on passe la main à cet 
  élément, qui doit chercher un père à son tour.
- si le père est trouvé, on considère l'élément étranger comme
  résolu, et on reprend la recherche précédente.

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
		QString        _tr;
	public:
		RegleS (QStringList lignes);
		QString         accord();
		QString         doc();
		QString         id();
		QString         fonction(Mot *super=0, Mot *sub=0);
		bool            estSub(Lemme *l, QString morpho, bool ante);
		bool            estSuper(Lemme *l, QString morpho);
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
		Mot           *_motCour;
		QList<Mot*>    _motsP;
		QList<Mot*>    _motsS;
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
