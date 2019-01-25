/*              modules.cpp              */

# include <QDir>
# include "modules.h"

DialogM::DialogM(QString d, MainWindow* parent)
{
    _dir = d;
    mainwindow = parent;
    setupUi();
    retranslateUi();

    connect(pushButton,   SIGNAL(clicked()), this, SLOT(activer()));
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

QString DialogM::module()
{
    QListWidgetItem* item = listWidget->currentItem();
    if (item != 0) return item->text();
    return 0;
}
