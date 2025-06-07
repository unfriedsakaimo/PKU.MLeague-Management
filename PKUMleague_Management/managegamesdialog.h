#ifndef MANAGEGAMESDIALOG_H
#define MANAGEGAMESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QToolBar>
#include <QAction>
#include "logindialog.h"
#include "matchdialog.h"
#include "DatabaseManager.h"

class ManageGamesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ManageGamesDialog(QWidget *parent = nullptr);
    ManageGamesDialog(QWidget *parent,
                      LoginDialog::UserRole role,
                      int teamId);
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
    LoginDialog::UserRole role_;
    int                   teamId_;
    QAction              *acAdd_;
    QAction              *acEdit_;
    QAction              *acDel_;
    DatabaseManager    &db_    = DatabaseManager::instance();
};

#endif // MANAGEGAMESDIALOG_H

