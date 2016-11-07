#ifndef MOT_H
#define MOT_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QtCore/QCoreApplication>

#include "lemmatiseur.h"
#include "lemme.h"

class Mot : public QObject
{
    Q_OBJECT
public:
    Mot(QString forme, int rang, QObject *parent = 0);
    QString choisir(QString t = "", bool tout = true);
    int proba(QString t);
    QStringList tags();
    QString forme();

private:
    Lemmat* _lemmatiseur;
    QString _forme;
    int _rang;
    QString _tagEncl;
    MapLem _mapLem;
    QStringList _lemmes;
    QStringList _morphos;
    QStringList _tags;
    QList<int> _nbOcc;
    QMap<QString,int> _probas;
};

#endif // MOT_H
