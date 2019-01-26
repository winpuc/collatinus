/*               modules.h         */
/*
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
 * © Yves Ouvrard, 2009 - 2019
 */

#ifndef MODULES_H
#define MODULES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

# include "mainwindow.h"

QT_BEGIN_NAMESPACE

class DialogM : public QDialog
{
    Q_OBJECT

    private:
        QString     _dir;
        MainWindow* mainwindow;

    public:
        QString module();

    public slots:
        void    activer();

    public:
        DialogM(QString d, MainWindow* parent=0);

        QVBoxLayout *verticalLayout_2;
        QHBoxLayout *horizontalLayout;
        QListWidget *listWidget;
        QVBoxLayout *verticalLayout;
        QPushButton *pushButton;    // activer
        QPushButton *pushButton_2;  // désactiver
        QPushButton *pushButton_3;  // supprimer
        QPushButton *pushButton_4;  // fermer
        QSpacerItem *verticalSpacer;

QT_BEGIN_NAMESPACE

        //void setupUi(QDialog *Dialog)
        void setupUi()
        {
            verticalLayout_2 = new QVBoxLayout(this);
            horizontalLayout = new QHBoxLayout(this);
            listWidget = new QListWidget(this);
            horizontalLayout->addWidget(listWidget);
            verticalLayout = new QVBoxLayout();
            pushButton = new QPushButton(this);
            verticalLayout->addWidget(pushButton);
            pushButton_2 = new QPushButton(this);
            verticalLayout->addWidget(pushButton_2);
            pushButton_3 = new QPushButton(this);
            verticalLayout->addWidget(pushButton_3);
            pushButton_4 = new QPushButton(this);
            verticalLayout->addWidget(pushButton_4);
            verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
            verticalLayout->addItem(verticalSpacer);
            horizontalLayout->addLayout(verticalLayout);
            verticalLayout_2->addLayout(horizontalLayout);
            retranslateUi();
        } // setupUi

        void retranslateUi()
        {
            setWindowTitle(QApplication::translate("Collatinus", "Modules lexicaux", Q_NULLPTR));
            pushButton->setText(QApplication::translate("Modules", "Activer", Q_NULLPTR));
            pushButton_2->setText(QApplication::translate("Modules", "D\303\251sactiver", Q_NULLPTR));
            pushButton_3->setText(QApplication::translate("Modules", "Supprimer", Q_NULLPTR));
            pushButton_4->setText(QApplication::translate("Modules", "Fermer", Q_NULLPTR));
        } // retranslateUi
QT_END_NAMESPACE

};

#endif // MODULES_H
