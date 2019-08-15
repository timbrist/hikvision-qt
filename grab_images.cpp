#include "grab_images.h"
extern int grab_img_flag; //用于是否停止采集

GrabImages::GrabImages(MainWindow *gui)
{
    m_gui = gui;
    connect(this,
            SIGNAL(grabbing_img_thread()),
            this,
            SLOT(slotUpdateUi()) );
}

GrabImages::~GrabImages()
{
    delete m_gui;
}

void GrabImages::run()
{


        emit grabbing_img_thread();

}

//在界面总显示采集的图片
void GrabImages::slotUpdateUi()
{
    m_gui->display_imgs();
}
