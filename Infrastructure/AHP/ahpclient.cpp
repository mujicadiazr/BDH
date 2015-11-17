#include <QtNetwork/QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QThread>


#include "ahpclient.h"
#include "ahpframefactory.h"
#include "ahp_commands_enum.h"
#include "../serializer.h"

#include "Presentation/clientview.h"
#include "mt.h"

AHP::AHPClient::AHPClient(ClientView* clientView, QObject *parent)
    :QObject(parent)
    , m_clientView(clientView)
    , m_socket(new QTcpSocket)
    , m_t(new Mt(&m_channel, m_socket, this))

{
    m_currentMode = ClientMode::NONE;
    m_realtimeSending = false;
}

QList<Variable> AHP::AHPClient::varsConfigured() const
{
    return m_varsConfigured;
}

void AHP::AHPClient::setVarsConfigured(const QList<Variable> &varsConfigured)
{
    m_varsConfigured = varsConfigured;
}

bool AHP::AHPClient::connectToServer(QHostAddress host, quint16 port)
{
    m_socket->connectToHost(host,port);

    AHPFrame request = AHPFrameFactory::instance()->createFrame(AHPCommand::HELLO);

    if (m_channel.writeFrame(request,m_socket)) {
        AHPFrame response;
        if (m_channel.readFrame(response,m_socket) && response.code() == (quint8)AHPCommand::ACCEPT)
            return true;
    }
    return false;
}

bool AHP::AHPClient::setMode(AHP::ClientMode mode)
{
    AHPFrame request ;
    Serializer msgPack;

    if (mode == ClientMode::REALTIME_MODE) {
        request = AHPFrameFactory::instance()->createFrame(AHPCommand::SET_REALTIME_MODE);
        m_currentMode = ClientMode::REALTIME_MODE;
    }
    else {
        request = AHPFrameFactory::instance()->createFrame(AHPCommand::SET_HISTORIAN_MODE);
        m_currentMode = ClientMode::HISTORIAN_MODE;
    }

    if (m_channel.writeFrame(request,m_socket)) {
        AHPFrame response;
        if (m_channel.readFrame(response,m_socket) && response.code() == (quint8)AHPCommand::MODE_SUCCESSFULL){
            VariablesConfigured vc;
            QByteArray pay = response.payload();

            msgPack.deserialize(pay,vc);

            m_varsConfigured = QList<Variable>::fromVector(QVector<Variable>::fromStdVector(vc.vars()));

            return true;
        }
    }
    return false;
}

AHP::HistorianVariableResponse AHP::AHPClient::getHistorianVariables(HistorianVariableRequest request)
{
    Serializer msgpackk;
    QByteArray payload;
    HistorianVariableResponse resp = HistorianVariableResponse();
    QByteArray responsePayload;
    request.serialize(&msgpackk,&payload);

    qDebug() << payload.size();

    AHPFrame requestFrame = AHPFrameFactory::instance()->createFrame(AHPCommand::GET_HISTORIAN_VARIABLES,payload);

    if (m_channel.writeFrame(requestFrame,m_socket)) {
        AHPFrame responseFrame;

        if (!m_channel.readFrame(responseFrame,m_socket))
            qDebug() << "Trama de respuesta incorrecta\n";

        responsePayload = responseFrame.payload();

        msgpackk.deserialize(responsePayload,resp);

    }

    return resp;
}

AHP::HistorianAlarmResponse AHP::AHPClient::getHistorianAlarms(HistorianAlarmRequest request)
{
    Serializer msgpackk;
    QByteArray payload;
    HistorianAlarmResponse resp = HistorianAlarmResponse();
    QByteArray responsePayload;
    request.serialize(&msgpackk,&payload);

    qDebug() << payload.size();

    AHPFrame requestFrame = AHPFrameFactory::instance()->createFrame(AHPCommand::GET_HISTORIAN_ALARMS,payload);

    if (m_channel.writeFrame(requestFrame,m_socket)) {
        AHPFrame responseFrame;

        if (!m_channel.readFrame(responseFrame,m_socket))
            qDebug() << "Trama de respuesta incorrecta\n";

        responsePayload = responseFrame.payload();

        msgpackk.deserialize(responsePayload,resp);

    }

    return resp;
}

bool AHP::AHPClient::getRealtimeVariables(RealtimeVariableRequest request)
{
    Serializer msgpackk;
    QByteArray payload;

    request.serialize(&msgpackk,&payload);

    qDebug() << payload.size();

    AHPFrame requestFrame = AHPFrameFactory::instance()->createFrame(AHPCommand::GET_REALTIME_VARIABLES,payload);

    bool ret{false};

    if (m_channel.writeFrame(requestFrame,m_socket)){
      m_socket->flush();
      m_realtimeSending = true;
      ret = true;
      connect(m_t, SIGNAL(arriveRealtimeResponseTT(RealtimeVariableResponse)), m_clientView, SLOT(setRealtimeValues(RealtimeVariableResponse)));
      m_t->start();
    }
    return ret;
}


//void AHP::AHPClient::ReceiveNewRealtimeVariablePoint()
//{
//    Serializer msgpackk;
//    QByteArray payload;
//    RealtimeVariableResponse resp = RealtimeVariableResponse();
//    QByteArray responsePayload;
//    AHPFrame responseFrame;

//    if (!m_channel.readFrame(responseFrame,m_socket)) {
//        qDebug() << "Trama de respuesta incorrecta\n";
//        return;
//    }

//    responsePayload = responseFrame.payload();

//    msgpackk.deserialize(responsePayload,resp);

//    emit arriveRealtimeResponse(resp);
//}

bool AHP::AHPClient::stopRealtimeSending()
{
    bool ret = false;

    m_t->terminate();
    m_t->wait();

    m_realtimeSending = false;

    AHPFrame request = AHPFrameFactory::instance()->createFrame(AHPCommand::STOP_REALTIME_SENDING);

    if (m_channel.writeFrame(request,m_socket)) {
        AHPFrame response;

        if (m_channel.readFrame(response,m_socket) && response.code() == (quint8)AHPCommand::ACCEPT){
            qDebug() << "STOP REALTIME SENDING SUCCESSFULL!!\n";
            ret = true;

            disconnect(m_t, SIGNAL(arriveRealtimeResponseTT(RealtimeVariableResponse)), this, SIGNAL(arriveRealtimeResponse(RealtimeVariableResponse)));

        }
    }
    return ret;
}

bool AHP::AHPClient::disconnectFromServer()
{
    AHPFrame request = AHPFrameFactory::instance()->createFrame(AHPCommand::RESET_MODE);
    bool resetRet = false;

    if (m_channel.writeFrame(request,m_socket)) {
        AHPFrame response;
        if (m_channel.readFrame(response,m_socket) && response.code() == (quint8)AHPCommand::ACCEPT)
            resetRet = true;
        qDebug() << "RESET SUCCESSFULL!!\n";
    }

    if (resetRet)
    {
        request = AHPFrameFactory::instance()->createFrame(AHPCommand::DISCONNECT);

        if (m_channel.writeFrame(request,m_socket))
        {
            AHPFrame response;
            if (m_channel.readFrame(response,m_socket) && response.code() == (quint8)AHPCommand::ACCEPT)
            {
                qDebug() << "DISCONNECT SUCCESSFULL!!\n";
                return true;
            }
        }

    }
    return false;
}






