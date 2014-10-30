#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "optionswindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QRect>
#include <QPoint>
#include <QSettings>
#include <QMessageBox>
#include <QFile>

// http://blog.harrix.org/?p=1813

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->moveToCenter();
    QAction * about = new QAction(0);
    ui->menuBar->addAction(about);
    ui->graphWidget->hide();
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
    clearData();

    QSettings settings("Egor Churaev", "Draftsman");
    settings.beginGroup("/Options");

    QString separator = settings.value("/separator", ";").toString();
    int source = settings.value("/source", FILE_OPTIONS).toInt();
    QString path = settings.value("/path", "").toString();
    //int period = settings.value("/period", 15).toInt();
    //QString host = settings.value("/host", "54.191.174.225").toString();
    //QString port = settings.value("/port", "1430").toString();

    settings.endGroup();

    if (source == FILE_OPTIONS)
    {
        if (path.length() == 0)
        {
            QMessageBox::critical(0, "Enter path to file!",
                                  "Please, enter path to a file with data!\nGo:\nFile->Options", QMessageBox::Ok);
            return;
        }
        QFile inputFile(path);
        QVector<QString> linesVector;
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while ( !in.atEnd() )
            {
                linesVector.append(in.readLine());
            }
            inputFile.close();
        }
        else
        {
            QMessageBox::critical(0, "Error!", "Cannot open file with data!", QMessageBox::Ok);
            return;
        }

        parseData(linesVector, separator);
    }
    else if (source == WEB_OPTIONS)
    {
        QMessageBox::information(0, "Web version doesn't work!", "Web version doesn't implemented!", QMessageBox::Ok);
        return;
    }

    ui->graphWidget->show();
    ui->graphWidget->addGraph();
    QPen blueDotPen;
    blueDotPen.setColor(QColor(30, 40, 255, 150));
    blueDotPen.setWidthF(4);
    ui->graphWidget->graph(0)->setPen(blueDotPen);
    ui->graphWidget->graph(0)->setData(m_data.time, m_data.population);
    ui->graphWidget->xAxis->setLabel(m_xLabel);
    ui->graphWidget->yAxis->setLabel("Population, bln");
    QVector<double> tmp = m_data.population;
    qSort(tmp);
    ui->graphWidget->xAxis->setRange(m_data.time.at(0), m_data.time.last());
    ui->graphWidget->yAxis->setRange(tmp.at(0), tmp.last());
    ui->graphWidget->plotLayout()->insertRow(0);
    ui->graphWidget->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->graphWidget, "World Population"));
    ui->graphWidget->replot();
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

void MainWindow::parseData(QVector<QString> lines, QString separator)
{
    double firstTime = -1;
    double period = -1;
    foreach (QString str, lines)
    {
        QStringList list = str.split(separator, QString::SkipEmptyParts);
        if (list.count() != 2)
            continue;
        QString time = list.at(0);
        if (firstTime < 0)
        {
            firstTime = time.toDouble();
            continue;
        }
        if (period < 0 && firstTime > 0)
        {
            period = time.toDouble() - firstTime;
            break;
        }
    }
    if (period < 60)
        m_xLabel = "Seconds";
    else if (period >= 60 && period < 3600)
        m_xLabel = "Minutes";
    else if (period >= 3600 && period < 86400)
        m_xLabel = "Hours";
    else
        m_xLabel = "Days";
    qDebug() << period;
    foreach (QString str, lines)
    {
        QStringList list = str.split(separator, QString::SkipEmptyParts);
        if (list.count() != 2)
            continue;
        QString time = list.at(0);
        QString population = list.at(1);
        m_data.time.append((time.toDouble() - firstTime)/period);
        m_data.population.append(population.toDouble());
    }
}

void MainWindow::clearData()
{
    m_data.time.clear();
    m_data.population.clear();
}
