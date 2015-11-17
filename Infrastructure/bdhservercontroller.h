#ifndef BDHSERVERCONTROLLER_H
#define BDHSERVERCONTROLLER_H

#include <QXmlAttributes>

#include "sqlitedatabase.h"
#include "realtimecache.h"
#include "Common/globals.h"

namespace AHP {
  class HistorianVariableResponse;
  class HistorianVariableRequest;
  class HistorianAlarmResponse;
  class HistorianAlarmRequest;
  class AHPServer;
}

class QVariant;
class QHostAddress;
class TcpProxy;
class XmlLoader;
class DataAcquisition;
class Configuration;


class BDHServerController : public QObject
{
    Q_OBJECT

    //!SERVER COMPONENTS
    SQLiteDatabase  *m_dataStorage;
    AHP::AHPServer  *m_ahpServer;
    DataAcquisition *m_dataAcquisition;
    RealtimeCache   *m_realtimeCache;
    TcpProxy        *m_collectorInterface;
    XmlLoader       *m_configurationLoader;

    //! DATA
    Configuration   m_configuration;
    QString         m_myIP;
    quint16         m_myPort;
    quint32         m_alarmCounter;

public:
    explicit BDHServerController(QObject *parent = 0);
    ~BDHServerController();

    void start();

    void loadConfiguration();

    AHP::HistorianVariableResponse getHistorianVariables(
        const AHP::HistorianVariableRequest &request);
    AHP::HistorianAlarmResponse getHistorianAlarms(
        const AHP::HistorianAlarmRequest &request);

    RealtimeCache *realtimeCache() const;
    void setRealtimeCache(RealtimeCache *realtimeCache);

    DataAcquisition *dataAcquisition() const;
    void setDataAcquisition(DataAcquisition *dataAcquisition);

    AHP::AHPServer *ahpServer() const;
    void setAhpServer(AHP::AHPServer *ahpServer);

    SQLiteDatabase *dataStorage() const;
    void setDataStorage(SQLiteDatabase *dataStorage);

    TcpProxy *collectorInterface() const;
    void setCollectorInterface(TcpProxy *collectorInterface);

signals:
    void newVariablePoint(quint32 idVariable, quint8 quality , qulonglong timeStamp, QVariant value);
    void newAlarmPoint(quint32 idAlarm,quint16 state,quint64 timeStamp);

public slots:
    void emitVariablePoint(int varId, ushort quality, qulonglong timestamp, QVariant value);
    void emitAlarmPoint(int varId, ushort alarmId, qulonglong timestamp, QVariant state);

    void addConfigurationVariable(QXmlAttributes a);
    void addConfigurationAlarm(QXmlAttributes a);
    void addConfigurationCollector(QXmlAttributes a);
    void addConfigurationBDH(QXmlAttributes a);

    void configurationLoadFinish();

private:
    QHostAddress selectHostAddress();
    void run();
};

#endif // BDHSERVERCONTROLLER_H
