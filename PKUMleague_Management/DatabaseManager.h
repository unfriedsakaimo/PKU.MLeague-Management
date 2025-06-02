#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QMap>
#include <QVariant>
#include <QList>
#include <QDateTime>
struct GameRecord {
    int         id;
    int         eTeam=0, sTeam=1, wTeam=2, nTeam=3;
    QString     ePlayer, sPlayer, wPlayer, nPlayer;
    int         eScore = 250, sScore = 250, wScore = 250, nScore = 250;
    double      ePt, sPt, wPt, nPt;
    int         ePos, sPos, wPos, nPos;
    QDateTime   time;
    int         state = 0;                            // 0=未进行 1=已完成
};
Q_DECLARE_METATYPE(GameRecord)
class DatabaseManager
{
public:
    static DatabaseManager& instance();
    bool initialize();

    bool isDatabaseOpen() const;

    bool addGame   (const GameRecord& rec);   // 新增
    bool updateGame(const GameRecord& rec);   // 以 id 为主键更新
    bool deleteGame(int gameId);              // 删除

    // 队伍信息查询
    QList<QMap<QString, QVariant>> getAllTeams();
    QMap<QString, QVariant> getTeamById(int teamId);

    // 选手信息查询
    QList<QMap<QString, QVariant>> getAllPlayers();
    QMap<QString, QVariant> getPlayerById(const QString& playerId);

    // 比赛信息查询
    QList<QMap<QString, QVariant>> getAllGames();
    QMap<QString, QVariant> getGameById(int gameId);
    QList<QMap<QString, QVariant>> getGamesByTeam(int teamId);
    QList<QMap<QString, QVariant>> getGamesByPlayer(const QString& playerId);
    QList<QMap<QString, QVariant>> getUpcomingGames();  // 未进行的比赛
    QList<QMap<QString, QVariant>> getCompletedGames(); // 已完成的比赛

private:
    DatabaseManager();
    QSqlDatabase db;

    // 通用查询结果处理
    QList<QMap<QString, QVariant>> processQueryResults(QSqlQuery& query);
};

#endif // DATABASEMANAGER_H
