#ifndef QALARM_H
#define QALARM_H

#include <QtGui>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QPalette>
#include <QColor>
#include <QToolTip>
#include <QString>
#include <QDir>
#include <phonon>
#include "qalarmdialog.h"
#include "alarm.h"

class Qalarm : public QDialog
{  
  Q_OBJECT     
  public:
    Qalarm(); 
  private slots:
    void setAlarm();
    void playAudio();
    void stopAudio();
    void openUrl();    
    void _about(); 
    void startAlarm();
    void launchAlarm();
    void updateTrayIcon();  
    
  private:
    void createActions();
    void createTrayIcon();    
    void init();    
    void repeatAlarm();
    void setAlarmTime();
    
    QAction *aboutAction;
    QAction *quitAction;
    QAction *setalarm;
    QAction *playaudio;
    QAction *stopaudio;
    QAction *openurl;
    QAction *aboutQtAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QUrl * url;   
    Phonon::MediaObject *music;
    QString path;
    QString imgfile;    
    QalarmDialog alarmDialog;
    Alarm alarm;    
    int repeat;
}; 

#endif 
