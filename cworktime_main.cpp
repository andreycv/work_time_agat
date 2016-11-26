#include "cworktime_main.h"

CWorkTime_main::CWorkTime_main(QObject *parent) : QObject(parent)
{

}

QString CWorkTime_main::minusTime(const QString *Time_1, const QString *Time_2)
{
    //qDebug() << *Time_1 << *Time_2;
    int delta = Time_1->mid(0, 2).toInt() * 60 * 60 + Time_1->mid(3, 2).toInt() * 60 + Time_1->mid(6, 2).toInt();
    delta -= Time_2->mid(0, 2).toInt() * 60 * 60 + Time_2->mid(3, 2).toInt() * 60 + Time_2->mid(6, 2).toInt();
    QString res;
    if(delta < 0){
        res ="-";
        delta*=-1;
    }
    if(delta / 60 / 60 < 10) res.append("0");
    res.append(QString::number(delta / 60 / 60));
    res.append(":");
    if((delta / 60) % 60 < 10) res.append("0");
    res.append(QString::number((delta / 60) % 60));
    res.append(":");
    if(delta % 60 < 10) res.append("0");
    res.append(QString::number(delta % 60));
    //qDebug() << "res_min" << res;
    return res;
}

QString CWorkTime_main::plusTime(const QString *Time_1, const QString *Time_2)
{
    int delta = Time_1->mid(0, 2).toInt() * 60 * 60 + Time_1->mid(3, 2).toInt() * 60 + Time_1->mid(6, 2).toInt();
    delta+=Time_2->mid(0, 2).toInt() * 60 * 60 + Time_2->mid(3, 2).toInt() * 60 + Time_2->mid(6, 2).toInt();
    QString res;
    if(delta / 60 / 60 < 10) res = "0";
    res.append(QString::number(delta / 60 / 60));
    res.append(":");
    if((delta / 60) % 60 < 10) res.append("0");
    res.append(QString::number((delta / 60) % 60));
    res.append(":");
    if(delta % 60 < 10) res.append("0");
    res.append(QString::number(delta % 60));
    return res;
}

QString CWorkTime_main::get_work_time_day(int day, int month)
{
    QDate date(QDate::currentDate().year(), month, day);
    switch(date.dayOfWeek()){
    case 1: return "09:00:00";
    case 2: return "09:00:00";
    case 3: return "09:00:00";
    case 4: return "09:00:00";
    case 5: return "07:45:00";
    case 6: return "06:00:00";
    case 7: return "06:00:00";
    default: return "09:00:00";
    }
}

int CWorkTime_main::get_work_time_day_to_min(int day_of_week)
{
    switch(day_of_week){
    case 1: return 9 * 60;
    case 2: return 9 * 60;
    case 3: return 9 * 60;
    case 4: return 9 * 60;
    case 5: return 7 * 60 + 45;
    case 6: return 6 * 60;
    case 7: return 6 * 60;
    default: return 9 * 60;
    }
}

QString CWorkTime_main::getDayOfWeek(QString data)
{
    QDate date = QDate::fromString(data.append(".").append(QString::number(QDate::currentDate().year())), "dd.MM.yyyy");
    if(QDate::currentDate().month() < date.month())
        date.addYears(-1);
    switch(date.dayOfWeek()){
    case 1: return QObject::tr("ПН");
        break;
    case 2: return QObject::tr("ВТ");
        break;
    case 3: return QObject::tr("СР");
        break;
    case 4: return QObject::tr("ЧТ");
        break;
    case 5: return QObject::tr("ПТ");
        break;
    case 6: return QObject::tr("СБ");
        break;
    case 7: return QObject::tr("ВС");
        break;
    }
}



