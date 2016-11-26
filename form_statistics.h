#ifndef FORM_STATISTICS_H
#define FORM_STATISTICS_H

#include "../qcustomplot/qcustomplot.h"

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>
#include <QTime>

namespace Ui {
class Form_statistics;
}

class Form_statistics : public QWidget
{
    Q_OBJECT

public:
    explicit Form_statistics(QWidget *parent = 0);
    ~Form_statistics();
    double mDeltaTimeOfWeek;

private:
    Ui::Form_statistics *ui;
    void get_this_week(QVector<double> &vector_days);
    void setPlot();
//    bool event(QEvent *event);
//    QTimer m_timerMouseMove;
//    QVector<QPoint> m_vectorMouseMove;

//    void newMouseMove(QPoint newPoint);

public slots:
    void show_form_statistics();
    void show_form_graph();

//private slots:
//    void onTimerMouseMove();
};

#endif // FORM_STATISTICS_H
