#include "tcplistener.h"
#include <QtDebug>
#include <QtGlobal>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QDataStream>
TcpListener::TcpListener()
{
    server.reset(new QTcpServer(this));
    if (!server->listen(QHostAddress::Any,5503)) {
        qCritical() << "Unable to start server: " << server->errorString();
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qInfo() << " Listening on " << ipAddress << ":" << server->serverPort();

    connect(server.get(), &QTcpServer::newConnection, this, &TcpListener::HandleNewClient);
}

void TcpListener::HandleNewClient()
{
    QTcpSocket *clientConnection = server->nextPendingConnection();
    QObject::connect(clientConnection, &QAbstractSocket::disconnected,
                     clientConnection, &QObject::deleteLater);
    QObject::connect(clientConnection, &QIODevice::readyRead,
                     clientConnection,
                     [=]()
    {
        clientConnection->startTransaction();
        auto data = clientConnection->readAll();
        clientConnection->commitTransaction();
        emit packetArrived(data);
    }
    );

}
