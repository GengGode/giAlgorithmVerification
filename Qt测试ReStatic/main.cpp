#include "QtReStatic.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtReStatic w;
    w.show();
    return a.exec();
}
