#ifndef WIDGET_H
#define WIDGET_H

#include "csavetofile.h"
#include "cworktime_main.h"
#include "form_settings.h"
#include "form_statistics.h"

#include <QWidget>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class Widget;
}


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void signal_exit();

public slots:
    void slot_pushbutton_start();
    void slot_pushbutton_finish();
    void slot_timer_exit();
    void slot_timer_sec();
    void onTimerMouseMove();

private:
    Ui::Widget *ui;
    CSaveToFile toFile;
    day_data this_day;
    QTimer timer;
    QTimer timer_sec;
    QString day_work_time;
    Form_settings *m_pSettings;
    Form_statistics *m_pStatistics;
    QVector<QPoint> m_vectorMouseMove;
    QTimer m_timerMouseMove;

    void newMouseMove(QPoint newPoint);
    bool event(QEvent *event);
    void showInfoDbClick();
    void setStyleButtons(bool true_start); //true - start, false - finish
};

#endif // WIDGET_H
