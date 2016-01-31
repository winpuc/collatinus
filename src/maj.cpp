/*       maj.cpp      */

# include <QFileDialog>
# include <QDir>
# include <QLabel>
# include <QVBoxLayout>

# include "maj.h"

Maj::Maj (QDialog *parent): QDialog (parent)
{
	// label d'information
	QString texte = tr 
		("Sur Collatinus peuvent se greffer une collection "
		 "de lexiques et de dictionnaires. À l'installation, "
		 "on ne dispose que d'une partie de ces ressources. "
		 "Pour en ajouter, il faut se rendre sur le site "
		 "(<em>ici, donner le site</em>)"
		 ", consulter la liste des fichirs disponibles et leur "
		 "version, et les télécharger en notant bien l'endroit "
		 "où on les enregistre.\n"
		 "Lorsque ce sera fait, il faudra aller les chercher en "
		 "cliquant sur le bouton <em>Installer</em> ci-dessous."
		 "Il est conseillé de revenir régulièrement sur "
		 "(<em>encore le site</em>) "
		 "pour vérifier que l'on possède la dernière version "
		 "des lexiques et dictionnaires. Voici la liste de "
		 "ce qui est installé sur votre ordinateur, et les "
		 "versions correspondantes :"
		 );
	// liste des lexiques et dictionnaires + version
    QVBoxLayout *verticalLayout = new QVBoxLayout (this);
	QLabel *label = new QLabel (this);
	label->setWordWrap(true);
	label->setText(texte);
	verticalLayout->addWidget (label);
}

QStringList Maj::selectionne ()
{
	QStringList nfichiers = QFileDialog::getOpenFileNames
		(this, "Sélectionner un ou plusieurs paquets",
		 QDir::homePath(), "paquets dictionnaires (*.col)");
	return nfichiers;
}
