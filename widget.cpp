#include "widget.h"
#include "settings.h"
#include "settingdialog.h"
#include <QScopedPointer>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QDesktopWidget>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    QIcon icon(":/icon/icon");
    setWindowIcon(icon);

    settingAction = new QAction(tr("Preference (&S)"), this);
    aboutAction = new QAction(tr("About (&A)"), this);
    quitAction = new QAction(tr("Quit (&Q)"), this);
    connect(settingAction, &QAction::triggered, this, &Widget::showSetting);
    connect(aboutAction, &QAction::triggered, this, &Widget::showAbout);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    // Set system tray icon
    trayIcon = new QSystemTrayIcon(icon, this);
    trayIcon->show();
    trayIcon->setToolTip(tr("Count Down"));
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(settingAction);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);

    setting = new QSettings(ST::ORG_NAME, ST::APP_NAME, this);
    readSetting();
    firstRun();

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool); // no window border, no taskbar icon

    // Update every minute
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::setText);
    timer->start(5000);
}

Widget::~Widget()
{
    setting->setValue(ST::ACTION_POS, pos());
}

/// Make it possible to drag the window without titlebar
void Widget::mousePressEvent(QMouseEvent *event)
{
    dPos = event->globalPos() - this->pos();
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - dPos);
}

void Widget::setText()
{
    label->setText(text.replace(ST::DEFAULT_SYMBOL, QString::number(QDate::currentDate().daysTo(examDate))));
}

void Widget::readSetting()
{
    examDate = setting->value(ST::DATA_EXAMDATE, ST::DEFAULT_DATE).toDate();
    text = setting->value(ST::DATA_TEXT, ST::DEFAULT_TEXT).toString();
    setWindowOpacity(setting->value(ST::ACTION_OPACITY, ST::DEFAULT_OPACITY).toDouble());
    setText();
    adjustSize();
    // Move to the position last time
    // If it's the first execution, move to the rightside
    move(setting->value(ST::ACTION_POS,
                        QPoint(QApplication::desktop()->width() - width() - 35, 35)).toPoint());
}

void Widget::showAbout()
{
    QMessageBox::about(this, tr("About"),
                       tr("<h1>Count Down</h1><p><strong>Copyright 2015 <a href='http://ld.mmyz.net/'>LingDong Computer Society</a></strong><br/>This is an opensource software under The MIT License.</p><p>Home Page: <a href='http://github.com/ziqin/CountDown'>http://github.com/ziqin/CountDown</a></p>"));
}

void Widget::showSetting()
{
    QScopedPointer<SettingDialog> settingDialog(new SettingDialog(this));
    if (settingDialog->exec() == QDialog::Accepted)
        readSetting();
}

/// Show Setting Dialog & Tip Message if it's the first execution
void Widget::firstRun()
{
    if (setting->value(ST::ACTION_FIRSTRUN, true).toBool()) {
        showSetting();
        trayIcon->showMessage(tr("Tip"), tr("Find me here next time!"));
        setting->setValue(ST::ACTION_FIRSTRUN, false);
    }
}
