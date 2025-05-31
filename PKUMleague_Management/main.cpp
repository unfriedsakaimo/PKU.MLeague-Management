#include "mainwindow.h"
#include "DatabaseManager.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 初始化数据库
    if (!DatabaseManager::instance().initialize()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to initialize database");
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
