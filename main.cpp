#include "SimpleTextEditor.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleTextEditor w;
    w.show();
    return a.exec();
}
