#include "widget.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator appTr;
    appTr.load(QLocale::system(), "CountDown", "_", ":/translation");
    a.installTranslator(&appTr);
    Widget w;
    w.show();

    return a.exec();
}
