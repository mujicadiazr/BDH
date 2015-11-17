#include "sqlitedatabase.h"
#include <QDir>
#include <QDebug>
#include <vector>

SQLiteDatabase::SQLiteDatabase(QObject *parent)
{
    m_mainDB = QSqlDatabase::addDatabase(QString("QSQLITE"),QString("mainConnection"));
    m_currentBlockDB = QSqlDatabase::addDatabase(QString("QSQLITE"),QString("currentBlockConnection"));
    m_consultBlockDB = QSqlDatabase::addDatabase(QString("QSQLITE"),QString("consultBlockConnection"));
    m_dataDir = QDir(QString("data"));  //! Directorio que contiene las DB SQLite
    m_conf = Configuration();
}

SQLiteDatabase::~SQLiteDatabase()
{
    m_mainDB.close();
    m_currentBlockDB.close();
    m_consultBlockDB.close();
}

QList<time_t> SQLiteDatabase::onlyInsertTimes() const
{
    return m_onlyInsertTimes;
}

void SQLiteDatabase::setOnlyInsertTimes(const QList<time_t> &onlyInsertTimes)
{
    m_onlyInsertTimes = onlyInsertTimes;
}

QList<time_t> SQLiteDatabase::changeBlockTimes() const
{
    return m_changeBlockTimes;
}

void SQLiteDatabase::setChangeBlockTimes(const QList<time_t> &changeBlockTimes)
{
    m_changeBlockTimes = changeBlockTimes;
}

void SQLiteDatabase::storeIncommingVariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value)
{
    storeVariablePoint(idVariable,quality,timeStamp,value);
}

void SQLiteDatabase::storeIncommingAlarmPoint(quint32 idAlarm, AlarmState state, quint64 timeStamp)
{
    storeAlarmPoint(AlarmPoint(idAlarm,state,timeStamp));
}

bool SQLiteDatabase::isConfigured()
{
    QDir dataDir(QString("data"));

    if (dataDir.exists()) {
        QFile mainDB(dataDir.filePath("main.db"));
        QFile b1DB(dataDir.filePath("b1.db"));

        if (mainDB.exists()){ //Si existe el archivo main.db
            m_mainDB.setDatabaseName(mainDB.fileName());
            if (m_mainDB.open()) { //Si abre correctamente la DB
                QSqlQuery query("SELECT COUNT(*) as cant FROM Variable;",m_mainDB);

                if (query.exec()){ //Si la consulta se ejecuta correctamente
                    query.next();
                    quint32 cantVar = query.value(query.record().indexOf("cant")).toUInt();
                    if (cantVar > 0){ //Si estan agregadas las variables
//                        qDebug() << QString("DB IS CONFIGURED:)\n");

                        if (b1DB.exists()) //Si existe al menos un bloque
                            return true;
                        /*else
                            qDebug() << QString("NO EXISTEN BLOQUES");*/

                    } /*else
                        qDebug() << QString("NO EXISTEN VARIABLES CONFIGURADAS");*/
                } /*else
                    qDebug() << QString("Fallo la ejecucion de: \n SELECT COUNT(*) as cant FROM Variable; \n en: ")+mainDB.fileName();*/

                m_mainDB.close();
            } /*else
                qDebug() << QString("NO PUDO ABRIRSE LA DB "+mainDB.fileName());*/
        } /*else
            qDebug() << QString("NO EXISTE ARCHIVO "+mainDB.fileName());*/

    } /*else
        qDebug() << QString("NO EXISTE DIRECTORIO DATA");*/

    return false;
}

bool SQLiteDatabase::configureTest()
{
    QList<Variable> varList;
    QList<Alarm> alarmList;

    int cont = 1;

    QList<QString> des;
    des << "Temperatura"
        << "Presion"
        << "Altura"
        << "Humedad"
        << "Nivel del agua"
        << "Sedosidad"
        << "Calor"
        << "Nivel del fuego"
        << "Nivel del aire"
        << "Viscosidad";

    for (int i = 0; i < des.size(); ++i) {
        Variable v = Variable(i,(quint8)DOUBLE,1000,des.at(i).toStdString());
        varList.append(v);

        for (int j = 1; j <= 3; ++j) {
            Alarm a = Alarm(i,cont++,rand() % 5,rand() % 10,rand() % 3,QString("Alarma de %1").arg(des.at(i)));
            alarmList.append(a);
        }
    }

    Configuration c;
    c.setVars(&varList);
    c.setAlarms(&alarmList);

    if (configure(c))
        qDebug() << "TEST: Configurado correctamente!!!";
    else
        qDebug() << "TEST: Error en la configuracion!!!";
}

bool SQLiteDatabase::configure(const Configuration &c)
{
    m_conf = c;

    QFile mainDB(m_dataDir.filePath("main.db"));

    m_mainDB.setDatabaseName(mainDB.fileName()); //! data/main.db nombre por default de la BD principal

    if (!m_mainDB.open()) {
        qDebug() << QString("NO PUDO ABRIRSE LA DB "+mainDB.fileName());
        return false;
    }

    int cantVariables = c.vars()->count();     //! Guardando variables
    for (int i=0; i<cantVariables; ++i)
        storeVariable(c.vars()->at(i));

    int cantAlarmas = c.alarms()->count();     //! Guardando alarmas
    for (int i=0; i<cantAlarmas; ++i)
        storeAlarm(c.alarms()->at(i));

    saveConfiguration(c);                           //! Guardando configuracion

    m_mainDB.close();                             //! Cierro la base de datos main.db

    return true;
}

bool SQLiteDatabase::connect()
{
    if (!isConfigured()) {
        qDebug() << QString("No se puede conectar, la BD no esta configurada.\n");
        return false;
    }

    QFile mainDB(m_dataDir.filePath("main.db"));
    m_mainDB.setDatabaseName(mainDB.fileName()); //! data/main.db nombre por default de la BD principal

    if (!m_mainDB.open()) {                                               //! Intento abrir la base de datos principal
        qDebug() << QString("NO PUDO ABRIRSE LA DB "+mainDB.fileName());
        return false;
    }

    loadConfiguration();                                                    //! Cargo la configuracion

//    QSqlQuery query(m_mainDB);

//    for (int i = 2; i <= 100; ++i) {
//        query.exec(QString("INSERT INTO Catalog VALUES(%1,1426128879,1426277889);").arg(i));
//    }

    QFile cbDB(m_dataDir.filePath(QString("b%1.db").arg(m_currentBlockNumber)));

    m_currentBlockDB.setDatabaseName(cbDB.fileName());                          //! Le seteo el filename del bloque actual

    if (!m_currentBlockDB.open()) {                                             //! Intento abrir el bloque actual
        qDebug() << QString("NO PUDO ABRIRSE EL BLOQUE ACTUAL"+cbDB.fileName());
        return false;
    }

    return true;
}

bool SQLiteDatabase::isConnected()
{
    return m_mainDB.isOpen() && m_currentBlockDB.isOpen();
}

bool SQLiteDatabase::storeAlarmPoint(const AlarmPoint ap)
{
    QSqlQuery query(m_mainDB);

    query.prepare("INSERT INTO Alarm_Point(idAlarm,state,timeStamp) VALUES(?,?,?);");

    query.addBindValue(ap.idAlarm());
    query.addBindValue(ap.state());
    query.addBindValue(ap.timeStamp());

    if (!query.exec()) {
        qDebug() << "FALLO al guardar un AlarmPoint en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
        return false;
    }

    return true;
}

bool SQLiteDatabase::storeVariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value)
{
    time_t t0 = NULL, t1 = NULL, t2 = NULL;

    //TODO: Esto hay q arreglarlo, por ahora provicional
    if (currentBlockUsedSize() >= m_conf.blockSize()) {  //! Si el bloque actual esta lleno

        gettimeofday(&m_tv, NULL);                          //! Prueba de tiempos
        t0 = m_tv.tv_sec * 1000 + m_tv.tv_usec / 1000;

        updateCatalog();                                 //! Actualizo el catalogo con el bloque actual

        if (remainingSize() >= m_conf.blockSize())       //! Si queda espacio para crear otro bloque
            createNewBlock();                            //! Creo otro bloque
        else
            deleteOldestBlock();                         //! Sino borro el bloque mas viejo

        qDebug() << QString("Ahora escribiendo en:  %1").arg(m_currentBlockDB.databaseName());
    }


    gettimeofday(&m_tv, NULL);                          //! Prueba de tiempos
    t1 = m_tv.tv_sec * 1000 + m_tv.tv_usec / 1000;

    QSqlQuery query(m_currentBlockDB);

    query.prepare("INSERT INTO VariablePoint(variableId,value,quality,timeStamp) VALUES(?,?,?,?);");

    query.addBindValue(idVariable);
    query.addBindValue(value);
    query.addBindValue(quality);
    query.addBindValue(timeStamp);

    if (!query.exec()) {
        qDebug() << "FALLO al guardar un VariablePoint en DB: "<< m_currentBlockDB.databaseName() << "\n"
                 << m_currentBlockDB.driver()->lastError().text();
        return false;
    }

    //qDebug() <<QString("Tamanno del currentBlock: %1 bytes").arg(currentBlockUsedSize());

    gettimeofday(&m_tv, NULL);                          //! Prueba de tiempos
    t2 = m_tv.tv_sec * 1000 + m_tv.tv_usec / 1000;

    if (t0 != NULL) {
        time_t diff = difftime(t2, t0);            //! Tiempo que demora un insert con cambio de bloque
        m_changeBlockTimes.push_back(diff);
    }

    time_t diff = std::difftime(t2, t1);                //! Tiempo que demora un insert en el mismo bloque
    m_onlyInsertTimes.push_back(diff);


    return true;
}

QList<VariablePoint> SQLiteDatabase::getVariableInRange(quint32 idVariable, quint64 beginTimeStamp, quint64 endTimeStamp)
{
    QList<VariablePoint> vars;

    if (m_currentBlockNumber == 1 && m_oldestBlockNumber == 1) {                        //! Si no se ha llenado el bloque 1 busco directo en el
        mountConsultBlock(1);
        vars.append(getDataFromConsultBlock(idVariable,beginTimeStamp,endTimeStamp));
    } else {                                                                            //! Sino busco en el catalogo

        QList<quint32> blocksNumber = BlocksInTimeRange(beginTimeStamp,endTimeStamp);       //! Busco en el catalogo los bloques que tengo que cargar

        if (!blocksNumber.count()) {
            qDebug() << QString("No existen puntos de la variable %1 entre los timeStamp %2 y %3\n").arg(idVariable).arg(beginTimeStamp).arg(endTimeStamp);
            return vars;
        }

        foreach (quint32 b, blocksNumber) {                                                 //! Por cada bloque q contiene esa variable en ese rango de tiempo
            if (!mountConsultBlock(b)) {
                qDebug()<< QString("El bloque %1 no pudo ser montado para consultar\n").arg(b);
                continue;
            }
            vars.append(getDataFromConsultBlock(idVariable,beginTimeStamp,endTimeStamp));
        }
    }

    return vars;
}

QList<AlarmPoint> SQLiteDatabase::getAlarmByID(quint32 idVariable)
{
  QSqlQuery query(m_mainDB);
  QList<AlarmPoint> alams;

  query.prepare("SELECT * FROM Alarm_Point ap INNER JOIN Alarm a ON (a.idVariable = ? AND ap.idAlarm = a.idAlarm);");
  qDebug() << "query: " <<query.lastQuery()<<"\n";
  query.addBindValue(idVariable);

  if(!query.exec())
      qDebug() << "FALLO al obtener un AlarmPoint en DB: "<< m_mainDB.databaseName() << "\n"
               << m_mainDB.driver()->lastError().text();

  while (query.next()) {
    AlarmPoint a;
    a.setIdAlarm(query.value(query.record().indexOf("idAlarm")).toUInt());
    a.setState(static_cast<AlarmState>(
                 query.value(query.record().indexOf("state")).toUInt()));
    a.setTimeStamp(
          query.value(query.record().indexOf("timeStamp")).toULongLong());
    alams.push_back(a);
  }
  return std::move(alams);
}

QList<AlarmPoint> SQLiteDatabase::getAlarmInRange(quint32 idAlarm, quint64 beginTimeStamp, quint64 endTimeStamp)
{
    QSqlQuery query(m_mainDB);
    QList<AlarmPoint> alams;

    query.prepare("SELECT * FROM Alarm_Point WHERE idAlarm = ? AND timeStamp BETWEEN ? AND ?");

    query.addBindValue(idAlarm);
    query.addBindValue(beginTimeStamp);
    query.addBindValue(endTimeStamp);

    if(!query.exec())
        qDebug() << "FALLO al obtener un AlarmPoint en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();

    while (query.next()) {
        AlarmPoint a;
        a.setIdAlarm(query.value(query.record().indexOf("idAlarm")).toUInt());
        a.setState(static_cast<AlarmState> (query.value(query.record().indexOf("state")).toUInt()));
        a.setTimeStamp(query.value(query.record().indexOf("timeStamp")).toULongLong());

        alams.push_back(a);
    }

    return alams;
}

QList<AlarmPoint> SQLiteDatabase::getAlarmInRange(quint64 beginTimeStamp, quint64 endTimeStamp)
{
    QSqlQuery query(m_mainDB);
    QList<AlarmPoint> alams;

    query.prepare("SELECT * FROM Alarm_Point WHERE timeStamp BETWEEN ? AND ?");

    query.addBindValue(beginTimeStamp);
    query.addBindValue(endTimeStamp);

    if(!query.exec())
        qDebug() << "FALLO al obtener un AlarmPoint en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();

    while (query.next()) {
        AlarmPoint a;
        a.setIdAlarm(query.value(query.record().indexOf("idAlarm")).toUInt());
        a.setState(static_cast<AlarmState> (query.value(query.record().indexOf("state")).toUInt()));
        a.setTimeStamp(query.value(query.record().indexOf("timeStamp")).toULongLong());

        alams.push_back(a);
    }

    return alams;
}

QList<AlarmPoint> SQLiteDatabase::getAlarmInState(quint32 idAlarm, const AlarmState &state)
{
    QSqlQuery query(m_mainDB);
    QList<AlarmPoint> alams;

    query.prepare("SELECT * FROM Alarm_Point WHERE idAlarm = ? AND state = ?");

    query.addBindValue(idAlarm);
    query.addBindValue(state);

    if(!query.exec())
        qDebug() << "FALLO al obtener un AlarmPoint en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();

    while (query.next()) {
        AlarmPoint a;
        a.setIdAlarm(query.value(query.record().indexOf("idAlarm")).toUInt());
        a.setState(static_cast<AlarmState> (query.value(query.record().indexOf("state")).toUInt()));
        a.setTimeStamp(query.value(query.record().indexOf("timeStamp")).toULongLong());

        alams.push_back(a);
    }

    return alams;
}

QList<AlarmPoint> SQLiteDatabase::getAlarmInState(const AlarmState &state)
{
    QSqlQuery query(m_mainDB);
    QList<AlarmPoint> alams;

    query.prepare("SELECT * FROM Alarm_Point WHERE state = ?");

    query.addBindValue(state);

    if(!query.exec())
        qDebug() << "FALLO al obtener un AlarmPoint en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();

    while (query.next()) {
        AlarmPoint a;
        a.setIdAlarm(query.value(query.record().indexOf("idAlarm")).toUInt());
        a.setState(static_cast<AlarmState> (query.value(query.record().indexOf("state")).toUInt()));
        a.setTimeStamp(query.value(query.record().indexOf("timeStamp")).toULongLong());

        alams.push_back(a);
    }

    return alams;
}

QList<Variable> SQLiteDatabase::getVariablesConfigured()
{
    QSqlQuery query(m_mainDB);
    QList<Variable> vars;

    query.prepare("SELECT * FROM Variable;");

    if(!query.exec())
        qDebug() << "FALLO al obtener una Variable en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();

    while (query.next()) {
        Variable a;
        a.setId(query.value(query.record().indexOf("idVariable")).toUInt());
        a.setDataType(static_cast<ValueDataType>(query.value(query.record().indexOf("dataType")).toUInt()));
        a.setSampleTime(query.value(query.record().indexOf("sampleTime")).toUInt());
        a.setDescription(query.value(query.record().indexOf("description")).toString().toStdString());

        vars.push_back(a);
    }

    return vars;
}

void SQLiteDatabase::storeVariable(Variable v)
{
    QSqlQuery query(m_mainDB);

    query.prepare("INSERT INTO Variable(idVariable,dataType,sampleTime,description) VALUES(?,?,?,?)");

    query.addBindValue(v.id());
    query.addBindValue(quint8(v.dataType()));
    query.addBindValue(v.sampleTime());
    query.addBindValue(QString().fromStdString(v.description()));

    if (!query.exec())
        qDebug() << "FALLO al guardar una variable en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
}

void SQLiteDatabase::storeAlarm(Alarm a)
{
    QSqlQuery query(m_mainDB);

    query.prepare("INSERT INTO Alarm(idAlarm,idVariable,typeAlarm,severity,priority,description) VALUES(?,?,?,?,?,?)");

    query.addBindValue(a.idAlarm());
    query.addBindValue(a.idVariable());
    query.addBindValue(a.alarmType());
    query.addBindValue(a.severity());
    query.addBindValue(a.priority());
    query.addBindValue(a.description());

    if (!query.exec())
        qDebug() << "FALLO al guardar una alarma en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
}

bool SQLiteDatabase::saveConfiguration(const Configuration &c)
{
    bool ret = true;

    QSqlQuery query(m_mainDB);

    query.prepare("INSERT INTO DB_Config(diskSize,mainSize,blockSize,currentBlock,oldestBlock) VALUES(?,?,?,?,?);");

    query.addBindValue(c.diskSize());
    query.addBindValue(c.mainSize());
    query.addBindValue(c.blockSize());
    query.addBindValue(1);      //! Inicialmente los bloques actual y mas viejo es el mismo, en este caso 1
    query.addBindValue(1);

    if (!query.exec()) {
        qDebug() << "FALLO al guardar la configuracion en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
        ret = false;
    }


    //Agrego el 1er bloque en el catalogo y le pongo un timeStamp final absurdo que luego actualizo cuando se llena el bloque
    query.prepare("INSERT INTO Catalog(blockId,beginTimeStamp,endTimeStamp) VALUES(?,?,?);");
    query.addBindValue(1);
    query.addBindValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
    query.addBindValue(QDateTime::currentDateTime().addYears(100).toMSecsSinceEpoch());

    if (!query.exec()) {
        qDebug() << "FALLO al guardar la configuracion de 1er bloque en el catalogo: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
        ret = false;
    }

    return ret;

}

bool SQLiteDatabase::loadConfiguration()
{
    QSqlQuery query(m_mainDB);

    query.prepare("SELECT * FROM DB_Config;");

    if (!query.exec()) {
        qDebug() << "FALLO al cargar la configuracion de DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
        return false;
    }

    if (query.next()) {
        m_conf.setDiskSize(query.value(query.record().indexOf("diskSize")).toULongLong());
        m_conf.setMainSize(query.value(query.record().indexOf("mainSize")).toULongLong());
        m_conf.setBlockSize(query.value(query.record().indexOf("blockSize")).toULongLong());

        m_currentBlockNumber = query.value(query.record().indexOf("currentBlock")).toULongLong();
        m_oldestBlockNumber = query.value(query.record().indexOf("oldestBlock")).toULongLong();
    } else {
        qDebug() << "No se ha guardado la configuracion en: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
        return false;
    }
    return true;
}

void SQLiteDatabase::updateCurrentBlock()
{
    QSqlQuery query(m_mainDB);

    query.prepare("UPDATE DB_Config SET currentBlock = ? WHERE ROWID = 1;");

    query.addBindValue(m_currentBlockNumber);

    if (!query.exec())
        qDebug() << "FALLO al actualizar el currentBlock en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
}

void SQLiteDatabase::updateOldestBlock()
{
    QSqlQuery query(m_mainDB);

    query.prepare("UPDATE DB_Config SET oldestBlock = ? WHERE ROWID = 1;");

    query.addBindValue(m_oldestBlockNumber);

    if (!query.exec())
        qDebug() << "FALLO al actualizar el oldestBlock en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
}

void SQLiteDatabase::updateCatalog()
{
    QSqlQuery query(m_mainDB);

    QString q("SELECT * FROM Catalog WHERE blockId = ?;");
    query.prepare(q);

    query.addBindValue(m_currentBlockNumber);

    if (!query.exec())
        qDebug() << QString("FALLO la consulta %1 en DB: %2").arg(q).arg(m_mainDB.databaseName()) << "\n"
                 << m_mainDB.driver()->lastError().text();

    if (!query.next())
        query.exec(QString("INSERT INTO Catalog(blockId,beginTimeStamp,endTimeStamp) VALUES(%1,%2,%3);").arg(m_currentBlockNumber).arg(firstTimeStampInCurrentBlock()).arg(lastTimeStampInCurrentBlock()));
    else
        query.exec(QString("UPDATE Catalog SET beginTimeStamp = %2, endTimeStamp = %3 WHERE blockId = %1;").arg(m_currentBlockNumber).arg(firstTimeStampInCurrentBlock()).arg(lastTimeStampInCurrentBlock()));



}

quint32 SQLiteDatabase::maxBlockNumber()
{
    QSqlQuery query(m_mainDB);

    if (!query.exec(QString("SELECT MAX(blockId) as max_block FROM Catalog;"))) {
        qDebug() << "FALLO al obtener maximo numero de bloque en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();
        return -1;
    }

    query.next();

    return query.value(query.record().indexOf("max_block")).toUInt();
}

quint64 SQLiteDatabase::firstTimeStampInCurrentBlock()
{
    QSqlQuery query(m_currentBlockDB);

    if (!query.exec(QString("SELECT MIN(timeStamp) as min_ts FROM VariablePoint;"))) {
        qDebug() << QString("FALLO al %1 en DB: %2 ").arg(QString("SELECT MIN(timeStamp) as min_ts FROM VariablePoint;")).arg(m_currentBlockDB.databaseName())<< "\n"
                 << m_currentBlockDB.driver()->lastError().text();
        return -1;
    }

    if (query.next())
        return query.value(query.record().indexOf("min_ts")).toULongLong();

    return -1;
}

quint64 SQLiteDatabase::lastTimeStampInCurrentBlock()
{
    QSqlQuery query(m_currentBlockDB);

    if (!query.exec(QString("SELECT MAX(timeStamp) as max_ts FROM VariablePoint;"))) {
        qDebug() << "FALLO al obtener maximo timeStamp en DB: "<< m_currentBlockDB.databaseName() << "\n"
                 << m_currentBlockDB.driver()->lastError().text();
        return -1;
    }

    if (query.next())
        return query.value(query.record().indexOf("max_ts")).toUInt();

    return -1;
}

QList<quint32> SQLiteDatabase::BlocksInTimeRange(quint64 beginTimeStamp, quint64 endTimeStamp)
{
    QSqlQuery query(m_mainDB);
    QList<quint32> blocks = QList<quint32>();

    query.prepare("SELECT blockId FROM Catalog "
                  "WHERE beginTimeStamp >= ? and beginTimeStamp <= ? or endTimeStamp >= ? and endTimeStamp <= ? "
                  "ORDER BY beginTimeStamp;");

    query.addBindValue(beginTimeStamp);
    query.addBindValue(endTimeStamp);
    query.addBindValue(beginTimeStamp);
    query.addBindValue(endTimeStamp);

    if(!query.exec())
        qDebug() << "FALLO al obtener un AlarmPoint en DB: "<< m_mainDB.databaseName() << "\n"
                 << m_mainDB.driver()->lastError().text();

    while (query.next())
        blocks.push_back(query.value(query.record().indexOf("blockId")).toUInt());

    return blocks;
}

quint64 SQLiteDatabase::mainUsedSize()
{
    QFile mainDB(m_dataDir.filePath("main.db"));
    return mainDB.size();
}

quint64 SQLiteDatabase::blocksUsedSize()
{
    QDir dir("data");
    dir.setFilter(QDir::Files);
    quint64 blockSize = 0;

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
        if (list.at(i).fileName().at(0) == QChar('b'))
            blockSize += list.at(i).size();

    return blockSize;
}

quint64 SQLiteDatabase::currentBlockUsedSize()
{
    QFile f(m_currentBlockDB.databaseName());
    return f.size();
}

quint64 SQLiteDatabase::totalUsedSize()
{
    return m_conf.mainSize() + blocksUsedSize();
}

quint64 SQLiteDatabase::remainingSize()
{
    return m_conf.diskSize() - totalUsedSize();
}

bool SQLiteDatabase::createNewBlock()
{
    QFile btemp(m_dataDir.filePath("btemp.db"));                                  //! Obtengo el archivo de bloque temporal
    btemp.copy(m_dataDir.filePath(QString("b%1.db").arg(++m_currentBlockNumber)));//! Copio el bloque temporal con un numero mas del current
    //! e incremento el current

    updateCurrentBlock();                                                       //! Actualizo el currentBlock en la DB
    QFile cb(m_dataDir.filePath(QString("b%1.db").arg(m_currentBlockNumber)));
    m_currentBlockDB.close();                                                   //! Cierro la conexion al bloque actual
    m_currentBlockDB.setDatabaseName(cb.fileName());                            //! Le paso el nuevo nombre del bloque actual a la DB

    if (!m_currentBlockDB.open()) {                                             //! Intento abrir el bloque actual
        qDebug() << QString("NO PUDO ABRIRSE EL BLOQUE ACTUAL: "+cb.fileName());
        return false;
    }
    return true;
}

bool SQLiteDatabase::deleteOldestBlock()
{
    QFile oldblock(m_dataDir.filePath(QString("b%1.db").arg(m_oldestBlockNumber)));
    //TODO: Verificar que no este siendo leido este bloque (Posible semaforo)

    if (!oldblock.remove()) {                                                   //! Intento borrar el archivo de bloque mas viejo
        qDebug() << QString("El bloque: %1 no se ha podido borrar correctamente").arg(oldblock.fileName());
        return false;
    }

    qDebug() << QString("El bloque %1 ha sido borrado").arg(oldblock.fileName());

    QFile btemp(m_dataDir.filePath("btemp.db"));                                  //! Obtengo el archivo de bloque temporal
    btemp.copy(m_dataDir.filePath(QString("b%1.db").arg(m_oldestBlockNumber)));   //! Copio el nuevo currentBlock a partir del bloque temporal
    //! y el mismo tendra el numero del bloque borrado

    m_currentBlockNumber = m_oldestBlockNumber;                                   //! Ahora el bloque actual es el que era el mas viejo antes


    if (m_oldestBlockNumber == maxBlockNumber())                                  //! Cola circular: si oldestBlock es el maximo bloque
        m_oldestBlockNumber = 1;                                                  //!   entonces pasa a ser el primero
    else                                                                          //! sino
        ++m_oldestBlockNumber;                                                    //!   oldestBlock sera el proximo mas viejo

    updateCurrentBlock();                                                         //! Actualizo en la DB el valor de currentBlock u oldestBlock
    updateOldestBlock();

    QFile cb(m_dataDir.filePath(QString("b%1.db").arg(m_currentBlockNumber)));
    m_currentBlockDB.close();                                                   //! Cierro la conexion al bloque actual
    m_currentBlockDB.setDatabaseName(cb.fileName());                            //! Le paso el nuevo nombre del bloque actual a la DB

    if (!m_currentBlockDB.open()) {                                             //! Intento abrir el bloque actual
        qDebug() << QString("NO PUDO ABRIRSE EL BLOQUE ACTUAL: %1").arg(cb.fileName());
        return false;
    }
    return true;
}

bool SQLiteDatabase::mountConsultBlock(quint32 block)
{
    m_consultBlockDB.close();                                                   //! Cierro el bloque de consulta actual
    QFile cb(m_dataDir.filePath(QString("b%1.db").arg(block)));

    m_consultBlockDB.setDatabaseName(cb.fileName());

    if (!m_consultBlockDB.open()) {                                             //! Intento abrir el bloque actual
        qDebug() << QString("NO PUDO ABRIRSE EL BLOQUE PARA CONSULTA: ").arg(cb.fileName());
        return false;
    }
    return true;
}

QList<VariablePoint> SQLiteDatabase::getDataFromConsultBlock(quint32 idVariable, quint64 beginTimeStamp, quint64 endTimeStamp)
{
    QSqlQuery query(m_consultBlockDB);
    QList<VariablePoint> vars;

    query.prepare("SELECT * FROM VariablePoint WHERE variableId = ? AND timeStamp BETWEEN ? AND ?");

    query.addBindValue(idVariable);
    query.addBindValue(beginTimeStamp);
    query.addBindValue(endTimeStamp);

    if(!query.exec())
        qDebug() << "FALLO al obtener un VariablePoint en DB: "<< m_consultBlockDB.databaseName() << "\n"
                 << m_consultBlockDB.driver()->lastError().text();

    while (query.next()) {

        if (query.value(query.record().indexOf("value")).type() == QMetaType::LongLong){
            vars.push_back(VariablePoint(idVariable,
                                         query.value(query.record().indexOf("quality")).toInt(),
                                         query.value(query.record().indexOf("timeStamp")).toULongLong(),
                                         query.value(query.record().indexOf("value")).toLongLong(),
                                         INTEGER));
            qDebug() <<"LongLong\n";
        }
        else {

            vars.push_back(VariablePoint(idVariable,
                                           query.value(query.record().indexOf("quality")).toInt(),
                                           query.value(query.record().indexOf("timeStamp")).toLongLong(),
                                           query.value(query.record().indexOf("value")).toDouble(),
                                           DOUBLE));
            qDebug() <<"Double\n";
        }
    }

    return vars;
}
