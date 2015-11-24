#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDate>
#include <QString>
#include <QObject>


namespace ST {
static const char *ORG_NAME = "LingDong";
static const char *APP_NAME = "CountDown";

static const char *DATA = "data";
static const char *DATA_EXAMDATE = "data/examDate";
static const char *DATA_TEXT = "data/text";

static const char *ACTION = "action";
static const char *ACTION_FIRSTRUN = "action/firstRun";
static const char *ACTION_POS = "action/pos";
static const char *ACTION_OPACITY = "action/opacity";

static const char DEFAULT_SYMBOL = '%';
static const QString DEFAULT_TEXT = QObject::tr("%0 Days Before Entrance Exam").arg(DEFAULT_SYMBOL);
static const QDate DEFAULT_DATE(QDate::currentDate().year(), 6, 7);
static const double DEFAULT_OPACITY = 0.75;
}

#endif // SETTINGS_H
