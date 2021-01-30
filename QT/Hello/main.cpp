#include "dialog.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog d;
    d.show();

    return a.exec();

}
