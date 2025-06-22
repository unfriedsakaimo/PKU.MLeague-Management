#include "DatabaseManager.h"
#include <QSqlRecord>

// ---- 单例实现 ----
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

// ---- 构造函数 ----
DatabaseManager::DatabaseManager()
{
    // 设置数据库路径（确保数据库文件目录存在）
    QDir().mkpath("数据库文件");
}

// ---- 初始化代码 ----
bool DatabaseManager::initialize()
{
    QString dbPath = "数据库文件/mleague.db";
    // if (QFile::exists(dbPath)) {
    //     if (QFile::remove(dbPath)) {
    //         qDebug() << "已删除旧数据库文件";
    //     } else {
    //         qDebug() << "无法删除旧数据库文件";
    //     }
    // }

    // 只创建一次数据库连接
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);
    }

    // 如果数据库已打开，直接返回成功
    if (db.isOpen()) {
        return true;
    }

    // 打开数据库
    if (!db.open()) {
        qDebug() << "数据库连接失败：" << db.lastError().text();
        qDebug() << "数据库路径：" << QDir("数据库文件").absolutePath();
        return false;
    }

    // 创建赛程表
    QSqlQuery query;
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS games ("
        "id INTEGER PRIMARY KEY, "
        "eTeam INTEGER NOT NULL, " //队伍ID
        "sTeam INTEGER NOT NULL, "
        "wTeam INTEGER NOT NULL, "
        "nTeam INTEGER NOT NULL, "
        "ePlayer INTEGER, "
        "sPlayer INTEGER, "
        "wPlayer INTEGER, "
        "nPlayer INTEGER, "
        "time DATETIME, "
        "state INTEGER DEFAULT 0, " // 0=未进行, 1=已进行
        "eScore INTEGER, "
        "sScore INTEGER, "
        "wScore INTEGER, "
        "nScore INTEGER, "
        "ePos INTEGER, "
        "sPos INTEGER, "
        "wPos INTEGER, "
        "nPos INTEGER, "
        "ePt REAL, "
        "sPt REAL, "
        "wPt REAL, "
        "nPt REAL, "
        "CHECK (eTeam != sTeam AND eTeam != wTeam AND eTeam != nTeam AND sTeam != wTeam AND sTeam != nTeam AND wTeam != nTeam)" //座次唯一性
        ");"
        );

    if (!success) {
        qDebug() << "Create games table error:" << query.lastError();
        return false;
    }

    // 创建队伍表
    success = query.exec(
        "CREATE TABLE IF NOT EXISTS teams ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "color TEXT NOT NULL, "
        "init_pt REAL NOT NULL, "
        "stage_pt REAL DEFAULT 0.0, "
        "total_pt REAL NOT NULL, "
        "count_total INTEGER DEFAULT 0, "
        "count_1 INTEGER DEFAULT 0, "
        "count_2 INTEGER DEFAULT 0, "
        "count_3 INTEGER DEFAULT 0, "
        "count_4 INTEGER DEFAULT 0"
        ");"
        );

    if (!success) {
        qDebug() << "Create teams table error:" << query.lastError();
        return false;
    }

    // 创建选手表
    success = query.exec(
        "CREATE TABLE IF NOT EXISTS players ("
        "id TEXT PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "pt REAL DEFAULT 0.0, "
        "count_total INTEGER DEFAULT 0, "
        "count_1 INTEGER DEFAULT 0, "
        "count_2 INTEGER DEFAULT 0, "
        "count_3 INTEGER DEFAULT 0, "
        "count_4 INTEGER DEFAULT 0, "
        "max_score INTEGER DEFAULT -1000"
        ");"
        );

    if (!success) {
        qDebug() << "Create players table error:" << query.lastError();
        return false;
    }

    // 创建认证表
    success = query.exec(
        "CREATE TABLE IF NOT EXISTS auth ("
        "username TEXT PRIMARY KEY, "
        "password TEXT NOT NULL, "
        "role TEXT NOT NULL, " // admin/captain
        "team_id INTEGER"      // 如果是队长，关联队伍ID
        ");"
        );

    if (!success) {
        qDebug() << "Create auth table error:" << query.lastError();
        return false;
    }

    //测试阶段：清空测试数据
    query.exec("DELETE FROM teams;");

    // 插入预定义队伍数据（如果表为空）
    query.exec("SELECT COUNT(*) FROM teams;");
    if (query.next() && query.value(0).toInt() == 0) {
        QStringList teamInserts = {
            "INSERT INTO teams (id, name, color, init_pt, total_pt) VALUES (0, '格斗', '#D00000', 130.1, 130.1);",
            "INSERT INTO teams (id, name, color, init_pt, total_pt) VALUES (1, '樱花', '#FBD6FB', 87.4, 87.4);",
            "INSERT INTO teams (id, name, color, init_pt, total_pt) VALUES (2, '凤凰', '#EC8600', 18.8, 18.8);",
            "INSERT INTO teams (id, name, color, init_pt, total_pt) VALUES (3, '野兽', '#072F79', 9.2, 9.2);"
        };

        for (const QString& sql : teamInserts) {
            if (!query.exec(sql)) {
                qDebug() << "Insert team error:" << query.lastError();
            }
        }
    }

    //清空测试数据
    query.exec("DELETE FROM players;");

    // 插入预定义选手数据（如果表为空）
    query.exec("SELECT COUNT(*) FROM players;");
    if (query.next() && query.value(0).toInt() == 0) {
        QStringList playerInserts = {
            "INSERT INTO players (id, name) VALUES ('0', '前原Nagi');",
            "INSERT INTO players (id, name) VALUES ('1', '小鹿');",
            "INSERT INTO players (id, name) VALUES ('2', 'Rexxx');",
            "INSERT INTO players (id, name) VALUES ('3', '忆水');",
            "INSERT INTO players (id, name) VALUES ('10', '炸洋芋');",
            "INSERT INTO players (id, name) VALUES ('11', 'zombie');",
            "INSERT INTO players (id, name) VALUES ('12', 'Spectator');",
            "INSERT INTO players (id, name) VALUES ('13', 'Xtzzzz');",
            "INSERT INTO players (id, name) VALUES ('20', '玖夜');",
            "INSERT INTO players (id, name) VALUES ('21', '树');",
            "INSERT INTO players (id, name) VALUES ('22', '立直里三');",
            "INSERT INTO players (id, name) VALUES ('23', 'pope');",
            "INSERT INTO players (id, name) VALUES ('30', '岸谷');",
            "INSERT INTO players (id, name) VALUES ('31', 'Yuan');",
            "INSERT INTO players (id, name) VALUES ('32', '才彻');",
            "INSERT INTO players (id, name) VALUES ('33', '凛冬');"
        };

        for (const QString& sql : playerInserts) {
            if (!query.exec(sql)) {
                qDebug() << "Insert player error:" << query.lastError();
            }
        }
    }

    // 清空测试数据
    query.exec("DELETE FROM games;");


    // 插入昨天的比赛（编号1，有选手和得点）
    {
        QString sql = R"(
        INSERT INTO games (
            id, eTeam, sTeam, wTeam, nTeam, ePlayer, sPlayer, wPlayer, nPlayer,
            eScore, sScore, wScore, nScore,
            ePt, sPt, wPt, nPt,
            ePos, sPos, wPos, nPos,
            time, state
        )
        VALUES (
            1, 0, 1, 2, 3, '1', '10', '23', '32',
            330, 260, 233, 177,
            53.0, 6.0, -16.7, -42.3,
            1, 2, 3, 4,
            '2025-05-30 19:00:00', 1
        )
    )";
        if (!query.exec(sql)) {
            qDebug() << "插入编号1失败:" << query.lastError();
        }
    }

    // 插入明天的比赛（编号2，仅队伍）
    {
        QDateTime tomorrow = QDateTime::currentDateTime().addDays(1);
        QString timeStr = tomorrow.toString("yyyy-MM-dd") + " 19:00:00";

        QString sql = QString(
                          "INSERT INTO games (id, eTeam, sTeam, wTeam, nTeam, time, state) "
                          "VALUES (2, 3, 2, 1, 0, '%1', 0)"
                          ).arg(timeStr);

        if (!query.exec(sql)) {
            qDebug() << "插入编号2失败:" << query.lastError();
        }
    }
    // 清空旧的 auth 表，仅测试阶段用
    query.exec("DELETE FROM auth;");
    // 插入管理员账号（如果表为空）
    query.exec("SELECT COUNT(*) FROM auth;");
    if (query.next() && query.value(0).toInt() == 0) {
        // 管理员
        query.exec("INSERT INTO auth (username, password, role, team_id) "
                   "VALUES ('admin', '123456', 'admin', NULL);");

        // 队长 0  —— 关联队伍 id = 0
        query.exec("INSERT INTO auth (username, password, role, team_id) "
                   "VALUES ('captain0', 'c0pass', 'captain', 0);");

        // 队长 1  —— 关联队伍 id = 1
        query.exec("INSERT INTO auth (username, password, role, team_id) "
                   "VALUES ('captain1', 'c1pass', 'captain', 1);");
    }

    return true;
}

// ---- 状态检查 ----
bool DatabaseManager::isDatabaseOpen() const {
    return db.isOpen();
}

// ---- 通用查询结果处理函数 ----
QList<QMap<QString, QVariant>> DatabaseManager::processQueryResults(QSqlQuery& query) {
    QList<QMap<QString, QVariant>> results;
    while (query.next()) {
        QMap<QString, QVariant> record;
        for (int i = 0; i < query.record().count(); ++i) {
            record.insert(query.record().fieldName(i), query.value(i));
        }
        results.append(record);
    }
    return results;
}

// ---- 队伍信息查询 ----
QList<QMap<QString, QVariant>> DatabaseManager::getAllTeams() {
    QSqlQuery query;
    if (!query.exec("SELECT * FROM teams ORDER BY id")) {
        qDebug() << "获取队伍列表失败:" << query.lastError().text();
        qDebug() << "执行的SQL:" << query.lastQuery();
        return {};
    }
    return processQueryResults(query);
}

QMap<QString, QVariant> DatabaseManager::getTeamById(int teamId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM teams WHERE id = :id");
    query.bindValue(":id", teamId);
    if (!query.exec() || !query.next()) {
        qDebug() << "获取队伍信息失败:" << query.lastError();
        return {};
    }
    QMap<QString, QVariant> record;
    for (int i = 0; i < query.record().count(); ++i) {
        record.insert(query.record().fieldName(i), query.value(i));
    }
    return record;
}

// ---- 选手信息查询 ----
QList<QMap<QString, QVariant>> DatabaseManager::getAllPlayers() {
    QSqlQuery query("SELECT * FROM players ORDER BY CAST(id AS INTEGER)");
    if (!query.exec()) {
        qDebug() << "获取选手列表失败:" << query.lastError();
        return {};
    }
    return processQueryResults(query);
}

QMap<QString, QVariant> DatabaseManager::getPlayerById(const QString& playerId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM players WHERE id = :id");
    query.bindValue(":id", playerId);
    if (!query.exec() || !query.next()) {
        qDebug() << "获取选手信息失败:" << query.lastError();
        return {};
    }
    QMap<QString, QVariant> record;
    for (int i = 0; i < query.record().count(); ++i) {
        record.insert(query.record().fieldName(i), query.value(i));
    }
    return record;
}

// ---- 比赛信息查询 ----
QList<QMap<QString, QVariant>> DatabaseManager::getAllGames() {
    QSqlQuery query("SELECT * FROM games ORDER BY id ASC");
    if (!query.exec()) {
        qDebug() << "获取比赛列表失败:" << query.lastError();
        return {};
    }
    return processQueryResults(query);
}

QMap<QString, QVariant> DatabaseManager::getGameById(int gameId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM games WHERE id = :id");
    query.bindValue(":id", gameId);
    if (!query.exec() || !query.next()) {
        qDebug() << "获取比赛详情失败:" << query.lastError();
        return {};
    }
    QMap<QString, QVariant> record;
    for (int i = 0; i < query.record().count(); ++i) {
        record.insert(query.record().fieldName(i), query.value(i));
    }
    return record;
}

QList<QMap<QString, QVariant>> DatabaseManager::getUpcomingGames() {
    QSqlQuery query(
        "SELECT * FROM games "
        "WHERE state = 0 "  // 未进行
        "ORDER BY time ASC"
        );
    if (!query.exec()) {
        qDebug() << "获取未开始比赛失败:" << query.lastError();
        return {};
    }
    return processQueryResults(query);
}

QList<QMap<QString, QVariant>> DatabaseManager::getCompletedGames() {
    QSqlQuery query(
        "SELECT * FROM games "
        "WHERE state = 1 "  // 已进行
        "ORDER BY time DESC"
        );
    if (!query.exec()) {
        qDebug() << "获取已完成比赛失败:" << query.lastError();
        return {};
    }
    return processQueryResults(query);
}

/* ---------- 新增比赛 ---------- */
bool DatabaseManager::addGame(const GameRecord& r)
{
    if (!db.isOpen()) return false;

    QSqlQuery q;
    q.prepare(R"(
        INSERT INTO games (
            id,eTeam,sTeam,wTeam,nTeam,
            ePlayer,sPlayer,wPlayer,nPlayer,
            eScore,sScore,wScore,nScore,
            ePt,sPt,wPt,nPt,
            ePos,sPos,wPos,nPos,
            time,state
        ) VALUES (
            :id,:eTeam,:sTeam,:wTeam,:nTeam,
            :ePlayer,:sPlayer,:wPlayer,:nPlayer,
            :eScore,:sScore,:wScore,:nScore,
            :ePt,:sPt,:wPt,:nPt,
            :ePos,:sPos,:wPos,:nPos,
            :time,:state
        )
    )");
    q.bindValue(":id",      r.id);
    q.bindValue(":eTeam",   r.eTeam);   q.bindValue(":sTeam",   r.sTeam);
    q.bindValue(":wTeam",   r.wTeam);   q.bindValue(":nTeam",   r.nTeam);
    q.bindValue(":ePlayer", r.ePlayer); q.bindValue(":sPlayer", r.sPlayer);
    q.bindValue(":wPlayer", r.wPlayer); q.bindValue(":nPlayer", r.nPlayer);
    q.bindValue(":eScore",  r.eScore);  q.bindValue(":sScore",  r.sScore);
    q.bindValue(":wScore",  r.wScore);  q.bindValue(":nScore",  r.nScore);
    q.bindValue(":ePt",     r.ePt);     q.bindValue(":sPt",     r.sPt);
    q.bindValue(":wPt",     r.wPt);     q.bindValue(":nPt",     r.nPt);
    q.bindValue(":ePos",    r.ePos);    q.bindValue(":sPos",    r.sPos);
    q.bindValue(":wPos",    r.wPos);    q.bindValue(":nPos",    r.nPos);
    q.bindValue(":time",    r.time.toString(Qt::ISODate));
    q.bindValue(":state",   r.state);

    if (!q.exec()) {
        qDebug() << "创建比赛失败:" << q.lastError();
        return false;
    }
    return true;
}

/* ---------- 更新比赛 ---------- */
bool DatabaseManager::updateGame(const GameRecord& r)
{
    if (!db.isOpen()) return false;

    // 获取旧状态
    int oldState = 0;
    QSqlQuery query;
    query.prepare("SELECT state FROM games WHERE id = :id");
    query.bindValue(":id", r.id);
    if (query.exec() && query.next()) {
        oldState = query.value(0).toInt();
    } else {
        qDebug() << "获取旧状态失败:" << query.lastError();
    }

    QSqlQuery q;
    q.prepare(R"(
        UPDATE games SET
            eTeam=:eTeam, sTeam=:sTeam, wTeam=:wTeam, nTeam=:nTeam,
            ePlayer=:ePlayer, sPlayer=:sPlayer, wPlayer=:wPlayer, nPlayer=:nPlayer,
            eScore=:eScore, sScore=:sScore, wScore=:wScore, nScore=:nScore,
            ePt=:ePt, sPt=:sPt, wPt=:wPt, nPt=:nPt,
            ePos=:ePos, sPos=:sPos, wPos=:wPos, nPos=:nPos,
            time=:time, state=:state
        WHERE id=:id
    )");
    q.bindValue(":id",      r.id);
    q.bindValue(":eTeam",   r.eTeam);   q.bindValue(":sTeam",   r.sTeam);
    q.bindValue(":wTeam",   r.wTeam);   q.bindValue(":nTeam",   r.nTeam);
    q.bindValue(":ePlayer", r.ePlayer); q.bindValue(":sPlayer", r.sPlayer);
    q.bindValue(":wPlayer", r.wPlayer); q.bindValue(":nPlayer", r.nPlayer);
    q.bindValue(":eScore",  r.eScore);  q.bindValue(":sScore",  r.sScore);
    q.bindValue(":wScore",  r.wScore);  q.bindValue(":nScore",  r.nScore);
    q.bindValue(":ePt",     r.ePt);     q.bindValue(":sPt",     r.sPt);
    q.bindValue(":wPt",     r.wPt);     q.bindValue(":nPt",     r.nPt);
    q.bindValue(":ePos",    r.ePos);    q.bindValue(":sPos",    r.sPos);
    q.bindValue(":wPos",    r.wPos);    q.bindValue(":nPos",    r.nPos);
    q.bindValue(":time",    r.time.toString(Qt::ISODate));
    q.bindValue(":state",   r.state);

    if (!q.exec()) {
        qDebug() << "编辑比赛失败:" << q.lastError();
        return false;
    }

    // 如果状态从未完成(0)变为已完成(1)，更新统计
    if (oldState == 0 && r.state == 1) {
        // 更新队伍统计数据
        updateTeamStats(r.eTeam, r.ePos, r.ePt);
        updateTeamStats(r.sTeam, r.sPos, r.sPt);
        updateTeamStats(r.wTeam, r.wPos, r.wPt);
        updateTeamStats(r.nTeam, r.nPos, r.nPt);

        // 更新选手统计数据
        updatePlayerStats(r.ePlayer, r.ePos, r.ePt);
        updatePlayerStats(r.sPlayer, r.sPos, r.sPt);
        updatePlayerStats(r.wPlayer, r.wPos, r.wPt);
        updatePlayerStats(r.nPlayer, r.nPos, r.nPt);
    }

    return q.numRowsAffected() == 1;
}

/* ---------- 删除比赛 ---------- */
bool DatabaseManager::deleteGame(int id)
{
    if (!db.isOpen()) return false;

    QSqlQuery q;
    q.prepare("DELETE FROM games WHERE id=:id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "删除比赛失败:" << q.lastError();
        return false;
    }
    return q.numRowsAffected() == 1;
}

// 更新队伍统计
void DatabaseManager::updateTeamStats(int teamId, int position, double ptChange) {
    QSqlQuery query;
    query.prepare(
        "UPDATE teams SET "
        "total_pt = total_pt + :pt, "
        "stage_pt = stage_pt + :pt, "
        "count_total = count_total + 1, "
        "count_1 = count_1 + :pos1, "
        "count_2 = count_2 + :pos2, "
        "count_3 = count_3 + :pos3, "
        "count_4 = count_4 + :pos4 "
        "WHERE id = :id"
        );

    query.bindValue(":pt", ptChange);
    query.bindValue(":pos1", position == 1 ? 1 : 0);
    query.bindValue(":pos2", position == 2 ? 1 : 0);
    query.bindValue(":pos3", position == 3 ? 1 : 0);
    query.bindValue(":pos4", position == 4 ? 1 : 0);
    query.bindValue(":id", teamId);

    if (!query.exec()) {
        qDebug() << "更新队伍统计失败:" << query.lastError();
    }
}

//更新个人统计
void DatabaseManager::updatePlayerStats(const QString& playerId, int position, double ptChange) {
    QSqlQuery query;
    query.prepare(
        "UPDATE players SET "
        "pt = pt + :pt, "
        "count_total = count_total + 1, "
        "count_1 = count_1 + :pos1, "
        "count_2 = count_2 + :pos2, "
        "count_3 = count_3 + :pos3, "
        "count_4 = count_4 + :pos4 "
        "WHERE id = :id"
        );

    query.bindValue(":pt", ptChange);
    query.bindValue(":pos1", position == 1 ? 1 : 0);
    query.bindValue(":pos2", position == 2 ? 1 : 0);
    query.bindValue(":pos3", position == 3 ? 1 : 0);
    query.bindValue(":pos4", position == 4 ? 1 : 0);
    query.bindValue(":id", playerId);

    if (!query.exec()) {
        qDebug() << "更新选手统计失败:" << query.lastError();
    }
}
