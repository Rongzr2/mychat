#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg=new LoginDialog(this);
    setCentralWidget(_login_dlg);

    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    _register_dlg = new RegisterDialog(this);

    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _register_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    _register_dlg->hide();
}

void MainWindow::SlotSwitchReg()
{
    _login_dlg->hide();
    setCentralWidget(_register_dlg);
    _register_dlg->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

