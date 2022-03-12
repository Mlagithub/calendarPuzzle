#include <QDebug>


#include "datastorge.h"


DataStorge::DataStorge()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("calendarPuzzle.db");
        if(isOpen())
        {
            query = QSqlQuery{database.database()};
            query.prepare(create_answer_sql);
            this->checkExec(QString("create table answer"));
        }
    }
    methodNumberOfAll();
}

DataStorge::~DataStorge()
{
    database.close();
}

void DataStorge::insert(const int month, const int day, const int week, const std::vector<shape> &val)
{
    query.prepare(insert_answer_sql);

    auto dt = this->formatDateStr(month, day, week);
    query.bindValue(":dt", dt);
    int method = 0;
    for(auto it : val)
    {
        query.bindValue(":id", ++nRow_);
        query.bindValue(":method", ++method);
        query.bindValue(":data", this->formatBoardStr(it));
        checkExec(QString("insert answer of %1-%2").arg(dt).arg(method));
    }
}

std::vector<shape> DataStorge::get(const int month, const int day, const int week)
{
    std::vector<shape> rst;

    auto dt = this->formatDateStr(month, day, week);
    query.prepare(query_answer_sql);
    query.bindValue(0, dt);
    checkExec("query answer of " + dt);
    while(query.next())
    {
        auto tmp = query.value(3).toString();
        rst.push_back(this->fromBoardStr(tmp));
    }

    return rst;
}

bool DataStorge::good()
{
    return isGood_;
}

int DataStorge::methodNumberOfAll()
{
    query.exec("select count(*) from answer");
    while (query.next()) {
        nRow_ = query.value(0).toInt();
    }
    return nRow_;
}

int DataStorge::methodNumberOfDate(const int month, const int day, const int week)
{
    auto str = QString("select count(*) from answer where dt == \"%1\"").arg(this->formatDateStr(month, day, week));
    query.exec(str);
    int rst;
    while (query.next()) {
        rst = query.value(0).toUInt();
    }
    return rst;
}

bool DataStorge::isOpen()
{
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        qDebug() << QSqlDatabase::drivers();
    }
    isGood_ = true;
    return true;
}

bool DataStorge::checkExec(QString str)
{
    if(!query.exec())
    {
        qDebug() << "Error to exec " << str << query.lastError();
        return false;
    }
    else
    {
        qDebug() << "Successfully exec " << str;
    }
    return true;
}

QString DataStorge::formatDateStr(const int month, const int day, const int week)
{
    return QString("%1%2%3").arg(month,2,10,QLatin1Char('0')).arg(day,2,10,QLatin1Char('0')).arg(week,2,10,QLatin1Char('0'));
}

QString DataStorge::formatBoardStr(const shape &bd)
{
    QString rst;
    for(auto it : bd)
    {
        for(size_t jt = 0; jt <it.size(); ++jt)
        {
            rst += mapI2C_[it[jt]];
        }
    }
    return rst;
}

shape DataStorge::fromBoardStr(const QString &str)
{
    shape rst(height, std::vector<int>(width, 0));

    int n = 0, i = 0, j = 0;
    for(auto it : str)
    {
        rst[i][j] = mapC2I_.at(it);
        ++n;
        i = n / width;
        j = n % width;
    }

    return rst;
}
