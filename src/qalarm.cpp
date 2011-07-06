#include "qalarm.h"

Qalarm::Qalarm()
{     
    path = QCoreApplication::applicationDirPath();
    if (path.data()[path.size() - 1] != '/') path += "/";
    init();
    startAlarm();
    createActions();
    createTrayIcon();
    trayIcon = new QSystemTrayIcon(this);
    url = new QUrl();    
    updateTrayIcon();
    music = Phonon::createPlayer(Phonon::MusicCategory,Phonon::MediaSource(alarmDialog.getAudio()));
} 
 
void Qalarm::init()
{        
    connect(&alarmDialog, SIGNAL(alarmChanged()), this, SLOT(startAlarm()));
    connect(&alarmDialog, SIGNAL(alarmChanged()), this, SLOT(updateTrayIcon()));
    connect(&alarm, SIGNAL(itsTime()), this, SLOT(launchAlarm()));
}
 
void Qalarm::createActions()
{
    setalarm = new QAction(tr("Set ala&rm"), this);
    connect(setalarm, SIGNAL(triggered()), this, SLOT(setAlarm()));   
    
    playaudio = new QAction(tr("&Play Audio"), this);
    connect(playaudio, SIGNAL(triggered()), this, SLOT(playAudio()));
    
    stopaudio = new QAction(tr("S&top Audio"), this);
    connect(stopaudio, SIGNAL(triggered()), this, SLOT(stopAudio())); 
        
    openurl = new QAction(tr("Op&en Url"), this);
    connect(openurl, SIGNAL(triggered()), this, SLOT(openUrl()));
    
    aboutAction = new QAction(tr("&About"), this);
    connect( aboutAction, SIGNAL(triggered()), this, SLOT(_about()));
    
    aboutQtAction = new QAction(tr("About Qt"), this);
    connect( aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));   
}

void Qalarm::createTrayIcon()
{    
    QColor *c = new QColor ( 238, 238, 157, 255 );    
    QPalette * p = new QPalette();
    p->setColor(QPalette::ToolTipBase,*c);
    QFont *f = new QFont ( "sans", 10, -1, false );    
    QToolTip::setFont ( *f );
    QToolTip::setPalette(*p);    
    trayIconMenu = new QMenu(this);        
    trayIconMenu->addAction(setalarm);
    trayIconMenu->addAction(playaudio);
    trayIconMenu->addAction(stopaudio);
    trayIconMenu->addAction(openurl);    
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(aboutQtAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction); 
    stopaudio->setEnabled(false);  
    playaudio->setEnabled(true);          
}

void Qalarm::setAlarm()
{
    alarmDialog.show();
}

void Qalarm::playAudio()
{    
    music->setCurrentSource(alarmDialog.getAudio());
    stopaudio->setEnabled(true);   
    playaudio->setEnabled(false);        
    music->play();        
}

void Qalarm::stopAudio()
{  
    music->stop();
    stopaudio->setEnabled(false);   
    playaudio->setEnabled(true);    
}

void Qalarm::openUrl()
{           
    url->setUrl(alarmDialog.getUrl());
    QDesktopServices::openUrl(*url);
}

void Qalarm::updateTrayIcon()
{
    repeat = alarmDialog.getRepeat();    
    QString tooltipmsg1 = alarmDialog.getAudio();
    QStringList list = tooltipmsg1.split("/");
    tooltipmsg1 = list[list.size()-1];
    QString tooltipmsg2 = alarmDialog.getTime();
    QString tooltipmsg3 = alarmDialog.getUrl();
    QString tooltipmsg4;        
    if (repeat == 1){        
        QString list[7] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
        tooltipmsg4 = "Repeat once a week \n Day = "+list[alarmDialog.getDay()];        
    }
    else if (repeat == 2){         
           tooltipmsg4 = "Repeat week days";
    }    
    else if (repeat == 3){        
           tooltipmsg4 = "Repeat every day";        
    }
    else{
        tooltipmsg4 = "Repeat once \n Date = "+alarmDialog.getDate();
    }
    QString message = " Alarm = "+tooltipmsg2+"\n Repeat = "+tooltipmsg4+"\n Audio = "+tooltipmsg1+"\n Url = "+tooltipmsg3;
    trayIcon->setIcon(QIcon(path+"images/alarm.png"));    
    trayIcon->setToolTip(message);
    trayIcon->show();    
    trayIcon->setContextMenu(trayIconMenu);    
}

void Qalarm::_about()
{
    QString cp = "©";
    QMessageBox::about(this, tr("About Alarm"),
             "<b> Alarm application 0.5</b> Copyright " +  QString::fromUtf8(cp.toLatin1().data()) + " 2009 Skander Jabouzi skander@skanderjabouzi.com<br>"
                 " This is a free software distributed under the terms of the GNU General Public License version 3\n(http://www.gnu.org/licenses/gpl-3.0.html)");     
}

void Qalarm::startAlarm()
{
    alarm.init();   
    repeat = alarmDialog.getRepeat();
    if (0 == repeat)
    {
        QDate date = QDate::fromString(alarmDialog.getDate(), "dd/MM/yyyy");
        alarm.setYear(date.year());
        alarm.setMonth(date.month());
        alarm.setDay(date.day());
    }   
    setAlarmTime();
    if (alarm.getTimeLeft() > 0)
    {
        repeatAlarm();
    }
}

void Qalarm::launchAlarm()
{
    playAudio();
    openUrl();
    repeatAlarm();
}

void Qalarm::repeatAlarm()
{
    QDateTime today = QDateTime::currentDateTime();
    int year = today.date().year();
    int month = today.date().month();
    int day = today.date().day();
        
    alarm.setYear(year);
    alarm.setMonth(month);
    if (1 == repeat)
    {
        int weekDay = today.date().dayOfWeek();
        int alarmDay = alarmDialog.getDay()+1;
        int diffDays = alarmDay - weekDay; 
        if (diffDays <= 0) diffDays = 7 + diffDays;
        alarm.setDay(day+diffDays);       
    }
    else if (2 == repeat)
    {
        int weekDay = today.date().dayOfWeek();
        if (weekDay == 5) alarm.setDay(day+3);   
        else alarm.setDay(day+1);       
    }
    else if (3 == repeat)
    {
        alarm.setDay(day+1);       
    }       
    setAlarmTime();
}

void Qalarm::setAlarmTime()
{
    QTime time = QTime::fromString(alarmDialog.getTime(), "HH:mm");
    alarm.setHours(time.hour());
    alarm.setMinutes(time.minute());
    alarm.setSeconds(time.second());
    alarm.setAlarm();
}
//
 
