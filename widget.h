#ifndef WIDGET_H
#define WIDGET_H

#include "ui_widget.h"
#include <QDate>

class QSystemTrayIcon;
class QSettings;

class Widget : public QWidget, private Ui::Widget
{
    Q_OBJECT

private:
    QPoint dPos;
    QSystemTrayIcon *trayIcon;
    QSettings *setting;
    QMenu *trayIconMenu;
    QAction *quitAction;
    QAction *settingAction;
    QAction *aboutAction;
    QDate examDate;
    QTimer *timer;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void setTrayIcon();
    void setText();
    void readSetting();
    void showAbout();
    void showSetting();
    void firstRun();

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
