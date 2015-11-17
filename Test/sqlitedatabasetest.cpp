#include "sqlitedatabasetest.h"
#include "../Infrastructure/sqlitedatabase.h"
#include <QTime>
#include <stdlib.h>

SQLiteDatabaseTest::SQLiteDatabaseTest(SQLiteDatabase *db, QObject *parent) :
    QObject(parent), m_db(db)
{
}

void SQLiteDatabaseTest::configure()
{
    QList<Variable> varList;
    QList<Alarm> alarmList;

    int cont = 1;
    //srand(QTime::currentTime().msec());

    for (int i = 1; i <= 50; ++i) {
        Variable v(i,(quint8)DOUBLE,1000,QString("variable de pruebavariable de pruebavariable de pruebavariable de prueba").toStdString());
        varList.append(v);

        for (int j = 1; j <= 3; ++j) {
            Alarm a = Alarm(i,cont++,rand() % 5,rand() % 10,rand() % 3,QString("alarma de pruebaalarma de pruebaalarma de prueba"));
            alarmList.append(a);
        }
    }
    Configuration c;
    c.setVars(&varList);
    c.setAlarms(&alarmList);

    if (m_db->configure(c))
        qDebug() << "TEST: Configurado correctamente!!!";
    else
        qDebug() << "TEST: Error en la configuracion!!!";
}

void SQLiteDatabaseTest::connectDB()
{

    if (m_db->connect())
        qDebug() << "TEST: DB CONECTADA correctamente!!!";
    else
        qDebug() << "TEST: Error en al CONECTAR!!!";

}

void SQLiteDatabaseTest::testStoreVariablePoint()
{
    qsrand(uint(std::time(NULL)));
    VariablePoint vp = VariablePoint(rand() % 50, rand() % 5,std::time(NULL),qint64(rand() % 1000000000000),INTEGER);

    if (m_db->storeVariablePoint(vp.idVariable(),vp.quality(),vp.timeStamp(),vp.value()));/*
        qDebug() << "TEST: Variable point guardado correctamente!!!";
    else
        qDebug() << "TEST: Error al guardar Variable point!!!";*/

}

void SQLiteDatabaseTest::testStoreAlarmPoint()
{
    qsrand(uint(std::time(NULL)));
    AlarmPoint ap(rand() % 150, static_cast<AlarmState>(qrand() % 2), std::time(NULL));

    if (m_db->storeAlarmPoint(ap));/*
        qDebug() << "TEST: Alarm point guardado correctamente!!!";
    else
        qDebug() << "TEST: Error al guardar Variable point!!!";*/
}

void SQLiteDatabaseTest::testTime()
{
    if (m_db->changeBlockTimes().count() >= 5) {

        QList<time_t> l1 = m_db->onlyInsertTimes();
        m_db->setOnlyInsertTimes(QList<time_t>());

        QList<time_t> l2 = m_db->changeBlockTimes();
        m_db->setChangeBlockTimes(QList<time_t>());

        qDebug() << "Insert en el mismo bloque:\n";
        qDebug() << "# consulta     time (ms)\n";

        int sum = 0;
        for (int i = 0; i < l1.count(); ++i) {
            qDebug() <<i<<"             "<<l1.at(i);
            sum += l1.at(i);
        }

        qDebug() << "El promedio es: " <<double(sum)/double(l1.count());

        //! Insert con cambio de bloque

        qDebug() << "Insert en un cambio de bloque:\n";
        qDebug() << "# consulta     time (ms)\n";
        sum = 0;

        for (int i = 0; i < l2.count(); ++i) {
            qDebug() <<i<<"             "<<l2.at(i);
            sum += l2.at(i);
        }

        qDebug() << "El promedio es: " <<double(sum)/double(l2.count());
    }

}

void SQLiteDatabaseTest::testConsultDB()
{
    struct timeval m_tv;
    time_t t0 = NULL, t1 = NULL;

    qsrand(uint(std::time(NULL)));

    gettimeofday(&m_tv, NULL);                          //! Prueba de tiempos
    t0 = m_tv.tv_sec * 1000 + m_tv.tv_usec / 1000;

    QList<VariablePoint> l = m_db->getVariableInRange(qrand() % 50, 1426089972, 1526091103);

    gettimeofday(&m_tv, NULL);                          //! Prueba de tiempos
    t1 = m_tv.tv_sec * 1000 + m_tv.tv_usec / 1000;

//    qDebug() << "|ID     |   Value    |    Quality   |   TimeStamp|";
//    qDebug() << "-------------------------------------------------";
//    for (int i = 0; i < l.size(); ++i){
//        VariablePoint vp = l.at(i);
//        IntegerVariablePoint ivp = static_cast<IntegerVariablePoint&>(vp);
//        DoubleVariablePoint dvp = static_cast<DoubleVariablePoint&>(vp);

//        if (ivp.value() == WRONG_VALUE)
//            qDebug() << QString("%1        %2          %3            %4").arg(dvp.idVariable())
//                                                                         .arg(dvp.value().toDouble())
//                                                                         .arg(dvp.quality())
//                                                                         .arg(dvp.timeStamp());
//        else
//            qDebug() << QString("%1        %2          %3            %4").arg(ivp.idVariable())
//                                                                         .arg(ivp.value().toULongLong())
//                                                                         .arg(ivp.quality())
//                                                                         .arg(ivp.timeStamp());


//    }
    qDebug() << "-------------------------------------------------";

    qDebug() << QString("Consulta hecha en %1 ms\n\nCant. de elementos: %2").arg(std::difftime(t1,t0)).arg(l.size());
}
