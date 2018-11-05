#include "DisplayData.h"
#include <QTimer>

DisplayData::DisplayData()
{
    setArtist1("Ismeretlen Előadó");
    setTitle1("Közepes minőségű szám");
    setGenre1("Angol keringő");
    setGenre2("Rocky");
    setMinLeft(2);
    setSecLeft(30);
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
