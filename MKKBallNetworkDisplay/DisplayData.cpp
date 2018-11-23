#include "DisplayData.h"
#include <QTimer>
#include <QDebug>
#include <QtGlobal>
#include <QtMessageHandler>
#include <random>
#include <QFileDialog>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QRegExp>

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
    setRotating(true);
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
    setRotating(true);
    if (m_tickets.empty())
    setWinningTicket("EMPTY");
}

static std::mt19937_64 rnd(std::random_device{}());

void DisplayData::getTombola()
{
    setRotating(false);
    if (m_tickets.size() == 0)
    {
        setWinningTicket("EMPTY");
        return;
    }
    int winnerIdx = rnd() % m_tickets.size();
    QString winner = m_tickets[winnerIdx];
    setWinningTicket(winner);
    removeTicket(winner);
}

void DisplayData::loadTombola(QUrl file)
{
    setResult("");
    QFile inp(file.toLocalFile());
    if(!inp.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setResult(QString("Couldn't open file: %1").arg(inp.errorString()));
    }
    auto data = QString(inp.readAll());
    auto x =  data.split(QRegExp("\\s+"),QString::SkipEmptyParts);
    setTickets(x);
}

void DisplayData::rotateWinningTicketStep()
{
    if (m_rotating && m_tickets.size())
    {
        int winnerIdx = rnd() % m_tickets.size();
        QString tempWinner = m_tickets[winnerIdx];
        setWinningTicket(tempWinner);
    }
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
