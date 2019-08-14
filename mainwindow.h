#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MvCameraControl.h>
#include "hk/MyCamera.h"
#include <Windows.h>

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
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

private:
    void EnumDevices();
    int OpenDevice();
private:
    Ui::MainWindow *ui;

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

};

#endif // MAINWINDOW_H
