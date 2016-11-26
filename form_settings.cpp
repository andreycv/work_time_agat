#include "form_settings.h"
#include "ui_form_settings.h"

Form_settings::Form_settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_settings)
{
    ui->setupUi(this);
    connect(ui->pushButton_save_change, &QPushButton::clicked, this, &Form_settings::slot_rewrite_data);

    setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.517, y1:0, x2:0.511, y2:1,"
                  "stop:0 rgba(150, 150, 150, 200),"
                  "stop:1 rgba(150, 150, 150, 200));");
}

Form_settings::~Form_settings()
{
    delete ui;
}

void Form_settings::show_form()
{
    day_data dayData = CSaveToFile::get_this_day();

    if(dayData.start == ""){
        ui->lineEdit_start->setVisible(false);
        ui->lineEdit_finish->setVisible(false);
        ui->pushButton_save_change->setVisible(false);
    }
    else if(dayData.finish == ""){
        ui->lineEdit_start->setText(dayData.start);
        ui->lineEdit_finish->setVisible(false);
    }
    else{
        ui->lineEdit_start->setText(dayData.start);
        ui->lineEdit_finish->setText(dayData.finish);
    }
    this->show();
}

void Form_settings::slot_rewrite_data()
{
    if(ui->lineEdit_start->text().size() != 8){
        QMessageBox::warning(this, tr("Некорректные данные"),
                                tr("Пожалуйста, убедитесь в корректности вводимых данных о начале дня"));
        return;
    }
    if((ui->lineEdit_finish->text() != "") && (ui->lineEdit_finish->text().size() != 8)){
        QMessageBox::warning(this, tr("Некорректные данные"),
                                tr("Пожалуйста, убедитесь в корректности вводимых данных об окончании дня"));
        return;
    }
    CSaveToFile::rewrite_this_day(ui->lineEdit_start->text(), ui->lineEdit_finish->text());
    QMessageBox::information(this, tr("Изменения внесены"),
                            tr("Пожалуйста, перезагрузите приложение что бы увидеть изменения"));
    this->hide();
}

//void Form_settings::onTimerMouseMove()
//{
//    if(m_vectorMouseMove.empty()) return;
//    //qDebug() << m_vectorMouseMove.last().x() - m_vectorMouseMove.at(0).x() << m_vectorMouseMove.back().y() - m_vectorMouseMove.at(0).y();
//    int x = m_vectorMouseMove.last().x() - m_vectorMouseMove.at(0).x();
//    int y = m_vectorMouseMove.last().y() - m_vectorMouseMove.at(0).y();
//    if(abs(x) > abs(y)){
//        if(x > 0){
//            if(abs(x)/2 > abs(y))   //Right
//                return;
//        }
//        else{
//            if(abs(x)/2 > abs(y))   //Left
//                return;
//        }
//    }
//    else{
//        if(y > 0){
//            if(abs(y)/2 > abs(x))   //Bottom
//                slot_rewrite_data();
//        }
//        else{
//            if(abs(y)/2 > abs(x))   //"Top
//                hide();
//        }
//    }
//    m_vectorMouseMove.clear();
//    m_timerMouseMove.stop();
//}

//void Form_settings::newMouseMove(QPoint newPoint)
//{
//    m_vectorMouseMove.append(newPoint);
//    m_timerMouseMove.start(200);
//}
