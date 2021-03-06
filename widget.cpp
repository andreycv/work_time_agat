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
    setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                  "stop:0 rgba(90, 90, 90, 150),"
                  "stop:1 rgba(90, 90, 90, 150));");
    QPalette paletteMain = ui->lcdNumber_start->palette();
    paletteMain.setColor(paletteMain.WindowText, QColor(200, 200, 200));
    paletteMain.setColor(paletteMain.Background, QColor(20, 20, 20, 200));
    paletteMain.setColor(paletteMain.Light, QColor(220, 220, 220));
    paletteMain.setColor(paletteMain.Dark, QColor(170, 170, 170));
    ui->lcdNumber_start->setPalette(paletteMain);
    ui->lcdNumber_finish->setPalette(paletteMain);
    QPalette palette = ui->lcdNumber_stat->palette();
    palette.setColor(palette.WindowText, QColor(130, 130, 130));
    palette.setColor(palette.Background, QColor(20, 20, 20, 200));
    palette.setColor(palette.Light, QColor(150, 150, 150));
    palette.setColor(palette.Dark, QColor(110, 110, 110));
    ui->lcdNumber_end->setPalette(palette);
    ui->lcdNumber_stat->setPalette(palette);
    m_pSettings = new Form_settings();
    m_pStatistics = new Form_statistics();
    m_speech = new QTextToSpeech(this);
    m_speech->setPitch(-0.1);

    day_work_time = CWorkTime_main::get_work_time_day(QDate::currentDate().day(), QDate::currentDate().month());

    this_day = toFile.get_this_day();

    //Начало дня
    if(this_day.start == ""){
        //QPixmap img(IMAGE1_ADDRESS);
        //ui->pushButton->setIcon(img);
        //ui->pushButton->setIconSize(img.size());
        setStyleButtons(true);
        ui->lcdNumber_start->hide();
        ui->lcdNumber_finish->hide();
        ui->lcdNumber_end->hide();
        ui->lcdNumber_stat->hide();

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
    QString DeltaTime = QString::number(m_pStatistics->mDeltaTimeOfWeek);
    if(DeltaTime.at(0) == '-' && DeltaTime.size() > 4)
        DeltaTime = DeltaTime.mid(0, 5);
    else
        DeltaTime = DeltaTime.mid(0, 4);
    if(m_pStatistics->mDeltaTimeOfWeek < 0)
        DeltaTime = QString("\nнедоработка ").append(DeltaTime).append(" часов").append(".\nНе унывай,\nвсе получится!");
    else
        DeltaTime = QString("\nпереработка +").append(DeltaTime).append(" часов").append(".\nОтличного дня,\nты молодец!");




    int randI = QTime::currentTime().second() % 3;
    m_speech->setRate(0);
    if(randI == 2){
        m_speech->setPitch(0.4);
        m_speech->say(CWorkTime_main::getJoke());
    }
    else
    {
        m_speech->setPitch(-0.1);
        int delta = static_cast<int>(m_pStatistics->mDeltaTimeOfWeek * 100);
        bool isMines = false;
        if(delta < 0){
            isMines = true;
            delta *= -1;
        }
        QString say = QString(tr("Сегодня ")).append(CWorkTime_main::getDayOfWeekFull());
        if(isMines){
            say.append(", недоработка ");
        }
        else say.append(", переработка ");
        int deltaMin = (delta % 100) * 60 / 100;
        delta = delta / 100;
        switch(delta){
        case 0: break;
        case 1: say.append(QString::number(delta)).append(" час ");
            break;
        case 2: say.append(QString::number(delta)).append(" часа ");
            break;
        case 3: say.append(QString::number(delta)).append(" часа ");
            break;
        case 4: say.append(QString::number(delta)).append(" часа ");
            break;
        default: say.append(QString::number(delta)).append(" часов ");
            break;
        }
        switch(deltaMin){
        case 0: if(delta != 0) {say.append("0 минут ");}
            else {say = QString(tr("Сегодня ")).append(CWorkTime_main::getDayOfWeekFull());}
            break;
        case 1: say.append(QString::number(deltaMin)).append(" минута ");
            break;
        case 2: say.append(QString::number(deltaMin)).append(" минуты ");
            break;
        case 3: say.append(QString::number(deltaMin)).append(" минуты ");
            break;
        case 4: say.append(QString::number(deltaMin)).append(" минуты ");
            break;
        default: say.append(QString::number(deltaMin)).append(" минут ");
            break;
        }
        m_speech->say(say.append(", хорошего дня"));
    }
    QMessageBox::information(this, tr("Инфо"),
                             QString("На ").append(QDate::currentDate().toString("dd.MM")).append(DeltaTime),
                             QMessageBox::Ok);
}

void Widget::setStyleButtons(bool true_start)
{
    ui->pushButton->setMinimumHeight(300);
//    this->setStyleSheet(QString::fromUtf8(
//        "QWidget {"
//                     "background-color: rgba(10, 10, 10, 250));}"
//                                      ));

//    QGridLayout layout;
//    layout.addWidget(ui->pushButton);
//    setLayout(&layout);
    if(true_start)
    {
        ui->pushButton->setStyleSheet(QString::fromUtf8(
            "QPushButton {"
                         "font: bold;"
                         "border-style: flat;"
                         "border-radius: 20px;"
                         "padding: 30px;"
                         "background-color: qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                                            "stop:0 rgba(100, 100, 100, 10),"
                                            "stop:1 rgba(10, 10, 10, 250));}"
            "QPushButton:hover {"
                               "background-color: qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                                                                 "stop:0 rgba(70, 70, 70, 150),"
                                                                 "stop:1 rgba(10, 10, 10, 250));"
                               "border-style: inset;"
                               "border-width: 3px;"
                               "border-color: grey;}"
            "QPushButton:pressed {"
                                 "border-style: inset;"
                                 "border-width: 1px;"
                                 "border-color: grey;}"
            ));
    }
    else
    {
        ui->pushButton->setStyleSheet(QString::fromUtf8(
            "QPushButton {"
                         "font: bold;"
                         "border-style: flat;"
                         "border-radius: 20px;"
                         "padding: 30px;"
                         "background-color: qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                                            "stop:0 rgba(110, 110, 110, 10),"
                                            "stop:1 rgba(30, 30, 30, 250));}"
            "QPushButton:hover {"
                               "background-color: qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                                                                 "stop:0 rgba(70, 70, 70, 150),"
                                                                 "stop:1 rgba(10, 10, 10, 250));"
                               "border-style: inset;"
                               "border-width: 3px;"
                               "border-color: grey;}"
            "QPushButton:pressed {"
                                 "border-style: inset;"
                                 "border-width: 1px;"
                                 "border-color: grey;}"
            ));
    }
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
    ui->pushButton->hide();
    ui->lcdNumber_start->show();
    ui->lcdNumber_end->show();
    ui->lcdNumber_stat->hide();
    timer.start(3000);

    m_speech->setPitch(0.1);
    m_speech->setRate(0.1);
    if(QTime::currentTime().hour() > 9)
        m_speech->say("Ты сегодня позднее, но это не беда, ты всеравно молодец");
    else
        m_speech->say("ok, я отметила начало дня");
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
    timer.start(3000);
    m_speech->setPitch(0.1);
    m_speech->setRate(0.1);
    m_speech->say("я отметила окончание дня, ты молодец, пока.");
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
