#ifndef AHPCHANNEL_H
#define AHPCHANNEL_H

#include <QTcpSocket>
#include <QtWidgets/QApplication>
#include <cstring>

#define VERSION_SIZE    1
#define CODE_SIZE       1
#define SIZE_SIZE       4
#define CRC_SIZE        4
#define TIMEOUT         5 * 1000

namespace AHP {

class AHPFrame;

class AHPChannel : public QObject
{
    Q_OBJECT
public:
    explicit AHPChannel(QObject *parent = 0);

    bool readFrame(AHPFrame &frame, QTcpSocket *socket);
    bool writeFrame(const AHPFrame &frame, QTcpSocket* socket);

signals:

public slots:

};

}

#endif // AHPCHANNEL_H
