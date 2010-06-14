#include "qalarmdialog.h"
//
QalarmDialog::QalarmDialog( QWidget * parent, Qt::WFlags f) 
    : QDialog(parent, f)
{
    path = QCoreApplication::applicationDirPath();
    if (path.data()[path.size() - 1] != '/') path += "/";
    setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path+"data/.alarm.db");
    if (!tableExists())
    {
        createTable();
    }
    init();   
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(radioOnce, SIGNAL(clicked()), this, SLOT(enable()));
    connect(radioOnceWeek, SIGNAL(clicked()), this, SLOT(enable()));
    connect(radioWeekdays, SIGNAL(clicked()), this, SLOT(enable()));
    connect(radioAlldays, SIGNAL(clicked()), this, SLOT(enable()));
    connect(selctAudioButton, SIGNAL(clicked()), this, SLOT(load()));
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
    bool ok;
    db.open();    
    QSqlQuery query;
    query.exec("SELECT day FROM alarmTable");
    int field = query.record().indexOf("day");
    query.next();
    return query.value(field).toInt(&ok);
}
int QalarmDialog::getRepeat()
{
    bool ok;
    db.open();    
    QSqlQuery query;
    query.exec("SELECT repeat FROM alarmTable");
    int field = query.record().indexOf("repeat");
    query.next();
    return query.value(field).toInt(&ok);
}

QString QalarmDialog::getTime()
{
    db.open();    
    QSqlQuery query;
    query.exec("SELECT time FROM alarmTable");
    int field = query.record().indexOf("time");
    query.next();
    return query.value(field).toString();
}

QString QalarmDialog:: getAudio()
{
    db.open();    
    QSqlQuery query;
    query.exec("SELECT audio FROM alarmTable");
    int field = query.record().indexOf("audio");
    query.next();
    return query.value(field).toString();
}

QString QalarmDialog::getUrl()
{
    db.open();    
    QSqlQuery query;
    query.exec("SELECT url FROM alarmTable");
    int field = query.record().indexOf("url");
    query.next();
    return query.value(field).toString();
}

QString QalarmDialog::getDate()
{
    db.open();    
    QSqlQuery query;
    query.exec("SELECT date FROM alarmTable");
    int field = query.record().indexOf("date");
    query.next();
    return query.value(field).toString();
}

void QalarmDialog::setDay(int day)
{
    db.open();    
    QSqlQuery query;
    query.exec("UPDATE alarmTable SET day = '"+QString::number(day)+"'");
}

void QalarmDialog::setRepeat(int repeat)
{
    db.open();    
    QSqlQuery query;
    query.exec("UPDATE alarmTable SET repeat = '"+QString::number(repeat)+"'");
}

void QalarmDialog::setTime(QString time)
{
    db.open();    
    QSqlQuery query;
    query.exec("UPDATE alarmTable SET time = '"+time+"'");
}

void QalarmDialog::setAudio(QString audio)
{
    db.open();    
    QSqlQuery query;
    query.exec("UPDATE alarmTable SET audio = '"+audio+"'");
}

void QalarmDialog::setUrl(QString url)
{
    db.open();    
    QSqlQuery query;
    query.exec("UPDATE alarmTable SET url = '"+url+"'");
}

void QalarmDialog::setDate(QString date)
{
    db.open();    
    QSqlQuery query;
    query.exec("UPDATE alarmTable SET date = '"+date+"'");
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

bool QalarmDialog::tableExists()
{
    QStringList tables = db.tables(QSql::Tables);
    return tables.indexOf("alarmTable") >= 0;
}

void QalarmDialog::createTable()
{
    db.open();    
    QSqlQuery query;
    query.exec("CREATE TABLE alarmTable (id integer, date string, time string, day integer, repeat integer, audio string, url string)");
    query.exec("INSERT INTO alarmTable VALUES ('','1/1/2010','8:00',1,0,'','')");
}
//
