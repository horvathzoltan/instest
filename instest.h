#ifndef CAMTEST_H
#define CAMTEST_H

#include <QFileInfo>
#include <QPixmap>
#include <QSqlDatabase>
#include <QString>
#include "common/helper/downloader/downloader.h"

class Instest
{
public:
    /*
INSERT INTO CPU.hw.InsoleTypes (Id,LastModified,Name,InsoleGenderId,InsoleAgeCategoryId,InsoleSideId,EUSize,GeometryCSV,R,VMax,VMin) VALUES
     (1,'2020-02-08 14:15:00.000','42Jobb',1,2,3,42.0,'""',390,53,46);
*/
    struct InsoleType
    {
        int id;
        QString name;
        int r;
        int min;
        int max;
    };

    struct InsSettings{
//        int brightnest;
//        int contrast;
//        int saturation;
//        int gain;
//        int wb;
    };

    static com::helper::Downloader* _d;
    static QUrl CamUrl;

    struct StartR{
        QString msg;
        QString serial;
        bool isActive;
        QString version;
        InsSettings _settings;
    };


    static QList<InsoleType> _insoletypes;
    static InsSettings _camSettings;
    static StartR Start();
    struct StopR{};
    static StopR Stop();
    static QString NewSerial(const QSqlDatabase &db);
    //static bool Ping(const QString &ip, int port=-1);

    static bool DeviceUpdateStorageStatus()
    {
        if(!_d) return false;
        auto a = _d->download("get_storage_status", "");
        return a.startsWith("ok");
    }

    static bool DeviceMountStorage()
    {
        if(!_d) return false;
        auto a = _d->download("set_storage_mount", "");
        return a.startsWith("ok");
    }
    //static bool OpenCamera(){return Instest::_d.download("set_cam_open", "")=="ok";}
    static bool DeviceActive()
    {
        if(!_d) return false;
        auto a = _d->download("active", "");
        auto b = a=="active";
        return b;
    }

    static QString DeviceBatt()
    {
        if(!_d) return nullptr;
        auto a = _d->download("get_batt", "");
        return a;
    }


    static QString DeviceVersion()
    {
        if(!_d) return nullptr;
        auto a = _d->download("version", "");
        return a;
    }

    static QString DeviceUpdate()
    {
        if(!_d) return nullptr;
        auto a = _d->download("update", "");
        return a;
    }

    static QString DeviceShutdown()
    {
        if(!_d) return nullptr;
        auto a = _d->download("shutdown", "");
        return a;
    }

    static QString DeviceGetData();
    struct UploadR
    {
        QString err;
    };
    static QString GetDriverName();
    static QFileInfo GetMostRecent(const QString &path, const QString &pattern);
    static QString GetUrl(int i1, int i2, int p);

    static QStringList IpScan(int i1, int i2, int p);    
    static void FilterLocalIp(QStringList *l);
    static Instest::InsoleType GetInsoleType(int v);

    struct UpdateR
    {
      bool isOk;
      QString msg;
    };

    static Instest::UpdateR Update();

    struct ShutdownR
    {
      bool isOk;
      QString msg;
    };
    static Instest::ShutdownR Shutdown();

private:
    static QByteArray _DeviceGetData(){return Instest::_d->download("get_data", "");}

};


#endif // CAMTEST_H
