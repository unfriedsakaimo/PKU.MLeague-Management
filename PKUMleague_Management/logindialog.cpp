#include "ui_logindialog.h"
#include "logindialog.h"

#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("登录");
    QPushButton *loginBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
    loginBtn->setText("登录(&L)");
    loginBtn->setDefault(true);
    /* -- 基本输入属性 -- */
    ui->pwdEdit->setEchoMode(QLineEdit::Password);

    ui->userEdit->setFocus();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

/*-------------------------------------------------
 * 槽函数：点击 “登录” 按钮
 *------------------------------------------------*/
//void LoginDialog::on_loginButton_clicked()
//{
//    doLogin();
//}

/* 回车触发同样调用登录 */
void LoginDialog::on_pwdEdit_returnPressed()
{
    doLogin();
}

/* buttonBox 内 OK 按钮 */
void LoginDialog::on_buttonBox_accepted()
{
    //QSqlQuery q0("SELECT username, password, role, team_id FROM auth");
    //while (q0.next()) {
    //    qDebug() << "auth row:"
    //             << q0.value(0).toString()
    //             << q0.value(1).toString()
    //             << q0.value(2).toString()
    //             << q0.value(3).toString();
    //}
    if (doLogin())
        accept();   // 登录成功，关闭对话框返回 Accepted
}

/* buttonBox 内 Cancel 按钮 */
void LoginDialog::on_buttonBox_rejected()
{
    reject();       // 直接关闭
}

/*=================================================
 *  真正的登录验证逻辑
 *================================================*/
bool LoginDialog::doLogin()
{
    QString u = ui->userEdit->text().trimmed();
    QString p = ui->pwdEdit ->text();

    if (u.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名和密码不能为空！");
        return false;
    }

    /* ---- 查询 auth 表 ---- */
    QSqlQuery q;
    q.prepare(R"(
        SELECT role, team_id
        FROM   auth
        WHERE  username = :u AND password = :p
    )");
    q.bindValue(":u", u);
    q.bindValue(":p", p);        // 简单示例：明文密码；生产请改哈希
    if (!q.exec()) {
        QMessageBox::critical(this, "数据库错误", q.lastError().text());
        return false;
    }
    if (!q.next()) {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
        return false;
    }

    /* ---- 解析角色 ---- */
    QString roleStr = q.value(0).toString();
    if (roleStr == "admin") {
        role_ = UserRole::Admin;
    } else if (roleStr == "captain") {
        role_   = UserRole::Captain;
        teamId_ = q.value(1).toInt();
    } else {
        QMessageBox::warning(this, "登录失败", "未知角色！");
        return false;
    }

    user_ = u;      // 记录当前用户名
    return true;    // 登录成功
}
