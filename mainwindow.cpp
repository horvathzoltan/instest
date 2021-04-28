#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "instest.h"
#include "common/macrofactory/macro.h"
#include "common/helper/string/stringhelper.h"
//#include "common/helper/downloader/downloader.h"
#include <QTimer>

#define CHNUM 16

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_timerTimeout);    
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}


void MainWindow::on_radioButton_start_clicked()
{
   setUi(Instest::Start());
}

void MainWindow::on_radioButton_stop_clicked()
{        
    auto m = Instest::Stop();
    setUi(m);
}

void MainWindow::on_timerTimeout()
{
    auto data_txt = Instest::DeviceGetData();
    auto b = Instest::DeviceBatt();
    //QString str;

    QString insoletype_txt;
    QString batt_txt;
    QString status_txt;
    //QString data_txt;
    QVarLengthArray<int> values(CHNUM);


    if(!data_txt.isEmpty())
    {                
        auto b = data_txt.split(';');
        //values.clear();

        int chnum = b.length();
        if(CHNUM<chnum) chnum = CHNUM;

        bool isok_all = true;
        for(int i=0;i<b.length();i++)
        {
            bool isok;
            auto v = b[i].toInt(&isok);
            values[i] = (isok)?v:0;
            if(!isok) isok_all=false;
        }

        //bool isok;
        //auto v = b[0].toInt(&isok);

        if(isok_all)
        {            
            int v = values[0];
            if(v>1000) insoletype_txt = "no insole connected";
            else
            {
                Instest::InsoleType i = Instest::GetInsoleType(v);
                if(i.id>0) insoletype_txt=i.name + ' '+"R="+QString::number(i.r)+"Ω";
                else insoletype_txt = "unknown insole";
            }
            status_txt = "data ok";
        }
        else
        {
            status_txt = "data error";
        }

        SetValues(values);
    }

    if(b.isEmpty()) batt_txt = "no battery status";
    else batt_txt = "batt:"+b;

    ui->label_pic->setText(data_txt);
    ui->label_status->setText(status_txt);
    ui->label_insoletype->setText(insoletype_txt);
    ui->label_batt->setText(batt_txt);
}

void MainWindow::setUi(const Instest::StartR& m){
    _camera_active = m.isActive;
    ui->label_txt->setText(m.msg);
    ui->label_serial->setText(m.serial);
    if(_camera_active)
    {
        timer->start(100);
        //if(Instest::OpenCamera()) timer->start(10);
    }
//    setLabelB(m._settings.brightnest);
//    setLabelC(m._settings.contrast);
//    setLabelS(m._settings.saturation);
//    setLabelG(m._settings.gain);
}

void MainWindow::SetValues(const QVarLengthArray<int>& values)
{
    if(values.length()<8) return;
    ui->progressBar_1->setValue(values[0]);
    ui->progressBar_2->setValue(values[1]);
    ui->progressBar_3->setValue(values[2]);
    ui->progressBar_4->setValue(values[3]);

    ui->progressBar_5->setValue(values[4]);
    ui->progressBar_6->setValue(values[5]);
    ui->progressBar_7->setValue(values[6]);
    ui->progressBar_8->setValue(values[7]);

    if(values.length()<16) return;
    ui->progressBar_9->setValue(values[8]);
    ui->progressBar_10->setValue(values[9]);
    ui->progressBar_11->setValue(values[10]);
    ui->progressBar_12->setValue(values[11]);

    ui->progressBar_13->setValue(values[12]);
    ui->progressBar_14->setValue(values[13]);
    ui->progressBar_15->setValue(values[14]);
    ui->progressBar_16->setValue(values[15]);
}

void MainWindow::ClrValues()
{
    ui->progressBar_1->setValue(0);
    ui->progressBar_2->setValue(0);
    ui->progressBar_3->setValue(0);
    ui->progressBar_4->setValue(0);

    ui->progressBar_5->setValue(0);
    ui->progressBar_6->setValue(0);
    ui->progressBar_7->setValue(0);
    ui->progressBar_8->setValue(0);

    ui->progressBar_9->setValue(0);
    ui->progressBar_10->setValue(0);
    ui->progressBar_11->setValue(0);
    ui->progressBar_12->setValue(0);

    ui->progressBar_13->setValue(0);
    ui->progressBar_14->setValue(0);
    ui->progressBar_15->setValue(0);
    ui->progressBar_16->setValue(0);
}

void MainWindow::setUi(const Instest::StopR& m){    
    timer->stop();

    ClrValues();

    ui->label_pic->setText("");
    ui->label_status->setText("");
    ui->label_insoletype->setText("");
    ui->label_batt->setText("");

    ui->label_txt->setText("");
    ui->label_serial->setText("");
    ui->label_msg->setText("");
//    if(_camera_active)
//    {
//        //Instest::CloseCamera();
//    }
}



//void MainWindow::on_pushButton_upload_clicked()
//{
//    if(!_camera_active)
//    {
//        ui->label_pic->setText("No active camera to upload.");
//    }
//    if(_camera_active && !timer->isActive())
//    {
//        setUi(Instest::Upload("kurutty.txt"));
//    }
//}

void MainWindow::setUi(const Instest::UploadR& m){
    ui->label_pic->setText(m.err);    
}

//void MainWindow::setLabelB(int i){ ui->label_b->setText(QString::number(i));}
//void MainWindow::setLabelC(int i){ ui->label_c->setText(QString::number(i));}
//void MainWindow::setLabelS(int i){ ui->label_s->setText(QString::number(i));}
//void MainWindow::setLabelG(int i){ ui->label_g->setText(QString::number(i));}
//void MainWindow::setLabelWB(int i){ ui->label_wb->setText(QString::number(i));}

//void MainWindow::on_pushButton_bp_clicked()
//{
//    setLabelB(Instest::brightnest_p());
//}

//void MainWindow::on_pushButton_bm_clicked()
//{
//    setLabelB(Camtest::brightnest_m());
//}

//void MainWindow::on_pushButton_cp_clicked()
//{
//    setLabelC(Camtest::contrast_p());
//}

//void MainWindow::on_pushButton_cm_clicked()
//{
//    setLabelC(Camtest::contrast_m());
//}

//void MainWindow::on_pushButton_sm_clicked()
//{
//    setLabelS(Camtest::saturation_p());
//}

//void MainWindow::on_pushButton_sp_clicked()
//{
//    setLabelS(Camtest::saturation_m());
//}

//void MainWindow::on_pushButton_gp_clicked()
//{
//    setLabelG(Camtest::gain_p());
//}

//void MainWindow::on_pushButton_gm_clicked()
//{
//    setLabelG(Camtest::gain_m());
//}

//void MainWindow::on_pushButton_wbp_clicked()
//{
//    setLabelWB(Camtest::wb_p());
//}

//void MainWindow::on_pushButton_wbm_clicked()
//{
//    setLabelWB(Camtest::wb_m());
//}

void MainWindow::on_pushButton_update_clicked()
{
    ui->label_msg->setText("Waiting for update...");
    setUi(Instest::Update());
}

void MainWindow::setUi(const Instest::UpdateR& m){

    ui->label_msg->setText(QStringLiteral("Update ")+(m.isOk?"ok\n":"error\n")+m.msg);
}

void MainWindow::on_pushButton_shutdown_clicked()
{
    ui->label_msg->setText("Waiting for shutdown...");
    setUi(Instest::Shutdown());
}

void MainWindow::setUi(const Instest::ShutdownR& m){

    ui->label_msg->setText(QStringLiteral("Shutdown ")+(m.isOk?"ok\n":"error\n")+m.msg);
    ui->radioButton_stop->setChecked(true);
}


