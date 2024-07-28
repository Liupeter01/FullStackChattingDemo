#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_login(new LoginInterface(this))
    , m_register(new registerinterface(this))
{
    ui->setupUi(this);

    /*deploy windows flags setting*/
    setFramelessWindow(m_login);
    setFramelessWindow(m_register);

    /*register signal slots*/
    registerSignalSlots();

    /*display login window as default*/
    displayDefaultWindow(m_login);
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

void MainWindow::registerSignalSlots()
{
    connect(this->m_login, &LoginInterface::switchWindow, this, &MainWindow::interfaceSwitchingHandler);
}

void MainWindow::interfaceSwitchingHandler()
{
    displayDefaultWindow(m_register);
}
