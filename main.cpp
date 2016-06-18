#include "shadow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Shadow w;
    w.show();

    return a.exec();
}
