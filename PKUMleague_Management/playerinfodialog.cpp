#include "playerinfodialog.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QMessageBox>

PlayerInfoDialog::PlayerInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("选手积分表");
    resize(960, 640);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建表格
    table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setVisible(false);
    table->setSortingEnabled(true); // 启用排序功能

    // 设置样式表，选中行时文字颜色为黑色
    table->setStyleSheet("QTableWidget::item:selected {"
                         "    color: black;"
                         "}");

    layout->addWidget(table);
    setLayout(layout);

    // 加载数据
    loadPlayerData();
}

void PlayerInfoDialog::loadPlayerData()
{
    if (!dbManager.isDatabaseOpen()) {
        QMessageBox::warning(this, "数据库错误", "数据库未打开！");
        return;
    }

    // 获取所有选手数据
    QList<QMap<QString, QVariant>> players = dbManager.getAllPlayers();
    if (players.isEmpty()) {
        QMessageBox::information(this, "提示", "没有找到选手数据");
        return;
    }

    // 按积分降序排序
    std::sort(players.begin(),
              players.end(),
              [](const QMap<QString, QVariant> &a, const QMap<QString, QVariant> &b) {
                  return a["pt"].toDouble() > b["pt"].toDouble();
              });

    // 设置表格列数和标题
    table->setRowCount(players.size());
    table->setColumnCount(13);

    QStringList headers = {"顺位",
                           "所属队伍",
                           "选手名",
                           "积分",
                           "比赛数",
                           "平均顺位",
                           "1位",
                           "2位",
                           "3位",
                           "4位",
                           "一位率",
                           "连对率",
                           "避四率"};
    table->setHorizontalHeaderLabels(headers);

    // 填充数据
    for (int i = 0; i < players.size(); ++i) {
        const QMap<QString, QVariant> &player = players[i];
        QString playerId = player["id"].toString();
        int totalGames = player["count_total"].toInt();

        // 从选手ID获取队伍ID (第一位字符)
        int teamId = playerId.toInt() / 10;
        QMap<QString, QVariant> teamInfo = dbManager.getTeamById(teamId);
        QString teamName = teamInfo.contains("name") ? teamInfo["name"].toString() : "未知队伍";

        // 计算统计数据
        double avgRank = 0.0;
        double firstRate = 0.0;
        double top2Rate = 0.0;
        double top3Rate = 0.0;
        bool hasGames = (totalGames > 0);

        if (hasGames) {
            int count1 = player["count_1"].toInt();
            int count2 = player["count_2"].toInt();
            int count3 = player["count_3"].toInt();
            int count4 = player["count_4"].toInt();

            avgRank = (1.0 * count1 + 2.0 * count2 + 3.0 * count3 + 4.0 * count4) / totalGames;
            firstRate = (100.0 * count1) / totalGames;
            top2Rate = (100.0 * (count1 + count2)) / totalGames;
            top3Rate = (100.0 * (count1 + count2 + count3)) / totalGames;
        }

        // 填充表格行
        int col = 0;
        table->setItem(i, col++, createCenteredItem(QString::number(i + 1)));           // 顺位
        table->setItem(i, col++, createTeamColorItem(teamId, teamName));                // 所属队伍
        table->setItem(i, col++, createCenteredItem(player["name"].toString()));        // 选手名
        table->setItem(i, col++, createCenteredNumberItem(player["pt"].toDouble(), 1)); // 积分
        table->setItem(i, col++, createCenteredItem(QString::number(totalGames)));      // 比赛数

        // 平均顺位（无比赛时不显示）
        if (hasGames) {
            table->setItem(i, col++, createCenteredNumberItem(avgRank, 2)); // 平均顺位
        } else {
            table->setItem(i, col++, createCenteredItem("")); // 空单元格
        }

        table->setItem(i, col++, createCenteredItem(player["count_1"].toString())); // 1位次数
        table->setItem(i, col++, createCenteredItem(player["count_2"].toString())); // 2位次数
        table->setItem(i, col++, createCenteredItem(player["count_3"].toString())); // 3位次数
        table->setItem(i, col++, createCenteredItem(player["count_4"].toString())); // 4位次数

        // 一位率、连对率、避四率（无比赛时不显示）
        if (hasGames) {
            table->setItem(i, col++, createCenteredNumberItem(firstRate, 1)); // 一位率
        } else {
            table->setItem(i, col++, createCenteredItem("")); // 空单元格
        }
        if (hasGames) {
            table->setItem(i, col++, createCenteredNumberItem(top2Rate, 1)); // 连对率
        } else {
            table->setItem(i, col++, createCenteredItem("")); // 空单元格
        }
        if (hasGames) {
            table->setItem(i, col++, createCenteredNumberItem(top3Rate, 1)); // 避四率
        } else {
            table->setItem(i, col++, createCenteredItem("")); // 空单元格
        }

        // // 最高分（无比赛时不显示）
        // if (hasGames) {
        //     int maxScore = player["max_score"].toInt();
        //     table->setItem(i, col++, createCenteredItem(QString::number(maxScore * 100)));
        // } else {
        //     table->setItem(i, col++, createCenteredItem("")); // 空单元格
        // }
    }

    // 调整列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->resizeColumnsToContents();

    // 设置行高
    for (int i = 0; i < table->rowCount(); ++i) {
        table->setRowHeight(i, 35);
    }
}

QTableWidgetItem *PlayerInfoDialog::createCenteredNumberItem(double value, int precision)
{
    QString text = (value == 0.0 && precision == 0) ? "0" : QString::number(value, 'f', precision);
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}

QTableWidgetItem *PlayerInfoDialog::createCenteredItem(const QString &text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}

QTableWidgetItem *PlayerInfoDialog::createTeamColorItem(int teamId, const QString &teamName)
{
    QMap<QString, QVariant> team = dbManager.getTeamById(teamId);
    QColor teamColor(team["color"].toString());

    QTableWidgetItem *item = new QTableWidgetItem(teamName);
    item->setBackground(teamColor);
    item->setTextAlignment(Qt::AlignCenter);

    // 根据颜色亮度设置文字颜色
    double brightness = (0.299 * teamColor.red() + 0.587 * teamColor.green()
                         + 0.114 * teamColor.blue())
                        / 255.0;
    item->setForeground(brightness > 0.5 ? Qt::black : Qt::white);

    return item;
}
