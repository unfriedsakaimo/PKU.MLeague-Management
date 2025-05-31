#include "teaminfodialog.h"
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>

TeamInfoDialog::TeamInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("队伍分数表");
    resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建表格
    table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setVisible(false); // 隐藏行号

    layout->addWidget(table);
    setLayout(layout);

    // 加载数据
    loadTeamData();
}

void TeamInfoDialog::loadTeamData()
{
    if (!dbManager.isDatabaseOpen()) {
        QMessageBox::warning(this, "数据库错误", "数据库未打开！");
        return;
    }

    // 获取队伍数据
    QList<QMap<QString, QVariant>> teams = dbManager.getAllTeams();

    if (teams.isEmpty()) {
        QMessageBox::information(this, "提示", "没有找到队伍数据");
        return;
    }

    // 按总PT降序排序
    std::sort(teams.begin(), teams.end(), [](const QMap<QString, QVariant>& a, const QMap<QString, QVariant>& b) {
        return a["total_pt"].toDouble() > b["total_pt"].toDouble(); // 降序
    });

    // 设置表格
    table->setRowCount(teams.size());
    table->setColumnCount(12); // 顺位, 队名, 总pt, 阶段pt, 初始pt, 差, 一位差, 比赛数, 1-4位计数

    QStringList headers = {"顺位", "队名", "积分", "半决赛积分", "持越", "差", "一位差", "比赛数", "1位", "2位", "3位", "4位"};
    table->setHorizontalHeaderLabels(headers);

    // 计算一位值（最高总分）
    double topScore = teams.isEmpty() ? 0.0 : teams[0]["total_pt"].toDouble();

    // 填充数据
    for (int i = 0; i < teams.size(); ++i) {
        const QMap<QString, QVariant>& team = teams[i];
        double totalPt = team["total_pt"].toDouble();
        double stagePt = team["stage_pt"].toDouble();
        double initPt = team["init_pt"].toDouble();

        // 计算差值
        double difference = (i > 0) ? teams[i-1]["total_pt"].toDouble() - totalPt : 0.0;
        double topDifference = topScore - totalPt;

        // 顺位（排名）
        QTableWidgetItem *rankItem = new QTableWidgetItem(QString::number(i + 1));
        rankItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 0, rankItem);

        // 队名
        QTableWidgetItem *nameItem = new QTableWidgetItem(team["name"].toString());
        nameItem->setTextAlignment(Qt::AlignCenter);

        //设置背景色
        QColor teamColor(team["color"].toString());
        nameItem->setBackground(teamColor);

        // 根据颜色亮度自动选择文字颜色
        double brightness = (0.299 * teamColor.red() +
                             0.587 * teamColor.green() +
                             0.114 * teamColor.blue()) / 255.0;
        nameItem->setForeground(brightness > 0.5 ? Qt::black : Qt::white);

        table->setItem(i, 1, nameItem);

        // 分数相关列
        table->setItem(i, 2, createCenteredNumberItem(totalPt, 1));
        table->setItem(i, 3, createCenteredNumberItem(stagePt, 1));
        table->setItem(i, 4, createCenteredNumberItem(initPt, 1));
        table->setItem(i, 5, createCenteredNumberItem(difference, 1));
        table->setItem(i, 6, createCenteredNumberItem(topDifference, 1));

        // 比赛计数相关列

        // 比赛数显示为"实际比赛数/12"
        int raceCount = team["count_total"].toInt();
        QTableWidgetItem *raceItem = new QTableWidgetItem(QString("%1/12").arg(raceCount));
        raceItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 7, raceItem);

        table->setItem(i, 8, createCenteredNumberItem(team["count_1"].toInt(), 0));
        table->setItem(i, 9, createCenteredNumberItem(team["count_2"].toInt(), 0));
        table->setItem(i, 10, createCenteredNumberItem(team["count_3"].toInt(), 0));
        table->setItem(i, 11, createCenteredNumberItem(team["count_4"].toInt(), 0));
    }

    // 调整列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->resizeColumnsToContents();

    // 设置行高
    for (int i = 0; i < table->rowCount(); ++i) {
        table->setRowHeight(i, 40);
    }
}

QTableWidgetItem* TeamInfoDialog::createCenteredNumberItem(double value, int precision)
{
    QString text = (precision == 0) ?
                       QString::number(static_cast<int>(value)) :
                       QString::number(value, 'f', precision);

    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}
