#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDate>
#include <QString>

namespace St {
const char * const OrgName = "LingDong";
const char * const AppName = "CountDown";

const char * const Date = "data";
const char * const Date_ExamDate = "data/examDate";
const char * const Date_Text = "data/text";

const char * const Action = "action";
const char * const Action_FirstRun = "action/firstRun";
const char * const Action_Pos = "action/pos";
const char * const Action_Opacity = "action/opacity";

const char Default_Symbol = '%';
const QDate Default_Date(QDate::currentDate().year(), 6, 7);
const double Default_Opacity = 0.75;
}

#endif // SETTINGS_H
