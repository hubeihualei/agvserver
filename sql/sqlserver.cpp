﻿#include "sqlserver.h"
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include "util/global.h"

SqlServer::SqlServer()
{

}

SqlServer::~SqlServer()
{
    closeConnection();
}

//创建数据库连接
bool SqlServer::createConnection()
{
    if (QSqlDatabase::contains("mysqlserverconnection"))
    {
        database = QSqlDatabase::database("mysqlserverconnection");
    }
    else
    {
        database=QSqlDatabase::addDatabase("QODBC","mysqlserverconnection");
        database.setDatabaseName("QyhTestDb");
        database.setUserName("sa");
        database.setPassword("6980103");
    }
    if(!database.isValid())return false;
    if (!database.open())
    {
        g_log->log(AGV_LOG_LEVEL_ERROR,"Error: Failed to connect database."+database.lastError().text().toStdString());
        return false;
    }

    return true;
}

//关闭数据库连接
bool SqlServer::closeConnection()
{
    database.close();
    return true;
}

//执行sql语句
bool SqlServer::exec(QString qeurysql,QStringList args)
{
    //g_log->log(AGV_LOG_LEVEL_DEBUG,"qeurysql="+qeurysql.toStdString());

    QSqlQuery sql_query(database);
    sql_query.prepare(qeurysql);
    for(int i=0;i<args.length();++i){
        sql_query.addBindValue(args[i]);
        //g_log->log(AGV_LOG_LEVEL_DEBUG,args.at(i).toStdString());
    }

    if(!sql_query.exec())
    {
        g_log->log(AGV_LOG_LEVEL_ERROR,"Error: Fail to sql_query.exec()."+sql_query.lastError().text().toStdString());
        return false;
    }

    return true;
}

//查询数据
QList<QStringList> SqlServer::query(QString qeurysql, QStringList args)
{
    //g_log->log(AGV_LOG_LEVEL_DEBUG,"qeurysql="+qeurysql.toStdString());
    QList<QStringList> xx;
    QSqlQuery sql_query(database);
    sql_query.prepare(qeurysql);
    for(int i=0;i<args.length();++i){
        sql_query.addBindValue(args[i]);
        //g_log->log(AGV_LOG_LEVEL_DEBUG,args.at(i).toStdString());
    }

    if(!sql_query.exec())
    {
        g_log->log(AGV_LOG_LEVEL_ERROR,"Error: Fail to sql_query.exec()."+sql_query.lastError().text().toStdString());
        return xx;
    }
    while(sql_query.next()){
        int columnNum=sql_query.record().count();
        QStringList qsl;
        for(int i=0;i<columnNum;++i)
            qsl.append(sql_query.value(i).toString());
        xx.append(qsl);
    }

    return xx;
}


