#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QRemoteObjectHost>
#include "DisplayData.h"
#include <QQmlContext>
#include <QSettings>
#include "settings.h"
#include "tcplistener.h"
#include "packetparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, QCoreApplication::applicationDirPath());
    QQmlApplicationEngine engine;

    QRemoteObjectHost hostNode(QUrl("tcp://0.0.0.0:6112"));
    DisplayData data;
    hostNode.enableRemoting(&data,"mainData");
    engine.rootContext()->setContextProperty("mainData",&data);

    Settings settings;
    engine.rootContext()->setContextProperty("mainSettings",&settings);

    TcpListener tcpListener;
    PacketParser parser(&data);

    QObject::connect(&tcpListener, &TcpListener::packetArrived, &parser, &PacketParser::processPacket);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
