#include "widget.h"
#include "settingdialog.h"
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

    setting = new QSettings("LingDong", "CountDown", this);
    readSetting();
    firstRun();

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool); // no window border, no taskbar icon
    setWindowOpacity(0.75); // Set transparency
    move(QApplication::desktop()->width()-this->width() - 35, 35); // move to rightside

    // Update every minute
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::setText);
    timer->start(60000);
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
    label->setText(QString(tr("%0+2Days Before Summer Holiday")).arg(QDate::currentDate().daysTo(examDate)));
}

void Widget::readSetting()
{
    examDate = setting->value("data/examDate", QDate(QDate::currentDate().year(), 6, 7)).toDate();
    setText();
}

void Widget::showAbout()
{
    QMessageBox::about(this, tr("About"),
                       tr("<h2>Count Down</h2><p><a href='http://ld.mmyz.net/'>LingDong Computer Society</a> 2015</p>"));
}

void Widget::showSetting()
{
    SettingDialog *settingDialog = new SettingDialog(this);
    if (settingDialog->exec() == QDialog::Accepted)
        readSetting();
    delete settingDialog;
}

void Widget::firstRun()
{
    if (setting->value("action/firstRun", true).toBool()) {
        showSetting();
        trayIcon->showMessage(tr("Tip"), tr("Find me here next time"));
        setting->setValue("action/firstRun", false);
    }
}
