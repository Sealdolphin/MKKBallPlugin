#ifndef ROCLIENT_H
#define ROCLIENT_H

#include <QObject>
#include <QRemoteObjectNode>
#include <rep_displaydata_replica.h>
#include <QQmlContext>

class ROClient : public QObject
{
    Q_OBJECT
public:
    ROClient( QQmlContext* context);

public slots:
    void connect(QString text);
signals:
    void connected();

private:
    QRemoteObjectNode connection;
    QSharedPointer<DisplayDataReplica> data;
    QQmlContext* context;
};

#endif // ROCLIENT_H
