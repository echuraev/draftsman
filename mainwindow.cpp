#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "optionswindow.h"
#include <QDebug>

// http://blog.harrix.org/?p=1813

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QAction * about = new QAction(0);
    ui->menuBar->addAction(about);
    about->setText("About...");
    connect(ui->actionStart, SIGNAL(triggered()), SLOT(onActionStart()));
    connect(ui->actionOptions, SIGNAL(triggered()), SLOT(onActionOptions()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(about, SIGNAL(triggered()), SLOT(onActionAbout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onActionStart()
{
    qDebug() << "On Start";
}

void MainWindow::onActionOptions()
{
    OptionsWindow * window = OptionsWindow::getInstance();
    window->show();
    window->raise();
    window->activateWindow();
}

void MainWindow::onActionAbout()
{
    AboutWindow * window = new AboutWindow();
    window->show();
}
