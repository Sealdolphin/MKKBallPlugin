#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_PROPERTY(QString bgColor READ bgColor WRITE setBgColor NOTIFY bgColorChanged)
    Q_PROPERTY(QString bgImage READ bgImage WRITE setBgImage NOTIFY bgImageChanged)
    Q_OBJECT

public:
    explicit Settings(QObject *parent = nullptr);

    QString bgColor() const
    {
        return m_bgColor;
    }

    QString bgImage() const
    {
        return m_bgImage;
    }

public slots:
    int getFontSize(QString name, int windowHeight);
    QString getFontColor(QString name);
    int getX(QString name, double xr, int windowHeight);
    int getY(QString name, double yr, int windowHeight);

    void setBgColor(QString bgColor)
    {
        if (m_bgColor == bgColor)
            return;

        m_bgColor = bgColor;
        emit bgColorChanged(m_bgColor);
    }

    void setBgImage(QString bgImage)
    {
        if (m_bgImage == bgImage)
            return;

        m_bgImage = bgImage;
        emit bgImageChanged(m_bgImage);
    }

signals:
    void bgColorChanged(QString bgColor);

    void bgImageChanged(QString bgImage);

private:
    QSettings settings;
    QString defaultName;
    QVariant getValue(QString name, QString suffix, QVariant defVal);
    QString m_bgColor;
    QString m_bgImage;
};

#endif // SETTINGS_H
