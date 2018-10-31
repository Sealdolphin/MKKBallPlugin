#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent), settings("mkksettings.ini",QSettings::IniFormat), defaultName("All")
{
    settings.beginGroup("BackGround");
    setBgColor(settings.value("Color","Green").toString());
    settings.setValue("Color",bgColor());
    setBgImage(settings.value("Image","").toString());
    settings.endGroup();
    settings.sync();

}

int Settings::getFontSize(QString name, int windowHeight)
{
    return getValue(name,"/FontSize",0.1).toDouble() * windowHeight;
}

QString Settings::getFontColor(QString name)
{
    return getValue(name,"/FontColor","White").toString();
}

int Settings::getX(QString name, double xr, int windowHeight)
{
    return getValue(name,"/X",xr).toDouble() * windowHeight;
}

int Settings::getY(QString name, double yr, int windowHeight)
{
    return  getValue(name,"/Y",yr).toDouble() * windowHeight;

}

QVariant Settings::getValue(QString name, QString suffix, QVariant defVal)
{
    return settings.value(name + suffix,settings.value(defaultName+suffix,defVal));
}
