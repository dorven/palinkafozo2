#include "palinkafozo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Palinkafozo w;
    w.show();

    return a.exec();
}
