#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QObject>
#include <QByteArray>
#include <DisplayData.h>
class PacketParser : public QObject
{

    Q_OBJECT
public:
    PacketParser(DisplayData* displayData);

public slots:
    void processPacket(QByteArray data);
private:
    DisplayData* displayData;
};

#endif // PACKETPARSER_H
