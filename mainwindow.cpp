#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "optionswindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QRect>
#include <QPoint>

// http://blog.harrix.org/?p=1813

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->moveToCenter();
    QAction * about = new QAction(0);
    ui->menuBar->addAction(about);
    about->setText("About...");
    connect(ui->actionStart, SIGNAL(triggered()), SLOT(onActionStart()));
    connect(ui->actionOptions, SIGNAL(triggered()), SLOT(onActionOptions()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(about, SIGNAL(triggered()), SLOT(onActionAbout()));
}

void MainWindow::closeEvent(QCloseEvent * e){
    e->accept();
    qApp->quit();
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

void MainWindow::moveToCenter()
{
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    QPoint center = rect.center();
    center.setX(center.x() - (this->width()/2));
    center.setY(center.y() - (this->height()/2));
    move(center);
}
