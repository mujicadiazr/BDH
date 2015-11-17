#ifndef MT_H
#define MT_H

#include <QThread>

#include "realtimevariableresponse.h"
#include <QtNetwork/QTcpSocket>

namespace AHP {
class AHPChannel;
class AHPClient;

class Mt : public QThread {
  Q_OBJECT
 public:
  Mt(AHPChannel *ch, QTcpSocket *sck, AHPClient *cl);
  virtual void run() override {
   while (true) {
     qDebug() << "ReceiveNewRealtimeVariablePoint();";
     ReceiveNewRealtimeVariablePoint();
   }
  }
  void ReceiveNewRealtimeVariablePoint();

  AHPChannel *m_c;
  QTcpSocket *m_sck;
  AHPClient *m_cl;


  signals:
   void arriveRealtimeResponseTT(RealtimeVariableResponse);
};

}

#endif // MT_H
