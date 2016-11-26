#ifndef CSAVETOFILE_H
#define CSAVETOFILE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDate>

//ANDROID
static QString FILE_ADDRESS = QString("/mnt/sdcard/work_time/").append("Work Time ").append(QDate::currentDate().toString("MM.yyyy")).append(".txt"); //for android
static QString IMAGE1_ADDRESS = QString("/mnt/sdcard/work_time/").append("1").append(".jpg"); //for android
static QString IMAGE2_ADDRESS = QString("/mnt/sdcard/work_time/").append("2").append(".jpg"); //for android
static QString IMAGE_BACK_ADDRESS = QString("/mnt/sdcard/work_time/").append("3").append(".jpg"); //for android
static QString IMAGE_SETTINGS_ADDRESS = QString("/mnt/sdcard/work_time/").append("4").append(".jpg"); //for android
static QString IMAGE_STAT_ADDRESS = QString("/mnt/sdcard/work_time/").append("5").append(".jpg"); //for android

//WINDOWS
//static const QString FILE_ADDRESS = QString("Work Time ").append(QDate::currentDate().toString("MM.yyyy")).append(".txt"); //for PC
//static const QString IMAGE1_ADDRESS = QString("1").append(".jpg"); //for PC
//static const QString IMAGE2_ADDRESS = QString("2").append(".jpg"); //for PC
//static QString IMAGE_BACK_ADDRESS = QString("3").append(".jpg"); //for PC
//static QString IMAGE_SETTINGS_ADDRESS = QString("4").append(".jpg"); //for PC
//static QString IMAGE_STAT_ADDRESS = QString("5").append(".jpg"); //for PC

enum status_file{
    CANT_MAKE_FILE,
    MAKE_FILE,
    NEW_DAY,
    STARTED,
    FINISHED
};

struct day_data{
    day_data(QString DATA = "", QString START = "", QString FINISH = ""){
        start = START;
        finish = FINISH;
        data = DATA;
    }
    QString start = "";
    QString finish = "";
    QString data = "";
//    bool work_day = false;
//    bool friday = false;
//    bool weekend = false;
};

class CSaveToFile : public QObject
{
    Q_OBJECT
public:
    explicit CSaveToFile(QObject *parent = 0);
    ~CSaveToFile();
    static status_file static_check_file();
    void write_start();
    void write_finish();
    static day_data get_this_day();
    static day_data get_day(int day, int month);
    static void get_month(int month, int year, QList<day_data> *list);
    static void rewrite_this_day(QString start = "", QString finish = "");
    static void get_lists_statistics(QList<QString> *data, QList<QString> *start, QList<QString> *finish);

private:
    QFile *m_file;
    status_file check_file();

signals:

public slots:
};

#endif // CSAVETOFILE_H
