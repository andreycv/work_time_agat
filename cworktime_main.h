#ifndef CWORKTIME_MAIN_H
#define CWORKTIME_MAIN_H

#include <QObject>
#include <QDate>
#include <QDebug>

enum day_of_week{
    MONDAY,
    FRIDAY,
    WEEKEND
};

class CWorkTime_main : public QObject
{
    Q_OBJECT
public:
    explicit CWorkTime_main(QObject *parent = 0);
    static QString minusTime(const QString *Time_1, const QString *Time_2);
    static QString plusTime(const QString *Time_1, const QString *Time_2);
    static QString get_work_time_day(int day, int month);
    static int get_work_time_day_to_min(int day_of_week);
    static QString getDayOfWeek(QString data);

signals:

public slots:
};

#endif // CWORKTIME_MAIN_H
