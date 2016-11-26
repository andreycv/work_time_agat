#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //work_time_agat
    QApplication a(argc, argv);
    Widget w;
    w.show();
    QObject::connect(&w, SIGNAL(signal_exit()), &a, SLOT(quit()));

    return a.exec();
}
