#include "lissajous.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lissajous w;
    w.show();

    return a.exec();
}
