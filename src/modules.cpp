/*              modules.cpp              */
/*  This file is part of COLLATINUS.
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
 * © Yves Ouvrard, 2018 - 2019
 */

# include <QDir>
# include "modules.h"

DialogM::DialogM(QString d, MainWindow* parent)
{
    _dir = d;
    mainwindow = parent;
    setupUi();
    retranslateUi();

    connect(pushButton,   SIGNAL(clicked()), this, SLOT(activer()));
    connect(pushButton_2, SIGNAL(clicked()), this, SLOT(desactiver()));
    connect(pushButton_4, SIGNAL(clicked()), this, SLOT(close()));

    QDir dir(_dir);
    QStringList lm = dir.entryList(QStringList() << "*",
                                         QDir::NoDotAndDotDot | QDir::Dirs);
    QListWidgetItem* item = 0;
    QString m = mainwindow->module();
    for (int i=0;i<lm.count();++i)
    {
        QListWidgetItem* ni = new QListWidgetItem(lm.at(i), listWidget);
        if (lm.at(i) == mainwindow->module()) item = ni;
    }
    if (item != 0) listWidget->setCurrentItem(item);
}

void DialogM::activer()
{
    QListWidgetItem* item = listWidget->currentItem();
    if (item != 0) mainwindow->setModule(item->text());
}

void DialogM::desactiver()
{
	listWidget->setCurrentItem(0);
    QListWidgetItem* item = 0;
    if (item == 0) mainwindow->setModule("");
}

QString DialogM::module()
{
    QListWidgetItem* item = listWidget->currentItem();
    if (item != 0) return item->text();
    return 0;
}
