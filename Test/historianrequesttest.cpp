#include "historianrequesttest.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QTimer>
#include <QTime>
#include <vector>

#include "../../Domain/variablepoint.h"
#include "../Infrastructure/AHP/ahpclient.h"

HistorianRequestTest::HistorianRequestTest(QObject *parent) :
    QObject(parent)
{
}

void HistorianRequestTest::run()
{
    AHP::AHPClient client;

    QHostAddress address = selectHostAddress();

    if (client.connectToServer(address,(quint16)666))
        qDebug() << "CONNECTED TO: "<<address.toString().append(":666\n");
    else
        qDebug() << "CAN'T CONNECT *!* \n";

    if (client.setMode(AHP::ClientMode::HISTORIAN_MODE))
        qDebug() << "HISTORIAN MODE SUCCESSFULL !!!\n";
    else
        qDebug() << "CAN'T SET HISTORIAN MODE *!* \n";

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));


    AHP::HistorianVariableRequest req;

    req.appendIdVariable(qrand() % 50);
    req.appendRange(std::pair<quint64,quint64>(1426089972, 1526091103));

    req.appendIdVariable(qrand() % 50);
    req.appendRange(std::pair<quint64,quint64>(1426089972, 1526091103));

    req.appendIdVariable(qrand() % 50);
    req.appendRange(std::pair<quint64,quint64>(1426089972, 1526091103));

    AHP::HistorianVariableResponse resp = client.getHistorianVariables(req);

    std::vector<std::vector<VariablePoint> > data = resp.responseData();
    std::vector<quint32> dataIds = resp.responseIds();

    qDebug() << "|ID     |   Value    |    Quality   |   TimeStamp|";
    qDebug() << "-------------------------------------------------";

    for (quint32 i = 0; i < dataIds.size(); ++i) {
        std::vector<VariablePoint> vec = data[i];

        for (quint64 j = 0; j < vec.size(); ++j) {
            VariablePoint vp = vec.at(j);

            if (vp.value().type() == QVariant::Type::LongLong)
                qDebug() << QString("%1        %2          %3            %4")
                            .arg(vp.idVariable())
                            .arg(vp.value().toLongLong())
                            .arg(vp.quality())
                            .arg(vp.timeStamp());
            else
                qDebug() << QString("%1        %2          %3            %4")
                            .arg(vp.idVariable())
                            .arg(vp.value().toDouble())
                            .arg(vp.quality())
                            .arg(vp.timeStamp());
        }

    }

    if (client.disconnectFromServer()) {
        qDebug() << "DISCONNECTED SUCCESSFULL !!!\n";
        exit(0);
    }
    else {
        qDebug() << "CAN'T DISCONNECT *!* \n";
        exit(1);
    }
}

QHostAddress HistorianRequestTest::selectHostAddress()
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
