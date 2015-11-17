#ifndef REALTIMEREQUESTTEST_H
#define REALTIMEREQUESTTEST_H

#include <QObject>
#include <../Infrastructure/AHP/ahpclient.h>

class QHostAddress;
class QVariant;

class RealtimeRequestTest : public QObject
{
    Q_OBJECT
public:
    explicit RealtimeRequestTest(QObject *parent = 0);

    void run();
    QHostAddress selectHostAddress();

signals:

public slots:
    void receiveRealtimeVariable(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value);


private:
    AHP::AHPClient m_client;

};

#endif // REALTIMEREQUESTTEST_H
