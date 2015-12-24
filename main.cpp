#include "widget.h"
#include "settings.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator appTr;
    appTr.load(QLocale::system(), St::AppName, "_", ":/translation");
    a.installTranslator(&appTr);
    Widget w;
    w.show();

    return a.exec();
}
