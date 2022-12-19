#include "ipscanner.h"
#include "qthread.h"

#include <QHostAddress>



QStringList IpScanner::Scan(int i1, int i2, int port)
{
    if(i1<1||i1>255) return {};
    if(i2<1||i2>255) return {};
    if(i1>i2) return {};
    if(port<1||port>UINT16_MAX) return {};

    unsigned char a[4];
    a[3]=10;
    a[2]=10;
    a[1]=10;
    qint32* c = (int*)a;

    QTcpSocket socket;
    QStringList ipList;
    QHostAddress address;

    for(unsigned char u=i1;u<i2;u++){
        a[0]=u;
        address.setAddress(*c);
        socket.connectToHost(address, port, QIODevice::ReadWrite);
        if(socket.waitForConnected(10))
        {
            socket.disconnectFromHost();
            ipList.append(address.toString());
        }
    }

    return ipList;
}
