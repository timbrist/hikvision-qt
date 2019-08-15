#ifndef GRAB_IMAGES_H
#define GRAB_IMAGES_H

#include<QThread>
#include<mainwindow.h>

class GrabImages:public QThread
{
    Q_OBJECT
public:
    explicit GrabImages(MainWindow *gui);
    ~GrabImages();
    void run();//用于线程的重载

signals:
    //需要用线程处理的函数
    void grabbing_img_thread();

public slots:
    void slotUpdateUi();

public:
    MainWindow *m_gui;

};

#endif // GRAB_IMAGES_H
