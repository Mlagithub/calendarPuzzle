#ifndef DATASTORGE_H
#define DATASTORGE_H

#include <QThread>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QSqlQuery>
#include <unordered_map>
#include <map>

#include "piece.h"

class DataStorge  : public QThread
{
    Q_OBJECT

public:
    DataStorge();
    ~DataStorge();

signals:
    void endUpdateDB();

public:
    void updateDB();
    void insert(const int month, const int day, const int week, const std::vector<shape>& val);
    std::vector<shape> get(const int month, const int day, const int week);
    bool good();

    int methodNumberOfAll();
    int methodNumberOfDate(const int month, const int day, const int week);

    bool isOpen();
    bool checkExec(QString str);
    QString formatDateStr(const int month, const int day, const int week);
    QString formatBoardStr(const shape& bd);
    shape fromBoardStr(const QString& val);
    shape str2Int(QString str);

protected:
    void run() override;

private:
    const QString create_answer_sql = "create table if not exists answer (id int primary key, dt char(6) not null, method int, data char(56) not null)";
    const QString insert_answer_sql = "insert into answer(id, dt, method, data) values (:id, :dt, :method, :data)";
    const QString query_answer_sql = "select * FROM answer WHERE dt == ? ;";

    QSqlDatabase database;
    QSqlQuery query;
    int nRow_ = 0;
    int m_, d_, w_;
    std::vector<shape> curAnswer_;
    std::unordered_map<int, QChar> mapI2C_ = {
        {1, '1'},
        {2, '2'},
        {3, '3'},
        {4, '4'},
        {5, '5'},
        {6, '6'},
        {7, '7'},
        {8, '8'},
        {9, '9'},
        {10, 'A'},
        {11, 'B'},
        {-1, 'C'},
        {0, 'D'}
    };
    std::map<QChar, int> mapC2I_ = {
        {'1', 1},
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'A', 10},
        {'B', 11},
        {'C', -1},
        {'D', 0}
    };
    const int height = 8, width = 7;
    bool isGood_ = false;
};

#endif // DATASTORGE_H
