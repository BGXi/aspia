/*
* PROJECT:         Aspia Remote Desktop
* FILE:            network/socket_tcp.h
* LICENSE:         See top-level directory
* PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
*/

#ifndef _ASPIA_NETWORK__SOCKET_TCP_H
#define _ASPIA_NETWORK__SOCKET_TCP_H

#include "aspia_config.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <memory>

#include "base/macros.h"
#include "network/socket.h"

class SocketTCP : public Socket
{
public:
    SocketTCP();
    ~SocketTCP();

    void Connect(const char *hostname, int port) override;
    int Write(const char *buf, int len) override;
    int Read(char *buf, int len) override;
    void Bind(const char *hostname, int port) override;
    void Listen() override;
    std::unique_ptr<Socket> Accept() override;
    void Close() override;
    std::string GetIpAddress() override;
    void SetWriteTimeout(int timeout) override;
    void SetReadTimeout(int timeout) override;
    void SetNoDelay(bool enable) override;

private:
    SocketTCP(SOCKET sock);

private:
    SOCKET sock_;
    bool ref_;

    DISALLOW_COPY_AND_ASSIGN(SocketTCP);
};

#endif // _ASPIA_NETWORK__SOCKET_TCP_H
