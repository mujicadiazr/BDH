#include "clientworker.h"

#include "Presentation/clientview.h"

ClientWorker::ClientWorker(ClientView *clientView, QObject *parent)
    :QThread(parent)
  , m_quit(false)
  , real_time_sender_working_(false)
  , m_clientView(clientView)

{
}

ClientWorker::~ClientWorker()
{
}

void ClientWorker::run()
{
    AHP::AHPClient client(m_clientView);
    AHP::RealtimeVariableResponse rr;
    AHP::HistorianVariableResponse histResp = AHP::HistorianVariableResponse();
    AHP::HistorianAlarmResponse alarmResp = AHP::HistorianAlarmResponse{};

    m_mutex.lock();

    m_client = &client;
    QHostAddress add = m_address;
    quint16 port = m_port;
    m_quit = false;

    m_mutex.unlock();

    if (!client.connectToServer(add,port)){
        qDebug() << QString("CONNECTION FAILED TO: %1:%2\n").arg(add.toString()).arg(port);
        emit isConnected(false);
        return;
    } else {
        qDebug() << QString("CONNECTION OK TO: %1:%2\n").arg(add.toString()).arg(port);
        emit isConnected(true);
    }


    while (!m_quit)
    {
        m_mutex.lock();
        m_cond.wait(&m_mutex); //AKI se duerme

        Command cmd = m_command; //AKI revive el hilo pues llego un nuevo comando
        m_mutex.unlock();

        switch (cmd)
        {
        case Command::SET_MODE:
            m_mutex.lock();
            if(!client.setMode(m_clientMode)) {
                if (m_clientMode == AHP::ClientMode::HISTORIAN_MODE)
                    qDebug() << QString("FAILED SET MODE HISTORIAN");
                else
                    qDebug() << QString("FAILED SET MODE REALTIME");
            }
            m_mutex.unlock();
            break;

        case Command::GET_HISTORIAN:
            m_mutex.lock();
            histResp = client.getHistorianVariables(m_historianRequest);
            m_mutex.unlock();
            emit arriveNewHistorian(histResp);
            break;
        case Command::GET_ALARMS:
            m_mutex.lock();
            alarmResp = client.getHistorianAlarms(m_historianAlarmsRequest);
            m_mutex.unlock();
            emit arriveNewAlarm(alarmResp);
          break;
        case Command::GET_REALTIME:
            m_mutex.lock();
            connect(m_client,SIGNAL(arriveRealtimeResponse(AHP::RealtimeVariableResponse)),this,SIGNAL(arriveNewRealtime(AHP::RealtimeVariableResponse)));
            m_client->getRealtimeVariables(m_realtimeRequest);
            m_mutex.unlock();
            break;
        case Command::DISCONNECT:
            m_mutex.lock();
            if (!client.disconnectFromServer())
                qDebug()<< "FAILED DISCONNECT !!!\n";
            else
                m_quit = true;
            m_mutex.unlock();
            break;
        case Command::STOP_REALTIME_SENDING:
            m_mutex.lock();
            disconnect(&client,SIGNAL(arriveRealtimeResponse(AHP::RealtimeVariableResponse)),this,SIGNAL(arriveNewRealtime(AHP::RealtimeVariableResponse)));
            if (!client.stopRealtimeSending())
                qDebug()<< "FAILED STOP REALTIME SENDING !!!\n";;
            m_mutex.unlock();
            break;

        default:
            break;
        }
    }

}

void ClientWorker::connectToServer(QHostAddress address, quint16 port)
{
    QMutexLocker locker(&m_mutex);

    m_address = address;
    m_port = port;
    m_command = Command::CONNECT;

    start();
}

void ClientWorker::setMode(AHP::ClientMode mode)
{
    QMutexLocker locker(&m_mutex);

    m_command = Command::SET_MODE;

    m_clientMode = mode;

    m_cond.wakeOne();

}

void ClientWorker::disconnectFromServer()
{
    QMutexLocker locker(&m_mutex);

    m_command = Command::DISCONNECT;

    m_cond.wakeOne();
}

void ClientWorker::stopRealtimeSending()
{
    QMutexLocker locker(&m_mutex);

    m_command = Command::STOP_REALTIME_SENDING;

    m_cond.wakeOne();
}

void ClientWorker::getHistorianVariables(AHP::HistorianVariableRequest request)
{
    QMutexLocker locker(&m_mutex);

    m_command = Command::GET_HISTORIAN;

    m_historianRequest = request;

    m_cond.wakeOne();
}

void ClientWorker::getHistorianAlarms(AHP::HistorianAlarmRequest request)
{
  QMutexLocker locker(&m_mutex);

  m_command = Command::GET_ALARMS;

  m_historianAlarmsRequest = request;

  m_cond.wakeOne();
}

void ClientWorker::getRealtimeVariables(AHP::RealtimeVariableRequest request)
{
    QMutexLocker locker(&m_mutex);

    m_command = Command::GET_REALTIME;

    m_realtimeRequest = request;

    m_cond.wakeOne();
}

QList<Variable> ClientWorker::varsConfigured()
{
    QMutexLocker locker(&m_mutex);
    return m_client->varsConfigured();
}

QString ClientWorker::getDescriptionByID(quint32 id)
{
    QMutexLocker l{&m_mutex};
    auto vars = m_client->varsConfigured();
    for (auto var : vars) {
        if (var.id() == id) {
            return var.description().c_str();
        }
    }

    return "null";

}


