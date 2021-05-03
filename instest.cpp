#include "instest.h"
#include "networkhelper.h"
#include "settings.h"
#include "sqlhelper.h"
#include "common/helper/ProcessHelper/processhelper.h"
#include "common/helper/string/stringhelper.h"
#include <QDirIterator>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "common/logger/log.h"
#include <QUrl>
#include <QNetworkInterface>
#include <QThread>

extern Settings _settings;

QUrl Instest::CamUrl;
com::helper::Downloader* Instest::_d = nullptr;
Instest::InsSettings Instest::_camSettings;
QList<Instest::InsoleType> Instest::_insoletypes;

// ping cél ip
// ha ok, akkor arp -a cél ip -> mac addr
// ha ismeretlen mac

/*
nmap -Pn -p22 --open 10.10.10.100-130
Nmap scan report for\s+(?:\S+\s+)?\(?([0-9\.]+)\)?
*/
QStringList Instest::IpScan(int i1, int i2, int p)
{
    if(i1<1||i1>255) return {};
    if(i2<1||i2>255) return {};
    if(i1>i2) return {};
    if(p<1||p>UINT16_MAX) return {};

    auto cmd = QStringLiteral(R"(nmap -Pn -p%3 --open 10.10.10.%1-%2)").arg(i1).arg(i2).arg(p);
    auto out = com::helper::ProcessHelper::Execute(cmd);
    if(out.exitCode) return {};

    static const QRegularExpression r(R"(Nmap scan report for\s+(?:\S+\s+)?\(?([0-9\.]+)\)?)");
    auto mi = r.globalMatch(out.stdOut);

    QStringList e;
    while (mi.hasNext())
    {
        auto a = mi.next().captured(1);
        e.append(a);
    }

    return e;
}

void Instest::FilterLocalIp(QStringList *l)
{
    auto lip = QNetworkInterface::allAddresses();

    for(auto&i:lip) l->removeAll(i.toString());
}



Instest::StartR Instest::Start(){
    int cam_p = 8080;
    auto iplist=IpScan(100,155,cam_p);
    FilterLocalIp(&iplist);

    QString cam_ip;

    if(iplist.count()>0) cam_ip= iplist[0];
    else cam_ip=QStringLiteral("10.10.10.151");

    CamUrl = QUrl(QStringLiteral("http://%1:%2").arg(cam_ip).arg(cam_p));

    if(_d) delete _d;
    _d = new com::helper::Downloader(CamUrl.toString());

    if(!NetworkHelper::Ping(cam_ip)) return {"cannot ping insole at "+cam_ip, "", 0, "",{}};
    auto isActive = DeviceActive();
    auto version = DeviceVersion();
    auto batt = DeviceBatt();

    //GetCamSettings();

    auto cmd = QStringLiteral(R"(arp -a -n %1)").arg(cam_ip);
    auto out = com::helper::ProcessHelper::Execute(cmd);
    if(out.exitCode!=0) return {"arp error", "", 0, version, _camSettings};
    // ? (172.16.3.235) at dc:a6:32:74:92:dd [ether] on enp4s0
    if(out.stdOut.isEmpty()) return {"no arp output", "", 0, version,{}};

    QString& x = out.stdOut;
    QRegularExpression re(QStringLiteral(R"(at\s+((?:[0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2})\s+)"));
    QRegularExpressionMatch m = re.match(x);

    QString mac;
    if (m.hasMatch())
    {
        mac = m.captured(m.lastCapturedIndex());
    }    

    QString msg;
    QString serial;

    append_value(&msg, cam_ip);
    append_value(&msg, mac);
    append_value(&msg, isActive);

    SQLHelper sqlh;
    static const QString CONN = QStringLiteral("conn1");
    auto db = sqlh.Connect(_settings._sql_settings, CONN);



    if(db.isValid())
    {
        bool db_ok = db.open();
        QString dberr="";
        QString project = "5";//mestercipo
        QString board_rev = "logger_2v0";//a válzozat
        bool isNew = false;
        int rows=0;
        if(db_ok)
        {
            QSqlQuery query(db);
            db_ok = query.exec(QStringLiteral("SELECT id, serial FROM ManufacturingInfo WHERE mac = '%1'").arg(mac));
            if(db_ok)
            {
                //int fieldNo = query.record().indexOf("country");
                rows = 0;
                while(query.next()){
                    rows++;
                    serial = query.value(1).toString();
                    break;
                }
                if(!rows){
                    isNew = true;
                    serial = NewSerial(db);

                    QSqlQuery query2(db);
                    db_ok = query2.exec(QStringLiteral("INSERT into ManufacturingInfo (mac,serial,project,board_rev) VALUES('%1','%2', '%3', '%4')")
                                .arg(mac).arg(serial).arg(project).arg(board_rev));

                }
            }

            static const QString cmd2 = QStringLiteral("SELECT id,Name,R,VMax,VMin FROM MasterMove_UWPClient.hw.InsoleTypes").arg(mac);
            db_ok = query.exec(cmd2);
            if(db_ok)
            {
                rows = 0;
                while(query.next()){
                    rows++;
                    InsoleType a
                    {
                        query.value(0).toInt(),
                        query.value(1).toString(),
                        query.value(2).toInt(),
                        query.value(4).toInt(),
                        query.value(3).toInt()
                    };
                    _insoletypes.append(a);
                }
                if(!rows)
                {
                    dberr+="no insoletypes\n";
                }
            }
        }

        if(!db_ok)
        {
            QSqlError a = db.lastError();
            dberr = a.text().trimmed();
        }
        db.close();

        append_value(&msg, db_ok);
        append_value(&msg, dberr);
        append_value(&msg, isNew);
        append_value(&msg, serial);
        append_value(&msg, rows);
        append_value(&msg, version);
        append_value(&msg, batt);
    }
    QSqlDatabase::removeDatabase("conn1");

    return {msg, serial, isActive, version, _camSettings};
}

Instest::InsoleType Instest::GetInsoleType(int v)
{
    for(auto&i:_insoletypes)
    {
        if(i.min<=v && v<=i.max)
            return i;
    }
    return {};
}

Instest::UpdateR Instest::Update()
{
    QString msg;

    auto a = DeviceActive();
    if(!a)
    {
        com::helper::StringHelper::AppendLine(&msg, "not active");
        return {false, msg};
    }

    auto v_old = DeviceVersion();

    append_value(&msg, v_old);

    auto updstatus = DeviceUpdateStorageStatus();
    if(!updstatus) updstatus=DeviceMountStorage();
    if(!updstatus){
        com::helper::StringHelper::AppendLine(&msg, "cannot mount");
        return {false, msg};
    }

    DeviceUpdate();

    int i;
    QString v_new;
    bool isok;        
    for(i=0;i<10;i++)
    {
        QThread::sleep(3);
        v_new = DeviceVersion();
        if(v_new.isEmpty()) continue;
        isok = v_new!=v_old;
        if(isok) break;
    }

    if(i>10) msg+="Timeout\n";

    append_value(&msg, v_new);
    append_value(&msg, i);

    return {isok, msg};
}

//bool Instest::Ping(const QString& ip, int port){
//    Q_UNUSED(port)
//    auto cmd = QStringLiteral(R"(ping -c1 -W1 %1)").arg(ip);
//    auto out = com::helper::ProcessHelper::Execute(cmd);
//    if(out.exitCode) return false;
//    //test ping port
//    return true;
//}

QString Instest::NewSerial(const QSqlDatabase& db){
    QSqlQuery q(db);
    auto db_ok = q.exec(QStringLiteral("SELECT MAX(serial) AS maxserial FROM ManufacturingInfo"));
    if(!db_ok) return QString();
    int serial;
    q.first();
    auto a = q.value(0);
    if(a.isNull()) serial = 1001; else serial = a.toInt()+1;
    return QString::number(serial);
}

Instest::StopR Instest::Stop(){return {};}

QString Instest::DeviceGetData()
{
     return Instest::_DeviceGetData();
}

Instest::ShutdownR Instest::Shutdown()
{
    QString msg;

    auto a = DeviceActive();
    if(!a)
    {
        com::helper::StringHelper::AppendLine(&msg, "not active");
        return {false, msg};
    }

    DeviceShutdown();

    int i;
    bool ping;

    auto host = CamUrl.host();
    for(i=0;i<10;i++)
    {
        QThread::sleep(3);
        ping = NetworkHelper::Ping(host);
        if(!ping) break;
    }

    if(i>10) msg+="Timeout\n";

    return {!ping, msg};
}
