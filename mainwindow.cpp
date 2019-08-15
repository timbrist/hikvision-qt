#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

//mainwindow.h 已经定义过了
//#include <MvCameraControl.h>
//#include "hk/MyCamera.h"

#include <QFileDialog>
#include <iostream>

#include "grab_images.h"

//不先定义， 在display 中容易内存爆表
#define HEIGHT 1024
#define WIDTH  1280

using namespace std;


//停止采集的标志
int grab_img_flag = 0;
static int g_run = 0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //添加子线程ui的链接
    //start grabbing
    connect(ui->pushButton_5,
            SIGNAL(clicked()),
            this,
            SLOT(on_pushButton_5_clicked()));

    //end of grabbing
    connect(ui->pushButton,
            SIGNAL(clicked()),
            this,
            SLOT(on_pushButton_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::EnumDevices()
{

    // ch:设备信息列表结构体变量，用来存储设备列表
    // ch:初始化设备信息列表
    memset(&this->m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = CMyCamera::EnumDevices(&this->m_stDevList);
    if(nRet != MV_OK)
    {
        throw "Can not Initialized";
    }
    for(size_t i = 0; i < this->m_stDevList.nDeviceNum; i++)
    {
        //设备信息
        MV_CC_DEVICE_INFO *pDeviceInfo = this->m_stDevList.pDeviceInfo[i];
        if(pDeviceInfo == NULL)
        {
            continue;
        }
        //usb device info
        cout << pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName << endl;
        cout << pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName << endl;
        cout << pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber << endl;
        cout << pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName << endl;
        unsigned char *pUserName = pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName;
        string pUserName2 = (char*)pUserName;
        QString pUserName3 = QString::fromStdString(pUserName2);
        ui->comboBox->addItem(pUserName3);
    }
    if(this->m_stDevList.nDeviceNum == 0)
    {
        throw "No device" ;
    }
}
int MainWindow::OpenDevice()
{
    //注册回调函数
    //int nRet2 = CMyCamera::RegisterImageCallBack();

    int nIndex = ui->comboBox->currentIndex();
    cout << "nIndex: "<< nIndex << endl;
    if( (nIndex < 0) || (nIndex >= MV_MAX_DEVICE_NUM))
    {
        throw "Please select device ";
        return -1;
    }
    if(this->m_stDevList.pDeviceInfo[nIndex] == NULL)
    {
        cout << "What the hell" << endl;
    }

    this->m_pcMyCamera = new CMyCamera;
    if(this->m_pcMyCamera == NULL)
    {
        return -1;
    }
    int nRet = m_pcMyCamera->Open(this->m_stDevList.pDeviceInfo[nIndex]);
    if(nRet != MV_OK)
    {
        delete this->m_pcMyCamera;
        this->m_pcMyCamera = NULL;
        return nRet;
    }
    return MV_OK;
}

//===========================================以下为按钮==============================
// ch:按下查找设备按钮:枚举
void MainWindow::on_pushButton_2_clicked()
{
    using namespace std;

    cout << " hello world " << endl;

    EnumDevices();
    ui->label->setStyleSheet("border:2px solid black;");
}
// ch:按下打开设备按钮：打开设备
void MainWindow::on_pushButton_3_clicked()
{
    int nRet = OpenDevice();
    if(nRet != MV_OK)
    {
        QMessageBox::information(this,"hint","Camera failed");
        throw "Open Fail";
    }
    else {
        QMessageBox::information(this,"hint","Camera OK");
    }

    ui->pushButton_3->setDisabled(true);

}

//采集图像
void MainWindow::on_pushButton_5_clicked()
{
    //开始采集
    int nRet = -1;
    if(m_pcMyCamera != NULL)
    {
        nRet = m_pcMyCamera->StartGrabbing();
    }
    else {
        return;
    }
    if(nRet != MV_OK)
        return;


    //获取图像
    //获取1张图
    unsigned int nRecvBufSize = 0;
    nRet = m_pcMyCamera->GetIntValue("PayloadSize", &nRecvBufSize);
    if(nRet != MV_OK)
    {
        QMessageBox::information(this,"hint","failed in get payloadsize");
    }
    this->m_nBufSizeForDriver = nRecvBufSize;
    this->m_pBufForDriver = (unsigned char *)malloc(this->m_nBufSizeForDriver);
    if (NULL == this->m_pBufForDriver)
    {
        QMessageBox::information(this,"hint","malloc m_pBuffordriver failed");
    }


    grab_img_flag = 1;
    g_run = 1;
    GrabImages *grab_img_ptr = new GrabImages(this);
    grab_img_ptr->start();


/*
    // 使用线程替换以下原始代码
    unsigned int nDataSize = nRecvBufSize ;
    unsigned int nImageNum = 1;
    unsigned int nDataLen = 0;

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));


    while(1)
    {
        m_pcMyCamera->GetOneFrameTimeout(this->m_pBufForDriver, &nDataLen, this->m_nBufSizeForDriver, &stImageInfo, 1000);
        //nRet2 = MV_CC_GetOneFrame(m_pcMyCamera->m_hDevHandle, pFrameBuf, nBufSize, &stInfo);
        HWND MainWndID = (HWND)this->ui->label->winId();
        nRet = m_pcMyCamera->Display(MainWndID);
        if(nRet != MV_OK)
        {
            QMessageBox::information(this,"hint", "Display failed");
            return;
        }
        size_t height = stImageInfo.nHeight;
        size_t width = stImageInfo.nWidth;
        QImage *img = new QImage( static_cast<int>(width), static_cast<int>(height), QImage::Format_Grayscale16);
        memcpy( (*img).bits(), this->m_pBufForDriver, stImageInfo.nFrameLen);
        //QImage *image = new QImage(width, height, QImage::Format_RGB888);
        //memcpy( (*image).bits(), this->m_pBufForDriver, width*height*3);
        //ui->label->setScaledContents(true);
        img->scaled(ui->label->size(), Qt::KeepAspectRatio);
        ui->label->setPixmap(QPixmap::fromImage(*img) );
    }
*/



    //hGrab_img_thread = CreateThread(NULL, 0, grabbing_img_thread,this,0,0);
    this->m_bStartGrabbing = TRUE;


}

//关闭设备
void MainWindow::on_pushButton_4_clicked()
{

}

//停止采集
void MainWindow::on_pushButton_clicked()
{
    if(this->m_pcMyCamera != NULL)
    {
        if(m_pcMyCamera->StopGrabbing() != MV_OK)
            return;
    }
    else
    {
        return;
    }
}
void MainWindow::display_images(QImage *img,
                                unsigned int nDataLen,
                                MV_FRAME_OUT_INFO_EX stImageInfo)
{
    m_pcMyCamera->GetOneFrameTimeout(this->m_pBufForDriver, &nDataLen, this->m_nBufSizeForDriver, &stImageInfo, 1000);
    //nRet2 = MV_CC_GetOneFrame(m_pcMyCamera->m_hDevHandle, pFrameBuf, nBufSize, &stInfo);
    HWND MainWndID = (HWND)this->ui->label->winId();
    if (m_pcMyCamera->Display(MainWndID) != MV_OK)
    {
        QMessageBox::information(this,"hint", "Display failed");
        exit(0);
    }
    memcpy( (*img).bits(), this->m_pBufForDriver, stImageInfo.nFrameLen);
    img->scaled(ui->label->size(), Qt::KeepAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(*img) );
}

void MainWindow::display_imgs()
{
    //ui->pushButton_5->setDisabled(true);
    //获取图像
    //获取1张图
    unsigned int nRecvBufSize = 0;
    int nRet = m_pcMyCamera->GetIntValue("PayloadSize", &nRecvBufSize);
    if(nRet != MV_OK)
    {
        QMessageBox::information(this,"hint","failed in get payloadsize");
    }
    this->m_nBufSizeForDriver = nRecvBufSize;
    this->m_pBufForDriver = (unsigned char *)malloc(this->m_nBufSizeForDriver);
    if (NULL == this->m_pBufForDriver)
    {
        QMessageBox::information(this,"hint","malloc m_pBuffordriver failed");
    }

    //int nBufSize = stIntvalue.nCurValue + 2048;


    //unsigned int nDataSize = nRecvBufSize ;
    //unsigned int nImageNum = 1;
    unsigned int nDataLen = 0;
    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    QImage *img = new QImage( WIDTH, HEIGHT, QImage::Format_Grayscale16);
    this->display_images(img, nDataLen, stImageInfo);
}
