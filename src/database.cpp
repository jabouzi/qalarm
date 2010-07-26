#include "database.h"

bool Database::instanceFlag = false;
Database* Database::single = NULL;

Database::Database() 
{    
    db = QSqlDatabase::addDatabase("QSQLITE");    
}

Database* Database::getInstance()
{
    if(! instanceFlag)
    {
        single = new Database();
        instanceFlag = true;
        return single;       
    }
    else
    {
        return single;
    }
}

Database::~Database()
{
    db.close();
}

void Database::setDatabaseName(QString dbName)
{
    database = dbName;    
}

void Database::setDatabase()
{    
    db.setDatabaseName(database);
}

void Database::prepareDB()
{    
    db.open(); 
    if (!tablesExists())
    {        
        createTables();
    }   
} 

void Database::setTable(QString tableName)
{
    table = tableName;
} 

void Database::where(QString whereString)
{
    sqlWhere = whereString;
}

QStringList Database::selectAll(QString select)
{
    prepareDB();
    QSqlQuery query;
    QStringList resultList;
    QString sql = "SELECT "+select+" FROM "+table;
    if (sqlWhere != "") sql += " WHERE "+sqlWhere;
    pLog->Write(sql);
    query.exec(sql);
    int field = query.record().indexOf(select);
    while (query.next()) {
        resultList << query.value(field).toString();
    }
    return resultList;
}

QStringList Database::selectAllDistinct(QString select)
{
    prepareDB();
    QSqlQuery query;
    QStringList resultList;
    QString sql = "SELECT DISTINCT "+select+" FROM "+table;
    if (sqlWhere != "") sql += " WHERE "+sqlWhere;
    pLog->Write(sql);
    query.exec(sql);
    int field = query.record().indexOf(select);
    while (query.next()) {
        resultList << query.value(field).toString();
    }
    return resultList;
}

QString Database::select(QString select)
{       
    prepareDB();
    QSqlQuery query;
    QString sql = "SELECT "+select+" FROM "+table;
    if (sqlWhere != "") sql += " WHERE "+sqlWhere;
    pLog->Write(sql);
    query.exec(sql);
    int field = query.record().indexOf(select);
    query.next();
    return query.value(field).toString();
}

void Database::insert(QString sql)
{
    prepareDB();
    pLog->Write(sql);
    QSqlQuery query;
    query.exec(sql);
}

void Database::update(QString field, QString value)
{
    prepareDB();
    QSqlQuery query;    
    QString sql = "UPDATE "+table+" SET "+field+" = '"+value+"'";
    if (sqlWhere != "") sql += " WHERE "+sqlWhere;
    pLog->Write(sql);
    query.exec(sql);
}

QString Database::getStringResult(QString filedName)
{
    int field = dbQuery.record().indexOf(filedName);
    dbQuery.next();
    return dbQuery.value(field).toString();
}

int Database::getIntResult(QString filedName)
{
    bool ok;
    int field = dbQuery.record().indexOf(filedName);
    dbQuery.next();
    return dbQuery.value(field).toInt(&ok);
}

bool Database::tableExists(QString tableName)
{
    QStringList tables = db.tables(QSql::Tables);
    return tables.indexOf(tableName) >= 0;
}

void Database::createTable(QString sqlQuery)
{
    prepareDB(); 
    QSqlQuery query;
    query.exec(sqlQuery);
}

void Database::sqlQuery(QString sql)
{ 
    prepareDB(); 
    pLog->Write(sql);
    QSqlQuery query;
    query.exec(sql);
}

bool Database::tablesExists()
{
    QStringList tables = db.tables(QSql::Tables);
    return (tables.indexOf("alarmTable") >= 0);
}

void Database::createTables()
{
    pLog->Write("##CREATE##");
    QSqlQuery query;
    query.exec("CREATE TABLE alarmTable (id integer, date string, time string, day integer, repeat integer, audio string, url string)");
    query.exec("INSERT INTO alarmTable VALUES ('1','1/1/2010','8:00',1,0,'','http://www.google.com')");
}

void Database::setPath(QString lpath)
{
    path = lpath;
    pLog = new Log(path+"errors.log");
}
