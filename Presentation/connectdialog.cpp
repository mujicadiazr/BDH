#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QNetworkInterface>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";

    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");

    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

    ui->spinBox->setMinimum(0);
    ui->spinBox->setMaximum(65536);


    //Valores por default al address y port
    ui->lineEdit->setText(selectHostAddress().toString());
    ui->spinBox->setValue(666);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

QHostAddress ConnectDialog::getAddress()
{
    return QHostAddress(ui->lineEdit->text());
}

quint16 ConnectDialog::getPort()
{
    return (quint16)ui->spinBox->value();
}

QHostAddress ConnectDialog::selectHostAddress()
{
    QHostAddress address;

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
            address = ipAddressesList.at(i);
            break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if (address.isNull())
        address = QHostAddress(QHostAddress::LocalHost);

    return address;
}
