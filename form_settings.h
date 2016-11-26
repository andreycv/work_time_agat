#ifndef FORM_SETTINGS_H
#define FORM_SETTINGS_H

#include "csavetofile.h"
#include <QWidget>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class Form_settings;
}

class Form_settings : public QWidget
{
    Q_OBJECT

public:
    explicit Form_settings(QWidget *parent = 0);
    ~Form_settings();

private:
    Ui::Form_settings *ui;
//    QVector<QPoint> m_vectorMouseMove;
//    QTimer m_timerMouseMove;

//    bool event(QEvent *event);
//    void newMouseMove(QPoint newPoint);

public slots:
    void show_form();

private slots:
    void slot_rewrite_data();
};

#endif // FORM_SETTINGS_H
