
#include "mt.h"

#include "ahpclient.h"

#include "../serializer.h"


namespace AHP {
Mt::Mt(AHPChannel *ch, QTcpSocket *sck, AHPClient *cl)
  : m_c{ch}
  , m_sck{sck}
  , m_cl{cl} {
}


void Mt::ReceiveNewRealtimeVariablePoint() {
    Serializer msgpackk;
    QByteArray payload;
    RealtimeVariableResponse resp = AHP::RealtimeVariableResponse();
    QByteArray responsePayload;
    AHPFrame responseFrame;

    if (!m_c->readFrame(responseFrame,m_sck))
        qDebug() << "Trama de respuesta incorrecta\n";

    responsePayload = responseFrame.payload();

    msgpackk.deserialize(responsePayload,resp);

    qDebug() << "|" << resp.data().value() << "|";
    emit arriveRealtimeResponseTT(resp);

}

}
