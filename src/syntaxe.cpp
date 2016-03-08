/*         syntaxe.cpp      */

#include <QFile>
#include <QRegExp>
#include "syntaxe.h"

ElS::ElS(QString lin, RegleS *parent)
{
	_regle = parent;	
	QStringList ecl = lin.split(':', QString::KeepEmptyParts);
	_pos    = ecl.at(1).split(',');
	_lemmes = ecl.at(2).split(',');
	_morphos = ecl.at(3).split(' ');
}

RegleS::RegleS(QStringList lignes)
{
	QStringList cles = QStringList()
		<<"id"<<"doc"<<"pere"<<"super"<<"sub"
	//     0     1       2       3       4
		<<"sens"<<"accord"<<"tr";
	//      5        6        7
	foreach (QString lin, lignes)
	{
		QStringList ecl = lin.split(':');
		int i = cles.indexOf(ecl.at(0));
		switch (i)
		{
			case 0: _id     = ecl.at(1);break;
			case 1: _doc    = ecl.at(1);break;
			case 2: _idPere = ecl.at(1);break;
			case 3: _super  = new ElS(lin, this);
			case 4: _sub    = new ElS(lin, this);
			case 5: _sens   = ecl.at(1);break;
			case 6: _accord = ecl.at(1);break;
			case 7: _tr     = ecl.at(1);break;
			default:break;
		}
	}
}

/**
 * \fn Mot::Mot(QString g)
 * \brief Créateur de la classe Mot.
 */

Mot::Mot(QString g)
{
	_gr = g;
}

void Mot::setPonctD(QChar p)
{
	_ponctD = p;
}

void Mot::setPonctG(QChar p)
{
	_ponctG = p;
}

Syntaxe::Syntaxe(QString t, Lemmat *parent)
{
	//_lemmatiseur = qobject_cast<Lemmat*>(parent);
	_lemmatiseur = parent;
	setText (t);
	// lecture des données
	QFile fs (qApp->applicationDirPath () +"/data/syntaxe.la");
    fs.open (QFile::ReadOnly);
    QTextStream fls (&fs);
	QStringList slr; // liste des lignes de la dernière règle lue
    while (!fls.atEnd ())
	{
		QString l = fls.readLine ().simplified();
		if ((l.isEmpty() && !fls.atEnd()) || l.startsWith ("!"))
			continue;
		/*
		// variable
		if (l.startsWith ('$'))
		{
		_variables[l.section('=',0,0)]=l.section('=',1,1);
		continue;
		}
		*/
		QStringList eclats = l.split (":");
		if ((eclats.at (0) == "id" || fls.atEnd()) && !slr.empty())
		{
			_regles.append (new RegleS (slr));
			slr.clear();
		}
		else slr.append (l);
	}
    fs.close ();
}

QString Syntaxe::analyse (QString t, int p)
{
	// déterminer les limites de la phrase
	const QString ponct (".;!?");
	int lg = p;
	while (lg > 0 && (!ponct.contains(t.at(lg))) 
		   && (!(t.at(lg) == '\n' && t.at(lg-1) == '\n'))) --lg;
	int ld = p;
	while ((ld < t.length()-1) && (!ponct.contains(t.at(ld)))
		   && (!(t.at(ld) == '\n' && t.at(ld+1) == '\n'))) ++ld;
	++ld;
	QString phr = t.mid(lg, ld-lg);
	_txt = phr.simplified();
	_txt.replace('\n', ' ');
	// mots et séparateurs de mots
	QStringList ecl = _txt.split(QRegExp("\\b"));
	for (int i=1;i<ecl.count();i+=2)
		_mots.append(new Mot(ecl.at(i)));
	for (int i=0;i<ecl.count();i+=2)
	{
		QString sep = ecl.at(i);
		for (int is=0;is<sep.length();++is)
			if (!sep.at(is).isSpace())
			{
				if (is>0) _mots.at(is-1)->setPonctD (sep.at(is));
				if (is<sep.length()) _mots.at(is+1)->setPonctG(sep.at(is));
			}
	}
	return motCourant->gr();
}

QString Syntaxe::motSous(int p)
{
	QString mot;
	// chercher la limite de mot vers la droite
	while (p < _texte.length() && _texte.at(p).isLetter()) ++p;
	// remonter d'un caractère
	--p;
	while (p > 0 && _texte.at(p).isLetter())
	{
		mot.prepend(_texte.at(p));
		--p;
	}
	return mot;
}

void Syntaxe::setText (QString t)
{
	_texte = t;
}
