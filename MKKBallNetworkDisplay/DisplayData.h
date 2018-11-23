#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <rep_displaydata_source.h>
#include <QList>
class DisplayData : public DisplayDataSimpleSource
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> tickets READ tickets WRITE setTickets NOTIFY ticketsChanged)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    QList<QString> m_tickets;

public:
    DisplayData();

    QList<QString> tickets() const
    {
        return m_tickets;
    }

    QString result() const
    {
        return m_result;
    }

public slots:
    void rotateTombola();
    void getTombola();
    void loadTombola(QUrl file);

    void setTickets(QList<QString> tickets)
    {
        if (m_tickets == tickets)
            return;

        m_tickets = tickets;
        emit ticketsChanged(m_tickets);
    }
    void setResult(QString result)
    {
        if (m_result == result)
            return;

        m_result = result;
        emit resultChanged(m_result);
    }

private:
    void removeTicket(QString ticket);
    QString m_result;

signals:
void ticketsChanged(QList<QString> tickets);
void resultChanged(QString result);
};

#endif // DISPLAYDATA_H
