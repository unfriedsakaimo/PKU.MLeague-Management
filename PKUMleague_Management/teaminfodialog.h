#ifndef TEAMINFODIALOG_H
#define TEAMINFODIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "DatabaseManager.h"

class TeamInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeamInfoDialog(QWidget *parent = nullptr);


private:
    void loadTeamData();
    QTableWidgetItem* createCenteredNumberItem(double value, int precision);

    QTableWidget *table;
    DatabaseManager& dbManager = DatabaseManager::instance();
};

#endif // TEAMINFODIALOG_H
