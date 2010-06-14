#ifndef QALARMDIALOG_H
#define QALARMDIALOG_H
//
#include <QDialog>
#include <QtSql>
#include <QFileDialog>
#include "ui_qalarm.h"
//
class QalarmDialog : public QDialog, public Ui::Qalarm
{
Q_OBJECT
public:
    QalarmDialog( QWidget * parent = 0, Qt::WFlags f = 0 );
    int getDay();
    int getRepeat();
    QString getTime();    
    QString getAudio();
    QString getUrl();
    QString getDate();
    
private:
    void init();   
    void setDay(int);
    void setRepeat(int);
    void setTime(QString);
    void setAudio(QString);
    void setUrl(QString);
    void setDate(QString);
    bool tableExists();
    void createTable();
    QSqlDatabase db;
    QSqlQuery query;
    QString path;
    
private slots:
    void save();
    void enable();
    void load();
    
signals:
    void alarmChanged();
};
#endif




