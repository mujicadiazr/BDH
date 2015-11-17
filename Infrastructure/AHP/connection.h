#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdio>

#include <QThread>
#include <QTcpSocket>
#include <vector>
#include <QVector>
#include <QMutex>

#include "Common/globals.h"
#include "ahpframe.h"
#include "ahp_commands_enum.h"
#include "ahp_states_enum.h"
#include "ahpframefactory.h"
#include "historianvariablerequest.h"
#include "historianvariableresponse.h"
#include "realtimevariablerequest.h"
#include "realtimevariableresponse.h"
#include "variablesconfigured.h"
#include "Infrastructure/bdhservercontroller.h"
#include "Infrastructure/serializer.h"
#include "ahpchannel.h"

#define MAX_CONNECTIONS 100

namespace AHP {

class Connection : public QThread
{
    Q_OBJECT

private:
    qintptr m_socketDescriptor;
    AHPState m_state;
    quint32 m_connectionCount;
    BDHServerController *m_controller;
    QTcpSocket *m_tcpSocket;
    QVector<quint32> m_realtimeSendingVariables;
    AHPChannel *m_channel;
    QMutex m_mutex;

public:
    Connection(BDHServerController *controller, qintptr socketDescriptor, quint32 connectionCount, QObject *parent = 0);
    ~Connection();

    void run();

    void processBeginState(const AHPFrame &request);
    void processConnectedState(const AHPFrame &request);
    void processRealtimeState(const AHPFrame &request);
    void processRealtimeSendingState(const AHPFrame &request);
    void processHistorianState(const AHPFrame &request);


    BDHServerController *controller() const;
    void setController(BDHServerController *controller);

signals:
    void error(int socketError, const QString &message);
public slots:
    void sendRealtimeVariable(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value);
};

}

#endif // CONNECTION_H
