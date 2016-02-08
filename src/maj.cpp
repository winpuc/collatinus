/*       maj.cpp      */

# include <QApplication>
# include <QDir>
# include <QFileDialog>
# include <QFileInfo>
# include <QMessageBox>
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
		 "pour vérifier que l'on possède les dernières versions "
		 "des lexiques et dictionnaires. Voici la liste de "
		 "ce qui est installé sur cet ordinateur. "
		 "Par exemple, le nom\n"
		 "<b>Lewis_and_Short_1879-fev16.cz</b>\n"
		 "signifie que ce dictionnaire a été mis en ligne en février 2016.\n<ul>\n<li>"
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

void Maj::installe (QString nfcol)
{
	// nom du paquet
	QString nom = QFileInfo(nfcol).baseName();
	// fichiers destination
    QString nf (qApp->applicationDirPath()+"/data/dicos/"+nom);
	QString nfcz  = nf+".cz"; //"Lewis_and_Short_1879-fev16.cz";
	QString nfidx = nf+".idx";
	QString nfcfg = nf+".cfg";
	// adresses
	qint64 aidx;
	qint64 acfg;
	qint64 tcfg;
	// ouvertures
	QFile fcol(nfcol);
	fcol.open (QFile::ReadOnly);
	QFile fcz(nfcz);
	if (!fcz.open(QFile::WriteOnly))
	{
		QMessageBox::critical (this, 
			 tr("Collatinus 11"),
			 tr("Impossible d'ouvrir le fichier"
			 	+nfcz.toUtf8()+
				". Vérifiez vos drois d'accès, et éventuellent "
				"connectez-vous en administrateur avant de lancer Collatinus."));
		return;
	}
	QFile fidx(nfidx);
	fidx.open(QFile::WriteOnly);
	QFile fcfg(nfcfg);
	fcfg.open(QFile::WriteOnly);
	// lecture des adresses en queue de fichier
	fcol.seek (fcol.size()-100);
	QString lin;
	while (!lin.startsWith("idx:"))
		lin = fcol.readLine().trimmed();
	aidx = lin.section(':',1,1).toLongLong();
	lin = fcol.readLine().trimmed();
	acfg = lin.section(':',1,1).toLongLong();
	tcfg = lin.section(':',2,2).toLongLong();
	// écriture cz
	fcol.reset();
	fcz.write(fcol.read(aidx));
	// décompression et écriture idx
	fidx.write(qUncompress(fcol.read(acfg-fcol.pos())));
	// décompression et écriture cfg
	fcfg.write(qUncompress(fcol.read(tcfg)));
	// fermetures
	fcol.close();
	fcz.close();
	fidx.close();
	fcfg.close();
}

void Maj::selectionne ()
{
	QStringList nfichiers = QFileDialog::getOpenFileNames
		(this, "Sélectionner un ou plusieurs paquets",
		 QDir::homePath(), "paquets dictionnaires (*.col)");
	listeF = nfichiers;
	if (listeF.empty()) return;
	foreach (QString nfcol, listeF) 
	{
		installe(nfcol);
		qDebug()<<"installé"<<nfcol;
	}
	// info
	QMessageBox::information(this,
			 tr("Collatinus 11"),
			 tr("L'installation s'est bien passée. "
				"Au prochain lancement, les nouveaux lexiques "
				"et dictionnaires seront disponibles."));
}

void Maj::setFont(QFont font)
{
	label->setFont(font);
}
