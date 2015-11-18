#include "settingdialog.h"
#include <QSettings>

/// Read Settings
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setting = new QSettings("LingDong", "CountDown", this);
    reg = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                    QSettings::NativeFormat, this);

    // Read exam date
    calendar->setSelectedDate(setting->value("data/examDate",
                              QDate(QDate::currentDate().year(), 6, 7)).toDate());

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

    // Change startup state
    if (autoRunCheckBox->isChecked()) {
        reg->setValue("CountDown", QApplication::applicationFilePath().replace("/", "\\"));
    } else {
        reg->remove("CountDown");
    }
}
