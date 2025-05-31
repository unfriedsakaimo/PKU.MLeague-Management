#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "DatabaseManager.h"

namespace Ui {
class ScheduleDialog;
}

class ScheduleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleDialog(QWidget *parent = nullptr);
    ~ScheduleDialog();

private:
    Ui::ScheduleDialog *ui;
    void loadSchedule();
    QTableWidget *table;
    DatabaseManager& dbManager = DatabaseManager::instance();
};

#endif // SCHEDULEDIALOG_H
