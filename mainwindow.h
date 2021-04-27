#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "instest.h"
#include "common/helper/downloader/downloader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    void setLabelB(int i);
//    void setLabelC(int i);
//    void setLabelS(int i);
//    void setLabelG(int i);
//    void setLabelWB(int i);
private slots:
    void on_radioButton_start_clicked();
    void on_radioButton_stop_clicked();
    void on_timerTimeout();
    void on_pushButton_update_clicked();

    //void on_pushButton_upload_clicked();

//    void on_pushButton_bp_clicked();

//    void on_pushButton_bm_clicked();

//    void on_pushButton_cp_clicked();

//    void on_pushButton_cm_clicked();

//    void on_pushButton_sm_clicked();

//    void on_pushButton_sp_clicked();

//    void on_pushButton_gp_clicked();

//    void on_pushButton_gm_clicked();

//    void on_pushButton_wbp_clicked();

//    void on_pushButton_wbm_clicked();


private:
    Ui::MainWindow *ui;
    QTimer *timer;
    bool _camera_active;
    void setUi(const Instest::StartR&);
    void setUi(const Instest::StopR&);
    void setUi(const Instest::UploadR &m);
    void setUi(const Instest::UpdateR &m);
};
#endif // MAINWINDOW_H
