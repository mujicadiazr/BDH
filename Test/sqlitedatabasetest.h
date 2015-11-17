#ifndef SQLITEDATABASETEST_H
#define SQLITEDATABASETEST_H

#include <QObject>

class SQLiteDatabase;

class SQLiteDatabaseTest : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteDatabaseTest(SQLiteDatabase *db, QObject *parent = 0);
    void configure();
    void connectDB();
signals:

public slots:
    void testStoreVariablePoint();
    void testStoreAlarmPoint();
    void testTime();
    void testConsultDB();
private:
    SQLiteDatabase *m_db;


};

#endif // SQLITEDATABASETEST_H
