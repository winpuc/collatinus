/*       maj.cpp      */

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "maj.h"

#include <QDebug>

Maj::Maj(bool dic, QDialog *parent) : QDialog(parent)
{
    _dico = dic;
    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/res/collatinus.ico"));
    // label d'information
    QString texte =
        tr("Sur Collatinus peut se greffer une collection "
           "de lexiques et de dictionnaires. À l'installation, "
           "on ne dispose que d'une partie de ces ressources. "
           "Pour en ajouter, il faut se rendre sur le site "
           "(<em>http://outils.biblissima.fr/fr/collatinus/</em>), "
           "consulter la liste des fichiers disponibles et leur "
           "version, et les télécharger en notant bien l'endroit "
           "où on les enregistre.<br/>\n"
           "Lorsque ce sera fait, il faudra aller les chercher en "
           "cliquant sur le bouton <em>Installer les paquets téléchargés</em> "
           "ci-dessous.<br/>\n"
           "Il est conseillé de revenir régulièrement sur "
           "(<em>http://outils.biblissima.fr/fr/collatinus/</em>) "
           "pour vérifier que l'on possède les dernières versions "
           "des lexiques et dictionnaires. Voici la liste de "
           "ce qui est installé sur cet ordinateur. "
           "Par exemple, le nom\n"
           "<b>Lewis_and_Short_1879-fev16.cz</b>\n"
           "signifie que ce dictionnaire a été mis en ligne en février "
           "2016.\n<br>\n<table><tr><td>• ");
    // liste des lexiques et dictionnaires + version
    label = new QLabel(this);
    label->setFont(this->font());
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignJustify);
    // liste des paquets installés
    if (dic)
    {
//        texte.append("<ul>\n<li>");
        QDir chDicos(qApp->applicationDirPath() + "/data/dicos");
        QStringList lcfg = chDicos.entryList(QStringList() << "*.cfg");
        for (int i = 0; i < lcfg.count(); ++i)
        {
            lcfg[i].remove(".cfg");
            if (lcfg[i][lcfg[i].size() - 6] == '-')
            {
                QString date = lcfg[i].section("-",-1);
                lcfg[i] = lcfg[i].section("-",0,-2);
                lcfg[i].append("&nbsp;</td><td>&nbsp;" + date);
            }
        }
        texte.append(lcfg.join("</td></tr>\n<tr><td>• "));
//        texte.append(lcfg.join("</li>\n<li>"));
//        texte.append("</li>\n</ul>");
    }
    else
    {
        // Les lexiques.
//        texte.append("<br>\n<table><tr><td>• ");
        QDir chDicos(qApp->applicationDirPath() + "/data");
        QStringList lcfg = chDicos.entryList(QStringList() << "lem*.*");
        for (int i = 0; i < lcfg.count(); ++i)
        {
            QFile fi(qApp->applicationDirPath() + "/data/" + lcfg[i]);
            fi.open(QFile::ReadOnly|QFile::Text);
            QString blabla = fi.readLine();
            blabla = fi.readLine();
            if (blabla.startsWith("!")) blabla = blabla.mid(1);
            lcfg[i].append("&nbsp;</td><td>&nbsp;" + blabla);
            blabla = fi.readLine();
            lcfg[i].append("&nbsp;</td><td>&nbsp;" + blabla.mid(1));
            fi.close();
        }
        texte.append(lcfg.join("</td></tr>\n<tr><td>• "));
    }
    texte.append("</td></tr></table>");
    label->setText(texte);

    // barre de boutons
    QPushButton *installButton =
        new QPushButton("Installer les paquets téléchargés");
    QPushButton *cloreButton = new QPushButton("Fermer");
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(installButton);
    bottomLayout->addWidget(cloreButton);

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->addWidget(icon);
    verticalLayout->addWidget(label);
    verticalLayout->addLayout(bottomLayout);

    // Connexions
    connect(installButton, SIGNAL(clicked()), this, SLOT(selectionne()));
    connect(cloreButton, SIGNAL(clicked()), this, SLOT(close()));
}

bool Maj::installe(QString nfcol)
{
    if (_dico)
    {
        // ouverture
        QFile fcol(nfcol);
        fcol.open(QFile::ReadOnly);
        // lecture des adresses en queue de fichier
        fcol.seek(fcol.size() - 100);
        QString lin;
        while (!lin.startsWith("idx:") && !fcol.atEnd()) lin = fcol.readLine().trimmed();
        if (!lin.startsWith("idx:"))
        {
            QMessageBox::critical(
                        this, tr("Collatinus 11"),
                        tr("Impossible de comprendre le fichier" + nfcol.toUtf8() +
                           ". Le format semble être inadéquat."));
            return false;
        }
        // nom du paquet
        QString nom = QFileInfo(nfcol).baseName();
        // fichiers destination
        QString nf(qApp->applicationDirPath() + "/data/dicos/" + nom);
        QString nfcz;
        QString nfidx = nf + ".idx";
        QString nfcfg = nf + ".cfg";
        // adresses de l'index
        qint64 aidx = lin.section(':', 1, 1).toLongLong();
        // 2e ligne pour le cfg
        lin = fcol.readLine().trimmed();
        if (lin.section(':',0,0) == "Cfg")
            nfcz = nf + ".djvu";  //"Gaffiot_1934.djvu";
        else nfcz = nf + ".cz";  //"Lewis_and_Short_1879-fev16.cz";
        // J'ai mis une majuscule à cfg pour distinguer les djvu des cz
        qint64 acfg = lin.section(':', 1, 1).toLongLong();
        qint64 tcfg = lin.section(':', 2, 2).toLongLong();
        // Créations
        QFile fcz(nfcz);
        if (!fcz.open(QFile::WriteOnly))
        {
            QMessageBox::critical(
                        this, tr("Collatinus 11"),
                        tr("Impossible de créer le fichier" + nfcz.toUtf8() +
                           ". Vérifiez vos drois d'accès, et éventuellent "
                           "connectez-vous en administrateur avant de lancer Collatinus."));
            return false;
        }
        QFile fidx(nfidx);
        fidx.open(QFile::WriteOnly);
        QFile fcfg(nfcfg);
        fcfg.open(QFile::WriteOnly);
        // écriture cz
        fcol.reset();
        fcz.write(fcol.read(aidx));
        // décompression et écriture idx
        fidx.write(qUncompress(fcol.read(acfg - fcol.pos())));
        // décompression et écriture cfg
        fcfg.write(qUncompress(fcol.read(tcfg)));
        // fermetures
        fcol.close();
        fcz.close();
        fidx.close();
        fcfg.close();
    }
    else
    {
        // installer un lexique
        // nom du paquet
        QString nom = QFileInfo(nfcol).fileName();
        // fichiers destination
        QString nfDest(qApp->applicationDirPath() + "/data/" + nom);
        if (QFile::exists(nfDest))
            QFile::remove(nfDest);
        // On ne peut pas copier si le fichier existe déjà
        QFile::copy(nfcol,nfDest);
    }
    return true;
}

void Maj::selectionne()
{
    if (_dico)
    {
        QStringList nfichiers = QFileDialog::getOpenFileNames(
                    this, "Sélectionner un ou plusieurs paquets", QDir::homePath(),
                    "paquets dictionnaires (*.col)");
        listeF = nfichiers;
    }
    else
    {
        QStringList nfichiers = QFileDialog::getOpenFileNames(
                    this, "Sélectionner un ou plusieurs paquets", QDir::homePath(),
                    "paquets lexiques (*.*)");
        listeF = nfichiers;
    }
    if (listeF.empty()) return;
    bool OK = true;
    foreach (QString nfcol, listeF)
        if (_dico || nfcol.contains("/lemmes.") || nfcol.contains("/lem_ext."))
        {
            bool OK1 = installe(nfcol);
            if (OK1) qDebug() << "installé" << nfcol;
            else OK = false;
        }
    // info
    if (OK) QMessageBox::information(this, tr("Collatinus 11"),
                             tr("L'installation s'est bien passée. "
                                "Au prochain lancement, les nouveaux lexiques "
                                "et dictionnaires seront disponibles."));
/*
    // Provisoirement, j'utilise la mise à jour pour créer les .col à partir des djvu.
    QStringList nfichiers = QFileDialog::getOpenFileNames(
        this, "Sélectionner un ou plusieurs paquets", qApp->applicationDirPath() + "/data/dicos/",
        "dictionnaires djvu (*.djvu)");
    listeF = nfichiers;
    if (listeF.empty()) return;
    bool OK = true;
    foreach (QString nfcol, listeF)
    {
        bool OK1 = djvu2col(nfcol);
        if (OK1) qDebug() << "installé" << nfcol;
        else OK = false;
    }
    // info
    if (OK) QMessageBox::information(this, tr("Collatinus 11"),
                             tr("La copie s'est bien passée. "));
*/
}

void Maj::setFont(QFont font) { label->setFont(font); }

/**
 * @brief Maj::djvu2col
 * @param nfdjvu
 * @return
 *
 * Fonction provisoire pour créer un fichier .col à partir
 * des fichiers djvu, idx et cfg présents dans /data/dicos.
 * C'est une fonction que je suis seul à utiliser, une seule fois.
 * Les utilisateurs utiliseront la fonction "installe" qui fait le contraire,
 * i.e. installer les fichiers djvu, idx et cfg dans /data/dicos
 * à partir d'un .col placé ailleurs.
 *
 */
bool Maj::djvu2col(QString nfdjvu)
{
    // nom du paquet
    QString nom = QFileInfo(nfdjvu).baseName();
    // fichiers destination
    QString nf(qApp->applicationDirPath() + "/data/dicos/" + nom);
    QString nfcol("/Users/Philippe/Documents/dicos_C11/" + nom + ".col");
    QString nfidx = nf + ".idx";
    QString nfcfg = nf + ".cfg";
    qDebug() << nfdjvu << nfcol << nf;

    if (QFile::exists(nfcol))
        QFile::remove(nfcol);
    // On ne peut pas copier si le fichier existe déjà
    QFile::copy(nfdjvu,nfcol);
    // Je copie le fichier dans /Users/Philippe/Documents/dicos_C11/.
    QFile fcol(nfcol);
    if (!fcol.open(QFile::ReadWrite)) return false;
    fcol.seek(fcol.size());

    QFile fzi(nfidx);
    fzi.open(QFile::ReadOnly|QFile::Text);
    QString lin = fzi.readAll();
    fzi.close();

    qint64 p = fcol.pos();
    qDebug() << p;
    QString nn = "%1:%2:%3\n";
    QByteArray ba = qCompress(lin.toUtf8(),9);
    fcol.write(ba);
    QString piedDeFichier = "\n";
    piedDeFichier += nn.arg("idx").arg(p).arg(ba.size());

    fzi.setFileName(nfcfg);
    fzi.open (QFile::ReadOnly|QFile::Text);
    QByteArray baIn = fzi.readAll();
    fzi.close();

    ba = qCompress(baIn,9);
    p = fcol.pos();
    fcol.write(ba);
    piedDeFichier += nn.arg("Cfg").arg(p).arg(ba.size());

    int n = 100 - piedDeFichier.size();
    //        if (n<1) n += 64;
    qDebug() << n;
    piedDeFichier.prepend(QString(n,' '));
    fcol.write(piedDeFichier.toUtf8());

    fcol.close();
    return true;
}
