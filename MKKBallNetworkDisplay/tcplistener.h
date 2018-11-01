#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <QObject>
#include <QTcpServer>

class TcpListener : public QObject
{
public:
    TcpListener();
    Q_OBJECT
public slots:
    void HandleNewClient();
signals:
    void packetArrived(QByteArray);

private:
    QSharedPointer<QTcpServer> server;
};

#endif // TCPLISTENER_H
