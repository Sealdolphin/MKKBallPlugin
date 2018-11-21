#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <rep_displaydata_source.h>
#include <QList>
class DisplayData : public DisplayDataSimpleSource
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> tickets READ tickets WRITE setTickets NOTIFY ticketsChanged)
    QList<QString> m_tickets;

public:
    DisplayData();

    QList<QString> tickets() const
    {
        return m_tickets;
    }

public slots:
    void rotateTombola();
    void getTombola();
    void loadTombola();

    void setTickets(QList<QString> tickets)
    {
        if (m_tickets == tickets)
            return;

        m_tickets = tickets;
        emit ticketsChanged(m_tickets);
    }
private:
    void removeTicket(QString ticket);
signals:
    void ticketsChanged(QList<QString> tickets);
};

#endif // DISPLAYDATA_H
