#include "connection.h"
#include "../bdhservercontroller.h"

BDHServerController *AHP::Connection::controller() const
{
    return m_controller;
}

void AHP::Connection::setController(BDHServerController *controller)
{
    m_controller = controller;
}
AHP::Connection::Connection(BDHServerController *controller, qintptr socketDescriptor, quint32 connectionCount, QObject *parent)
    :QThread(parent), m_socketDescriptor(socketDescriptor), m_connectionCount(connectionCount)
{
    m_state = AHPState::BEGIN_STATE;
    m_controller = controller;
    m_channel = new AHPChannel(this);
}


AHP::Connection::~Connection()
{
}


void AHP::Connection::run()
{
    QTcpSocket socket;

    m_tcpSocket = &socket;

    if (!m_tcpSocket->setSocketDescriptor(m_socketDescriptor)) {
        if (m_tcpSocket->error() != QTcpSocket::UnknownSocketError ){ //! Si hubo un error en el socket
            //emit error(tcpSocket.error(), tcpSocket.errorString());
            m_state = AHPState::DISCONNECTED_STATE;
        }
    }

    while (m_state != AHPState::DISCONNECTED_STATE) {

        //! Construyo la trama
        AHPFrame request;

        //! Leo la trama
        m_mutex.lock();
        m_channel->readFrame(request,m_tcpSocket);

        //! Parche para cuando se desconecta un cliente inesperadamente no entrar en un while infinito
        if (m_tcpSocket->state() == QTcpSocket::UnconnectedState)
            break;

        qDebug() << "RECEIVED: \n" << request.getAsQString();
        m_mutex.unlock();


        switch (m_state) {
        case AHPState::BEGIN_STATE:
            processBeginState(request);
            break;
        case AHPState::CONNECTED_STATE:
            processConnectedState(request);
            break;
        case AHPState::REALTIME_STATE:
            processRealtimeState(request);
            break;
        case AHPState::REALTIME_SENDING_STATE:
            processRealtimeSendingState(request);
            break;
        case AHPState::HISTORIAN_STATE:
            processHistorianState(request);
            break;
        default:
            break;
        }


    }
}

void AHP::Connection::processBeginState(const AHP::AHPFrame &request)
{
    switch (request.code()) {
    case (int)AHPCommand::HELLO:
        if (m_connectionCount < MAX_CONNECTIONS) { //! Si no llegue al limite de conexiones
             AHPFrame response = AHPFrameFactory::instance()->createFrame(AHPCommand::ACCEPT); //! Acepto

             m_state = AHPState::CONNECTED_STATE;

             m_mutex.lock(); //Cierro
             m_channel->writeFrame(response,m_tcpSocket);
             m_mutex.unlock();//Abro

             qDebug() << "NEW CLIENT CONNECTED SUCCESSFULL !!!\n";

        } else {
            AHPFrame response = AHPFrameFactory::instance()->createFrame(AHPCommand::REJECT); //!NO Acepto

            m_mutex.lock(); //Cierro
            m_channel->writeFrame(response,m_tcpSocket);

            m_tcpSocket->disconnectFromHost();
            m_tcpSocket->waitForDisconnected();
            m_mutex.unlock(); //Abro

            m_state = AHPState::DISCONNECTED_STATE;
        }
        break;
    case (int)AHPCommand::DISCONNECT:
        m_state = AHPState::DISCONNECTED_STATE;

        m_mutex.lock(); //Cierro
        m_tcpSocket->disconnectFromHost();
        qDebug() << "CLIENT DISCONNECT SUCCESSFULL!!\n";
        //m_tcpSocket->waitForDisconnected();
        m_mutex.unlock(); //Abro
        break;
    default:
        break;
    }
}

void AHP::Connection::processConnectedState(const AHP::AHPFrame &request)
{
    AHPFrame response;
    Serializer msgPack;
    VariablesConfigured responsePayload;
    QByteArray payload;



    switch (request.code()) {
    case (int)AHPCommand::SET_REALTIME_MODE:
            m_state = AHPState::REALTIME_STATE;

            m_mutex.lock(); //Cierro
            responsePayload.setVars(m_controller->dataStorage()->getVariablesConfigured().toVector().toStdVector());
            m_mutex.unlock(); //Abro

            responsePayload.serialize(&msgPack,&payload);

            response = AHPFrameFactory::instance()->createFrame(AHPCommand::MODE_SUCCESSFULL,payload); //! Por defecto mando lista de variables configuradas

            m_mutex.lock(); //Cierro
            if (m_channel->writeFrame(response,m_tcpSocket))
                qDebug() << "NEW CLIENT SET REALTIME MODE SUCCESSFULL !!!\n";
            else
                qDebug() << "ERROR!!! in SET REALTIME MODE\n";
            m_mutex.unlock(); //Abro
        break;
    case (int)AHPCommand::SET_HISTORIAN_MODE:
            m_state = AHPState::HISTORIAN_STATE;

            m_mutex.lock(); //Cierro
            responsePayload.setVars(m_controller->dataStorage()->getVariablesConfigured().toVector().toStdVector());
            m_mutex.unlock(); //Abro

            responsePayload.serialize(&msgPack,&payload);

            response = AHPFrameFactory::instance()->createFrame(AHPCommand::MODE_SUCCESSFULL,payload); //! Por defecto mando lista de variables configuradas

            m_mutex.lock(); //Cierro
            if (m_channel->writeFrame(response,m_tcpSocket))
                qDebug() << "NEW CLIENT SET HISTORIAN MODE SUCCESSFULL !!!\n";
            else
                qDebug() << "ERROR!!! in SET HISTORIAN MODE\n";
            m_mutex.unlock(); //Abro
        break;
    case (int)AHPCommand::DISCONNECT:
        m_state = AHPState::DISCONNECTED_STATE;

        response = AHPFrameFactory::instance()->createFrame(AHPCommand::ACCEPT);

        m_mutex.lock(); //Cierro

        if (m_channel->writeFrame(response,m_tcpSocket))
            qDebug()<<"CLIENT DISCONNECTED SUCCESSFULL!!\n";
        else
            qDebug() << "ERROR IN DISCONNECTING!!!\n";

        m_tcpSocket->disconnectFromHost();
        m_tcpSocket->waitForDisconnected();
        m_mutex.unlock(); //Abro
        break;
    default:
        break;
    }
}

void AHP::Connection::processRealtimeState(const AHP::AHPFrame &request)
{
    AHPFrame response;
    RealtimeVariableRequest requestPayloadDeserialized;
    QByteArray requestPayload = request.payload();
    Serializer msgpackk;

    switch (request.code()) {
    case (int)AHPCommand::GET_REALTIME_VARIABLES:
            m_state = AHPState::REALTIME_SENDING_STATE;

            //TODO: Aki hago la pincha de connectar algo q envie los valores de tiempo real continuamente
            msgpackk.deserialize(requestPayload,requestPayloadDeserialized);

            m_realtimeSendingVariables = QVector<quint32>::fromStdVector(requestPayloadDeserialized.varsId());

            qDebug() <<"NEW REALTIME REQUEST:\n";
            for (auto a : m_realtimeSendingVariables)
                qDebug() <<QString("id = %1\n").arg(a);

            connect(m_controller->realtimeCache(),SIGNAL(storedVariablePoint(quint32,quint8,quint64,QVariant)),this,SLOT(sendRealtimeVariable(quint32,quint8,quint64,QVariant)));

        break;
    case (int)AHPCommand::RESET_MODE:
            m_state = AHPState::CONNECTED_STATE;

            response = AHPFrameFactory::instance()->createFrame(AHPCommand::ACCEPT); //! Por defecto respondo con ACCEPT

            qDebug() <<"RESET MODE SUCCESSFULL !!!\n";

            m_mutex.lock(); //Cierro
            m_channel->writeFrame(response,m_tcpSocket);
            m_mutex.unlock(); //Abro
        break;
    default:
        break;
    }
}

void AHP::Connection::processRealtimeSendingState(const AHP::AHPFrame &request)
{
    AHPFrame response;


    switch (request.code()) {
    case (int)AHPCommand::STOP_REALTIME_SENDING:
            m_state = AHPState::REALTIME_STATE;

            //TODO: Aki paro de enviar los valores de tiempo real q estaba enviando continuamente

            disconnect(m_controller->realtimeCache(),SIGNAL(storedVariablePoint(quint32,quint8,quint64,QVariant)),this,SLOT(sendRealtimeVariable(quint32,quint8,quint64,QVariant)));

            m_realtimeSendingVariables.clear();

            response = AHPFrameFactory::instance()->createFrame(AHPCommand::ACCEPT); //! Por defecto respondo con ACCEPT

            qDebug() <<"STOP REALTIME SENDING SUCCESSFULL !!!\n";

            m_mutex.lock(); //Cierro
            m_channel->writeFrame(response,m_tcpSocket);
            m_mutex.unlock(); //Abro
        break;
    default:
        break;
    }
}

void AHP::Connection::processHistorianState(const AHP::AHPFrame &request)
{
    AHPFrame response;
    HistorianVariableRequest requestPayloadDeserialized;
    HistorianAlarmRequest requestPayloadAlarmDeserialized;
    HistorianVariableResponse responsePayload;
    HistorianAlarmResponse responseAlarmPayload;
    QByteArray requestPayload = request.payload();
    QByteArray payload;
    Serializer msgpackk;

    qDebug() << "Request payload: "<<requestPayload<<"\n";

    switch (request.code()) {
    case (int)AHPCommand::GET_HISTORIAN_VARIABLES:

            //TODO: Aki pido a la DB la info y la mando
            msgpackk.deserialize(requestPayload,requestPayloadDeserialized);

            responsePayload = m_controller->getHistorianVariables(requestPayloadDeserialized);

            responsePayload.serialize(&msgpackk,&payload);

            //msgpackk.deserialize(payload,r2);

            qDebug() << "Datos a enviar: \n";
            for (int i = 0; i < responsePayload.responseIds().size(); ++i)
                qDebug()<<"ID: " << responsePayload.responseIds()[i]
                        <<"CANT: "<<responsePayload.sizeAt(i);

            response = AHPFrameFactory::instance()->createFrame(AHPCommand::HISTORIAN_VARIABLES_RESPONSE,payload); //! Esta es la trama con la informacion pedida

            m_mutex.lock(); //Cierro
            m_channel->writeFrame(response,m_tcpSocket);
            m_mutex.unlock(); //Abro

            qDebug() << "CLIENT RETRIVE HISTORIAN VARIABLES SUCCESSFULL !!!\n";
        break;
    case (int)AHPCommand::GET_HISTORIAN_ALARMS:

            //TODO: Aki pido a la DB la info y la mando
            msgpackk.deserialize(requestPayload,requestPayloadAlarmDeserialized);

            responseAlarmPayload = m_controller->getHistorianAlarms(requestPayloadAlarmDeserialized);

            responseAlarmPayload.serialize(&msgpackk,&payload);

            //msgpackk.deserialize(payload,r3);

            qDebug() << "Datos a enviar: \n";
            for (int i = 0; i < responseAlarmPayload.responseIds().size(); ++i)
                qDebug() << responseAlarmPayload.responseIds()[i];

            response = AHPFrameFactory::instance()->createFrame(AHPCommand::HISTORIAN_ALARMS_RESPONSE,payload); //! Esta es la trama con la informacion pedida

            m_mutex.lock(); //Cierro
            m_channel->writeFrame(response,m_tcpSocket);
            m_mutex.unlock(); //Abro

            qDebug() << "CLIENT RETRIVE HISTORIAN ALARMS SUCCESSFULL !!!\n";
        break;
    case (int)AHPCommand::RESET_MODE:
            m_state = AHPState::CONNECTED_STATE;

            response = AHPFrameFactory::instance()->createFrame(AHPCommand::ACCEPT); //! Por defecto respondo con ACCEPT

            qDebug() <<"RESET MODE SUCCESSFULL !!!\n";

            m_mutex.lock(); //Cierro
            m_channel->writeFrame(response,m_tcpSocket);
            m_mutex.unlock(); //Cierro
        break;
    default:
        break;
    }
}

void AHP::Connection::sendRealtimeVariable(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value)
{
    Serializer msgpackk;

    if (m_realtimeSendingVariables.contains(idVariable)) {
        QByteArray payload;

        qDebug() << QString("Enviando ID = %1\n").arg(idVariable);

        if (value.type() == QVariant::LongLong) {
         RealtimeVariableResponse resp = RealtimeVariableResponse(VariablePoint(idVariable,quality,timeStamp,value.toLongLong(),INTEGER));
         resp.serialize(&msgpackk,&payload);
        }
        else {
            RealtimeVariableResponse resp = RealtimeVariableResponse(VariablePoint(idVariable,quality,timeStamp,value.toDouble(),DOUBLE));
            resp.serialize(&msgpackk,&payload);
        }

        AHPFrame response = AHPFrameFactory::instance()->createFrame(AHPCommand::REALTIME_VARIABLES_RESPONSE,payload);

//        m_mutex.lock(); //Cierro
        if (m_channel->writeFrame(response,m_tcpSocket)) {
            m_tcpSocket->flush();
            qDebug() << "Enviado correctamente!!!\n";
        }
//        m_mutex.unlock(); //Abro
    }
}


