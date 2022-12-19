#ifndef IPSCANNER_H
#define IPSCANNER_H

#include <QStringList>
#include <QTimer>
#include <QTcpSocket>


class IpScanner
{
public:        
    static QStringList Scan(int i1, int i2, int p);
};

#endif // IPSCANNER_H
