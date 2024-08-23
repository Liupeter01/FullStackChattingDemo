#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_login(nullptr)
    , m_register(nullptr)   /*we don't need to allocate memory at the beginning*/
{
    ui->setupUi(this);

    switchingToLoginDialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayDefaultWindow(QWidget *window)
{
    setCentralWidget(window);
    window->show();
}

void MainWindow::setFramelessWindow(QDialog *dialog)
{
    dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
}

void MainWindow::switchingToRegInterface()
{
    m_register = new registerinterface(this);

    /*m_register has to be created to prevent from UB*/
    connect(m_register, &registerinterface::switchToLogin, this, &MainWindow::switchingToLoginDialog);

    setFramelessWindow(m_register);
    displayDefaultWindow(m_register);
}

void MainWindow::switchingToLoginDialog()
{
    m_login = new LoginInterface(this);

    connect(this->m_login, &LoginInterface::switchWindow, this, &MainWindow::switchingToRegInterface);
    connect(this->m_login, &LoginInterface::switchReset ,this,&MainWindow::switchingToResetDialog);

    setFramelessWindow(m_login);
    displayDefaultWindow(m_login);
}

void MainWindow::switchingToResetDialog()
{
    //m_login = new LoginInterface(this);

    //connect(this->m_login, &LoginInterface::switchWindow, this, &MainWindow::switchingToRegInterface);

    //setFramelessWindow(m_login);
    //displayDefaultWindow(m_login);
}
