/*    maj.h   */

#ifndef MAJ_H
#define MAJ_H

#include <QDialog>
#include <QStringList>

class Maj: public QDialog
{

	Q_OBJECT

	private:
		void        installe (QString nfc);   // décompresse nfc et l'installe dans data/dicos/
		QStringList selectionne ();           // lance un dialogue de sélection de fichier
	public:
		Maj (QDialog *parent=0);
};

#endif
