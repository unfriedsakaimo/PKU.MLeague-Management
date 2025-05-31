#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "teaminfodialog.h"
#include "scheduledialog.h"
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


