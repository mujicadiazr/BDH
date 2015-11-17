#include "ahpchannel.h"
#include "ahpframe.h"
#include "ahp_commands_enum.h"
#include "QThread"

AHP::AHPChannel::AHPChannel(QObject *parent)
    :QObject(parent)
{
}

bool AHP::AHPChannel::readFrame(AHP::AHPFrame &frame, QTcpSocket* socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    quint8 version;
    quint8 code;
    quint32 size;
    QByteArray payload;
    quint32 crc;


    //! Espero hasta q llegue el campo version
    while(socket->bytesAvailable() < VERSION_SIZE) {
        socket->waitForReadyRead();
//        qDebug() <<"NO HA LLEGADO LA VERSION\n";

        if (socket->state() == QTcpSocket::UnconnectedState){ //! Si se desconecta el socket
            qDebug() << "SE DESCONECTO !\n";
            return false;                      //! salgo del hilo para evitar

        }
    }                                                        //! el loop infinito cuando se desconecta
    in >> version; //! Leo la version                       //! de forma imprevista el cliente


    //! Espero hasta q llegue el campo code
    while(socket->bytesAvailable() < CODE_SIZE){
        socket->waitForReadyRead();
        qDebug() <<"NO HA LLEGADO EL CODE\n";
    }
    in >> code; //! Leo el code

    //! Espero hasta q llegue el campo size
    while(socket->bytesAvailable() < SIZE_SIZE){
        socket->waitForReadyRead();
        qDebug() <<"NO HA LLEGADO EL SIZE\n";
    }


    in >> size; //! Leo el size

    if (size > 0) { //! Si size es mayor que cero es la trama tiene campo payload

        //! Espero hasta q llegue el campo de datos (payload) de la trama
        while(socket->bytesAvailable() < size){
            socket->waitForReadyRead();
            qDebug() <<"NO HA LLEGADO EL PAYLOAD\n";
        }

        payload = socket->read(size);

        //! Espero hasta q llegue el campo crc de la trama
        while(socket->bytesAvailable() < CRC_SIZE){
            socket->waitForReadyRead();
            qDebug() <<"NO HA LLEGADO EL CRC\n";
        }

        in >> crc; //! Leo el crc

        frame.setPayload(payload);
        frame.setCrc(crc);
    }

    frame.setVersion(version);
    frame.setCode(code);
    frame.setSize(size);

    return true;
}

//bool AHP::AHPChannel::readFrame(AHP::AHPFrame &frame, QTcpSocket* socket)
//{
//    char data[5]{0,0,0,0,0};

//    quint8 version;
//    quint8 code;
//    quint32 size;
//    QByteArray payload;
//    quint32 crc;

////--------------------------------------------------------------------
//    //! Espero hasta q llegue el campo version
//    while(socket->bytesAvailable() < VERSION_SIZE) {
//        socket->waitForReadyRead();
//    }

//    //! Leo la version
//    if (socket->read(data, VERSION_SIZE) != VERSION_SIZE) {
//        QApplication::exit(EXIT_FAILURE);
//    }
//    std::memcpy(&version, data, VERSION_SIZE);
//    std::memset(data,0,sizeof(data));
////--------------------------------------------------------------------
//    //! Espero hasta q llegue el campo code
//    while(socket->bytesAvailable() < CODE_SIZE)
//        socket->waitForReadyRead();

//    //! Leo el code
//    if (socket->read(data, CODE_SIZE) != CODE_SIZE) {
//        QApplication::exit(EXIT_FAILURE);
//    }
//    std::memcpy(&code, data, CODE_SIZE);
//    std::memset(data,0,sizeof(data));
////--------------------------------------------------------------------
//    //! Espero hasta q llegue el campo size
//    while(socket->bytesAvailable() < SIZE_SIZE)
//        socket->waitForReadyRead();


//    //! Leo el size
//    if (socket->read(data, SIZE_SIZE) != SIZE_SIZE) {
//        QApplication::exit(EXIT_FAILURE);
//    }
//    std::memcpy(&size, data, SIZE_SIZE);
//    std::memset(data,0,sizeof(data));
////--------------------------------------------------------------------

//    if (size > 0) { //! Si size es mayor que cero es q la trama tiene campo payload

//        //! Espero hasta q llegue el campo de datos (payload) de la trama
//        while(socket->bytesAvailable() < size)
//            socket->waitForReadyRead();

//        payload = socket->read(size);

//        //! Espero hasta q llegue el campo crc de la trama
//        while(socket->bytesAvailable() < CRC_SIZE)
//            socket->waitForReadyRead();

//        //! Leo el crc
//        if (socket->read(data, CRC_SIZE) != CRC_SIZE) {
//            QApplication::exit(EXIT_FAILURE);
//        }
//        std::memcpy(&crc, data, CRC_SIZE);
//        std::memset(data,0,sizeof(data));

//        frame.setPayload(payload);
//        frame.setCrc(crc);
//    }

//    frame.setVersion(version);
//    frame.setCode(code);
//    frame.setSize(size);

//    return true;
//}


bool AHP::AHPChannel::writeFrame(const AHP::AHPFrame &frame, QTcpSocket *socket)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << frame.version();
    out << frame.code();

    if (frame.size() > 0) {
        out << frame.payload();
        //block.append(frame.payload());
        out << frame.crc();
    } else
        out << frame.size();

    int ret = socket->write(block);

//    socket->flush();

    return  ret != -1;
}
