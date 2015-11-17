#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

    QHostAddress getAddress();
    quint16 getPort();

private slots:

private:
    Ui::ConnectDialog *ui;

    QHostAddress selectHostAddress();
};

#endif // CONNECTDIALOG_H
