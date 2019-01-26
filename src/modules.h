/*               modules.h         */

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

};

#endif // MODULES_H
