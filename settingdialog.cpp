#include "settingdialog.h"
#include <QSettings>
#include <QPushButton>

/// Read Settings
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setting = new QSettings("LingDong", "CountDown", this);
    reg = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                    QSettings::NativeFormat, this);
    readSetting();

    buttonBox->button(QDialogButtonBox::Save)->setText(tr("Save"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText(tr("Restore Defaults"));
}

void SettingDialog::readSetting()
{
    calendar->setSelectedDate(setting->value("data/examDate",
                              QDate(QDate::currentDate().year(), 6, 7)).toDate());
    textLineEdit->setText(setting->value("data/text", tr("% Days Before Entrance Exam")).toString());

    opacityDoubleSpinBox->setValue(setting->value("action/opacity", 0.75).toDouble());

    // Read startup state
    if (reg->value("CountDown").toString()
            == QApplication::applicationFilePath().replace("/", "\\")) {
        autoRunCheckBox->setChecked(true);
    } else {
        autoRunCheckBox->setChecked(false);
    }
}

/// Save new Settings
void SettingDialog::on_buttonBox_accepted()
{
    // Set exam date
    setting->setValue("data/examDate", calendar->selectedDate());
    setting->setValue("data/text", textLineEdit->text());
    setting->setValue("action/opacity", opacityDoubleSpinBox->value());

    // Change startup state
    if (autoRunCheckBox->isChecked()) {
        reg->setValue("CountDown", QApplication::applicationFilePath().replace("/", "\\"));
    } else {
        reg->remove("CountDown");
    }
}

/// Restore Defaults Settings
void SettingDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        setting->remove("data");
        setting->remove("action");
        reg->remove("CountDown");
        setting->setValue("action/firstRun", false);
        readSetting(); // Read Default Settings
    }
}

/// Sync opacitySlider to opacityDoubleSpinBox
void SettingDialog::on_opacitySlider_valueChanged(int value)
{
    opacityDoubleSpinBox->setValue(value * 0.01);
}

/// Sync opacityDoubleSpinBox to opacitySlider
void SettingDialog::on_opacityDoubleSpinBox_valueChanged(double value)
{
    opacitySlider->setValue(int(value * 100));
}
