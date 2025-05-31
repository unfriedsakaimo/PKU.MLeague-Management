/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *showScheduleButton;
    QPushButton *showTeamptButton;
    QPushButton *showPlayerptButton;
    QPushButton *captainLoginButton;
    QPushButton *adminLoginButton;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 720);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1280, 720));
        MainWindow->setMaximumSize(QSize(1280, 720));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(660, 60, 591, 601));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(14);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        showScheduleButton = new QPushButton(verticalLayoutWidget);
        showScheduleButton->setObjectName("showScheduleButton");
        showScheduleButton->setMinimumSize(QSize(0, 60));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\271\274\345\234\206")});
        font.setPointSize(14);
        showScheduleButton->setFont(font);

        verticalLayout->addWidget(showScheduleButton);

        showTeamptButton = new QPushButton(verticalLayoutWidget);
        showTeamptButton->setObjectName("showTeamptButton");
        showTeamptButton->setMinimumSize(QSize(0, 60));
        showTeamptButton->setFont(font);

        verticalLayout->addWidget(showTeamptButton);

        showPlayerptButton = new QPushButton(verticalLayoutWidget);
        showPlayerptButton->setObjectName("showPlayerptButton");
        showPlayerptButton->setMinimumSize(QSize(0, 60));
        showPlayerptButton->setFont(font);

        verticalLayout->addWidget(showPlayerptButton);

        captainLoginButton = new QPushButton(verticalLayoutWidget);
        captainLoginButton->setObjectName("captainLoginButton");
        captainLoginButton->setMinimumSize(QSize(0, 60));
        captainLoginButton->setFont(font);

        verticalLayout->addWidget(captainLoginButton);

        adminLoginButton = new QPushButton(verticalLayoutWidget);
        adminLoginButton->setObjectName("adminLoginButton");
        adminLoginButton->setMinimumSize(QSize(0, 60));
        adminLoginButton->setFont(font);

        verticalLayout->addWidget(adminLoginButton);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 60, 600, 600));
        label->setStyleSheet(QString::fromUtf8("border-image: url(:/prefix/logo.jpeg);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\344\270\273\347\225\214\351\235\242", nullptr));
        showScheduleButton->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\350\265\233\347\250\213", nullptr));
        showTeamptButton->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\351\230\237\344\274\215\345\210\206\346\225\260\350\241\250", nullptr));
        showPlayerptButton->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\344\270\252\344\272\272\345\210\206\346\225\260\350\241\250", nullptr));
        captainLoginButton->setText(QCoreApplication::translate("MainWindow", "\351\230\237\351\225\277\347\231\273\345\275\225", nullptr));
        adminLoginButton->setText(QCoreApplication::translate("MainWindow", "\347\256\241\347\220\206\345\221\230\347\231\273\345\275\225", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
