#include "form_statistics.h"
#include "ui_form_statistics.h"
#include "csavetofile.h"
#include "cworktime_main.h"

#include <QDebug>
#include <QMessageBox>

Form_statistics::Form_statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_statistics)
{
    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->plot->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->plot->setInteraction(QCP::iRangeZoom, false);
    ui->plot->setInteraction(QCP::iRangeDrag, false);
    ui->plot->yAxis->setLabel("Колличество часов");
    //ui->plot->yAxis->setLabelColor(Qt::blue);
    //ui->plot->graph(0)->setName("+/-");
    //ui->plot->legend->setVisible(true);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
    //ui->plot->setBackground(Qt::lightGray);
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    setPlot();
}

//bool Form_statistics::event(QEvent *event)
//{
//    if (event->type() == QEvent::MouseMove){
//        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
//        newMouseMove(mouseEvent->pos());
//        return true;
//    }
//    if(event->type() == QEvent::MouseButtonDblClick){
//        qDebug() << "Тут должна выйти справка";
//        QMessageBox::information(this, tr("Информация"), tr("График / Таблица"), QMessageBox::Ok);
//    }
//    return QWidget::event(event);
//}

//void Form_statistics::newMouseMove(QPoint newPoint)
//{
//    m_vectorMouseMove.append(newPoint);
//    m_timerMouseMove.start(200);
//}

Form_statistics::~Form_statistics()
{
    delete ui;
}

void Form_statistics::get_this_week(QVector<double> &vector_days)
{
    vector_days.clear();
    int this_day_of_week = QDate::currentDate().dayOfWeek();
    for(int i = 1; i <= this_day_of_week; i++){
        QDate date = QDate::currentDate().addDays(i - this_day_of_week);
        day_data dayData = CSaveToFile::get_day(date.day(), date.month());
        if(dayData.start == "" || dayData.finish == ""){
            vector_days.append(0);
        }
        else
        {
            QTime date_start = QTime::fromString(dayData.start, "HH:mm:ss");
            QTime date_finish = QTime::fromString(dayData.finish, "HH:mm:ss");
            vector_days.append(date_finish.hour() * 60 + date_finish.minute() - date_start.hour() * 60 - date_start.minute()
                             - CWorkTime_main::get_work_time_day_to_min(date.dayOfWeek()));
        }
    }
}

void Form_statistics::setPlot()
{
    QVector<double> vector_y;
    QVector<double> vector_y_singleDays;
    QVector<double> vector_x;
    //QVector<QString> vector_xString;
    get_this_week(vector_y);
    vector_y_singleDays = vector_y;
    double min, max;
    min = max = 0;
    ui->plot->graph(1)->setData(vector_x, vector_y);
    for(int i = 1; i < vector_y.size(); i++)
        vector_y[i] = vector_y[i] + vector_y[i - 1];
    double n = 1;
    for(int i = 0; i < vector_y.size(); i++){
        vector_y[i] = vector_y[i] / 60;
        vector_y_singleDays[i] = vector_y_singleDays[i] / 60;
        vector_x.append(n);
        n += 1;
        if(min > vector_y[i]) min = vector_y[i];
        if(min > vector_y_singleDays[i]) min = vector_y_singleDays[i];
        if(max < vector_y[i]) max = vector_y[i];
        if(max < vector_y_singleDays[i]) max = vector_y_singleDays[i];
        //vector_xString.append(CWorkTime_main::getDayOfWeek(QDate::currentDate().addDays(i - QDate::currentDate().dayOfWeek() + 1).toString("dd.MM")));
    }
    ui->plot->graph(0)->setData(vector_x, vector_y_singleDays);
    ui->plot->graph(1)->setData(vector_x, vector_y);
    ui->plot->xAxis->setAutoTicks(false);
    ui->plot->yAxis->setAutoTickStep(false);
    ui->plot->yAxis->setTickStep(0.5);
    ui->plot->xAxis->setRange(0.9, vector_x.last() + 0.1);
    ui->plot->yAxis->setRange(min - 0.5, max + 0.5);
    ui->plot->xAxis->setTickVector(vector_x);
    mDeltaTimeOfWeek = vector_y.last()*100/100;

    if(vector_y.last() < 0)
        ui->plot->xAxis->setLabel(QString(tr("Номер дня недели, на ")).append(QDate::currentDate().toString("dd.MM")).append(" ")
                                .append(QString::number(mDeltaTimeOfWeek)).append(tr(" часа")));
    else
        ui->plot->xAxis->setLabel(QString(tr("Номер дня недели, на ")).append(QDate::currentDate().toString("dd.MM")).append(" +")
                                .append(QString::number(mDeltaTimeOfWeek)).append(tr(" часа")));
    QPen pen_singleDay;
    pen_singleDay.setColor(QColor(0, 0, 100, 150));
    ui->plot->graph(0)->setPen(pen_singleDay);
    ui->plot->graph(0)->setBrush(QBrush(QColor(0,0,100,10)));
    ui->plot->graph(0)->setAntialiased(true);
    ui->plot->graph(0)->setAntialiasedFill(true);
    ui->plot->graph(0)->setAntialiasedScatters(true);
    ui->plot->graph(0)->setAdaptiveSampling(true);
    if(vector_y.last() < 0){
        QPen pen;
        pen.setColor(QColor(250, 10, 10, 50));
        ui->plot->graph(1)->setPen(pen);
        ui->plot->graph(1)->setBrush(QBrush(QColor(250,10,10,20)));
    }
    else{
        QPen pen;
        pen.setColor(QColor(10, 250, 10, 50));
        ui->plot->graph(1)->setPen(pen);
        ui->plot->graph(1)->setBrush(QBrush(QColor(10,250,10,20)));
    }
    ui->plot->replot();
}

void Form_statistics::show_form_statistics()
{
    QList<day_data> main_list;
    QList<QStandardItem*> listDayOfWeek_item;
    QList<QStandardItem*> listData_item;
    QList<QStandardItem*> listStart_item;
    QList<QStandardItem*> listFinish_item;
    QList<QStandardItem*> listDif_item;
    for(int i = -2; i < 1; i++){
        CSaveToFile::get_month(QDate::currentDate().addMonths(i).month(), QDate::currentDate().addMonths(i).year(), &main_list);
    }
    QStandardItemModel *model = new QStandardItemModel(main_list.size(), 5, this); //2 Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString(tr(""))));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString(tr("Дата"))));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString(tr("Начало"))));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString(tr("Конец"))));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString(tr("+/-"))));
    for(int i = 0; i < main_list.size(); i++){
        listDayOfWeek_item << new QStandardItem(CWorkTime_main::getDayOfWeek(main_list.at(i).data));
        listData_item << new QStandardItem(main_list.at(i).data);
        listStart_item << new QStandardItem(main_list.at(i).start);
        listFinish_item << new QStandardItem(main_list.at(i).finish);
        QString workTime = CWorkTime_main::get_work_time_day(main_list.at(i).data.left(2).toInt(), main_list.at(i).data.right(2).toInt());
        QString workedTime = CWorkTime_main::minusTime(&main_list.at(i).finish, &main_list.at(i).start);
        listDif_item << new QStandardItem(CWorkTime_main::minusTime(&workedTime, &workTime));
    }
    for(int i = 0; i < main_list.size(); i++){
        model->setItem(i, 0, listDayOfWeek_item.at(i));
        model->setItem(i, 1, listData_item.at(i));
        model->setItem(i, 2, listStart_item.at(i));
        model->setItem(i, 3, listFinish_item.at(i));
        model->setItem(i, 4, listDif_item.at(i));
    }

    ui->tableView->setModel(model);
    //ui->tableView->setShowGrid(true);
    //ui->tableView->setWordWrap(true);
    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 150);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 215);
    ui->stackedWidget->setCurrentIndex(1);
    this->show();
}

void Form_statistics::show_form_graph()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->show();
}

//void Form_statistics::onTimerMouseMove()
//{
//    if(m_vectorMouseMove.empty()) return;
//    //qDebug() << m_vectorMouseMove.last().x() - m_vectorMouseMove.at(0).x() << m_vectorMouseMove.back().y() - m_vectorMouseMove.at(0).y();
//    int x = m_vectorMouseMove.last().x() - m_vectorMouseMove.at(0).x();
//    int y = m_vectorMouseMove.last().y() - m_vectorMouseMove.at(0).y();
//    if(abs(x) > abs(y)){
//        if(x > 0){
//            if(abs(x)/2 > abs(y))   //Right
//                if(ui->stackedWidget->currentIndex() == 0)
//                    hide();
//        }
//        else{
//            if(abs(x)/2 > abs(y))   //Left
//                if(ui->stackedWidget->currentIndex() == 0)
//                    hide();
//        }
//    }
//    else{
//        if(y > 0){
//            if(abs(y)/2 > abs(x))   //Bottom
//                //emit signal_exit();
//                return;
//        }
//        else{
//            if(abs(y)/2 > abs(x))   //"Top
//                return;
//                //m_pSettings->show_form();
//        }
//    }
//    m_vectorMouseMove.clear();
//    m_timerMouseMove.stop();
//}
