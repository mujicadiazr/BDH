#include "clientview.h"
#include "ui_clientview.h"
#include "historicplot.h"
#include "connectdialog.h"

#include <QStandardItemModel>
#include <QMessageBox>

ClientView::ClientView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientView)
{
    ui->setupUi(this);
    //-------------------------------------
    //Historian tab components
    //------------------------------------

    m_historicState = DISCONNECT;

    m_historicClient = new ClientWorker(this,this);

    //! Sennal que envia el HistoricClient cuando llega la respuesta a una consulta historica
    connect(m_historicClient,SIGNAL(arriveNewHistorian(AHP::HistorianVariableResponse)),SLOT(setHistorianValues(AHP::HistorianVariableResponse)));

    m_ListViewModel = new QStandardItemModel();

    ui->listView->setModel(m_ListViewModel);

    ui->dateTimeEdit->setDate(QDate(2014,1,1));
    ui->dateTimeEdit_2->setDate(QDate(2020,1,1));

    //! Sennal para actualizar el campo de la descripcion
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateHistorianDescription(QString)));

    //!Sennal que envia el hilo de la conexion para notificar si se pudo conectar o no
    connect(m_historicClient,SIGNAL(isConnected(bool)),this,SLOT(enableTabHistorian(bool)));
    //--------------------------------------------
    //Realtime tab components
    //-------------------------------------------------

    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_10->setEnabled(false);

    m_realtimeState = DISCONNECT;

    m_realtimeClient = new ClientWorker(this,this);

    //! Sennal que envia el RealtimeClient cuando llega la respuesta a una consulta historica
    connect(m_realtimeClient,SIGNAL(arriveNewRealtime(AHP::RealtimeVariableResponse)),SLOT(setRealtimeValues(AHP::RealtimeVariableResponse)));

    m_ListViewModel2 = new QStandardItemModel();

    ui->listView_2->setModel(m_ListViewModel2);

    //! Sennal para actualizar el campo de la descripcion
    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateRealtimeDescription(QString)));

    //!Sennal que envia el hilo de la conexion para notificar si se pudo conectar o no
    connect(m_realtimeClient,SIGNAL(isConnected(bool)),this,SLOT(enableTabRealtime(bool)));
    //---------------------------------------------------
    //Alarms tab components
    //---------------------------------------------------
    m_alarmState = DISCONNECT;
    m_alarmsModel = new QStandardItemModel();
    m_ListViewModel3 = new QStandardItemModel();

    m_historianAlarmRequest = AHP::HistorianAlarmRequest();

    ui->listView_3->setModel(m_ListViewModel3);
    ui->listView_4->setModel(m_alarmsModel);

    m_historicAlarmClient = new ClientWorker(this,this);

    //! Sennal que envia el HistoricAlarmClient cuando llega la respuesta a una consulta de alarmas
    connect(m_historicAlarmClient,SIGNAL(arriveNewAlarm(AHP::HistorianAlarmResponse)),SLOT(setAlarmValues(AHP::HistorianAlarmResponse)));

    //!Sennal que envia el hilo de la conexion para notificar si se pudo conectar o no
    connect(m_historicAlarmClient,SIGNAL(isConnected(bool)),this,SLOT(enableTabAlarms(bool)));

    //Add Actions to Toolbar
    QList<QAction *> listActions;
    listActions.append(ui->actionHistorian_connect);
    listActions.append(ui->actionRealtime_connect);
    listActions.append(ui->actionAlarm_connect);

    ui->toolBar->addActions(listActions);

}

ClientView::~ClientView()
{
    delete ui;

    delete m_historicClient;
    delete m_realtimeClient;
    delete m_historicAlarmClient;
    delete ui->historicCanvas;
    //    delete m_realtimePlot;
    delete m_ListViewModel;
    delete m_ListViewModel2;
    delete m_alarmsModel;
}

void ClientView::updateHistorianDescription(QString idVariable)
{
    ui->plainTextEdit->clear();

    ui->plainTextEdit->appendPlainText(m_historicClient->getDescriptionByID(idVariable.toUInt()));
}

void ClientView::updateRealtimeDescription(QString idVariable)
{
    ui->plainTextEdit_2->clear();

    ui->plainTextEdit_2->appendPlainText(m_realtimeClient->getDescriptionByID(idVariable.toUInt()));
}

void ClientView::setHistorianValues(AHP::HistorianVariableResponse response)
{
    for (int i = 0; i < response.responseIds().size(); ++i) {
        ui->historicCanvas->addCurve(response.responseIds().at(i),response.responseData().at(i));
    }
}

void ClientView::setRealtimeValues(AHP::RealtimeVariableResponse response)
{
    qDebug() << QString("Llego id : %1, value : %2\n").arg(response.data().idVariable()).arg(response.data().value().toString());

    if (response.data().value().type() == QVariant::LongLong)
        ui->realtimeCanvas->setValueToCurve(response.data().idVariable(),(double)response.data().value().toLongLong());
    else
        ui->realtimeCanvas->setValueToCurve(response.data().idVariable(),response.data().value().toDouble());
}

void ClientView::setAlarmValues(AHP::HistorianAlarmResponse response)
{
    QString line = "";
    std::vector<quint32> ids = response.responseIds();
    std::vector<std::vector<AlarmPoint>> data = response.responseData();

    for (int i = 0; i < ids.size(); ++i) {
        for (AlarmPoint ap : data.at(i)){
            line.append(QString("VID: %1, ").arg(ids.at(i)));
            line.append(QString("AID: %1, STATE: %2, DATE/TIME: %3").arg(ap.idAlarm()).arg(ap.state()).arg(QDateTime::fromTime_t(ap.timeStamp()).toString()));

            m_alarmsModel->appendRow(new QStandardItem(line));

            line.clear();
        }
    }

}

void ClientView::enableTabHistorian(bool state)
{
    if (state) {
        m_historicClient->setMode(AHP::ClientMode::HISTORIAN_MODE);
        this->thread()->msleep(1000);
        ui->tabHistorian->setEnabled(true);

        QMessageBox::information(this , "OK" , QString("CONNECT SUCCESSFULL!!!"), QMessageBox::Ok);


        QList<Variable> vars = m_historicClient->varsConfigured();

        for(int i = 0; i < vars.size(); ++i){
            ui->comboBox->addItem(QString().setNum(vars.at(i).id()));
        }

        //! Pongo el icono y el estado de conectado
        ui->actionHistorian_connect->setIcon(QIcon(":/images/HistorianConnect.png"));
        m_historicState = CONNECT;

    } else {
        QMessageBox::critical(this , "ERROR" , QString("CAN'T CONNECT TO SERVER!!!"), QMessageBox::Ok);
    }
}

void ClientView::enableTabRealtime(bool state)
{
    if (state) {
        m_realtimeClient->setMode(AHP::ClientMode::REALTIME_MODE);
        this->thread()->msleep(1000);
        ui->tabRealtime->setEnabled(true);

        QMessageBox::information(this , "OK" , QString("CONNECT SUCCESSFULL!!!"), QMessageBox::Ok);

        QList<Variable> vars = m_realtimeClient->varsConfigured();

        for(int i = 0; i < vars.size(); ++i){
            ui->comboBox_2->addItem(QString().setNum(vars.at(i).id()));
        }

        //! Pongo el icono y el estado de conectado
        ui->actionRealtime_connect->setIcon(QIcon(":/images/RealtimeConnect.png"));
        m_realtimeState = CONNECT;

    } else {
        QMessageBox::critical(this , "ERROR" , QString("CAN'T CONNECT TO SERVER!!!"), QMessageBox::Ok);
    }
}

void ClientView::enableTabAlarms(bool state)
{
    if (state) {
        m_historicAlarmClient->setMode(AHP::ClientMode::HISTORIAN_MODE);
        this->thread()->msleep(1000);
        ui->tabAlarms->setEnabled(true);

        QMessageBox::information(this , "OK" , QString("CONNECT SUCCESSFULL!!!"), QMessageBox::Ok);

        QList<Variable> vars = m_historicAlarmClient->varsConfigured();

        for(int i = 0; i < vars.size(); ++i){
            ui->variableID->addItem(QString().setNum(vars.at(i).id()));
        }

        //! Pongo el icono y el estado de conectado
        ui->actionAlarm_connect->setIcon(QIcon(":/images/AlarmsConnect.png"));
        m_alarmState = CONNECT;
    } else {
        QMessageBox::critical(this , "ERROR" , QString("CAN'T CONNECT TO SERVER!!!"), QMessageBox::Ok);
    }
}

void ClientView::on_actionHistorian_connect_triggered()
{
    if (m_historicState == DISCONNECT){

        QHostAddress add = QHostAddress();
        quint16 port = 0;

        ConnectDialog dialog(this);

        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setWindowTitle("Historian connect to...");

        if (dialog.exec() == QDialog::Accepted) {
            add = dialog.getAddress();
            port = dialog.getPort();

            m_historicClient->connectToServer(add,port);
        }
    } else {
        m_historicClient->disconnectFromServer();
        this->thread()->sleep(1);
        ui->tabHistorian->setEnabled(false);

        ui->actionHistorian_connect->setIcon(QIcon(":/images/HistorianDisconnect.png"));
        m_historicState = DISCONNECT;

        QwtPlotItemList itemList = ui->historicCanvas->itemList(QwtPlotItem::Rtti_PlotCurve);
        for (QwtPlotItem *item : itemList )
            delete item;
    }
}

void ClientView::on_pushButton_clicked()
{
    quint32 id = ui->comboBox->currentText().toUInt();
    quint64 beginTimeStamp = ui->dateTimeEdit->dateTime().toMSecsSinceEpoch();
    quint64 endTimeStamp = ui->dateTimeEdit_2->dateTime().toMSecsSinceEpoch();

    m_historianRequest.appendIdVariable(id);
    m_historianRequest.appendRange(std::pair<quint64,quint64>(beginTimeStamp,endTimeStamp));

    QString line = QString("%1 : %2 : %3").arg(id).arg(ui->dateTimeEdit->dateTime().toString()).arg(ui->dateTimeEdit_2->dateTime().toString());

    m_ListViewModel->appendRow(new QStandardItem(line));
}

void ClientView::on_pushButton_3_clicked()
{
    m_ListViewModel->clear();

    m_historianRequest = AHP::HistorianVariableRequest();

    QwtPlotItemList itemList = ui->historicCanvas->itemList(QwtPlotItem::Rtti_PlotCurve);

    for (QwtPlotItem *item : itemList )
        delete item;
}

void ClientView::on_pushButton_2_clicked()
{
    m_ListViewModel->clear();

    m_historicClient->getHistorianVariables(m_historianRequest);

    m_historianRequest = AHP::HistorianVariableRequest();
}

void ClientView::on_actionHistorian_disconnect_2_triggered()
{
    m_historicClient->disconnectFromServer();
    this->thread()->sleep(1);
    ui->tabHistorian->setEnabled(false);

    ui->actionHistorian_connect->setIcon(QIcon(":/images/HistorianDisconnect.png"));
}

void ClientView::on_pushButton_4_clicked()
{
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_10->setEnabled(true);

    quint32 id = ui->comboBox_2->currentText().toUInt();

    m_realtimeRequest.appendIdVariable(id);

    quint32 index = m_realtimeClient->varsConfigured().indexOf(Variable(id));

    QString line = QString("%1 : %2").arg(id).arg(QString::fromStdString(m_realtimeClient->varsConfigured().at(index).description()));
    //TODO: Arreglar esta mierda
    m_ListViewModel2->appendRow(new QStandardItem(line));
}

void ClientView::on_pushButton_6_clicked()
{
    ui->comboBox_2->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->actionRealtime_connect->setEnabled(false);

    m_ListViewModel2->clear();

    m_realtimeClient->getRealtimeVariables(m_realtimeRequest);

    auto vectorIds = m_realtimeRequest.varsId();

    for (auto id : vectorIds)
        ui->realtimeCanvas->addCurve(QString("id : %1").arg(id),id,QColor(qrand() % 255, qrand() % 255, qrand() % 255));

    ui->pushButton_5->setEnabled(true);

}

void ClientView::on_actionRealtime_connect_triggered()
{
    if (m_realtimeState == DISCONNECT) {

        QHostAddress add = QHostAddress();
        quint16 port = 0;

        ConnectDialog dialog(this);
        dialog.setWindowTitle("Realtime connect to...");

        dialog.setWindowModality(Qt::ApplicationModal);

        if (dialog.exec() == QDialog::Accepted) {
            add = dialog.getAddress();
            port = dialog.getPort();

            m_realtimeClient->connectToServer(add,port);
        }
    } else {
        m_realtimeClient->disconnectFromServer();
        this->thread()->sleep(1);
        ui->tabRealtime->setEnabled(false);

        //! Pongo el icono y el estado de desconectado
        ui->actionRealtime_connect->setIcon(QIcon(":/images/RealtimeDisconnect.png"));
        m_realtimeState = DISCONNECT;
    }
}

void ClientView::on_actionRealtime_disconnect_2_triggered()
{
    m_realtimeClient->disconnectFromServer();
    this->thread()->sleep(1);
    ui->tabRealtime->setEnabled(false);
}

void ClientView::receiveRealtimeVariable(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value)
{
    qDebug() << QString("Llego id : %1, value : %2\n").arg(idVariable).arg(value.toString());

    if (value.toDouble() > ui->realtimeCanvas->getMaximumValue())
        ui->realtimeCanvas->setMaximumValue(value.toDouble());
    else if (value.toDouble() < ui->realtimeCanvas->getMinimumValue())
        ui->realtimeCanvas->setMinimumValue(value.toDouble());

    ui->realtimeCanvas->addCurve(QString("id : %1").arg(idVariable),idVariable,QColor(qrand() % 255, qrand() % 255, qrand() % 255));

    if (value.type() == QVariant::LongLong)
        ui->realtimeCanvas->setValueToCurve(idVariable,(double)value.toLongLong());
    else
        ui->realtimeCanvas->setValueToCurve(idVariable,value.toDouble());
}

void ClientView::on_pushButton_5_clicked()
{
    m_realtimeClient->stopRealtimeSending();
    ui->pushButton_4->setEnabled(true);
    ui->actionRealtime_connect->setEnabled(true);

    this->thread()->sleep(1);

    ui->comboBox_2->setEnabled(true);
    ui->realtimeCanvas->clear();
    ui->pushButton_5->setEnabled(false);

    //! limpio el model y el RealtimeRequest
    m_ListViewModel2->clear();
    m_realtimeRequest.clearIds();
}

void ClientView::on_pushButton_7_clicked() {
    quint32 id = ui->variableID->currentText().toUInt();
    m_historianAlarmRequest.appendIdVariable(id);
    QString line = QString("Alarms attached to variable: %1").arg(id);
    qDebug() << line;

    m_ListViewModel3->appendRow(new QStandardItem(line));
}

void ClientView::on_pushButton_9_clicked()
{
    m_ListViewModel3->clear();
    m_historicAlarmClient->getHistorianAlarms(m_historianAlarmRequest);
    m_historianAlarmRequest = AHP::HistorianAlarmRequest();
}

void ClientView::on_actionAlarm_connect_triggered()
{
    if (m_alarmState == DISCONNECT) {
        QHostAddress add = QHostAddress();
        quint16 port = 0;

        ConnectDialog dialog(this);

        dialog.setWindowModality(Qt::ApplicationModal);
        dialog.setWindowTitle("Alarm connect to...");

        if (dialog.exec() == QDialog::Accepted) {
            add = dialog.getAddress();
            port = dialog.getPort();

            m_historicAlarmClient->connectToServer(add,port);


        }
    } else {
        m_historicAlarmClient->disconnectFromServer();
        this->thread()->sleep(1);
        ui->tabAlarms->setEnabled(false);

        ui->actionAlarm_connect->setIcon(QIcon(":/images/AlarmsDisconnect.png"));
        m_alarmState = DISCONNECT;
    }
}

void ClientView::on_pushButton_8_clicked()
{
    m_ListViewModel3->clear();
    m_historianAlarmRequest = AHP::HistorianAlarmRequest();
}

void ClientView::on_actionAlarm_disconnect_triggered()
{
    m_historicAlarmClient->disconnectFromServer();
    this->thread()->sleep(1);
    ui->tabAlarms->setEnabled(false);
}

void ClientView::on_pushButton_10_clicked()
{
    //! limpio el model y el RealtimeRequest
    m_ListViewModel2->clear();
    m_realtimeRequest.clearIds();

    ui->pushButton_6->setEnabled(false);
    ui->pushButton_4->setEnabled(true);
}
