#ifndef AHPSERVER_H
#define AHPSERVER_H

#include "Common/globals.h"
#include <QtNetwork/QTcpServer>

class BDHServerController;

namespace AHP {

class AHPServer : public QTcpServer
{
    Q_OBJECT

private:
    int m_connectionCout;
    BDHServerController *m_controller;

public:
    AHPServer(QObject *parent = 0);
    AHPServer(BDHServerController * controller, QObject *parent = 0);

protected:
    void incomingConnection(qintptr socketDescriptor);

};

}

#endif // AHPSERVER_H
