#ifndef MANAGEGAMESDIALOG_H
#define MANAGEGAMESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QToolBar>
#include <QAction>

#include "matchdialog.h"
#include "DatabaseManager.h"

class ManageGamesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ManageGamesDialog(QWidget *parent = nullptr);

private slots:
    void onAdd();
    void onEdit();
    void onDel();
    void onRowDoubleClicked(const QModelIndex&);

private:
    void buildUi();
    void reloadGames();

    QStandardItemModel *model_ { nullptr };
    QTableView         *view_  { nullptr };
    QToolBar           *tb_    { nullptr };

    DatabaseManager    &db_    = DatabaseManager::instance();
};

#endif // MANAGEGAMESDIALOG_H

