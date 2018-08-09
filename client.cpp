#include "client.h"
#include <QDataStream>
#include <QDebug>

Client::Client(QObject* parent)
    : QObject(parent)
{
    connect(&m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(&m_socket, &QTcpSocket::readyRead, this, &Client::onReadServer);
    connect(&m_socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &Client::connetError);
    connect(&m_timer, &QTimer::timeout, this, &Client::onTimeOut);
    m_timer.setSingleShot(true);
}

void Client::sendData()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << static_cast<quint16>(25);
    out << static_cast<quint16>(m_power);
    out << static_cast<quint16>(m_hot);
    out << static_cast<quint16>(m_temp);
    out << static_cast<quint16>(m_fan);
    out << static_cast<quint16>(m_swing);

    qDebug() << block;
    m_socket.write(block);
}

void Client::sendKeepAlive()
{
}

void Client::startTimer()
{
    m_timer.stop();
    m_timer.start(timeOut);
}

QString Client::host() const
{
    return m_host;
}

bool Client::isConnect() const
{
    return m_isConnect;
}

int Client::port() const
{
    return m_port;
}

QString Client::error() const
{
    return m_socket.errorString();
}

bool Client::power() const
{
    return m_power;
}

bool Client::hot() const
{
    return m_hot;
}

int Client::fan() const
{
    return m_fan;
}

int Client::swing() const
{
    return m_swing;
}

int Client::temp() const
{
    return m_temp;
}

void Client::setHost(const QString& host)
{
    if (m_host == host)
        return;

    m_host = host;
    emit hostChanged(m_host);
}

void Client::tryConnect()
{
    this->m_socket.connectToHost(m_host, static_cast<quint16>(m_port));
}

void Client::disconnect()
{
    m_socket.close();
}

void Client::setPort(int port)
{
    if (m_port == port)
        return;

    if (m_port < 0)
        return;

    m_port = port;
    emit portChanged(static_cast<quint16>(m_port));
}

void Client::setPower(bool power)
{
    if (m_power == power)
        return;

    if (isConnect()) {
        m_power = power;
        emit powerChanged(m_power);
        startTimer();
    }
}

void Client::setHot(bool hot)
{
    if (m_hot == hot)
        return;

    if (isConnect()) {
        m_hot = hot;
        emit hotChanged(m_hot);
        startTimer();
    }
}

void Client::setFan(int fan)
{
    if (m_fan == fan)
        return;

    if (isConnect()) {
        m_fan = fan;
        emit fanChanged(m_fan);
        startTimer();
    }
}

void Client::setSwing(int swing)
{
    if (m_swing == swing)
        return;
    if (isConnect()) {
        m_swing = swing;
        emit swingChanged(m_swing);
        startTimer();
    }
}

void Client::setTemp(int temp)
{
    if (m_temp == temp)
        return;
    if (temp < 18 || temp > 25)
        return;

    if (isConnect()) {
        m_temp = temp;
        emit tempChanged(m_temp);
        startTimer();
    }
}

void Client::onConnected()
{
    qDebug() << "connected";
    m_isConnect = true;
    emit isConnectChanged(true);
}

void Client::onDisconnected()
{
    m_isConnect = false;
    emit isConnectChanged(false);
}

void Client::onError(QAbstractSocket::SocketError socketError)
{
    // TODO
}

void Client::onTimeOut()
{
    sendData();
}

void Client::onReadServer()
{
    auto* socket = qobject_cast<QTcpSocket*>(QObject::sender());
    if (socket->bytesAvailable() < packageSize + 1) {
        return;
    }
    QDataStream in(socket);
    quint16 type;
    in >> type;
    if (type == 0) {
        // send keep alive
        //        sendKeepAlive(socket);
        return;
    }
    quint16 data[packageSize];

    for (unsigned short& i : data) {
        in >> i;
    }
    setPower(data[0]);
    setHot(data[1]);
    setTemp(data[2]);
    setFan(data[3]);
    setSwing(data[4]);
    qDebug() << "power" << m_power;
    qDebug() << "hot" << m_hot << data[1];
    qDebug() << "temp" << m_temp;
    qDebug() << "fan" << m_fan;
    qDebug() << "swing" << m_swing;
}
