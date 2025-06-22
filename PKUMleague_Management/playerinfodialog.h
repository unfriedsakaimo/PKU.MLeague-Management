#ifndef PLAYERINFODIALOG_H
#define PLAYERINFODIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "DatabaseManager.h"

class QVBoxLayout;
class QTableWidgetItem;

class PlayerInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerInfoDialog(QWidget *parent = nullptr);
    ~PlayerInfoDialog() = default;

private:
    void loadPlayerData();
    QTableWidgetItem* createCenteredNumberItem(double value, int precision);
    QTableWidgetItem* createCenteredItem(const QString &text);
    QTableWidgetItem* createTeamColorItem(int teamId, const QString &teamName);

    QTableWidget *table;
    DatabaseManager& dbManager = DatabaseManager::instance();
};

#endif // PLAYERINFODIALOG_H
