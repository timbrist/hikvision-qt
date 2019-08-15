#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>//设备名字显示
#include <vector>//多个设备数组
#include <QMessageBox>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//查找设备按钮会与combobox, open button, displayLable控件交互
void MainWindow::on_EnumButton_clicked()
{
    ui->DisplayLabel->setStyleSheet("border:1px solid black;");
    vector< QString > QUserNames;
    if( hik.EnumDevices(QUserNames) != MV_OK)
    {
        QMessageBox::information(this,"异常","查找设备失败");
        return;
    }
    for(size_t i = 0; i < QUserNames.size(); i++)
    {
        ui->comboBox->addItem(QUserNames[i]);
    }
    ui->OpenButton->setDisabled(false);

}

//打开设备， 读取当前的combobox
//控件与关闭设备交互
void MainWindow::on_OpenButton_clicked()
{

    int nIndex = ui->comboBox->currentIndex();
    if ( hik.OpenDevice(nIndex) != MV_OK )
    {
        QMessageBox::information(this,"异常","打开设备失败");
        return;
    }
    ui->OpenButton->setDisabled(true);
    ui->CloseButton->setDisabled(false);
    ui->StartGrabbingButton->setDisabled(false);
}

//关闭设备按钮，
//与open button交互
void MainWindow::on_CloseButton_clicked()
{
   if(hik.CloseDevice() != MV_OK)
   {
        QMessageBox::information(this,"异常","关闭设备失败");
        return;
   }
   ui->OpenButton->setDisabled(false);
   ui->CloseButton->setDisabled(false);
}

//开始采集
void MainWindow::on_StartGrabbingButton_clicked()
{
    HWND MainWndID = (HWND)ui->DisplayLabel->winId();
    if( hik.StartGrabbing(MainWndID) != MV_OK)
    {
         QMessageBox::information(this,"异常","采集设备失败");
         return;
    }
    ui->StartGrabbingButton->setDisabled(true);
    ui->StopGrabbingButton->setDisabled(false);
    ui->SaveBmpButton->setDisabled(false);
    ui->SaveJpgButton->setDisabled(false);
}


void MainWindow::on_StopGrabbingButton_clicked()
{
   if( hik.StopGrabbing() != MV_OK)
   {
       QMessageBox::information(this,"异常","关闭采集失败");
       return;
   }
   ui->StartGrabbingButton->setDisabled(false);
   ui->StopGrabbingButton->setDisabled(true);
}

void MainWindow::on_SaveBmpButton_clicked()
{
   if( hik.SaveBmp() != MV_OK)
   {
       QMessageBox::information(this,"异常","保存bmp失败");
       return;
   }
   else {
       QMessageBox::information(this,"成功","保存图片成功");
   }
}
