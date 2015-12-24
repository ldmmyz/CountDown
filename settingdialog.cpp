#include "settingdialog.h"
#include "widget.h"
#include "settings.h"
#include <QSettings>
#include <QPushButton>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    filePath(QApplication::applicationFilePath().replace('/', '\\'))
{
    setupUi(this);

    setting = new QSettings(St::OrgName, St::AppName, this);
    reg = new QSettings(R"(HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run)",
                        QSettings::NativeFormat, this);
    readSetting();

    textLineEdit->setToolTip(tr("Use %0 to represent the number generated automatically").arg(St::Default_Symbol));

    buttonBox->button(QDialogButtonBox::Save)->setText(tr("Save"));
    buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText(tr("Restore Defaults"));
}

void SettingDialog::readSetting()
{
    calendar->setSelectedDate(setting->value(St::Date_ExamDate, St::Default_Date).toDate());
    textLineEdit->setText(
                setting->value(
                    St::Date_Text,
                    tr("%0 Days Before Entrance Exam").arg(St::Default_Symbol)
                ).toString());
    opacityDoubleSpinBox->setValue(setting->value(St::Action_Opacity, St::Default_Opacity).toDouble());

    // Read startup state
    if (reg->value(St::AppName).toString() == filePath) {
        autoRunCheckBox->setChecked(true);
    } else {
        autoRunCheckBox->setChecked(false);
    }
}

/// Save new Settings
void SettingDialog::on_buttonBox_accepted()
{
    // Set exam date
    setting->setValue(St::Date_ExamDate, calendar->selectedDate());
    setting->setValue(St::Date_Text, textLineEdit->text());
    setting->setValue(St::Action_Opacity, opacityDoubleSpinBox->value());

    // Change startup state
    if (autoRunCheckBox->isChecked()) {
        if (reg->value(St::AppName).toString() != filePath) {
            reg->setValue(St::AppName, filePath);
        }
    } else {
        reg->remove(St::AppName);
    }
}

/// Restore Defaults Settings
void SettingDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        bool isFirstRun = setting->value(St::Action_FirstRun, true).toBool();
        setting->remove(St::Date);
        setting->remove(St::Action);
        reg->remove(St::AppName);
        setting->setValue(St::Action_FirstRun, isFirstRun);
        readSetting(); // Read Default Settings
        Widget *p = dynamic_cast<Widget*>(parentWidget());
        if (p)
            p->readSetting();
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
