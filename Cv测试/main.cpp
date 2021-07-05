#include "Cv.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cv w;
    w.show();
    return a.exec();
}
