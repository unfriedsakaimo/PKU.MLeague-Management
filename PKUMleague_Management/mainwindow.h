#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_showTeamptButton_clicked();
    void on_showScheduleButton_clicked();
    void on_adminLoginButton_clicked();


private:
    Ui::MainWindow *ui;
    DatabaseManager& dbManager = DatabaseManager::instance(); // 获取数据库单例
};
#endif // MAINWINDOW_H
