#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include <QString>

namespace Ui { class LoginDialog; }

/**
 * @brief  登录对话框
 * 支持两种角色：管理员(admin) / 队长(captain, 绑定 team_id)
 */
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    enum class UserRole { None, Admin, Captain };

    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    UserRole role()      const { return role_;  }
    int      teamId()    const { return teamId_; }
    QString  username()  const { return user_;  }

private slots:
    //void on_loginButton_clicked();        // “登录”按钮
    void on_pwdEdit_returnPressed();      // 回车触发
    void on_buttonBox_accepted();         // OK in buttonBox
    void on_buttonBox_rejected();         // Cancel in buttonBox

private:
    bool doLogin();                       // 实际验证逻辑

    Ui::LoginDialog *ui;
    UserRole  role_   = UserRole::None;
    int       teamId_ = -1;
    QString   user_;
};

#endif // LOGINDIALOG_H


