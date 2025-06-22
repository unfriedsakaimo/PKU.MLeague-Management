#include "managegamesdialog.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

ManageGamesDialog::ManageGamesDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("比赛管理");
    resize(800, 600);
    buildUi();
    view_->setStyleSheet(R"(
        /* 深橙高亮：控件有焦点时 */
        QTableView::item:selected          { background:#ffbb55; color:#000; }

        /* 控件失焦仍保持较暗高亮 */
        QTableView::item:selected:!active  { background:#d0aa70; color:#000; }

        /* 鼠标悬停行 */
        QTableView::item:hover             { background:#ffe2b3; }

        /* 交替底色（可选）——若 buildUi() 中 setAlternatingRowColors(true) */
        /* QTableView { alternate-background-color:#f7f7f7; } */
    )");

    reloadGames();
}
ManageGamesDialog::ManageGamesDialog(QWidget *parent,
                                     LoginDialog::UserRole role,
                                     int teamId)
    : QDialog(parent)
    , role_(role)
    , teamId_(teamId)
{
    setWindowTitle("比赛管理");
    resize(800, 600);
    buildUi();
    view_->setStyleSheet(R"(
        /* 深橙高亮：控件有焦点时 */
        QTableView::item:selected          { background:#ffbb55; color:#000; }

        /* 控件失焦仍保持较暗高亮 */
        QTableView::item:selected:!active  { background:#d0aa70; color:#000; }

        /* 鼠标悬停行 */
        QTableView::item:hover             { background:#ffe2b3; }

        /* 交替底色（可选）——若 buildUi() 中 setAlternatingRowColors(true) */
        /* QTableView { alternate-background-color:#f7f7f7; } */
    )");
    // 如果不是管理员，就屏蔽“新建”和“删除”按钮
    if (role_ != LoginDialog::UserRole::Admin) {
        acAdd_->setEnabled(false);
        acDel_->setEnabled(false);
    }
    reloadGames();
}
/* ----- UI ----- */
void ManageGamesDialog::buildUi()
{
    model_ = new QStandardItemModel(this);
    view_  = new QTableView(this);
    view_->setModel(model_);
    view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    view_->setSelectionMode(QAbstractItemView::SingleSelection);
    view_->horizontalHeader()->setStretchLastSection(true);
    connect(view_, &QTableView::doubleClicked,
            this,  &ManageGamesDialog::onRowDoubleClicked);

    tb_ = new QToolBar(this);
    acAdd_ = tb_->addAction("新建");
    acEdit_= tb_->addAction("编辑");
    acDel_ = tb_->addAction("删除");
    connect(acAdd_ , &QAction::triggered, this, &ManageGamesDialog::onAdd);
    connect(acEdit_, &QAction::triggered, this, &ManageGamesDialog::onEdit);
    connect(acDel_ , &QAction::triggered, this, &ManageGamesDialog::onDel);

    auto lay = new QVBoxLayout(this);
    lay->addWidget(tb_);
    lay->addWidget(view_);
    setLayout(lay);
}

/* ----- 数据加载 ----- */
void ManageGamesDialog::reloadGames()
{
    model_->clear();
    model_->setHorizontalHeaderLabels(
        {"ID","时间","东","南","西","北","状态"}
        );

    for (const auto &m : db_.getAllGames())
    {
        GameRecord g;
        g.id    = m["id"].toInt();
        g.time  = QDateTime::fromString(m["time"].toString(), Qt::ISODate);
        g.eTeam = m["eTeam"].toInt();
        g.sTeam = m["sTeam"].toInt();
        g.wTeam = m["wTeam"].toInt();
        g.nTeam = m["nTeam"].toInt();
        g.ePlayer = m["ePlayer"].toString();
        g.sPlayer = m["sPlayer"].toString();
        g.wPlayer = m["wPlayer"].toString();
        g.nPlayer = m["nPlayer"].toString();
        g.state = m["state"].toInt();

        QList<QStandardItem*> row;
        auto *first = new QStandardItem(QString::number(g.id));
        first->setData(QVariant::fromValue(g), Qt::UserRole);
        row << first
            << new QStandardItem(g.time.toString("yyyy-MM-dd HH:mm"))
            << new QStandardItem(db_.getTeamById(g.eTeam)["name"].toString())
            << new QStandardItem(db_.getTeamById(g.sTeam)["name"].toString())
            << new QStandardItem(db_.getTeamById(g.wTeam)["name"].toString())
            << new QStandardItem(db_.getTeamById(g.nTeam)["name"].toString())
            << new QStandardItem(g.state ? "已完成" : "未进行");

        model_->appendRow(row);
    }
}

/* ----- CRUD 槽 ----- */
void ManageGamesDialog::onAdd()
{
    MatchDialog dlg(this);
    if (dlg.exec()==QDialog::Accepted)
        if (db_.addGame(dlg.result()))
            reloadGames();
}

void ManageGamesDialog::onEdit()
{
    QModelIndex idx = view_->currentIndex();
    if (!idx.isValid()) return;

    GameRecord g = idx.siblingAtColumn(0).data(Qt::UserRole).value<GameRecord>();
    MatchDialog dlg(this,g,role_,teamId_);
    if (dlg.exec()==QDialog::Accepted)
        if (db_.updateGame(dlg.result()))
            reloadGames();
}

void ManageGamesDialog::onDel()
{
    QModelIndex idx = view_->currentIndex();
    if (!idx.isValid()) return;

    int id = idx.siblingAtColumn(0).data().toInt();
    if (QMessageBox::Yes == QMessageBox::question(
            this,"确认","删除比赛 "+QString::number(id)+" ?"))
    {
        if (db_.deleteGame(id))
            reloadGames();
    }
}

void ManageGamesDialog::onRowDoubleClicked(const QModelIndex &)
{
    onEdit();
}
