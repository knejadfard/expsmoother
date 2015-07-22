#include <QtWidgets/QApplication>
#include "maindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDialog d;
    d.setWindowTitle("ExpSmoother (By Kian Nezhadfard)");
    d.show();
    
    return a.exec();
}
