/*    maj.h   */

#ifndef MAJ_H
#define MAJ_H

#include <QDialog>
#include <QLabel>
#include <QStringList>

class Maj: public QDialog
{

	Q_OBJECT

	private:
		QLabel     *label;
		void        installe (QString nfc);   // décompresse nfc et l'installe dans data/dicos/
	private slots:
		QStringList selectionne ();           // lance un dialogue de sélection de fichier
	public:
		Maj (QDialog *parent=0);
		void setFont (QFont font);
};

#endif
