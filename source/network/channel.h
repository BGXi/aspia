//
// PROJECT:         Aspia
// FILE:            network/channel.h
// LICENSE:         GNU Lesser General Public License 2.1
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#ifndef _ASPIA_NETWORK__CHANNEL_H
#define _ASPIA_NETWORK__CHANNEL_H

#include <QTcpSocket>
#include <QQueue>

namespace aspia {

class Channel : public QObject
{
    Q_OBJECT

public:
    Channel(QObject* parent = nullptr);
    ~Channel() = default;

    void connectToHost(const QString& address, int port);

signals:
    void channelConnected();
    void channelDisconnected();
    void channelError(const QString& message);
    void channelMessage(const QByteArray& buffer);

public slots:
    void writeMessage(const QByteArray& buffer);
    void stopChannel();

private slots:
    void onConnected();
    void onError();
    void onBytesWritten(qint64 bytes);
    void onReadyRead();

private:
    void scheduleWrite();

    using MessageSizeType = quint32;

    QTcpSocket* socket_;

    QQueue<QByteArray> write_queue_;
    MessageSizeType write_size_ = 0;
    qint64 written_ = 0;

    bool read_size_received_ = false;
    QByteArray read_buffer_;
    MessageSizeType read_size_ = 0;
    qint64 read_ = 0;

    Q_DISABLE_COPY(Channel)
};

} // namespace aspia

#endif // _ASPIA_NETWORK__CHANNEL_H