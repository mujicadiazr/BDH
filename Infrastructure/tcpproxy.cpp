#include <QTimer>

#include "tcpproxy.h"


TcpProxy::TcpProxy(QString host, int port, QObject *parent)
    :QObject(parent),
    m_host(host),
    m_port(port)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(manageError(QAbstractSocket::SocketError)));
}

TcpProxy::~TcpProxy()
{

}

bool TcpProxy::addWriteTask(int varId, QVariant value)
{
    qDebug() << "addWriteTask "<< varId << " " << value;

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);

    stream << varId;
    stream << value;

    socket->write(buffer);
}

bool TcpProxy::appBegin(bool begin)
{
   qDebug() << "appBegin......" ;
   connectToServer();
}

bool TcpProxy::isOnline()
{

}

void TcpProxy::onReadyRead()
{
    QByteArray data = socket->readAll();
    QDataStream stream(data);

    int deviceId;
    int varId;
    ushort quality;
    qulonglong timestamp;
    uchar readType;
    QVariant value;

    //unsigned varsCount = data.length()/

    while(!stream.atEnd())
    {
        stream >> deviceId;
        stream >> varId;
        stream >> quality;
        stream >> timestamp;
        stream >> readType;
        stream >> value;

        emit variableChanged(varId, quality, timestamp, value);
    }
}

void TcpProxy::onDisconnected()
{
    qDebug() << "onDisconnected";

    if(socket->state() == QAbstractSocket::UnconnectedState)
    {
         QTimer::singleShot(1000, this, SLOT(connectToServer()));
    }
}

void TcpProxy::manageError(QAbstractSocket::SocketError socketError)
{
//    switch (socketError)
//    {
//        case QAbstractSocket::RemoteHostClosedError:
//            break;
//        case QAbstractSocket::HostNotFoundError:
//            qDebug() << "COLLECTOR CONNECTION ERROR: " << "The host was not found" <<"\n";
//            break;
//        case QAbstractSocket::ConnectionRefusedError:
//            qDebug() << "COLLECTOR CONNECTION ERROR: " << "The connection was refused by the peer.\n";
//            break;
//        default:
//            qDebug() << "COLLECTOR CONNECTION ERROR: " << socket->errorString() <<"\n";
//    }

    if(socket->state() == QAbstractSocket::UnconnectedState)
    {
        QTimer::singleShot(1000, this, SLOT(connectToServer()));
    }
}

void TcpProxy::connectToServer()
{
    socket->abort();
    socket->connectToHost(m_host, m_port);
}
unsigned int TcpProxy::port() const
{
    return m_port;
}

void TcpProxy::setPort(unsigned int port)
{
    m_port = port;
}

QString TcpProxy::host() const
{
    return m_host;
}

void TcpProxy::setHost(const QString &host)
{
    m_host = host;
}



