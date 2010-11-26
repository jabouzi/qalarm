#include "qalarmdialog.h"
//
QalarmDialog::QalarmDialog( QWidget * parent, Qt::WFlags f) 
    : QDialog(parent, f)
{
    path = QCoreApplication::applicationDirPath();
    if (path.data()[path.size() - 1] != '/') path += "/";
    setupUi(this);
    initDB();
    init();   
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(radioOnce, SIGNAL(clicked()), this, SLOT(enable()));
    connect(radioOnceWeek, SIGNAL(clicked()), this, SLOT(enable()));
    connect(radioWeekdays, SIGNAL(clicked()), this, SLOT(enable()));
    connect(radioAlldays, SIGNAL(clicked()), this, SLOT(enable()));
    connect(selctAudioButton, SIGNAL(clicked()), this, SLOT(load()));
}

void QalarmDialog::initDB()
{    
    db = Database::getInstance();
    db->setPath(path);
    db->setDatabaseName(path+"data/.alarm.db");
    db->setDatabase();      
}

void QalarmDialog::init()
{
    audioEdit->setText(getAudio());
    urlEdit->setText(getUrl());
    timeEdit->setTime(QTime::fromString(getTime(), "HH:mm"));
    QStringList list;
    list << "Monday"<<"Tuesday"<<"Wednesday"<<"Thursday"<<"Friday"<<"Saturday"<<"Sunday";    
    daysList->addItems(list);
    daysList->setCurrentIndex(getDay());    
    calendarWidget->setSelectedDate(QDate::fromString(getDate(), "dd/MM/yyyy"));
    daysList->setEnabled(false);
    calendarWidget->setEnabled(false);    
    int repeat = getRepeat();
    if (0 == repeat) {
        radioOnce->setChecked(true); 
        calendarWidget->setEnabled(true);        
    }
    else if (1 == repeat) {
        radioOnceWeek->setChecked(true);        
        daysList->setEnabled(true);    
    }
     else if (2 == repeat) radioWeekdays->setChecked(true);
     else radioAlldays->setChecked(true);     
}

int QalarmDialog::getDay()
{
    db->setTable("alarmTable");
    return db->select("day").toInt();    
}

int QalarmDialog::getRepeat()
{
    db->setTable("alarmTable");
    return db->select("repeat").toInt();   
}

QString QalarmDialog::getTime()
{
    db->setTable("alarmTable");
    return db->select("time");   
}

QString QalarmDialog:: getAudio()
{
    db->setTable("alarmTable");
    return db->select("audio");   
}

QString QalarmDialog::getUrl()
{
    db->setTable("alarmTable");
    return db->select("url");   
}

QString QalarmDialog::getDate()
{
    db->setTable("alarmTable");
    return db->select("date");   
}

void QalarmDialog::setDay(int day)
{
    db->where("id = '1'");
    db->setTable("alarmTable");
    db->update("day", QString::number(day));
    db->where("");
}

void QalarmDialog::setRepeat(int repeat)
{
    db->where("id = '1'");
    db->setTable("alarmTable");
    db->update("repeat", QString::number(repeat));
    db->where("");
}

void QalarmDialog::setTime(QString time)
{
    db->where("id = '1'");
    db->setTable("alarmTable");
    db->update("time", time);
    db->where("");
}

void QalarmDialog::setAudio(QString audio)
{
    db->where("id = '1'");
    db->setTable("alarmTable");
    db->update("audio", audio);
    db->where("");
}

void QalarmDialog::setUrl(QString url)
{
    db->where("id = '1'");
    db->setTable("alarmTable");
    db->update("url", url);
    db->where("");
}

void QalarmDialog::setDate(QString date)
{
    db->where("id = '1'");
    db->setTable("alarmTable");
    db->update("date", date);
    db->where("");
}

void QalarmDialog::save()
{
    setTime(timeEdit->time().toString("HH:mm"));
    setDate(calendarWidget->selectedDate().toString("dd/MM/yyyy"));
    setDay(daysList->currentIndex());    
    setAudio(audioEdit->text());
    setUrl(urlEdit->text());    
    int repeat = 0;
    if (radioOnce->isChecked()) repeat = 0;
    else if (radioOnceWeek->isChecked()) repeat = 1;
    else if (radioWeekdays->isChecked()) repeat = 2;
    else if (radioAlldays->isChecked()) repeat = 3;
    setRepeat(repeat);   
    emit(alarmChanged());   
}

void QalarmDialog::enable()
{
    if (radioOnce->isChecked()) {
        daysList->setEnabled(false);
        calendarWidget->setEnabled(true);
    }
    else if (radioOnceWeek->isChecked()) {
        daysList->setEnabled(true);    
        calendarWidget->setEnabled(false);    
    }
    else if (radioWeekdays->isChecked()) {
        daysList->setEnabled(false);
        calendarWidget->setEnabled(false);
    }
    else if (radioAlldays->isChecked()) {
        daysList->setEnabled(false);
        calendarWidget->setEnabled(false);
    }
}

void QalarmDialog::load()
{
    audioEdit->setText(QFileDialog::getOpenFileName(this,tr("Open File"),".",tr("audios (*.mp3 *.wma *.ogg *.wave *.midi)")));
}
//
