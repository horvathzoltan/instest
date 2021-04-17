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
    struct InsSettings{
        int brightnest;
        int contrast;
        int saturation;
        int gain;
        int wb;
    };

    static com::helper::Downloader _d;
    static QUrl CamUrl;

    struct StartR{
        QString msg;
        QString serial;
        bool isActive;
        InsSettings _settings;
    };


    static InsSettings _camSettings;
    static StartR Start();
    struct StopR{};
    static StopR Stop();
    static QString NewSerial(const QSqlDatabase &db);
    static bool Ping(const QString &ip, int port=-1);

    //static bool OpenCamera(){return Instest::_d.download("set_cam_open", "")=="ok";}
    static bool ActiveCamera(){return Instest::_d.download("active", "")=="active";}
    //static bool GetCamSettings();
    //static bool CloseCamera(){return Instest::_d.download("set_cam_close", "")=="ok";}
    //static QPixmap GetPixmap();
    static QString GetData();
    struct UploadR
    {
        QString err;
    };
    //static UploadR Upload(const QString& fn);
    static QString GetDriverName();
    static QFileInfo GetMostRecent(const QString &path, const QString &pattern);

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
private:
    static QByteArray _GetData(){return Instest::_d.download("get_data", "");}

//    static QString UploadMetaData(const QString& fn, int len);
//    static void UploadData(const QString& key, const QByteArray& a);
//    static int UploadNext(const QString& key);
};

static QString GetIp(int i1, int i2, int p);
#endif // CAMTEST_H
