#ifndef QT_HIK_HPP
#define QT_HIK_HPP
//hikvision sdk 在qt上显示的内容。
//以Qt MainWindow接口为基础，对常用函数进行三次次封装，方便qt用户使用

#include "hk/MyCamera.h"
#include "Windows.h"//HWND
#include <vector>
#include <QString>
#include <QImage>

namespace QtHik {
class HikWindow
{
public:
    HikWindow();
    ~HikWindow();
//海康威视的接口及变量操作
public:
    //列举出说有设备并返回设备名字
    //私有变量 m_stDevList会保存设备信息
    int EnumDevices(std::vector<QString> &QUserNames);

    //输入需要打开设备的编号
    //需要EnumDevice函数得到 m_stDevList内容
    int OpenDevice(int &nIndex);

    //对私有变量进行回收
    int CloseDevice();

    //设置采集模式
    //对私有变量m_nTriggerMode 进行 0 或 1 赋值。
    int SetTriggerMode(unsigned int trigger_mode);

    //开始采集
    //用引用居然莫名其妙卡死
    int StartGrabbing(HWND MainWndId);

    //关闭采集
    int StopGrabbing();

    //保存Bmp图片
    int SaveBmp();
private:
    unsigned char*  m_pBufForDriver;            // ch:用于从驱动获取图像的缓存
    unsigned int    m_nBufSizeForDriver;

    /*ch:状态 | en:Status*/
    BOOL  m_bOpenDevice;                        // ch:是否打开设备
    BOOL  m_bStartGrabbing;                     // ch:是否开始抓图
    unsigned int   m_nTriggerMode;                       // ch:触发模式
    int   m_nTriggerSource;                     // ch:触发源
    MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      // ch:保存图像格式

    /*ch:设备相关 | en:Device Related*/
    CMyCamera*      m_pcMyCamera;               // ch:CMyCamera封装了常用接口
    HWND  m_hwndDisplay;                        // ch:显示句柄
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:设备信息列表结构体变量，用来存储设备列表
    unsigned char*  m_pBufForSaveImage;         // ch:用于保存图像的缓存 | en:Buffer to save image
    unsigned int    m_nBufSizeForSaveImage;



};

}

#endif // QT_HIK_HPP
