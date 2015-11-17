#ifndef AHPCLIENT_H
#define AHPCLIENT_H

#include <QObject>
#include <QAbstractSocket>
#include <QList>

#include "../../Common/globals.h"
#include "ahpframe.h"
#include "historianvariablerequest.h"
#include "historianalarmrequest.h"
#include "historianalarmresponse.h"
#include "historianvariableresponse.h"
#include "realtimevariablerequest.h"
#include "realtimevariableresponse.h"
#include "ahpchannel.h"
#include "../../Domain/variable.h"

class QTcpSocket;

class ClientView;

namespace AHP {

class Mt;

enum class ClientMode {
    REALTIME_MODE,
    HISTORIAN_MODE,
    NONE = -1
};

class AHPClient : public QObject
{
    Q_OBJECT

private:
    AHPChannel m_channel;
    QTcpSocket *m_socket;
    ClientMode m_currentMode;
    QList<Variable> m_varsConfigured;
    bool            m_realtimeSending;

public:
    explicit AHPClient(ClientView* clientView, QObject * parent = 0);
    ~AHPClient() {
        delete m_socket;
    }

    bool connectToServer(QHostAddress host, quint16 port);
    bool setMode(ClientMode mode);
    bool stopRealtimeSending();
    bool disconnectFromServer();

    HistorianVariableResponse getHistorianVariables(HistorianVariableRequest request);
    HistorianAlarmResponse getHistorianAlarms(HistorianAlarmRequest request);
    bool getRealtimeVariables(RealtimeVariableRequest request);

    QList<Variable> varsConfigured() const;
    void setVarsConfigured(const QList<Variable> &varsConfigured);

signals:
    void arriveRealtimeResponse(RealtimeVariableResponse response);

public slots:
//    void ReceiveNewRealtimeVariablePoint();

//void error(QAbstractSocket::SocketError err);

private:
    Mt *m_t;
    ClientView *m_clientView;

};

}

#endif // AHPCLIENT_H
