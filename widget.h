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
    QString text;
    QTimer *timer;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void setTrayIcon();
    void setText();
    void readSetting();
    void showAbout();
    void showSetting();
    void firstRun();

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
};

#endif // WIDGET_H
