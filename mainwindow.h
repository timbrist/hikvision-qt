#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MvCameraControl.h>
#include "hk/MyCamera.h"
#include <Windows.h>
#include <hk/qt_hik.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    //开始采集
    void on_pushButton_5_clicked();
    //停止采集
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;

//hikvision functions
public:
    void EnumDevices();
    int OpenDevice();
    //采集图片的线程显示
    void display_imgs();
    void display_images(QImage *img,
                        unsigned int nDataLen,
                        MV_FRAME_OUT_INFO_EX stImageInfo);




//海康威视变量
private:
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:设备信息列表结构体变量，用来存储设备列表
    CMyCamera*      m_pcMyCamera;               // ch:CMyCamera封装了常用接口
    HWND m_hwndDisplay;                          // ch:显示句柄 |
    BOOL  m_bStartGrabbing;                     // ch:是否开始抓图 |
    //unsigned char*  m_pBufForSaveImage;         // ch:用于保存图像的缓存 | en:Buffer to save image
    //unsigned int    m_nBufSizeForSaveImage;
    unsigned char*  m_pBufForDriver;            // ch:用于从驱动获取图像的缓存 | en:Buffer to get image from driver
    unsigned int    m_nBufSizeForDriver;
    HANDLE hGrab_img_thread;

    QtHik::HikWindow hik;


};

#endif // MAINWINDOW_H
