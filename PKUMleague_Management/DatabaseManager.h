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

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    bool initialize();

    bool isDatabaseOpen() const;

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
