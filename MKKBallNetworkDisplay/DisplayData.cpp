#include "DisplayData.h"
#include <QTimer>
#include <QDebug>
#include <QtGlobal>
#include <QtMessageHandler>
#include <random>
#include <QFileDialog>
#include <QApplication>

DisplayData::DisplayData()
{
    setWinningTicket("XXXX");
    setDisplayedState("music");
    setArtist1("Ismeretlen Előadó");
    setTitle1("Közepes minőségű szám");
    setGenre1("Angol keringő");
    setGenre2("Rocky");
    setMinLeft(2);
    setSecLeft(30);
    setTickets({"1111","2222","3333","4444"});
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            [=]()
    {
        int secs = secLeft();
        int min = minLeft();
        --secs;
        while (secs < 0)
        {
            --min;
            secs += 60;
        }
        if (min < 0)
        {
            min = 0;
            secs = 0;
        }
        this->setMinLeft(min);
        this->setSecLeft(secs);
    }
    );
    //timer->start(1000);
}

void DisplayData::rotateTombola()
{
    setWinningTicket("XXXX");
}

void DisplayData::getTombola()
{
    if (m_tickets.size() == 0)
    {
        setWinningTicket("EMPTY");
        return;
    }
    static std::mt19937_64 rnd(std::random_device{}());
    int winnerIdx = rnd() % m_tickets.size();
    QString winner = m_tickets[winnerIdx];
    setWinningTicket(winner);
    removeTicket(winner);
}

void DisplayData::loadTombola()
{
    //QString fileName = QFileDialog::getOpenFileName(static_cast<QApplication *>(QCoreApplication::instance()), nullptr,"Select firmware file","/","Firmware File (*.hex)");
}

void DisplayData::removeTicket(QString ticket)
{
    int dtc = m_tickets.removeAll(ticket);
    if (dtc != 1)
    {
        qCritical() << "Deleted " << dtc << " tickets for string '" << ticket << "'";
    }
    emit ticketsChanged(m_tickets);
}
