#include "widget.h"
#include "settings.h"
#include "settingdialog.h"
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QDesktopWidget>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    settingDialog(nullptr)
{
    setupUi(this);

    QIcon icon(":/icon/icon");
    setWindowIcon(icon);

    settingAction = new QAction(tr("Preference (&S)"), this);
    aboutAction = new QAction(tr("About (&A)"), this);
    quitAction = new QAction(tr("Quit (&Q)"), this);
    connect(settingAction, &QAction::triggered, this, &Widget::showSetting);
    connect(aboutAction, &QAction::triggered, [this]{
        QMessageBox::about(this, tr("About"),
            tr("<h1>Count Down</h1><p><strong>Copyright 2015 "
            "<a href='http://ld.mmyz.net/'>LingDong Computer Society</a></strong>"
            "<br>This is an opensource software under The MIT License.</p>"
            "<p>Home Page: <a href='https://github.com/ziqin/CountDown'>"
            "https://github.com/ziqin/CountDown</a></p>"));
    });
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

    setting = new QSettings(St::OrgName, St::AppName, this);
    readSetting();
    firstRun();

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool); // no window border, no taskbar icon

    // Update once every 10 seconds
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::setText);
    timer->start(10000);
}

Widget::~Widget()
{
    setting->setValue(St::Action_Pos, pos());
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
    label->setText(QString(text).replace(St::Default_Symbol, QString::number(QDate::currentDate().daysTo(examDate))));
}

void Widget::readSetting()
{
    examDate = setting->value(St::Date_ExamDate, St::Default_Date).toDate();
    text = setting->value(
                St::Date_Text,
                tr("%0 Days Before Entrance Exam").arg(St::Default_Symbol)
           ).toString();
    setWindowOpacity(setting->value(St::Action_Opacity, St::Default_Opacity).toDouble());
    setText();
    adjustSize();
    // Move to the position last time
    // If it's the first execution, move to the rightside
    move(setting->value(St::Action_Pos,
                        QPoint(QApplication::desktop()->width() - width() - 35, 35)).toPoint());
}

void Widget::showSetting()
{
    if (!settingDialog) {
        settingDialog = new SettingDialog(this);
        if (settingDialog->exec() == QDialog::Accepted)
            readSetting();
        delete settingDialog;
        settingDialog = nullptr;
    } else {
        settingDialog->raise();
    }
}

/// Show Setting Dialog & Tip Message if it's the first execution
void Widget::firstRun()
{
    if (setting->value(St::Action_FirstRun, true).toBool()) {
        showSetting();
        trayIcon->showMessage(tr("Tip"), tr("Find me here next time!"));
        setting->setValue(St::Action_FirstRun, false);
    }
}
