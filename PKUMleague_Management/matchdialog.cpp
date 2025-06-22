#include "matchdialog.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSet>
#include <QLabel>

/*-------------------------------------------------
 * 公共工具：把队伍列表灌进四个下拉框
 *------------------------------------------------*/
static void populateTeamCombo(QComboBox *combo)
{
    auto teams = DatabaseManager::instance().getAllTeams();
    for (const auto &m : teams)         // m 是 QMap<QString,QVariant>
        combo->addItem(m["name"].toString(), m["id"].toInt());
}
static void populatePlayerCombo(QComboBox *teamCb,
                                QComboBox *playerCb,
                                bool includeEmpty = true)
{
    playerCb->clear();
    if (includeEmpty)
        playerCb->addItem("— 请选择 —", QString());

    int tid = teamCb->currentData().toInt();
    for (auto &m : DatabaseManager::instance().getAllPlayers()) {
        // 假设选手ID 的十位是队伍ID
        if (m["id"].toString().toInt() / 10 == tid) {
            playerCb->addItem(
                m["name"].toString(),
                m["id"].toString()
                );
        }
    }
}
/*===== 1. 构造函数：新建 =====*/
MatchDialog::MatchDialog(QWidget *parent)
    : QDialog(parent)
{
    initUi();
    setWindowTitle("新建比赛");

}

/*===== 2. 构造函数：编辑 =====*/
/*
MatchDialog::MatchDialog(const GameRecord &src, QWidget *parent)
    : QDialog(parent), rec_(src)
{
    initUi();
    setWindowTitle("编辑比赛");


    leId->setText(QString::number(src.id));
    leId->setEnabled(false);                              // 主键不可改
    dtTime->setDateTime(src.time);
    cbE->setCurrentIndex(cbE->findData(src.eTeam));
    cbS->setCurrentIndex(cbS->findData(src.sTeam));
    cbW->setCurrentIndex(cbW->findData(src.wTeam));
    cbN->setCurrentIndex(cbN->findData(src.nTeam));
    Eplayer->setCurrentIndex(Eplayer->findData(src.ePlayer));
    Splayer->setCurrentIndex(Splayer->findData(src.sPlayer));
    Wplayer->setCurrentIndex(Wplayer->findData(src.wPlayer));
    Nplayer->setCurrentIndex(Nplayer->findData(src.nPlayer));
    state->setCurrentIndex(state->findData(src.state));

}
*/
MatchDialog::MatchDialog(QWidget *parent,
                         const GameRecord &rec,
                         LoginDialog::UserRole role,
                         int captainTeamId)
    : QDialog(parent)
    , role_(role)
    , captainTeamId_(captainTeamId)

{

    initUi();
    // 只有管理员可选队伍和比分
    bool admin = (role_ == LoginDialog::UserRole::Admin);
    cbE->setEnabled(admin);
    cbS->setEnabled(admin);
    cbW->setEnabled(admin);
    cbN->setEnabled(admin);
    dtTime->setEnabled(admin);
    state->setEnabled(admin);
    //spE->setEnabled(admin);
    //spS->setEnabled(admin);
    //spW->setEnabled(admin);
    //spN->setEnabled(admin);

    // 队长只能改自己队的选手：
    //    disable all player combos except the one for captainTeamId_
    if (!admin) {
        // 先禁用全部
        Eplayer->setEnabled(false);
        Splayer->setEnabled(false);
        Wplayer->setEnabled(false);
        Nplayer->setEnabled(false);
        // 再给“本队”对应那一家的下拉框开门
        if (rec.state == 0){
            if (captainTeamId_ == rec.eTeam) {
                Eplayer->setEnabled(true);
            }
            else if (captainTeamId_ == rec.sTeam) {
                Splayer->setEnabled(true);
            }
            else if (captainTeamId_ == rec.wTeam) {
                Wplayer->setEnabled(true);
            }
            else if (captainTeamId_ == rec.nTeam) {
                Nplayer->setEnabled(true);
            }
        }
    }

    // 其余代码：填数据、连接信号、OK 按钮
    setWindowTitle("编辑比赛");

    /* 将旧数据灌入控件 */
    leId->setText(QString::number(rec.id));
    leId->setEnabled(false);                              // 主键不可改
    dtTime->setDateTime(rec.time);
    cbE->setCurrentIndex(cbE->findData(rec.eTeam));
    cbS->setCurrentIndex(cbS->findData(rec.sTeam));
    cbW->setCurrentIndex(cbW->findData(rec.wTeam));
    cbN->setCurrentIndex(cbN->findData(rec.nTeam));
    Eplayer->setCurrentIndex(Eplayer->findData(rec.ePlayer));
    //int idx = Eplayer->findData(rec.ePlayer);
    //qDebug() << "Eplayer.findData(" << rec.ePlayer << ") =" << idx;
    //if (idx >= 0) {
    //    Eplayer->setCurrentIndex(idx);
    //} else {
    //    qDebug() << "  -- 找不到该选手 ID，列表里有哪些数据？";
    //    for (int i = 0; i < Eplayer->count(); ++i) {
    //        qDebug() << "     [" << i << "]"
    //                 << Eplayer->itemData(i).toString()
    //                 << "(" << Eplayer->itemText(i) << ")";
    //    }
        // （可选）给一个 fallback
    //    Eplayer->setCurrentIndex(0);
    //}
    Splayer->setCurrentIndex(Splayer->findData(rec.sPlayer));
    Wplayer->setCurrentIndex(Wplayer->findData(rec.wPlayer));
    Nplayer->setCurrentIndex(Nplayer->findData(rec.nPlayer));
    state->setCurrentIndex(state->findData(rec.state));


}
/*===== 3. initUi：统一建 UI =====*/
void MatchDialog::initUi()
{
    auto fillPlayers = [this](QComboBox *teamCb, QComboBox *playerCb,const QString &currentPlayerId = QString()){
        playerCb->clear();
        // 只有当 currentPlayerId 为空时，才加入 “— 请选择 —”
        if (currentPlayerId.isEmpty()) {
            playerCb->addItem("— 请选择 —", QString());
        }
        int tid = teamCb->currentData().toInt();
        for (auto m : DatabaseManager::instance().getAllPlayers()) {
            if (m["id"].toString().toInt()/10 == tid)       // 满足就加入
                playerCb->addItem(m["name"].toString(), m["id"].toString());
        }
        if (!currentPlayerId.isEmpty()) {
            int index = playerCb->findData(currentPlayerId);
            if (index >= 0) {
                playerCb->setCurrentIndex(index);
                return;
            }
        }
        playerCb->setCurrentIndex(0);
    };

    leId   = new QLineEdit;
    dtTime = new QDateTimeEdit(QDateTime::currentDateTime());
    dtTime->setDisplayFormat("yyyy-MM-dd HH:mm");

    cbE = new QComboBox; cbS = new QComboBox;
    cbW = new QComboBox; cbN = new QComboBox;
    Eplayer = new QComboBox; Splayer = new QComboBox;
    Wplayer = new QComboBox; Nplayer = new QComboBox;
    state = new QComboBox;
    state->addItem("未进行", 0);
    state->addItem("已完成", 1);

    for (QComboBox *cb : {cbE, cbS, cbW, cbN})
        populateTeamCombo(cb);

    /* ---------- 表单布局 ---------- */
    auto form = new QFormLayout;
    form->addRow(tr("比赛编号(&I)"), leId);
    form->addRow(tr("比赛时间(&T)"), dtTime);
    form->addRow(tr("东家队伍(&E)"), cbE);
    form->addRow(tr("东家选手(&PE)"),Eplayer);
    form->addRow(tr("南家队伍(&S)"), cbS);
    form->addRow(tr("南家选手(&PS)"),Splayer);
    form->addRow(tr("西家队伍(&W)"), cbW);
    form->addRow(tr("西家选手(&PW)"),Wplayer);
    form->addRow(tr("北家队伍(&N)"), cbN);
    form->addRow(tr("北家选手(&PN)"),Nplayer);
    form->addRow("比赛状态",state);
    btnBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                      QDialogButtonBox::Cancel, this);
    connect(btnBox, &QDialogButtonBox::accepted,
            this,       &MatchDialog::onAccept);
    connect(btnBox, &QDialogButtonBox::rejected,
            this,       &MatchDialog::reject);
    connect(cbE, &QComboBox::currentIndexChanged,
            this, [=]{ populatePlayerCombo(cbE, Eplayer); });
    connect(cbS, &QComboBox::currentIndexChanged,
            this, [=]{ populatePlayerCombo(cbS, Splayer); });
    connect(cbW, &QComboBox::currentIndexChanged,
            this, [=]{ populatePlayerCombo(cbW, Wplayer); });
    connect(cbN, &QComboBox::currentIndexChanged,
            this, [=]{ populatePlayerCombo(cbN, Nplayer); });
    populatePlayerCombo(cbE, Eplayer);
    populatePlayerCombo(cbS, Splayer);
    populatePlayerCombo(cbW, Wplayer);
    populatePlayerCombo(cbN, Nplayer);
    auto lay = new QVBoxLayout(this);
    lay->addLayout(form);
    lay->addWidget(btnBox);
    setLayout(lay);

    // ===== 新增：比赛结果输入部分 =====
    resultsGroup = new QGroupBox("比赛结果");
    resultsGroup->setVisible(false); // 默认隐藏

    auto resultsLayout = new QGridLayout;

    // 添加标签
    resultsLayout->addWidget(new QLabel("队伍"), 0, 0);
    resultsLayout->addWidget(new QLabel("Pt变化"), 0, 1);
    resultsLayout->addWidget(new QLabel("顺位"), 0, 2);

    // 东家
    spEpt = new QDoubleSpinBox;
    spEpt->setRange(-100.0, 100.0);
    spEpt->setSingleStep(0.1);
    spEpt->setDecimals(1);
    cbEpos = new QComboBox;
    cbEpos->addItems({"1", "2", "3", "4"});
    resultsLayout->addWidget(new QLabel("东家"), 1, 0);
    resultsLayout->addWidget(spEpt, 1, 1);
    resultsLayout->addWidget(cbEpos, 1, 2);

    // 南家
    spSpt = new QDoubleSpinBox;
    spSpt->setRange(-100.0, 100.0);
    spSpt->setSingleStep(0.1);
    spSpt->setDecimals(1);
    cbSpos = new QComboBox;
    cbSpos->addItems({"1", "2", "3", "4"});
    resultsLayout->addWidget(new QLabel("南家"), 2, 0);
    resultsLayout->addWidget(spSpt, 2, 1);
    resultsLayout->addWidget(cbSpos, 2, 2);

    // 西家
    spWpt = new QDoubleSpinBox;
    spWpt->setRange(-100.0, 100.0);
    spWpt->setSingleStep(0.1);
    spWpt->setDecimals(1);
    cbWpos = new QComboBox;
    cbWpos->addItems({"1", "2", "3", "4"});
    resultsLayout->addWidget(new QLabel("西家"), 3, 0);
    resultsLayout->addWidget(spWpt, 3, 1);
    resultsLayout->addWidget(cbWpos, 3, 2);

    // 北家
    spNpt = new QDoubleSpinBox;
    spNpt->setRange(-100.0, 100.0);
    spNpt->setSingleStep(0.1);
    spNpt->setDecimals(1);
    cbNpos = new QComboBox;
    cbNpos->addItems({"1", "2", "3", "4"});
    resultsLayout->addWidget(new QLabel("北家"), 4, 0);
    resultsLayout->addWidget(spNpt, 4, 1);
    resultsLayout->addWidget(cbNpos, 4, 2);

    resultsGroup->setLayout(resultsLayout);

    // 添加Pt变化信号连接
    connect(spEpt, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MatchDialog::updatePositionsFromPt);
    connect(spSpt, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MatchDialog::updatePositionsFromPt);
    connect(spWpt, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MatchDialog::updatePositionsFromPt);
    connect(spNpt, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MatchDialog::updatePositionsFromPt);

    // 添加结果分组框到主布局
    lay->addWidget(resultsGroup);

    // 连接状态变化信号
    connect(state, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MatchDialog::onStateChanged);
}

/*===== 4. onAccept：校验并生成 GameRecord =====*/
void MatchDialog::onAccept()
{
    bool ok;
    int idVal = leId->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "非法编号", "比赛编号必须是整数！");
        return;
    }

    /* 确保四个队伍各不相同 */
    QSet<int> set {
        cbE->currentData().toInt(),
        cbS->currentData().toInt(),
        cbW->currentData().toInt(),
        cbN->currentData().toInt()
    };
    if (set.size() != 4) {
        QMessageBox::warning(this, "座次冲突",
                             "四个座次不允许出现同一支队伍！");
        return;
    }

    /* ----- 写入 rec_ 结构体 ----- */
    rec_.id     = idVal;
    rec_.time   = dtTime->dateTime();
    rec_.eTeam  = cbE->currentData().toInt();
    rec_.sTeam  = cbS->currentData().toInt();
    rec_.wTeam  = cbW->currentData().toInt();
    rec_.nTeam  = cbN->currentData().toInt();
    rec_.state  = state->currentData().toInt();
    rec_.ePlayer = Eplayer->currentData().toString();
    rec_.sPlayer = Splayer->currentData().toString();
    rec_.wPlayer = Wplayer->currentData().toString();
    rec_.nPlayer = Nplayer->currentData().toString();

    // 如果比赛已完成，验证结果
    if (rec_.state == 1) {
        // 保存Pt数据
        rec_.ePt = spEpt->value();
        rec_.sPt = spSpt->value();
        rec_.wPt = spWpt->value();
        rec_.nPt = spNpt->value();

        // 保存顺位数据
        rec_.ePos = cbEpos->currentIndex() + 1; // 索引转顺位(1-4)
        rec_.sPos = cbSpos->currentIndex() + 1;
        rec_.wPos = cbWpos->currentIndex() + 1;
        rec_.nPos = cbNpos->currentIndex() + 1;

        // 验证顺位唯一
        QSet<int> positions = {
            rec_.ePos, rec_.sPos, rec_.wPos, rec_.nPos
        };

        if (positions.size() != 4) {
            QMessageBox::warning(this, "顺位错误", "顺位必须各不相同！");
            return;
        }
    }
    /* 若有分数/选手字段，在此继续 rec_.eScore = … */

    accept();                           // 关闭对话框并返回 Accepted
}

void MatchDialog::onStateChanged(int index) {
    bool completed = (state->itemData(index).toInt() == 1);
    resultsGroup->setVisible(completed);

    // 只有管理员可以编辑结果
        bool admin = (role_ == LoginDialog::UserRole::Admin);

    // 如果是管理员且状态变为"已完成"，启用结果输入
    if (admin && completed) {
        spEpt->setEnabled(true);
        cbEpos->setEnabled(true);
        spSpt->setEnabled(true);
        cbSpos->setEnabled(true);
        spWpt->setEnabled(true);
        cbWpos->setEnabled(true);
        spNpt->setEnabled(true);
        cbNpos->setEnabled(true);
    }
    // 如果是队长，禁用结果输入
    else if (!admin) {
        spEpt->setEnabled(false);
        cbEpos->setEnabled(false);
        spSpt->setEnabled(false);
        cbSpos->setEnabled(false);
        spWpt->setEnabled(false);
        cbWpos->setEnabled(false);
        spNpt->setEnabled(false);
        cbNpos->setEnabled(false);
    }
}

void MatchDialog::updatePositionsFromPt() {
    // 获取Pt值
    QMap<double, QComboBox*> ptMap = {
        {spEpt->value(), cbEpos},
        {spSpt->value(), cbSpos},
        {spWpt->value(), cbWpos},
        {spNpt->value(), cbNpos}
    };

    // 按Pt值降序排序
    QList<double> sortedPt = ptMap.keys();
    std::sort(sortedPt.begin(), sortedPt.end(), std::greater<double>());

    // 分配顺位
    for (int i = 0; i < sortedPt.size(); i++) {
        QComboBox* combo = ptMap[sortedPt[i]];
        combo->setCurrentIndex(i); // 1位=0, 2位=1, 3位=2, 4位=3
    }
}
