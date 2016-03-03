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

#include "lemmatiseur.h"

class ElS: public QObject
{
	Q_OBJECT

	private:
		QStringList    _lemmes;
		QStringList    _pos;
		QStringList    _morphos;
};

class RegleS: public QObject
{
	Q_OBJECT

	private:
		QString        _accord;
		QString        _id;
		RegleS        *_pere;
		QString        _sens;
		ElS           *_super;
		ElS           *_sub;
		QString        _traduction;
	public:
		RegleS (QStringList lignes);
};

class Syntaxe: public QObject
{

	Q_OBJECT

	private:
		QList<RegleS*> _regles;
	public:
		Syntaxe ();
};

#endif
