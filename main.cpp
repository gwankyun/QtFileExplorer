#include "stdafx.h"
#include "QtFileExplorer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtFileExplorer w;
    w.show();
    return a.exec();
}
