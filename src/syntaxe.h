/*           syntaxe.h        */

/*
   Module d'analyse syntaxique.







*/

#ifndef SYNTAXE_H
#define SYNTAXE_H

#include "lemmatiseur.h"

class RegleS: public QObject
{
	Q_OBJECT

	private:
		QString     _id;
		QStringList _lemmesF;
		QStringList _lemmesP;
		QStringList _morphosF;
		QStringList _morphosP;
		QString     _traduction;
	public:
		RegleS (QString lin);
};

class Syntaxe: public QObject
{

	Q_OBJECT

	private:
		QList<RegleS*> regles;
};

#endif
