#include <QtNetwork>
#include <QXmlSimpleReader>
#include <QXmlInputSource>

#include <vector>

#include "bdhservercontroller.h"
#include "AHP/ahpserver.h"
#include "AHP/historianvariablerequest.h"
#include "AHP/historianvariableresponse.h"
#include "AHP/historianalarmrequest.h"
#include "AHP/historianalarmresponse.h"
#include "tcpproxy.h"
#include "xmlloader.h"
#include "dataacquisition.h"
#include "Domain/configuration.h"
#include "globalfunctions.h"

BDHServerController::BDHServerController(QObject *parent)
    :QObject(parent)
{
    m_dataStorage = new SQLiteDatabase(this);
    m_ahpServer = new AHP::AHPServer(this,this);
    m_dataAcquisition = new DataAcquisition(this);
    m_realtimeCache = new RealtimeCache(this);
    m_collectorInterface = nullptr;
    m_configurationLoader = new XmlLoader(this);

    m_alarmCounter = 0;
    m_myIP = selectHostAddress().toString();
    m_myPort = 9999;

    connect(this,SIGNAL(newVariablePoint(quint32,quint8,qulonglong,QVariant)),m_realtimeCache,SLOT(storeVariablePoint(quint32,quint8,quint64,QVariant)));

    //! Acquisition Simulation
    connect(m_dataAcquisition,SIGNAL(incommingAlarmPoint(quint32,AlarmState,quint64)),m_dataStorage,SLOT(storeIncommingAlarmPoint(quint32,AlarmState,quint64)));
    connect(m_dataAcquisition,SIGNAL(incommingVariablePoint(quint32,quint8,quint64,QVariant)),m_realtimeCache,SLOT(storeVariablePoint(quint32,quint8,quint64,QVariant)));

    //! Connection between RealtimeCache and SQLiteDatabase to persist data
    connect(m_realtimeCache,SIGNAL(persistData(quint32,quint8,quint64,QVariant)),m_dataStorage,SLOT(storeIncommingVariablePoint(quint32,quint8,quint64,QVariant)));

    //! Configuration connects
    connect(m_configurationLoader,SIGNAL(loadFinished()),SLOT(configurationLoadFinish()));

    connect(m_configurationLoader,SIGNAL(newCollector(QXmlAttributes)),this,SLOT(addConfigurationCollector(QXmlAttributes)));
    connect(m_configurationLoader,SIGNAL(newVariable(QXmlAttributes)),this,SLOT(addConfigurationVariable(QXmlAttributes)));
    connect(m_configurationLoader,SIGNAL(newAlarm(QXmlAttributes)),this,SLOT(addConfigurationAlarm(QXmlAttributes)));
    connect(m_configurationLoader,SIGNAL(newBDH(QXmlAttributes)),this,SLOT(addConfigurationBDH(QXmlAttributes)));
}

BDHServerController::~BDHServerController()
{
    m_ahpServer->close();

    m_ahpServer->deleteLater();
    m_dataStorage->deleteLater();
    m_dataAcquisition->deleteLater();
    m_realtimeCache->deleteLater();
}

void BDHServerController::start()
{
    loadConfiguration();
}

RealtimeCache *BDHServerController::realtimeCache() const
{
    return m_realtimeCache;
}

void BDHServerController::setRealtimeCache(RealtimeCache *realtimeCache)
{
    m_realtimeCache = realtimeCache;
}

DataAcquisition *BDHServerController::dataAcquisition() const
{
    return m_dataAcquisition;
}

void BDHServerController::setDataAcquisition(DataAcquisition *dataAcquisition)
{
    m_dataAcquisition = dataAcquisition;
}

AHP::AHPServer *BDHServerController::ahpServer() const
{
    return m_ahpServer;
}

void BDHServerController::setAhpServer(AHP::AHPServer *ahpServer)
{
    m_ahpServer = ahpServer;
}

SQLiteDatabase *BDHServerController::dataStorage() const
{
    return m_dataStorage;
}

void BDHServerController::setDataStorage(SQLiteDatabase *dataStorage)
{
    m_dataStorage = dataStorage;
}

TcpProxy *BDHServerController::collectorInterface() const
{
    return m_collectorInterface;
}

void BDHServerController::setCollectorInterface(TcpProxy *collectorInterface)
{
    m_collectorInterface = collectorInterface;
}

void BDHServerController::run()
{
    //! Check if database configured, else configure--------
    if (!m_dataStorage->isConfigured()) {
        qDebug() <<"SETTING UP ...\n";
        m_dataStorage->configure(m_configuration);
    }

    //! Connect database--------------------------
    if (m_dataStorage->connect())
        qDebug() <<"CONNECTING TO BD ...\n";

    //! Put server listen
    m_ahpServer->listen(QHostAddress(m_myIP), m_myPort);
    qDebug() <<"LISTEN ON: "<< m_myIP <<":"<<m_myPort<<"\n";


    //! Connect with real collector
    m_collectorInterface->connectToServer();
    qDebug() <<"CONNECTING TO COLLECTOR ON: "<< m_collectorInterface->host() <<":"<<m_collectorInterface->port()<<"\n";

    //! Simulated collector
//    m_dataAcquisition->run();
//    qDebug()<<"SIMULATED COLLECTOR RUN...\n";
}

void BDHServerController::loadConfiguration()
{
    QFile file;

    //! Set XML dir
    file.setFileName(QString("./hmi.xml"));

    //! Open file
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Not hmi.xml file present and can't configure. \n CLOSING...";
        exit(-1);
    }

    //! Set XML reader
    QXmlSimpleReader reader;
    reader.setContentHandler(m_configurationLoader);
    reader.setErrorHandler(m_configurationLoader);
    QXmlInputSource xmlInputSource(&file);

    //! Parse hmi.xml file for proyect configuration
    if (!reader.parse(xmlInputSource))
    {
        //!Show error parsing hmi.xml
        qDebug() << QString("There was an error while loading the configuration: %1. \n CLOSING...").arg(m_configurationLoader->errorString());
        exit(-1);
    }

}

AHP::HistorianVariableResponse BDHServerController::getHistorianVariables(const AHP::HistorianVariableRequest &request)
{
    AHP::HistorianVariableResponse response;

    for (int i = 0; i < request.varsId().size(); ++i) {
        QList<VariablePoint> values = m_dataStorage->getVariableInRange(request.varsId().at(i),request.timeRanges().at(i).first,request.timeRanges().at(i).second);

        response.append((quint32)request.varsId().at(i), values.toVector().toStdVector());
    }

    return response;
}

AHP::HistorianAlarmResponse BDHServerController::getHistorianAlarms(const AHP::HistorianAlarmRequest &request)
{
    AHP::HistorianAlarmResponse response;

    for (int i = 0; i < request.varsId().size(); ++i)
    {
        QList<AlarmPoint> values = m_dataStorage->getAlarmByID(request.varsId().at(i));
        response.append(static_cast<quint32>(request.varsId().at(i)),
                        values.toVector().toStdVector());
    }
    return response;
}

QHostAddress BDHServerController::selectHostAddress()
{
    QHostAddress address;

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
            address = ipAddressesList.at(i);
            break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if (address.isNull())
        address = QHostAddress(QHostAddress::LocalHost);

    return address;
}

void BDHServerController::emitVariablePoint(int varId, ushort quality, qulonglong timestamp, QVariant value)
{
    emit newVariablePoint(varId,(quint8)quality,timestamp,value);

    /*qDebug() << QString("COLLECTOR VARIABLE SAMPLE: ID:%1, QUALITY:%2, TIMESTAMP:%3, VALUE:%4\n").arg(varId)
                .arg(quality)
                .arg(timestamp)
                .arg(value.toString());*/
}

void BDHServerController::emitAlarmPoint(int varId, ushort alarmId, qulonglong timestamp, QVariant state)
{
    emit newAlarmPoint(alarmId,state.toUInt(),timestamp);

    qDebug() << QString("COLLECTOR ALARM RAISED: VAR_ID:%1, ALARM_ID:%2, TIMESTAMP:%3, STATE:%4\n").arg(varId)
                .arg(alarmId)
                .arg(timestamp)
                .arg(state.toString());
}

void BDHServerController::addConfigurationVariable(QXmlAttributes a)
{
    Variable v;

    v.setId(a.value("id").toUInt());
    v.setDescription(a.value("name").toStdString());
    v.setDataType((quint8)a.value("dataType").toUShort());
    v.setSampleTime(a.value("requestTime").toUInt());

    m_configuration.addVariable(v);

//    qDebug() <<"Encontro una variable!!!!\n";
}

void BDHServerController::addConfigurationAlarm(QXmlAttributes a)
{
    Alarm alarm;

    alarm.setIdAlarm(m_alarmCounter++);
    alarm.setAlarmType((quint8)a.value("type").toInt());
    alarm.setPriority((quint8)a.value("priority").toInt());
    alarm.setSeverity((quint8)a.value("severity").toInt());
    alarm.setDescription(a.value("description"));

    m_configuration.addAlarm(alarm);

//    qDebug() <<"Encontro una alarma!!!!\n";
}

void BDHServerController::addConfigurationCollector(QXmlAttributes a)
{
    //Delete created instance of TcpProxy.
    m_collectorInterface->deleteLater();

    //Get host and port
    QString host = GetValue(a, "host", selectHostAddress().toString());
    int port = GetValue(a, "port", "8888").toInt();

    m_collectorInterface = new TcpProxy(host, port, this);

    connect(m_collectorInterface,SIGNAL(variableChanged(int,ushort,qulonglong,QVariant)),this,SLOT(emitVariablePoint(int,ushort,qulonglong,QVariant)));
    connect(m_collectorInterface,SIGNAL(alarmRaised(int,ushort,qulonglong,QVariant)),this,SLOT(emitAlarmPoint(int,ushort,qulonglong,QVariant)));

//    qDebug() <<"Encontro un collector!!!!\n";
}

void BDHServerController::addConfigurationBDH(QXmlAttributes a)
{
//    m_myIP = GetValue(a, "host", selectHostAddress().toString());
    m_myPort = GetValue(a, "port", "9999").toInt();

    m_configuration.setDiskSize(GetValue(a, "diskSize", "1073741824").toULongLong());
//    qDebug() <<"Encontro un BDH!!!!\n";
}

void BDHServerController::configurationLoadFinish()
{
    qDebug() <<"LOADING SETTINGS ...\n";
    run();
}


