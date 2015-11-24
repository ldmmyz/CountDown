#include "settingdialog.h"
#include "settings.h"
#include <QSettings>
#include <QPushButton>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setting = new QSettings(ST::ORG_NAME, ST::APP_NAME, this);
    reg = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                    QSettings::NativeFormat, this);
    readSetting();

    textLineEdit->setToolTip(tr("Use %0 to represent the number generated automatically").arg(ST::DEFAULT_SYMBOL));

    buttonBox->button(QDialogButtonBox::Save)->setText(tr("Save"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText(tr("Restore Defaults"));
}

void SettingDialog::readSetting()
{
    calendar->setSelectedDate(setting->value(ST::DATA_EXAMDATE, ST::DEFAULT_DATE).toDate());
    textLineEdit->setText(setting->value(ST::DATA_TEXT, ST::DEFAULT_TEXT).toString());

    opacityDoubleSpinBox->setValue(setting->value(ST::ACTION_OPACITY, ST::DEFAULT_OPACITY).toDouble());

    // Read startup state
    if (reg->value(ST::APP_NAME).toString()
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
    setting->setValue(ST::DATA_EXAMDATE, calendar->selectedDate());
    setting->setValue(ST::DATA_TEXT, textLineEdit->text());
    setting->setValue(ST::ACTION_OPACITY, opacityDoubleSpinBox->value());

    // Change startup state
    if (autoRunCheckBox->isChecked()) {
        reg->setValue(ST::APP_NAME, QApplication::applicationFilePath().replace("/", "\\"));
    } else {
        reg->remove(ST::APP_NAME);
    }
}

/// Restore Defaults Settings
void SettingDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        bool isFirstRun = setting->value(ST::ACTION_FIRSTRUN, true).toBool();
        setting->remove(ST::DATA);
        setting->remove(ST::ACTION);
        reg->remove(ST::APP_NAME);
        setting->setValue(ST::ACTION_FIRSTRUN, isFirstRun);
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
