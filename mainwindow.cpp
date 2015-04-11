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
#include <algorithm>
#include <QtSql>

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
    m_data = new DrawData();
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
    delete m_data;
}

void MainWindow::onActionStart()
{
    m_data->clear();

    QSettings settings("Egor Churaev", "Draftsman_2_0_0");
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
    }

    try
    {
        m_data->getMaxDataFromDBFile(path);
    }
    catch(QString e)
    {
        QMessageBox::critical(0, "Error!", e, QMessageBox::Ok);
        return;
    }

    double maxVal, minVal;
    maxVal = m_data->getValue().at(0);
    for(int i(0); i < m_data->getValue().length(); ++i)
    {
        if (m_data->getValue().at(i) > maxVal)
            maxVal = m_data->getValue().at(i);
    }

    QVector<double> items = m_data->getItems();
    QVector<double> value = m_data->getValue();
    qDebug() << value.length() << "\t" << value.at(0);

    m_data->clear();

    qDebug() << value.length() << "\t" << value.at(0);
    try
    {
        m_data->getMinDataFromDBFile(path);
    }
    catch(QString e)
    {
        QMessageBox::critical(0, "Error!", e, QMessageBox::Ok);
        return;
    }

    minVal = m_data->getValue().at(0);
    for(int i(0); i < m_data->getValue().length(); ++i)
    {
        if (m_data->getValue().at(i) < minVal)
            minVal = m_data->getValue().at(i);
    }

    ui->graphWidget->show();
    ui->graphWidget->legend->setVisible(true);
    ui->graphWidget->yAxis->setRange(minVal - 0.5, maxVal + 0.5);
    ui->graphWidget->xAxis->setRange(items.at(0) - 0.5, m_data->getItems().last() + 0.5);
    ui->graphWidget->xAxis->setAutoTicks(false);
    ui->graphWidget->xAxis->setAutoTickLabels(false);
    ui->graphWidget->xAxis->setTickVector(items);
    ui->graphWidget->xAxis->setTickVectorLabels(m_data->getTimes());

    ui->graphWidget->addGraph();
    ui->graphWidget->graph(0)->setPen(QPen(QColor(30, 40, 255, 150)));
    ui->graphWidget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->graphWidget->graph(0)->setData(items, value);
    ui->graphWidget->graph(0)->setName("Max Values");

    ui->graphWidget->addGraph();
    ui->graphWidget->graph(1)->setPen(QPen(QColor(Qt::red)));
    ui->graphWidget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->graphWidget->graph(1)->setData(m_data->getItems(), m_data->getValue());
    ui->graphWidget->graph(1)->setName("Min Values");

    QFont labelFont = font();
    labelFont.setPointSize(12);
    ui->graphWidget->xAxis->setLabel("Date");
    ui->graphWidget->xAxis->setLabelFont(labelFont);
    ui->graphWidget->yAxis->setLabel("Price, $");
    ui->graphWidget->yAxis->setLabelFont(labelFont);
    ui->graphWidget->plotLayout()->insertRow(0);
    ui->graphWidget->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->graphWidget, "Apple Inc. Stock Quotes"));
    ui->graphWidget->replot();

    qDebug() << value.length() << "\t" << value.at(0);
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
