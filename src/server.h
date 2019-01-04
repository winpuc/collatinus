/*            server.h
 *
 *  This file is part of COLLATINUS.
 *
 *  COLLATINUS is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  COLLATINVS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with COLLATINUS; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * © Yves Ouvrard, 2009 - 2016
 *
 * Je supprime tout ce qui était lié aux fenêtres pour ne garder que le serveur.
 * Philippe Janvier 2019.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
//#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
//#include <QPrintDialog>
//#include <QPrinter>

#include "flexion.h"
#include "lemCore.h"
#include "dicos.h"
#include "ch.h"
#include "lasla.h"
#include "tagueur.h"
#include "scandeur.h"
#include "lemmatiseur.h"

class Server : public QObject
{
    Q_OBJECT

   public:
    Server();
    // cœur
    LemCore *_lemCore;
    Flexion *flechisseur;
    // modules divers
    Lasla *lasla;
    Tagueur *tagueur;
    Scandeur *scandeur;
    Lemmatiseur *_lemmatiseur;
    bool html();
    // Pour le serveur
    QString startServer ();
    QString stopServer ();

    QString lem2csv(QString texte);

    void tagger(QString t, int p); // Je voudrais créer dans MainWindow l'interface du tagger.


   private slots:
    // Slots du serveur
    void connexion ();
    void exec ();

   private:
    // initialisation
    void createDicos(bool prim = true);

    // Pour le serveur
    QTcpServer * serveur;
    QTcpSocket * soquette;

    // fonctions des dictionnaires
    ListeDic listeD;
    QStringList ldic;
    QString consult(QString req, QString texte);

    // fonctions et variables diverses
    QString flechis(QString texte);
};

#endif
