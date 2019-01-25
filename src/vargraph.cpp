/*         vargraph.cpp   */

#include "vargraph.h"

DialogVG::DialogVG(MainWindow* parent)
{
    tabVarGraph = new QWidget();
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    splitterVarGraph = new QSplitter(tabVarGraph);
    splitterVarGraph->setOrientation(Qt::Horizontal);
    layoutWidget = new QWidget(splitterVarGraph);
    verticalLayoutConf = new QVBoxLayout(layoutWidget);
    verticalLayoutConf->setSpacing(6);
    verticalLayoutConf->setContentsMargins(11, 11, 11, 11);
    verticalLayoutConf->setContentsMargins(0, 0, 0, 0);
    label_3 = new QLabel(layoutWidget);
    verticalLayoutConf->addWidget(label_3);
    horizontalLayoutBtnPre = new QHBoxLayout();
    horizontalLayoutBtnPre->setSpacing(6);
    btnPre = new QPushButton(layoutWidget);
    horizontalLayoutBtnPre->addWidget(btnPre);
    horizontalSpacerPre = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayoutBtnPre->addItem(horizontalSpacerPre);
    verticalLayoutConf->addLayout(horizontalLayoutBtnPre);
    formLayoutCochesVar = new QFormLayout();
    formLayoutCochesVar->setSpacing(6);
    formLayoutCochesVar->setContentsMargins(13, 13, -1, -1);
    labelVariante = new QLabel(layoutWidget);
    formLayoutCochesVar->setWidget(0, QFormLayout::LabelRole, labelVariante);
    checkBoxAe = new QCheckBox(layoutWidget);
    formLayoutCochesVar->setWidget(1, QFormLayout::LabelRole, checkBoxAe);
    checkBox_H = new QCheckBox(layoutWidget);
    formLayoutCochesVar->setWidget(2, QFormLayout::LabelRole, checkBox_H);
    checkBox_Mihi = new QCheckBox(layoutWidget);
    formLayoutCochesVar->setWidget(3, QFormLayout::LabelRole, checkBox_Mihi);
    checkBox_IJ = new QCheckBox(layoutWidget);
    formLayoutCochesVar->setWidget(4, QFormLayout::LabelRole, checkBox_IJ);
    checkBox_UV = new QCheckBox(layoutWidget);
    formLayoutCochesVar->setWidget(5, QFormLayout::LabelRole, checkBox_UV);
    checkBox_TICI = new QCheckBox(layoutWidget);
    formLayoutCochesVar->setWidget(6, QFormLayout::LabelRole, checkBox_TICI);
    checkBox_MPN = new QCheckBox(layoutWidget);
    formLayoutCochesVar->setWidget(7, QFormLayout::LabelRole, checkBox_MPN);
    checkBox_PH = new QCheckBox(layoutWidget);
    formLayoutCochesVar->setWidget(8, QFormLayout::LabelRole, checkBox_PH);
    verticalLayoutConf->addLayout(formLayoutCochesVar);
    splitterVarGraph->addWidget(layoutWidget);
    splitter_2 = new QSplitter(splitterVarGraph);
    splitter_2->setOrientation(Qt::Vertical);
    plainTextEditVariantes = new QPlainTextEdit(splitter_2);
    splitter_2->addWidget(plainTextEditVariantes);
    layoutWidget1 = new QWidget(splitter_2);
    verticalLayout_4 = new QVBoxLayout(layoutWidget1);
    verticalLayout_4->setSpacing(6);
    verticalLayout_4->setContentsMargins(11, 11, 11, 11);
    verticalLayout_4->setContentsMargins(0, 0, 0, 0);
    splitter_2->addWidget(layoutWidget1);
    splitterVarGraph->addWidget(splitter_2);
    verticalLayout->addWidget(splitterVarGraph);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Save);
    verticalLayout->addWidget(buttonBox);
    // étiquettes
    etiquettes();
    connecte();
}

void DialogVG::etiquettes()
{
    label_3->setText("doc");
    btnPre->setText(QApplication::translate("MainWindow", "Pr\303\251analyse", Q_NULLPTR));
    labelVariante->setText(QApplication::translate("MainWindow", "variante", Q_NULLPTR));
    checkBoxAe->setText(QApplication::translate("MainWindow", "ae > e", Q_NULLPTR));
    checkBox_H->setText(QApplication::translate("MainWindow", "h > -", Q_NULLPTR));
    checkBox_Mihi->setText(QApplication::translate("MainWindow", "mihi > michi", Q_NULLPTR));
    checkBox_IJ->setText(QApplication::translate("MainWindow", "I > J", Q_NULLPTR));
    checkBox_UV->setText(QApplication::translate("MainWindow", "U > V", Q_NULLPTR));
    checkBox_TICI->setText(QApplication::translate("MainWindow", "ti > ci", Q_NULLPTR));
    checkBox_MPN->setText(QApplication::translate("MainWindow", "mn > mpn", Q_NULLPTR));
    checkBox_PH->setText(QApplication::translate("MainWindow", "ph > f", Q_NULLPTR));
}

void DialogVG::connecte()
{
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

