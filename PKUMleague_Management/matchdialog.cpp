#include "matchdialog.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSet>

/*-------------------------------------------------
 * 公共工具：把队伍列表灌进四个下拉框
 *------------------------------------------------*/
static void populateTeamCombo(QComboBox *combo)
{
    auto teams = DatabaseManager::instance().getAllTeams();
    for (const auto &m : teams)         // m 是 QMap<QString,QVariant>
        combo->addItem(m["name"].toString(), m["id"].toInt());
}

/*===== 1. 构造函数：新建 =====*/
MatchDialog::MatchDialog(QWidget *parent)
    : QDialog(parent)
{
    initUi();
    setWindowTitle("新建比赛");
}

/*===== 2. 构造函数：编辑 =====*/
MatchDialog::MatchDialog(const GameRecord &src, QWidget *parent)
    : QDialog(parent), rec_(src)
{
    initUi();
    setWindowTitle("编辑比赛");

    /* 将旧数据灌入控件 */
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
    /* 若增加分数/选手字段，同理在此处加载 */
}

/*===== 3. initUi：统一建 UI =====*/
void MatchDialog::initUi()
{
    auto fillPlayers = [this](QComboBox *teamCb, QComboBox *playerCb){
        playerCb->clear();
        playerCb->addItem("— 请选择 —", QString());
        int tid = teamCb->currentData().toInt();
        for (auto m : DatabaseManager::instance().getAllPlayers()) {
            if (m["id"].toString().toInt()/10 == tid)       // 满足就加入
                playerCb->addItem(m["name"].toString(), m["id"].toString());
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

    for (QComboBox *cb : {cbE, cbS, cbW, cbN,Eplayer,Splayer,Wplayer,Nplayer})
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
            this, [=]{ fillPlayers(cbE, Eplayer); });
    connect(cbS, &QComboBox::currentIndexChanged,
            this, [=]{ fillPlayers(cbS, Splayer); });
    connect(cbW, &QComboBox::currentIndexChanged,
            this, [=]{ fillPlayers(cbW, Wplayer); });
    connect(cbN, &QComboBox::currentIndexChanged,
            this, [=]{ fillPlayers(cbN, Nplayer); });
    fillPlayers(cbE, Eplayer);
    fillPlayers(cbS, Splayer);
    fillPlayers(cbW, Wplayer);
    fillPlayers(cbN, Nplayer);
    auto lay = new QVBoxLayout(this);
    lay->addLayout(form);
    lay->addWidget(btnBox);
    setLayout(lay);
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
    rec_.state  = state->currentData().toInt();                    // 新建默认未进行
    rec_.ePlayer = Eplayer->currentData().toString();
    rec_.sPlayer = Splayer->currentData().toString();
    rec_.wPlayer = Wplayer->currentData().toString();
    rec_.nPlayer = Nplayer->currentData().toString();
    /* 若有分数/选手字段，在此继续 rec_.eScore = … */

    accept();                           // 关闭对话框并返回 Accepted
}
