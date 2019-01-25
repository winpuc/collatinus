/*     vargraph.h   */

#ifndef VARGRAPH_H
#define VARGRAPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

# include "mainwindow.h"

class DialogVG : public QDialog
{
    Q_OBJECT

    public:
        DialogVG(MainWindow* parent=0);

    private:
        QWidget *tabVarGraph;
        QVBoxLayout *verticalLayout;
        QVBoxLayout *verticalLayout_4;
        QSplitter *splitterVarGraph;
        QWidget *layoutWidget;
        QVBoxLayout *verticalLayoutConf;
        QLabel *label_3;
        QHBoxLayout *horizontalLayoutBtnPre;
        QPushButton *btnPre;
        QSpacerItem *horizontalSpacerPre;
        QFormLayout *formLayoutCochesVar;
        QLabel *labelVariante;
        QCheckBox *checkBoxAe;
        QCheckBox *checkBox_H;
        QCheckBox *checkBox_Mihi;
        QCheckBox *checkBox_IJ;
        QCheckBox *checkBox_UV;
        QCheckBox *checkBox_TICI;
        QCheckBox *checkBox_MPN;
        QCheckBox *checkBox_PH;
        QSplitter *splitter_2;
        QPlainTextEdit *plainTextEditVariantes;
        QWidget *layoutWidget1;
        QDialogButtonBox* buttonBox;
        void etiquettes();
        void connecte();
};

# endif
