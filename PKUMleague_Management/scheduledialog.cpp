#include "scheduledialog.h"
#include "ui_scheduledialog.h"
#include <QVBoxLayout>
#include <QDateTime>
#include <QHeaderView>

ScheduleDialog::ScheduleDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScheduleDialog)
{
    ui->setupUi(this);
    setWindowTitle("赛程预览");
    resize(900, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);

    setLayout(layout);
    loadSchedule();
}



void ScheduleDialog::loadSchedule()
{
    QList<QMap<QString, QVariant>> games = dbManager.getAllGames();  // ← 所有比赛，顺序在数据库中排序

    table->setRowCount(games.size());
    table->setColumnCount(6); // 编号 + 时间 + 东南西北
    table->setHorizontalHeaderLabels({"编号", "时间", "东", "南", "西", "北"});

    for (int i = 0; i < games.size(); ++i) {
        const QMap<QString, QVariant>& game = games[i];

        // 1. 编号
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(game["id"].toInt()));
        idItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 0, idItem);

        // 2. 时间
        QDateTime dt = game["time"].toDateTime();
        QTableWidgetItem *timeItem = new QTableWidgetItem(dt.toString("yyyy-MM-dd HH:mm"));
        timeItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 1, timeItem);

        // 3. 队伍数据（东南西北）
        QList<int> teamIds = {
            game["eTeam"].toInt(),
            game["sTeam"].toInt(),
            game["wTeam"].toInt(),
            game["nTeam"].toInt()
        };

        QStringList posKeys = {"e", "s", "w", "n"};

        for (int j = 0; j < 4; ++j) {
            QMap<QString, QVariant> teamInfo = dbManager.getTeamById(teamIds[j]);

            QString name = teamInfo["name"].toString();
            QColor bgColor = QColor(teamInfo["color"].toString());

            // 拼接文字：名字 + ID + 得点（如有）
            QString pos = posKeys[j];
            QString text = name;

            QString playerKey = pos + "Player";
            QString ptKey = pos + "Pt";

            if (game.contains(playerKey) && !game[playerKey].isNull()) {
                QString playerId = game[playerKey].toString();
                QMap<QString, QVariant> playerInfo = dbManager.getPlayerById(playerId);
                if (!playerInfo.isEmpty()) {
                    QString playerName = playerInfo["name"].toString();
                    text += "\n" + playerName;
                }
            }

            if (game.contains(ptKey) && !game[ptKey].isNull()) {
                double pt = game[ptKey].toDouble();
                QString ptStr = pt >= 0 ? "+" + QString::number(pt, 'f', 1) : QString::number(pt, 'f', 1);
                text += "\n" + ptStr;
            }

            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackground(bgColor);

            double brightness = 0.299 * bgColor.red() + 0.587 * bgColor.green() + 0.114 * bgColor.blue();
            item->setForeground(brightness > 127 ? Qt::black : Qt::white);

            table->setItem(i, j + 2, item);
        }

        table->setRowHeight(i, 75); // 高度增大
    }
}

ScheduleDialog::~ScheduleDialog()
{
    delete ui;
}
