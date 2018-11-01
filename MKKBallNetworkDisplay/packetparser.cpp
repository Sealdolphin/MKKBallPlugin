#include "packetparser.h"
#include <QtDebug>
#include <QtGlobal>
#include <QJsonDocument>
#include <QJsonObject>
PacketParser::PacketParser(DisplayData* displayData) :displayData(displayData)
{

}

static QString get(QJsonObject& obj, QString key)
{
    if (obj.contains(key))
    {
        return obj.value(key).toString();
    }
    qInfo() << "Failed to get key: " << key;
    return "";
}

void PacketParser::processPacket(QByteArray data)
{
    qInfo() << "Packet arrived: " << data.toStdString().c_str();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() ||  doc.object().isEmpty())
    {
        qCritical() << "Invalid json packet: " << data.toStdString().c_str();
        return;
    }
    auto JObj = doc.object();
    displayData->setArtist1(get(JObj,"artist") );
    displayData->setTitle1(get(JObj,"title") );
    displayData->setGenre1(get(JObj,"genre") );
    displayData->setGenre2(get(JObj,"next_song") );
    displayData->setMinLeft(get(JObj,"min_left").toInt() );
    displayData->setSecLeft(get(JObj,"sec_left").toInt());
}
