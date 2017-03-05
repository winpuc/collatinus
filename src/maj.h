/*    maj.h   */

#ifndef MAJ_H
#define MAJ_H

#include <QDialog>
#include <QLabel>
#include <QStringList>

class Maj : public QDialog
{
    Q_OBJECT

   private:
    bool installe(
        QString nfcol);  // décompresse nfcol et l'installe dans data/dicos/
    QLabel *label;
    QStringList listeF;  // liste des fichiers téléchargés
   private slots:
    void selectionne();  // lance un dialogue de sélection de fichier
   public:
    Maj(QDialog *parent = 0);
    void setFont(QFont font);
};

#endif
