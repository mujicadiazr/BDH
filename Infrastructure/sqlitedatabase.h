#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <sys/time.h>
#include <ctime>
#include <QVariant>
#include <QtSql>

#include "Domain/configuration.h"
#include "Domain/variablepoint.h"
#include "Domain/alarmpoint.h"

#define SIZE_TO_COMPLETE_BLOCK 30

class QDir;


class SQLiteDatabase : public QObject
{
    Q_OBJECT

    QSqlDatabase m_mainDB;            //! Base de datos principal
    QSqlDatabase m_currentBlockDB;  //! Bloque actual donde se alamacenan los VariablePoint
    QSqlDatabase m_consultBlockDB;  //! Database donde monto cada bloque para hacerle consultas
    Configuration m_conf;
    QDir           m_dataDir;
    quint32        m_currentBlockNumber;
    quint32        m_oldestBlockNumber;

    //! Atributos para pruebas
    QList<time_t> m_onlyInsertTimes;
    QList<time_t> m_changeBlockTimes;
    struct timeval m_tv;

public:
    explicit SQLiteDatabase(QObject *parent = 0);
    ~SQLiteDatabase();

    bool isConfigured();
    bool configure(const Configuration& c);
    bool configureTest();
    bool connect();
    bool isConnected();
    bool storeAlarmPoint(const AlarmPoint ap);
    bool storeVariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value);

    QList<VariablePoint> getVariableInRange(quint32 idVariable, quint64 beginTimeStamp, quint64 endTimeStamp);
    QList<AlarmPoint> getAlarmByID(quint32 idVariable);

    QList<AlarmPoint> getAlarmInRange(quint32 idAlarm, quint64 beginTimeStamp, quint64 endTimeStamp);
    QList<AlarmPoint> getAlarmInRange(quint64 beginTimeStamp, quint64 endTimeStamp);
    QList<AlarmPoint> getAlarmInState(quint32 idAlarm, const AlarmState &state);
    QList<AlarmPoint> getAlarmInState(const AlarmState &state);

    QList<Variable> getVariablesConfigured();

    //! Getters and Setters
    QList<time_t> onlyInsertTimes() const;
    void setOnlyInsertTimes(const QList<time_t> &onlyInsertTimes);
    QList<time_t> changeBlockTimes() const;
    void setChangeBlockTimes(const QList<time_t> &changeBlockTimes);

public slots:
    void storeIncommingVariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value);
    void storeIncommingAlarmPoint(quint32 idAlarm, AlarmState state, quint64 timeStamp);

private:
    //! Metodos que alamcenan y devuelven informacion de la DB
    void storeVariable(Variable v);
    void storeAlarm(Alarm a);
    bool saveConfiguration(const Configuration &c);
    bool loadConfiguration();
    void updateCurrentBlock();
    void updateOldestBlock();
    void updateCatalog();
    quint32 maxBlockNumber();
    quint64 firstTimeStampInCurrentBlock();
    quint64 lastTimeStampInCurrentBlock();
    QList<quint32> BlocksInTimeRange(quint64 beginTimeStamp, quint64 endTimeStamp);

    //! Metodos que devuelven datos sobre el uso de espacio del disco
    quint64 mainUsedSize();
    quint64 blocksUsedSize();
    quint64 currentBlockUsedSize();
    quint64 totalUsedSize();
    quint64 remainingSize();

    //! Metodos para manipular los bloques
    bool createNewBlock();
    bool deleteOldestBlock();
    bool mountConsultBlock(quint32 block);
    QList<VariablePoint> getDataFromConsultBlock(quint32 idVariable, quint64 beginTimeStamp, quint64 endTimeStamp);

};

#endif // SQLITEDATABASE_H
