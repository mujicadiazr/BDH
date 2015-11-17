#ifndef CLIENTWORKER_H
#define CLIENTWORKER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QHostAddress>

#include "AHP/ahpclient.h"
#include "AHP/historianalarmrequest.h"

class ClientWorker : public QThread
{
    Q_OBJECT
public:
    enum class Command {
        CONNECT = 1,
        SET_MODE,
        GET_REALTIME,
        GET_HISTORIAN,
        GET_VARS,
        GET_ALARMS,
        DISCONNECT,
        STOP_REALTIME_SENDING
    };

    explicit ClientWorker(ClientView *clientView,QObject *parent = 0);
    virtual ~ClientWorker();

    void run();

    void connectToServer(QHostAddress address, quint16 port);
    void setMode(AHP::ClientMode mode);
    void disconnectFromServer();
    void stopRealtimeSending();

    void getHistorianVariables(AHP::HistorianVariableRequest request);
    void getHistorianAlarms(AHP::HistorianAlarmRequest request);
    void getRealtimeVariables(AHP::RealtimeVariableRequest request);

    QList<Variable> varsConfigured();
    QString getDescriptionByID(quint32 id);

    const bool& real_time_sender_stoped() {
        QMutexLocker l(&m_mutex);
        return real_time_sender_working_;
    }

signals:
    void arriveNewRealtime(AHP::RealtimeVariableResponse response);
    void arriveNewHistorian(AHP::HistorianVariableResponse response);
    void arriveNewAlarm(AHP::HistorianAlarmResponse response);

    void isConnected(bool state);

public slots:

private:
    AHP::AHPClient *m_client;
    bool m_quit;
    QHostAddress m_address;
    quint16 m_port;
    QMutex m_mutex;
    QWaitCondition m_cond;
    AHP::HistorianVariableRequest m_historianRequest;
    AHP::HistorianAlarmRequest m_historianAlarmsRequest;
    AHP::RealtimeVariableRequest m_realtimeRequest;
    AHP::ClientMode m_clientMode;
    Command m_command;
    ClientView  *m_clientView;

    bool real_time_sender_working_;

};

#endif // CLIENTWORKER_H
