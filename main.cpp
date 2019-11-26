#include "mainwindow.h"
#include <QApplication>
#include "mytool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setOverrideCursor(Qt::BlankCursor);
    MainWindow w;
    w.show();
    return a.exec();
}
