#include "dropmatrixmatcher.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DropMatrixMatcher window;
    window.show();

    return a.exec();
}
