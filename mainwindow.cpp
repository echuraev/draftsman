#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "qcustomplot.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QRect>
#include <QPoint>
#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
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
    about->setText("About...");
    m_data = new DrawData();
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(about, SIGNAL(triggered()), SLOT(onActionAbout()));
    connect(ui->pdfBrowseButton, SIGNAL(clicked()), SLOT(onPdfBrowseButton()));
    connect(ui->pathBrowseButton, SIGNAL(clicked()), SLOT(onPathBrowseButton()));
    connect(ui->createPDFButton, SIGNAL(clicked()), SLOT(onCreatePDFButton()));
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

void MainWindow::onActionAbout()
{
    AboutWindow * window = new AboutWindow();
    window->show();
}

void MainWindow::onPathBrowseButton()
{
    QString path = QFileDialog::getOpenFileName(0, "Open file...", "", "*.db");
    ui->pathLine->setText(path);
}

void MainWindow::onPdfBrowseButton()
{
    QString path = QFileDialog::getExistingDirectory(0, "Open file...", "");
    ui->pdfPathLine->setText(path);
}

void MainWindow::onCreatePDFButton()
{
    m_data->clear();

    QString dbPath = ui->pathLine->text();
    QString pdfDirPath = ui->pdfPathLine->text();
    QString pdfName = ui->pdfName->text();

    if (dbPath.length() == 0)
    {
        QMessageBox::critical(0, "Enter path to db file!", "Please, enter path to the db file!", QMessageBox::Ok);
        return;
    }
    if (pdfDirPath.length() == 0)
    {
        QMessageBox::critical(0, "Enter path to directory to new pdf file!",
                              "Please, enter path to directory for pdf!", QMessageBox::Ok);
        return;
    }
    if (pdfName.length() == 0)
    {
        QMessageBox::critical(0, "Enter the name for pdf file!", "Please, enter the name to pdf file!", QMessageBox::Ok);
        return;
    }

    QCustomPlot *plotWidget = new QCustomPlot();
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    plotWidget->resize(rect.width(), rect.height());

    try
    {
        m_data->getMaxDataFromDBFile(dbPath, ui->frequencyBox->currentIndex());
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
        m_data->getMinDataFromDBFile(dbPath, ui->frequencyBox->currentIndex());
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

    plotWidget->legend->setVisible(true);
    plotWidget->yAxis->setRange(minVal - 0.5, maxVal + 0.5);
    plotWidget->xAxis->setRange(items.at(0) - 0.5, m_data->getItems().last() + 0.5);
    plotWidget->xAxis->setAutoTicks(false);
    plotWidget->xAxis->setAutoTickLabels(false);
    plotWidget->xAxis->setTickVector(items);
    plotWidget->xAxis->setTickVectorLabels(m_data->getTimes());

    plotWidget->addGraph();
    plotWidget->graph(0)->setPen(QPen(QColor(30, 40, 255, 150)));
    plotWidget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    plotWidget->graph(0)->setData(items, value);
    plotWidget->graph(0)->setName("Max Values");

    plotWidget->addGraph();
    plotWidget->graph(1)->setPen(QPen(QColor(Qt::red)));
    plotWidget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    plotWidget->graph(1)->setData(m_data->getItems(), m_data->getValue());
    plotWidget->graph(1)->setName("Min Values");

    QFont labelFont = font();
    labelFont.setPointSize(12);
    plotWidget->xAxis->setLabel("Date");
    plotWidget->xAxis->setLabelFont(labelFont);
    plotWidget->yAxis->setLabel("Price, $");
    plotWidget->yAxis->setLabelFont(labelFont);
    plotWidget->plotLayout()->insertRow(0);
    plotWidget->plotLayout()->addElement(0, 0, new QCPPlotTitle(plotWidget, "Apple Inc. Stock Quotes"));
    plotWidget->replot();
    //plotWidget->show();
    QPixmap pixmap(plotWidget->size());
    plotWidget->render(&pixmap);
    //pixmap.save("test.png");

    QMessageBox::information(0, "Success!", "Success! PDF file has been created!", QMessageBox::Ok);
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
