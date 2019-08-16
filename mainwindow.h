#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_EnumButton_clicked();

    void on_OpenButton_clicked();

    void on_CloseButton_clicked();

    void on_StartGrabbingButton_clicked();

    void on_StopGrabbingButton_clicked();

    void on_SaveBmpButton_clicked();

    void on_HardwareRadioButton_clicked();

    void on_ContinuesRadioButton_clicked();

private:
    Ui::MainWindow *ui;

//海康威视接口对象
private:
    QtHik::HikWindow hik;


};

#endif // MAINWINDOW_H
