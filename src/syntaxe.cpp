/*         syntaxe.cpp      */

#include <QFile>
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

/*
QString Syntaxe::analyse (QString t, int p)
{
	setText (t);
	// se placer à la fin du mot
	while (p < t.length()-1 && t.at(p).isLetter()) ++p;
	QString motCour = motSous(p);
	QStringList ret;
	ret<<"\n-----------------\nRecherche à partir de "+motCour+"\n";
	MapLem maplem = _lemmatiseur->lemmatiseM(motCour, true);
	// pour chaque lemme de motCour
	foreach (Lemme *lem, maplem.keys())
	{
		_lemCour = lem->gr();
		_posCour = lem->pos();
		// pour chaque pos+morpho de motCour
		foreach (SLem sl, maplem.value(lem))
		{
			_morphCour = sl.morpho;
			int pr = p;
			QString motS;
			QChar chp = t.at(pr);
			// recherche régressive
			int incr = -1;
			do
			{
				bool limiteP = false;
				while (chp.isLetter())
				{
					pr += incr;
					chp = t.at(pr);
				}
                // TODO changer l'algo pour extraire la
                // chaîne entre deux mots, et
                // l'analyser globalement
                //  - pour pouvoir tenir compte de
                //    ponctuations != limite de P ;
                //  - pour détecter une limite de
                //    phrase.
                // aller jusqu'à la limite de mot
				while (pr > 0 && pr < t.length()-1 && !t.at(pr+1).isLetter())
				{
					pr += incr;
					chp = t.at(pr);
					// détecter une limite de phrase
					// deux lignes vides = limite de phrase
					if ((pr==0 || pr==t.length())
						|| (QString(".?!;:").contains(chp))
						|| (chp == '\n' && t.at(pr+incr) == '\n'))
						limiteP = true;
				}
				if (!limiteP)
				{
					motS = motSous(pr);
					ret << "nouveau mot "+motS;
					while (t.at(pr+incr).isLetter()) pr += incr;
				}
				else
				{
					ret << "limite de phrase";
					// revenir à la position de motCour
					pr = p+1;
					incr += 2;
				}
			} while (incr < 3);
		}
	}
	return ret.join('\n');
}
*/

QString Syntaxe::analyse (QString t, int p)
{
	setText (t);
	const QString ponct (".;!?");
	// déterminer les limites de la phrase
	int lg = p;
	while (lg > 0 && (!ponct.contains(t.at(lg))) 
		   && (t.at(lg) != '\n' && t.at(lg-1) != '\n')) --lg;
	int ld = p;
	while ((ld < t.length()-1) && (!ponct.contains(t.at(ld)))
		   && (t.at(ld) != '\n' && t.at(ld+1) != '\n')) ++ld;
	QString phr = t.mid(lg, ld-lg);
	phr = phr.simplified();
	phr.replace('\n', ' ');
	return phr;
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
