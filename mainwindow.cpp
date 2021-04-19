#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "instest.h"
#include "common/macrofactory/macro.h"
//#include "common/helper/downloader/downloader.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_timer_timeout);
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

void MainWindow::on_timer_timeout()
{
    auto p = Instest::GetData();
    if(!p.isEmpty())
    {        
        QString str;
        auto b = p.split(';');
        bool isok;
        auto v = b[0].toInt(&isok);
        if(isok)
        {
            Instest::InsoleType i = Instest::GetInsoleType(v);
            if(i.id>0)
            {
                str=i.name + ' '+"R="+QString::number(i.r)+"Ω";
            }
        }
        else{
            str = "no insole check";
        }
        if(!str.isEmpty()) str+= '\n';
        str+=p;
        ui->label_pic->setText(str);//setPixmap(p);

    }
    else
    {
        ui->label_pic->setText("upsz");
    }
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
    setLabelB(m._settings.brightnest);
    setLabelC(m._settings.contrast);
    setLabelS(m._settings.saturation);
    setLabelG(m._settings.gain);
}

void MainWindow::setUi(const Instest::StopR& m){
    timer->stop();
    //ui->label_txt->setText("");
    //ui->label_serial->setText("");
    if(_camera_active)
    {
        //Instest::CloseCamera();
    }
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

void MainWindow::setLabelB(int i){ ui->label_b->setText(QString::number(i));}
void MainWindow::setLabelC(int i){ ui->label_c->setText(QString::number(i));}
void MainWindow::setLabelS(int i){ ui->label_s->setText(QString::number(i));}
void MainWindow::setLabelG(int i){ ui->label_g->setText(QString::number(i));}
void MainWindow::setLabelWB(int i){ ui->label_wb->setText(QString::number(i));}

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
