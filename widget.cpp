#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QMenu>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("Время Агат"));
    m_pSettings = new Form_settings();
    m_pStatistics = new Form_statistics();

    day_work_time = CWorkTime_main::get_work_time_day(QDate::currentDate().day(), QDate::currentDate().month());

    this_day = toFile.get_this_day();
    ui->label_void->hide();

    //Начало дня
    if(this_day.start == ""){
        //QPixmap img(IMAGE1_ADDRESS);
        //ui->pushButton->setIcon(img);
        //ui->pushButton->setIconSize(img.size());
        setStyleButtons(true);

        //ui->label_start->hide();
        //ui->label_finish->hide();
        ui->lcdNumber_start->hide();
        ui->lcdNumber_finish->hide();
        ui->lcdNumber_end->hide();
        ui->lcdNumber_stat->hide();
        //ui->label_stat->hide();
        ui->label_void->show();

        connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_start()));
    }
    //Завершение дня
    else if(this_day.finish == ""){
        //QPixmap img(IMAGE2_ADDRESS);
        //ui->pushButton->setIcon(img);
        //ui->pushButton->setIconSize(img.size());
        setStyleButtons(false);

        ui->lcdNumber_start->display(this_day.start);
        ui->lcdNumber_end->display(CWorkTime_main::plusTime(&this_day.start, &day_work_time));
        slot_timer_sec();
        timer_sec.start(1000);
        QObject::connect(&timer_sec, SIGNAL(timeout()), this, SLOT(slot_timer_sec()));
        ui->lcdNumber_end->show();
        ui->lcdNumber_stat->show();
        //ui->label_stat->show();
        //ui->label_finish->hide();
        ui->lcdNumber_finish->hide();

        connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(slot_pushbutton_finish()));
    }
    //Рабочий день уже завершен
    else{
        ui->pushButton->hide();
        ui->lcdNumber_start->display(this_day.start);
        ui->lcdNumber_finish->display(this_day.finish);
        QString worked_time = CWorkTime_main::minusTime(&this_day.finish, &this_day.start);
        ui->lcdNumber_end->display(worked_time);
        ui->lcdNumber_stat->display(CWorkTime_main::minusTime(&worked_time, &day_work_time));
    }
    connect(&timer, SIGNAL(timeout()), this, SLOT(slot_timer_exit()));

    //Делаем нижние циферблаты серыми
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::gray);
    ui->lcdNumber_stat->setPalette(pal);
    ui->lcdNumber_end->setPalette(pal);

    connect(&m_timerMouseMove, &QTimer::timeout, this, &Widget::onTimerMouseMove);
    this->setFocus();
}

bool Widget::event(QEvent *event)
{
    if (event->type() == QEvent::MouseMove){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        newMouseMove(mouseEvent->pos());
        return true;
    }
    if(event->type() == QEvent::MouseButtonDblClick){
        showInfoDbClick();
    }
    return QWidget::event(event);
}

void Widget::showInfoDbClick()
{
//    QMessageBox::information(this, tr("Справка"), tr("Сверху - виджет изменения времени.\r\n"
//                            "Снизу - завершить приложение.\r\n"
//                            "Слева - отчетная таблица за последние 3 месяца.\r\n"
//                            "Справа - отчетный график за текущую неделю.\r\n"
//                                                     "На других виджетах жесты не работают."), QMessageBox::Ok);
    QString DeltaTime = QString::number(m_pStatistics->mDeltaTimeOfWeek);
    if(DeltaTime.size() > 4){
        DeltaTime = DeltaTime.mid(0, 4);
    }
    if(m_pStatistics->mDeltaTimeOfWeek < 0)
        DeltaTime = QString("\nнедоработка ").append(DeltaTime).append(" часов").append(".\nНе унывай,\nвсе получится!");
    else
        DeltaTime = QString("\nпереработка +").append(DeltaTime).append(" часов").append(".\nОтличного дня,\nты молодец!");
    QMessageBox::information(this, tr("Инфо"),
                             QString("На ").append(QDate::currentDate().toString("dd.MM")).append(DeltaTime),
                             QMessageBox::Ok);
}

void Widget::setStyleButtons(bool true_start)
{
    ui->pushButton->setMinimumHeight(300);
    if(true_start)
        ui->pushButton->setStyleSheet(QString::fromUtf8(
            "QPushButton {image: url(/mnt/sdcard/work_time/1.jpg);"
                         "font: bold;"
                         "border-style: flat;"
                         "border-radius: 3px;"
                         "padding: 6px;"
                         "background-color: qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                                            "stop:0 rgba(20, 255, 20, 0),"
                                            "stop:1 rgba(20, 255, 20, 70));}"
            "QPushButton:hover {image: url(/mnt/sdcard/work_time/1.jpg);"
                               "background-color: qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                                                                 "stop:0 rgba(0, 255, 0, 180),"
                                                                 "stop:1 rgba(0, 255, 0, 30));"
                               "border-style: inset;"
                               "border-width: 1px;"
                               "border-color: grey;}"
            "QPushButton:pressed {image: url(/mnt/sdcard/work_time/1.jpg);"
                                 "border-style: inset;"
                                 "border-width: 2px;"
                                 "border-color: grey;}"
            ));
    else
        ui->pushButton->setStyleSheet(QString::fromUtf8(
            "QPushButton {image: url(/mnt/sdcard/work_time/2.jpg);"
                         "font: bold;"
                         "border-style: flat;"
                         "border-radius: 3px;"
                         "padding: 6px;"
                         "background-color: qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                                            "stop:0 rgba(20, 255, 20, 0),"
                                            "stop:1 rgba(20, 255, 20, 70));}"
            "QPushButton:hover {image: url(/mnt/sdcard/work_time/2.jpg);"
                               "background-color: qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                                                                 "stop:0 rgba(55, 255, 55, 180),"
                                                                 "stop:1 rgba(55, 255, 55, 30));"
                               "border-style: inset;"
                               "border-width: 1px;"
                               "border-color: grey;}"
            "QPushButton:pressed {image: url(/mnt/sdcard/work_time/2.jpg);"
                                 "border-style: inset;"
                                 "border-width: 2px;"
                                 "border-color: grey;}"
            ));
}

Widget::~Widget()
{
    delete m_pSettings;
    delete m_pStatistics;
    delete ui;
}

void Widget::slot_pushbutton_start()
{
    toFile.write_start();
    ui->lcdNumber_start->display(QTime::currentTime().toString("HH:mm:ss"));
    QString now = QTime::currentTime().toString("HH:mm:ss");
    ui->lcdNumber_end->display(CWorkTime_main::plusTime(&now, &day_work_time));
    //timer_sec.start(1000);
    //connect(&timer_sec, SIGNAL(timeout()), this, SLOT(slot_timer_sec()));
    ui->pushButton->hide();
    //ui->label_start->show();
    ui->lcdNumber_start->show();
    ui->lcdNumber_end->show();
    ui->lcdNumber_stat->hide();
    //ui->label_stat->show();
    ui->label_void->hide();
    timer.start(2000);
}

void Widget::slot_pushbutton_finish()
{
    QString ending = CWorkTime_main::plusTime(&this_day.start, &day_work_time);
    QString now = QTime::currentTime().toString("HH:mm:ss");
    ending = CWorkTime_main::minusTime(&now, &ending);
    if(ending.left(1) == "-"){
        if(QMessageBox::warning(this, tr("Внимание"),
                                    tr("Ваш рабочий день еще не закончен.\n"
                                       "Вы уверены что хотите уйти пораньше?"),
                                    QMessageBox::Yes        // YES - 65536  NO - 16384
                                    | QMessageBox::No) == 65536) return;
    }
    else
    {
        if(QMessageBox::information(this, tr("Отличный день"),
                                     tr("Сохранить информацию об уходе с работы?"),
                                     QMessageBox::Save      // SAVE - 4194304
                                     | QMessageBox::Cancel) == 4194304) return;
    }

    toFile.write_finish();
    ui->lcdNumber_finish->display(QTime::currentTime().toString("HH:mm:ss"));
    ui->pushButton->hide();
    //ui->label_finish->show();
    ui->lcdNumber_finish->show();
    //QString now = QTime::currentTime().toString("HH:mm:ss");
    QString worked_time = CWorkTime_main::minusTime(&now, &this_day.start);
    ui->lcdNumber_end->display(worked_time);
    ui->lcdNumber_stat->display(CWorkTime_main::minusTime(&worked_time, &day_work_time));
    disconnect(&timer_sec, SIGNAL(timeout()), this, SLOT(slot_timer_sec()));
    timer.start(2000);
}

void Widget::slot_timer_exit()
{
    emit signal_exit();
}

void Widget::slot_timer_sec()
{
    QString end = CWorkTime_main::plusTime(&this_day.start, &day_work_time);
    QString now = QTime::currentTime().toString("HH:mm:ss");
    ui->lcdNumber_stat->display(CWorkTime_main::minusTime(&now, &end));
}

void Widget::onTimerMouseMove()
{
    if(m_vectorMouseMove.empty()) return;
    //qDebug() << m_vectorMouseMove.last().x() - m_vectorMouseMove.at(0).x() << m_vectorMouseMove.back().y() - m_vectorMouseMove.at(0).y();
    int x = m_vectorMouseMove.last().x() - m_vectorMouseMove.at(0).x();
    int y = m_vectorMouseMove.last().y() - m_vectorMouseMove.at(0).y();
    if(abs(x) > abs(y)){
        if(x > 0){
            if(abs(x)/2 > abs(y))   //Right
                m_pStatistics->show_form_statistics();
        }
        else{
            if(abs(x)/2 > abs(y))   //Left
                m_pStatistics->show_form_graph();
        }
    }
    else{
        if(y > 0){
            if(abs(y)/2 > abs(x))   //Bottom
                m_pSettings->show_form();
        }
        else{
            if(abs(y)/2 > abs(x))   //"Top
                emit signal_exit();
        }
    }
    m_vectorMouseMove.clear();
    m_timerMouseMove.stop();
}

void Widget::newMouseMove(QPoint newPoint)
{
    m_vectorMouseMove.append(newPoint);
    m_timerMouseMove.start(200);
}
