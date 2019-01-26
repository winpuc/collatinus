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
 * © Philippe Verkerk, 2018 - 2019
 */
#include <QCoreApplication>
#include <iostream>
#include <QtWidgets>
#include <QtNetwork>

class QTcpSocket;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString req = "";
    if (argc > 1)
    {
        int i = 1;
        while (i < argc)
        {
            QString suite(argv[i]);
            req += " " + suite;
            i++;
        }
    }
    else req = "-?"; // pour afficher l'aide.

    QTcpSocket * tcpSocket = new QTcpSocket();
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress::LocalHost, 5555);
    QByteArray ba = req.toUtf8();
    tcpSocket->write(ba);
    tcpSocket->waitForBytesWritten();
    tcpSocket->waitForReadyRead();
    ba = tcpSocket->readAll();
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    QString rep(ba);
    std::cout << rep.toStdString();

    a.quit();
}

