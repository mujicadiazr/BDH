#include <QtCore>

//#include "Infrastructure/sqlitedatabase.h"
//#include "Test/sqlitedatabasetest.h"
#include "Infrastructure/bdhservercontroller.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);

    BDHServerController c;
    c.start();


//    SQLiteDatabase db;
//    SQLiteDatabaseTest *dbtest = new SQLiteDatabaseTest(c.dataStorage()) ;

//    if (!db.isConfigured())
//        dbtest->configure();

//    dbtest->connectDB();

//    QTimer *t1 =new QTimer(), *t2 = new QTimer(), *t3 = new QTimer();
//    QObject::connect(t1,SIGNAL(timeout()),dbtest,SLOT(testStoreVariablePoint()));
//    t1->start(10);

//    QObject::connect(t2,SIGNAL(timeout()),dbtest,SLOT(testStoreAlarmPoint()));
//    t2->start(100);

//    QObject::connect(t3,SIGNAL(timeout()),dbtest,SLOT(testConsultDB()));
//    t3->start(20000);

//    BDHServerController c;
//    c.run();

    return app.exec();
}
