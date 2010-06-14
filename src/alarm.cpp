#include "alarm.h"

Alarm::Alarm(QObject* parent) :
    QObject(parent)
{
    
}

void Alarm::init()
{    
    QDateTime today = QDateTime::currentDateTime();
    year = today.date().year();
    month = today.date().month();
    day = today.date().day();
    hours = 0;
    minutes = 0;
    seconds = 0;
    timeToAlarm = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(finishAalarm()));
}

void Alarm::setAlarm()
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime alarmTime(QDate(year, month, day), QTime(hours, minutes, seconds));
    timeLeft = now.secsTo(alarmTime);
    if (timeLeft > 0)
    {
        //qDebug("There are %d seconds to alarm", now.secsTo(alarmTime));
        timeToAlarm = timeLeft*1000;
        startAlarm();
    }
    else
    {
        //qDebug("There are %d seconds passed", abs(now.secsTo(alarmTime)));
        stopAlarm();
    }
}

void Alarm::startAlarm()
{
    if (timer->isActive()) 
    {
        timer->stop();
    }
    else 
    {   
        timer->start(timeToAlarm);
    }
}

void Alarm::stopAlarm()
{
    timer->stop();
}

void Alarm::setYear(int year_)
{
    year = year_;
}

void Alarm::setMonth(int month_)
{
    month = month_;
}

void Alarm::setDay(int day_)
{
    day = day_;
}

void Alarm::setHours(int hours_)
{
    hours = hours_;
}

void Alarm::setMinutes(int minutes_)
{
    minutes = minutes_;
}

void Alarm::setSeconds(int seconds_)
{
    seconds = seconds_;
}

void Alarm::finishAalarm()
{
    timer->stop(); 
    emit(itsTime());   
}

int Alarm::getTimeLeft()
{
    return timeLeft;
}

bool Alarm::isActive()
{
    return timer->isActive();
}
