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
};

class RegleS: public QObject
{
	Q_OBJECT

	private:
		QString        _accord;
		QString        _doc;
		QString        _id;
		QString        _idPere;
		RegleS        *_pere;
		QString        _sens;
		ElS           *_super;
		ElS           *_sub;
		QString        _tr;
	public:
		RegleS (QStringList lignes);
		bool            estSub(QString m);
		bool            estSuper(QString m);
};

class Mot: public QObject
{

	Q_OBJECT

	private:
		QChar   _ponctD;
		QChar   _ponctG;
		QString _gr;
	public:
		Mot (QString g);
		QChar ponctD(QChar p);
		QChar ponctG(QChar p);
		void  setPonctD(QChar c);
		void  setPonctG(QChar c);
};

class Syntaxe: public QObject
{

	Q_OBJECT

	private:
		Lemmat        *_lemmatiseur;
		QList<RegleS*> _regles;
		QString        _texte;
		// variables motCour
		QString        _motCour;
		QString        _lemCour;
		QString        _posCour;
		QString        _morphCour;
		QList<Mot*>    _mots;
	public:
		Syntaxe (QString t, Lemmat *parent);
		QString analyse(QString t, int p);
		QString motSous(int p);
		void    setText(QString t);
};

#endif
