#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"

namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void SlotSwitchReg();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginDialog *_login_dlg;
    RegisterDialog *_register_dlg;

};

#endif // MAINWINDOW_H
