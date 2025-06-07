#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_logindialog.h"
#include "teaminfodialog.h"
#include "scheduledialog.h"
#include "managegamesdialog.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("PKUML管理系统");

    // 检查数据库连接
    if (!dbManager.isDatabaseOpen()) {
        QMessageBox::critical(this, "错误", "数据库连接未打开！");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_showTeamptButton_clicked()
{
    TeamInfoDialog *dialog = new TeamInfoDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}

void MainWindow::on_showScheduleButton_clicked()
{
    ScheduleDialog *dialog = new ScheduleDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}
void MainWindow::on_adminLoginButton_clicked()
{
    LoginDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        if (dlg.role() == LoginDialog::UserRole::Admin) {
            QMessageBox::information(this, "登录成功", "欢迎，管理员 " + dlg.username());
            // e.g. AdminWindow *w = new AdminWindow; w->show();
            ManageGamesDialog *dialog= new ManageGamesDialog(this,dlg.role(),dlg.teamId());
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->exec();

        } else {
            QMessageBox::warning(this, "权限错误", "当前账号不是管理员，请使用管理员账号登录。");
        }
    }
}

void MainWindow::on_captainLoginButton_clicked()
{
    LoginDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        if (dlg.role() == LoginDialog::UserRole::Captain) {
            // 队长登录成功，打开队长主界面并传入 teamId
            int teamId = dlg.teamId();
            QMap<QString, QVariant> teamRec = DatabaseManager::instance().getTeamById(teamId);
            QString teamName = teamRec.value("name").toString();
            QMessageBox::information(this, "登录成功",
                                     QString("欢迎，队长 %1，队伍 %2")
                                         .arg(dlg.username())
                                         .arg(teamName));
            // e.g. CaptainWindow *cw = new CaptainWindow(teamId); cw->show();
            ManageGamesDialog *dialog= new ManageGamesDialog(this,dlg.role(),dlg.teamId());
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->exec();

        } else {
            QMessageBox::warning(this, "权限错误", "当前账号不是队长，请使用队长账号登录。");
        }
    }
}
//void MainWindow::on_adminLoginButton_clicked()//暂时用管理员登录进入增删改
//{
//    ManageGamesDialog *dialog= new ManageGamesDialog(this);
//    dialog->setAttribute(Qt::WA_DeleteOnClose);
//    dialog->exec();
//}


