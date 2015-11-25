#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "ui_settingdialog.h"

class QSettings;

class SettingDialog : public QDialog, private Ui::SettingDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_opacitySlider_valueChanged(int value);
    void on_opacityDoubleSpinBox_valueChanged(double value);

private:
    QSettings *setting, *reg;
    const QString filePath;
    void readSetting();
};

#endif // SETTINGDIALOG_H
