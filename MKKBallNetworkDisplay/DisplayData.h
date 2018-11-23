#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <rep_displaydata_source.h>
#include <QList>
class DisplayData : public DisplayDataSimpleSource
{
    Q_OBJECT
    Q_PROPERTY(QList<QString> tickets READ tickets WRITE setTickets NOTIFY ticketsChanged)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(bool rotating READ rotating WRITE setRotating NOTIFY rotatingChanged)
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

    bool rotating() const
    {
        return m_rotating;
    }

public slots:
    void rotateTombola();
    void getTombola();
    void loadTombola(QUrl file);
    void rotateWinningTicketStep();

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

    void setRotating(bool rotating)
    {
        if (m_rotating == rotating)
            return;

        m_rotating = rotating;
        emit rotatingChanged(m_rotating);
    }

private:
    void removeTicket(QString ticket);
    QString m_result;

    bool m_rotating;

signals:
void ticketsChanged(QList<QString> tickets);
void resultChanged(QString result);
void rotatingChanged(bool rotating);
};

#endif // DISPLAYDATA_H
