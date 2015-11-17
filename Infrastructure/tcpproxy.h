#ifndef TCPPROXY_H
#define TCPPROXY_H

#include <QObject>
#include <QtNetwork/QTcpSocket>

class TcpProxy: public QObject
{
    Q_OBJECT

public:
    //CONTRUCTORS AND DESTROYERS
    explicit TcpProxy(QString host, int port, QObject *parent = 0);
    ~TcpProxy();

    QString host() const;
    void setHost(const QString &host);

    unsigned int port() const;
    void setPort(unsigned int port);

public slots:
    //METHODS
    /**
      @brief Envía hacia el collector una tarea de escritura.

      @param El id de la variable.
      @param El nuevo valor de la variable.

      @returns Devuelve true si fue realizada la escritura.
    */
    bool addWriteTask(int varId, QVariant value);
    /**
     * @brief Notifica al recolector el comiezo y fin del Runtime.
     * @param Un Booleano para decir si comienza o termina el Runtime.
     * @return Devuelve true si fue recibido el aviso.
     */
    bool appBegin(bool begin);
    /**
     * @brief Comprueba si hay buena conexión con el Collector.
     * @param Un Booleano para decir si comienza o termina el Runtime.
     * @return Devuelve true si hay buena conexión con el Collector.
     */
    bool isOnline();

    void onReadyRead();
    void onDisconnected();
    void manageError(QAbstractSocket::SocketError socketError);
    void connectToServer();

signals:
    /**
      @brief Señal enviada a la clase Communication para ser reenviada luego a otras clases.

      @param El id de la variable.
      @param La calidad de la variable.
      @param La marca de tiempo de la variable.
      @param El tipo de dato de la variable.
      @param El nuevo valor de la variable.
    */
    void variableChanged(int varId, ushort quality, qulonglong timestamp, QVariant value);
    /**
      @brief Señal enviada a la clase Communication para ser reenviada luego a otras clases.

      @param El id de la variable.
      @param El Tipo de la alarma.
      @param La marca de tiempo cuando se activó la alarma.
      @param El valor definido para la alarma (útil cuando es alarma de estado).
    */
    void alarmRaised(int varId, ushort alarmType, qulonglong timestamp, QVariant value);

private:
    QTcpSocket *socket;
    //DATA
    QString m_host;
    unsigned int m_port;
};

#endif // TCPPROXY_H
