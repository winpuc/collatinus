/*     vargraph.h   */
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
 * © Yves Ouvrard, 2009 - 2019

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
        DialogVG(QStringList l, MainWindow* parent=0);
        QStringList          lignes();

    private:
        MainWindow* mainwin;
        QWidget *tabVarGraph;
        QVBoxLayout *verticalLayout;
        QVBoxLayout *verticalLayout_4;
        QSplitter *splitterVarGraph;
        QWidget *layoutWidget;
        QVBoxLayout *verticalLayoutConf;
        QLabel *label_3;
        QHBoxLayout *horizontalLayoutBtnPre;
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

        QStringList lvg = QStringList()
            << "ae;e"
            << "h;"
            << "mihi;michi"
            << "I;J"
            << "U;V"
            << "([aeiourln])ci([aeiourl])>\\1ti\\2"
            << "mn;mpn"
            << "ph;f";
        QStringList tv;

        void etiquettes();
        void connecte();
        void initCoches(QStringList ll);

        private slots:
            void coche();
};

# endif
