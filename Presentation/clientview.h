#ifndef CLIENTVIEW_H
#define CLIENTVIEW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QStandardItemModel>


#include "../Infrastructure/AHP/ahpclient.h"
#include "qabstractplotter.h"
#include "../Infrastructure/clientworker.h"

namespace Ui {
class ClientView;
}

class HistoricPlot;

class ClientView : public QMainWindow
{
    Q_OBJECT

public:
    enum ClientState {
        CONNECT,
        DISCONNECT
    };
    explicit ClientView(QWidget *parent = 0);
    ~ClientView();

private slots:
    void updateHistorianDescription(QString idVariable);
    void updateRealtimeDescription(QString idVariable);

    void setHistorianValues(AHP::HistorianVariableResponse response);
    void setRealtimeValues(AHP::RealtimeVariableResponse response);
    void setAlarmValues(AHP::HistorianAlarmResponse response);

    void enableTabHistorian(bool state);
    void enableTabRealtime(bool state);
    void enableTabAlarms(bool state);

    void on_actionHistorian_connect_triggered();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_actionHistorian_disconnect_2_triggered();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_actionRealtime_connect_triggered();

    void on_actionRealtime_disconnect_2_triggered();

    void receiveRealtimeVariable(quint32 idVariable,quint8 quality,quint64 timeStamp,QVariant value);

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_actionAlarm_connect_triggered();

    void on_pushButton_8_clicked();

    void on_actionAlarm_disconnect_triggered();

    void on_pushButton_10_clicked();

private:
    Ui::ClientView *ui;

    QStandardItemModel *m_ListViewModel;
    QStandardItemModel *m_ListViewModel2;
    QStandardItemModel *m_ListViewModel3;
    QStandardItemModel *m_alarmsModel;

    //Historic querys
    ClientWorker *m_historicClient;
    AHP::HistorianVariableRequest m_historianRequest;
    AHP::HistorianVariableResponse m_query;
    ClientState m_historicState;

    //Realtime querys
    ClientWorker *m_realtimeClient;
    AHP::RealtimeVariableRequest m_realtimeRequest;
//    QAbstractPlotter *m_realtimePlot;
    ClientState m_realtimeState;

    //Alarms querys
    ClientWorker *m_historicAlarmClient;
    AHP::HistorianAlarmRequest m_historianAlarmRequest;
    ClientState m_alarmState;
};

#endif // CLIENTVIEW_H
