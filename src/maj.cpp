/*       maj.cpp      */

# include <QFileDialog>
# include <QDir>

# include "maj.h"

Maj::Maj (QDialog *parent): QDialog (parent)
{
}

QStringList Maj::selectionne ()
{
	QStringList nfichiers = QFileDialog::getOpenFileNames
		(this, "Sélectionner un ou plusieurs paquets",
		 QDir::homePath(), "paquets dictionnaires (*.col)");
	return nfichiers;
}
