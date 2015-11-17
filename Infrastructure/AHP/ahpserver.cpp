#include "ahpserver.h"
#include "../bdhservercontroller.h"
#include "connection.h"

AHP::AHPServer::AHPServer(QObject *parent)
    :QTcpServer(parent)
{
    m_controller = nullptr;
}

AHP::AHPServer::AHPServer(BDHServerController *controller, QObject *parent)
    :QTcpServer(parent), m_controller(controller)
{
    m_connectionCout = 0;
}

void AHP::AHPServer::incomingConnection(qintptr socketDescriptor)
{
    AHP::Connection *conn = new AHP::Connection(m_controller,socketDescriptor,m_connectionCout);
    connect(conn,SIGNAL(finished()),conn,SLOT(deleteLater()));
    conn->start();

    if (m_connectionCout < MAX_CONNECTIONS)
        ++m_connectionCout;
}
