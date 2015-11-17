#include "realtimerequesttest.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QTimer>
#include <QTime>
#include <QVariant>
#include <vector>

#include "../../Domain/variablepoint.h"
#include "../Infrastructure/AHP/ahpclient.h"

RealtimeRequestTest::RealtimeRequestTest(QObject *parent) :
    QObject(parent)
{
}

void RealtimeRequestTest::run()
{
    QHostAddress address = selectHostAddress();

    if (m_client.connectToServer(address,(quint16)666))
        qDebug() << "CONNECTED TO: "<<address.toString().append(":666\n");
    else
        qDebug() << "CAN'T CONNECT *!* \n";

    if (m_client.setMode(AHP::ClientMode::REALTIME_MODE))
        qDebug() << "REALTIME MODE SUCCESSFULL !!!\n";
    else
        qDebug() << "CAN'T SET REALTIME  MODE *!* \n";


    connect(&m_client,SIGNAL(arriveRealtimeVariablePoint(quint32,quint8,quint64,QVariant)),this,SLOT(receiveRealtimeVariable(quint32,quint8,quint64,QVariant)));

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));


    AHP::RealtimeVariableRequest req;

    req.appendIdVariable(qrand() % 50);
    req.appendIdVariable(qrand() % 50);
    req.appendIdVariable(qrand() % 50);

    m_client.getRealtimeVariables(req);

    qDebug() << "|ID     |   Value    |    Quality   |   TimeStamp|";
    qDebug() << "-------------------------------------------------";


}

QHostAddress RealtimeRequestTest::selectHostAddress()
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

void RealtimeRequestTest::receiveRealtimeVariable(quint32 idVariable,quint8 quality,quint64 timeStamp,QVariant value)
{

    if (value.type() == QVariant::Type::LongLong)
        qDebug() << QString("%1        %2          %3            %4")
                    .arg(idVariable)
                    .arg(value.toLongLong())
                    .arg(quality)
                    .arg(timeStamp);
    else
        qDebug() << QString("%1        %2          %3            %4")
                    .arg(idVariable)
                    .arg(value.toDouble())
                    .arg(quality)
                    .arg(timeStamp);
}
