#include "csavetofile.h"

CSaveToFile::CSaveToFile(QObject *parent) : QObject(parent)
{

}

CSaveToFile::~CSaveToFile()
{

}

status_file CSaveToFile::check_file(){
    QDateTime DataTime;
    DataTime = QDateTime::currentDateTime();
    m_file = new QFile(FILE_ADDRESS);
    if (!m_file->open(QIODevice::ReadWrite)) {
        //файл отсутствует, создаем его
        m_file->close();
        if (!m_file->open(QIODevice::WriteOnly)) {
            return CANT_MAKE_FILE;   //файл отсутствует и не создан, ошибка
        }
        return MAKE_FILE;   //файл отсутствовал, создан
    }
    else{
        //файл существует читаем
        QString text = QString(m_file->readAll());
        if(!text.contains(DataTime.toString("dd.MM"))){
            return NEW_DAY;   //информации о текущем дне еще нет
        }
        else
            {
            text = QString(text).remove(0, text.indexOf(DataTime.toString("dd.MM")) + 6);
            if(!text.contains(" "))
                return STARTED; // есть отметка о начале дня
            else
                return FINISHED;   // есть отметка о завершении дня
        }
    }
}

status_file CSaveToFile::static_check_file()
{
    QDateTime DataTime;
    DataTime = QDateTime::currentDateTime();
    QFile *file = new QFile(FILE_ADDRESS);
    if (!file->open(QIODevice::ReadWrite)) {
        //файл отсутствует, создаем его
        file->close();
        if (!file->open(QIODevice::WriteOnly)) {
            file->close();
            return CANT_MAKE_FILE;   //файл отсутствует и не создан, ошибка
        }
        file->close();
        return MAKE_FILE;   //файл отсутствовал, создан
    }
    else{
        //файл существует читаем
        QString text = QString(file->readAll());
        if(!text.contains(DataTime.toString("dd.MM"))){
            file->close();
            return NEW_DAY;   //информации о текущем дне еще нет
        }
        else{
            text = QString(text).remove(0, text.indexOf(DataTime.toString("dd.MM")) + 6);
            if(!text.contains(" ")){
                file->close();
                return STARTED; // есть отметка о начале дня
            }
            else{
                file->close();
                return FINISHED;   // есть отметка о завершении дня
            }
        }
    }
}

void CSaveToFile::write_start(){
    status_file stat = check_file();
    if(stat == CANT_MAKE_FILE || stat == STARTED || stat == FINISHED){
        return;
    }
    QDateTime DataTime;
    DataTime = QDateTime::currentDateTime();

    QTextStream out(m_file);
    out << "\r\n" << DataTime.toString("dd.MM") << " " << DataTime.toString("HH:mm:ss");
    m_file->close();
}

void CSaveToFile::write_finish()
{
    status_file stat = check_file();
    if(stat != STARTED){
        return;
    }
    QDateTime DataTime;
    DataTime = QDateTime::currentDateTime();

    QTextStream out(m_file);
    out << " " << DataTime.toString("HH:mm:ss");
    m_file->close();
}

day_data CSaveToFile::get_this_day()
{
    day_data res;
    QDateTime DataTime = QDateTime::currentDateTime();
    QFile *file = new QFile(FILE_ADDRESS);
    if (!file->open(QIODevice::ReadWrite)) {
        //файл отсутствует
        file->close();
        return res;
    }
    else {
        //файл существует читаем
        QString text = QString(file->readAll());
        if(!text.contains(DataTime.toString("dd.MM"))){
            file->close();
            return res;   //информации о текущем дне еще нет
        }
        else {
            text = QString(text).remove(0, text.indexOf(DataTime.toString("dd.MM")) + 6);
            if(text.size() < 10){
                file->close();
                res.start = text.mid(0, 8);
                return res; // есть отметка о начале дня
            }
            else{
                file->close();
                res.start = text.mid(0, 8);
                res.finish = text.mid(9, 8);
                return res;   // есть отметка о завершении дня
            }
        }
    }
}

day_data CSaveToFile::get_day(int day, int month)
{
    day_data res;
    QString str_month = "";
    if(month < 10)
        str_month = "0";
     str_month.append(QString::number(month));
    QFile *file = new QFile(FILE_ADDRESS.left(FILE_ADDRESS.indexOf("Work") + 10).append(str_month).append(".").append(QString::number(QDate::currentDate().year())).append(".txt"));
    if (!file->open(QIODevice::ReadOnly)) {
        //файл отсутствует
        file->close();
        //qDebug() << "file false" << FILE_ADDRESS.left(FILE_ADDRESS.indexOf("Work") + 10).append(str_month).append(".").append(QString::number(QDate::currentDate().year())).append(".txt");
        return res;
    }
    else {
        //qDebug() << "file ok";
        //файл существует читаем
        QString date = "";
        if(day < 10){
            date ="0";
        }
        date.append(QString::number(day)).append(".").append(str_month);

        QString text = QString(file->readAll());
        if(!text.contains(date)){
            file->close();
            return res;   //информации о текущем дне еще нет
        }
        else {
            //qDebug() << "date are";
            text = QString(text).remove(0, text.indexOf(date) + 6);
            //qDebug() << "1 " << text;
            if(text.contains("\r")){
                text = text.left(text.indexOf("\r"));
            }
            //qDebug() << "2 " << text;
            if(text.size() < 10){
                file->close();
                res.start = text.mid(0, 8);
                return res; // есть отметка о начале дня
            }
            else{
                file->close();
                res.start = text.mid(0, 8);
                res.finish = text.mid(9, 8);
                //qDebug() << "date fool day";
                return res;   // есть отметка о завершении дня
            }
        }
    }
}

void CSaveToFile::get_month(int month, int year, QList<day_data> *list)
{
    QString str_month = "";
    if(month < 10)
        str_month = "0";
     str_month.append(QString::number(month));
    QFile *file = new QFile(FILE_ADDRESS.left(FILE_ADDRESS.indexOf("Work") + 10).append(str_month).append(".").append(QString::number(year)).append(".txt"));
    if (!file->open(QIODevice::ReadOnly)) {
        //файл отсутствует
        file->close();
        //qDebug() << "file false" << FILE_ADDRESS.left(FILE_ADDRESS.indexOf("Work") + 10).append(str_month).append(".").append(QString::number(QDate::currentDate().year())).append(".txt");
        return;
    }
    else {
        QString text = QString(file->readAll());
        QStringList strList = text.split("\r\n", QString::SkipEmptyParts);
        if(strList.empty()) return;
        for(int i = 0; i < strList.size(); i++){
            QStringList dayList = strList.at(i).split(" ", QString::SkipEmptyParts);
            if(dayList.size() != 3) continue;
            list->append(day_data(dayList.at(0), dayList.at(1), dayList.at(2)));
        }
    }
}

void CSaveToFile::rewrite_this_day(QString start, QString finish)
{
    QFile file(FILE_ADDRESS);
    if (!file.open(QIODevice::ReadWrite)) {
        //Ошибка, файл отсутствует
    }
    else{
        //файл существует читаем
        QString text = QString(file.readAll());
        if(!text.contains(QDate::currentDate().toString("dd.MM"))){
            //информации о текущем дне еще нет
        }
        else
        {
            file.close();
            file.remove();
            //file(FILE_ADDRESS);
            if (!file.open(QIODevice::WriteOnly)) {
                //Ошибка, файл отсутствует
                file.close();
                return;
            }
            text = text.left(text.indexOf(QDate::currentDate().toString("dd.MM")) + 6);
            text.append(start);
            if(finish.size() != 0)
                text.append(" ").append(finish);
            file.write(text.toUtf8());
        }
    }
    file.close();
}

void CSaveToFile::get_lists_statistics(QList<QString> *data, QList<QString> *start, QList<QString> *finish)
{
    data->clear();
    start->clear();
    finish->clear();
    int month = 1;//QDate::currentDate().month();
    int year = 2015;//QDate::currentDate().year();
    for(int i = -2; i < 1; i++){    //Последние 3 месяца включая этот
        QString read_month = "";
        if(month + i < 1){
            if(12 + month + i < 10){
                read_month = QString("0");
            }
            read_month.append(QString::number(12 + month + i)).append('.').append(QString::number(year - 1));
        }
        else
        {
            if(month + i < 10){
                read_month = QString("0");
            }
            read_month.append(QString::number(month + i)).append('.').append(QString::number(year));
        }
        read_month = FILE_ADDRESS.left(FILE_ADDRESS.indexOf("Work") + 10).append(read_month).append(".txt");
        qDebug() << read_month;
        //открываем файл read_month и вычитываем информацию об отработанных днях
    }
}



