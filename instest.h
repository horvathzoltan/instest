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
    static bool Ping(const QString &ip, int port=-1);

    static bool DeviceUpdateStorageStatus()
    {
        auto a = Instest::_d->download("get_storage_status", "");
        return a.startsWith("ok");
    }

    static bool DeviceMountStorage()
    {
        auto a = Instest::_d->download("set_storage_mount", "");
        return a.startsWith("ok");
    }
    //static bool OpenCamera(){return Instest::_d.download("set_cam_open", "")=="ok";}
    static bool DeviceActive()
    {
        if(!Instest::_d) return false;
        auto a = Instest::_d->download("active", "");
        auto b = a=="active";
        return b;
    }

    static QString DeviceVersion()
    {
        auto a = Instest::_d->download("version", "");
        return a;
    }

    static QString DeviceBatt()
    {
        auto a = Instest::_d->download("get_batt", "");
        return a;
    }

    static QString DeviceUpdate()
    {
        auto a = Instest::_d->download("update", "");
        return a;
    }

    //static bool GetCamSettings();
    //static bool CloseCamera(){return Instest::_d.download("set_cam_close", "")=="ok";}
    //static QPixmap GetPixmap();
    static QString DeviceGetData();
    struct UploadR
    {
        QString err;
    };
    //static UploadR Upload(const QString& fn);
    static QString GetDriverName();
    static QFileInfo GetMostRecent(const QString &path, const QString &pattern);
    static QString GetUrl(int i1, int i2, int p);

    static QStringList GetIp(int i1, int i2, int p);
    static void FilterLocalIp(QStringList *l);
//    static int setCamSettings(const QString& s, int i);
//    static int brightnest_p();
//    static int brightnest_m();
//    static int contrast_p();
//    static int contrast_m();
//    static int saturation_p();
//    static int saturation_m();
//    static int gain_p();
//    static int gain_m();
//    static int wb_p();
    //    static int wb_m();
    static Instest::InsoleType GetInsoleType(int v);

    struct UpdateR
    {
      bool isOk;
      QString msg;
    };

    static Instest::UpdateR Update();
private:
    static QByteArray _DeviceGetData(){return Instest::_d->download("get_data", "");}

//    static QString UploadMetaData(const QString& fn, int len);
//    static void UploadData(const QString& key, const QByteArray& a);
//    static int UploadNext(const QString& key);
};


#endif // CAMTEST_H
