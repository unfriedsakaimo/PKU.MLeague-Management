#ifndef MATCHDIALOG_H
#define MATCHDIALOG_H

#include <QDialog>
#include <QDateTime>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSet>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <algorithm>
#include "logindialog.h"
#include "DatabaseManager.h"

/*-------------------------------------------------
 * MatchDialog  : 新建 / 编辑比赛的表单对话框
 * 使用方式：
 *    // 新建
 *    MatchDialog dlg(this);
 *    if (dlg.exec() == QDialog::Accepted)
 *        db.addGame(dlg.result());
 *
 *    // 编辑
 *    MatchDialog dlg(oldRecord, this);
 *    if (dlg.exec() == QDialog::Accepted)
 *        db.updateGame(dlg.result());
 *------------------------------------------------*/
class MatchDialog : public QDialog
{
    Q_OBJECT
public:
    /* 新建比赛 */
    explicit MatchDialog(QWidget *parent = nullptr);

    /* 编辑比赛（传入已有记录）*/
    //explicit MatchDialog(const GameRecord &src, QWidget *parent = nullptr);
    MatchDialog(QWidget *parent,
                const GameRecord &rec,
                LoginDialog::UserRole role,
                int captainTeamId);
    /* 交互结束后通过此函数拿到表单填好的 GameRecord */
    GameRecord result() const { return rec_; }

private slots:
    /* 点击“确定”触发：校验表单并填充 rec_ */
    void onAccept();
    void onStateChanged(int index);

private:
    /* 构造 UI 控件与布局（供两个 ctor 共用）*/
    void initUi();

    // ---------- 成员 ----------
    GameRecord           rec_;               // 表单对应的纪录
    LoginDialog::UserRole role_;
    int                   captainTeamId_;
    // —— 核心控件 —— //
    QLineEdit          *leId       = nullptr;
    QDateTimeEdit      *dtTime     = nullptr;
    QComboBox          *cbE        = nullptr;
    QComboBox          *cbS        = nullptr;
    QComboBox          *cbW        = nullptr;
    QComboBox          *cbN        = nullptr;
    QComboBox          *Eplayer    = nullptr;
    QComboBox          *Splayer    = nullptr;
    QComboBox          *Wplayer    = nullptr;
    QComboBox          *Nplayer    = nullptr;
    QComboBox          *state      = nullptr;
    QDialogButtonBox   *btnBox     = nullptr;

    // 结果输入控件
    QGroupBox *resultsGroup;
    QDoubleSpinBox *spEpt, *spSpt, *spWpt, *spNpt;
    QComboBox *cbEpos, *cbSpos, *cbWpos, *cbNpos;

    void updatePositionsFromPt();

};

#endif // MATCHDIALOG_H

