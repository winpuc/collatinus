/*       maj.cpp      */

# include <QApplication>
# include <QFileDialog>
# include <QDir>
# include <QPushButton>
# include <QVBoxLayout>

# include "maj.h"

# include <QDebug>

Maj::Maj (QDialog *parent): QDialog (parent)
{
    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/res/collatinus.ico"));
	// label d'information
	QString texte = tr 
		("Sur Collatinus peuvent se greffer une collection "
		 "de lexiques et de dictionnaires. À l'installation, "
		 "on ne dispose que d'une partie de ces ressources. "
		 "Pour en ajouter, il faut se rendre sur le site "
		 "(<em>ici, donner le site</em>), "
		 "consulter la liste des fichiers disponibles et leur "
		 "version, et les télécharger en notant bien l'endroit "
		 "où on les enregistre.<br/>\n"
		 "Lorsque ce sera fait, il faudra aller les chercher en "
		 "cliquant sur le bouton <em>Installer les paquets téléchargés</em> "
		 "ci-dessous.<br/>\n"
		"Il est conseillé de revenir régulièrement sur "
		 "(<em>encore le site</em>) "
		 "pour vérifier que l'on possède la dernière version "
		 "des lexiques et dictionnaires. Voici la liste de "
		 "ce qui est installé sur cet ordinateur. Les versions sont "
		 "à la fin de chaque ligne, après le dernier tiret. :\n<ul>\n<li>"
		 );
	// liste des lexiques et dictionnaires + version
	label = new QLabel (this);
	label->setFont(this->font());
	label->setWordWrap(true);
	label->setAlignment(Qt::AlignJustify);
	// liste des paquets installés
    QDir chDicos (qApp->applicationDirPath()+"/data/dicos");
    QStringList lcfg = chDicos.entryList (QStringList () << "*.cfg");
	for (int i=0;i<lcfg.count();++i)
		lcfg[i].remove(".cfg");
	texte.append(lcfg.join("</li>\n<li>"));
	texte.append("</li>\n</ul>");
	label->setText(texte);

	// barre de boutons
    QPushButton *installButton = new QPushButton("Installer les paquets téléchargés");
    QPushButton *cloreButton   = new QPushButton("Fermer");
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(installButton);
    bottomLayout->addWidget(cloreButton);

    QVBoxLayout *verticalLayout = new QVBoxLayout (this);
	verticalLayout->addWidget (icon);
	verticalLayout->addWidget (label);
    verticalLayout->addLayout(bottomLayout);

	// Connexions
	connect (installButton, SIGNAL(clicked()), this, SLOT(selectionne()));
	connect (cloreButton, SIGNAL(clicked()), this, SLOT(close()));
}

QStringList Maj::selectionne ()
{
	QStringList nfichiers = QFileDialog::getOpenFileNames
		(this, "Sélectionner un ou plusieurs paquets",
		 QDir::homePath(), "paquets dictionnaires (*.col)");
	return nfichiers;
}

void Maj::setFont(QFont font)
{
	label->setFont(font);
}
