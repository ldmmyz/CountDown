#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "ui_settingdialog.h"

class QSettings;

class SettingDialog : public QDialog, private Ui::SettingDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);

private slots:
    void on_buttonBox_accepted();

private:
    QSettings *setting, *reg;
};

#endif // SETTINGDIALOG_H
