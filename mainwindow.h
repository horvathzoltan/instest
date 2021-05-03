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

    void SetValues(const QVarLengthArray<int> &values);
    void ClrValues();
private slots:
    void on_radioButton_start_clicked();
    void on_radioButton_stop_clicked();
    void on_timerTimeout();
    void on_pushButton_update_clicked();

    void on_pushButton_shutdown_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    bool _camera_active;
    void setUi(const Instest::StartR&);
    void setUi(const Instest::StopR&);
    void setUi(const Instest::UploadR &m);
    void setUi(const Instest::UpdateR &m);
    void setUi(const Instest::ShutdownR &m);
};
#endif // MAINWINDOW_H
