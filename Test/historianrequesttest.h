#ifndef HISTORIANREQUESTTEST_H
#define HISTORIANREQUESTTEST_H

#include <QObject>

class QHostAddress;

class xHistorianRequestTest : public QObject
{
    Q_OBJECT
public:
    HistorianRequestTest(QObject *parent = 0);

    void run();
    QHostAddress selectHostAddress();

signals:

public slots:

};

#endif // HISTORIANREQUESTTEST_H
