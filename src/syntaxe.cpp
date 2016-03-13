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

QString Mot::gr()
{
	return _gr;
}

QString Mot::humain()
{
	QString ret;
	QTextStream fl(&ret);
	fl << _gr << "\n";
	foreach (Lemme *lem, _morphos.keys())
		fl << lem->grq() << "    - "<<lem->traduction("fr")<<"\n";
	return ret;
}

QChar Mot::ponctD()
{
	return _ponctD;
}

QChar Mot::ponctG()
{
	return _ponctG;
}

void Mot::setMorphos(MapLem m)
{
	_morphos = m;
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
	const QList<QChar> chl;
	const int tl = t.length()-1;
	const QString pp = ".;!?";
	_motsP.clear();
	_motsS.clear();
	// avancer jusqu'à la fin du mot sous le curseur
	while (p<tl-1 && t.at(p+1).isLetter()) ++p;
	bool limite=false;
	QString m;
	QChar ponctD = '\0';
	QChar ponctG = '\0';
	// mots à gauche de motCour
	int i = p;
	while (i>-1 && !limite)
	{
		QChar c = t.at(i);
		if (c.isLetter()) m.prepend (c);
		else if (!m.isEmpty())
		{
			Mot *nm = new Mot(m);
			nm->setMorphos(_lemmatiseur->lemmatiseM(m));
			_motsP << nm;
			m.clear();
			nm->setPonctG(ponctG);
			nm->setPonctD(ponctD);
			ponctG = ponctD;
			ponctD = '\0';
		}
		limite = (pp.contains(c) 
				  || (i>0 && c=='\n' && t.at(i-1)=='\n'));
		if (!limite && c.isPunct())
			ponctD = c;
		--i;
	}
	// mots à droite de motCour.
	limite = false;
	i = p+1;
	while (i<tl && !limite)
	{
		QChar c = t.at(i);
		if (c.isLetter()) m.append (c);
		else if (!m.isEmpty())
		{
			Mot *nm = new Mot(m);
			_motsS << nm;
			m.clear();
			nm->setPonctD(ponctG);
			nm->setPonctG(ponctD);
			ponctD = ponctG;
			ponctG = '\0';
		}
		limite = (pp.contains(c)
				  || (i<tl-1 && c=='\n' && t.at(i+1)=='\n'));
		if (!limite && c.isPunct())
			ponctG = c;
		++i;
	}
	// debog
	QStringList ret;
	foreach (Mot *mot, _motsP)
		ret << mot->humain();
	ret << "----------";
	foreach (Mot *mot, _motsS)
		ret << mot->humain();
	return ret.join (' ');
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
