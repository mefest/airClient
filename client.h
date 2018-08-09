#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class Client : public QObject {
    Q_OBJECT

    const static int packageSize = 6;
    const int timeOut = 1000;

    QString m_host;
    bool m_isConnect = false;
    QTcpSocket m_socket;
    int m_port;
    QString m_error;
    int m_temp = 18;
    bool m_power = false;
    bool m_hot = true;
    int m_fan = 2;
    int m_swing = 0;

    quint16 data[packageSize];
    QTimer m_timer;

    void sendData();
    void sendKeepAlive();
    void startTimer();

public:
    explicit Client(QObject* parent = nullptr);

    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool isConnect READ isConnect NOTIFY isConnectChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)

    Q_PROPERTY(int temp READ temp WRITE setTemp NOTIFY tempChanged)
    Q_PROPERTY(bool power READ power WRITE setPower NOTIFY powerChanged)
    Q_PROPERTY(bool hot READ hot WRITE setHot NOTIFY hotChanged)
    Q_PROPERTY(int fan READ fan WRITE setFan NOTIFY fanChanged)
    Q_PROPERTY(int swing READ swing WRITE setSwing NOTIFY swingChanged)

    QString host() const;
    bool isConnect() const;
    int port() const;
    QString error() const;
    bool power() const;
    bool hot() const;
    int fan() const;
    int swing() const;
    int temp() const;

signals:
    void hostChanged(QString host);
    void isConnectChanged(bool isConnect);
    void portChanged(quint16 port);
    void connetError(QAbstractSocket::SocketError socketError);
    void errorChanged(QString error);
    void tempChanged(int temp);
    void powerChanged(bool power);
    void hotChanged(bool hot);
    void fanChanged(int fan);
    void swingChanged(int swing);

public slots:
    void setHost(const QString& host);
    void tryConnect();
    void disconnect();
    void setPort(int port);
    void setPower(bool power);
    void setHot(bool hot);
    void setFan(int fan);
    void setSwing(int swing);
    void setTemp(int temp);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onTimeOut();
    void onReadServer();
};

#endif // CLIENT_H
