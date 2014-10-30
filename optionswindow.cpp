#include "optionswindow.h"
#include "ui_optionswindow.h"
#include <QDesktopWidget>
#include <QRect>
#include <QPoint>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

OptionsWindow * OptionsWindow::m_instance = NULL;
QMutex * OptionsWindow::m_mutex = new QMutex();

OptionsWindow * OptionsWindow::getInstance(QWidget *parent)
{
    if (m_instance == NULL)
    {
        m_mutex->lock();
        if (m_instance == NULL)
        {
            m_instance = new OptionsWindow(parent);
        }
        m_mutex->unlock();
    }
    return m_instance;
}


OptionsWindow::OptionsWindow(QWidget *parent) :
    QWidget(parent),
    m_settings("Egor Churaev", "Draftsman"),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    this->moveToCenter();
    readSettings();
    changeSource(ui->sourcesBox->currentIndex());
    connect(ui->sourcesBox, SIGNAL(currentIndexChanged(int)), SLOT(changeSource(int)));
    connect(ui->pathBtowseButton, SIGNAL(clicked()), SLOT(onPathBtowseButton()));
    connect(ui->saveButton, SIGNAL(clicked()), SLOT(onSaveButton()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}

void OptionsWindow::moveToCenter()
{
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    QPoint center = rect.center();
    center.setX(center.x() - (this->width()/2));
    center.setY(center.y() - (this->height()/2));
    move(center);
}

void OptionsWindow::onPathBtowseButton()
{
    QString path = QFileDialog::getOpenFileName(0, "Open file...", "", "*.txt *.csv");
    ui->pathLine->setText(path);
}

void OptionsWindow::onSaveButton()
{
    writeSettings();
    close();
}

void OptionsWindow::changeSource(int val)
{
    if (val == FILE_OPTIONS)
    {
        ui->pathBtowseButton->show();
        ui->pathLabel->show();
        ui->pathLine->show();

        ui->webHostLabel->hide();
        ui->webHostLine->hide();
        ui->webPeriodLabel->hide();
        ui->webPeriodLine->hide();
        ui->webPeriodSeconds->hide();
        ui->webPortLabel->hide();
        ui->webPortLine->hide();
    }
    else if (val == WEB_OPTIONS)
    {
        ui->webHostLabel->show();
        ui->webHostLine->show();
        ui->webPeriodLabel->show();
        ui->webPeriodLine->show();
        ui->webPeriodSeconds->show();
        ui->webPortLabel->show();
        ui->webPortLine->show();

        ui->pathBtowseButton->hide();
        ui->pathLabel->hide();
        ui->pathLine->hide();
    }
}

void OptionsWindow::writeSettings()
{
    m_settings.beginGroup("/Options");

    if (ui->separatorLine->text().length() == 0)
    {
        QMessageBox::critical(0, "Error!", "The separator field is empty!", QMessageBox::Ok);
        return;
    }

    if (ui->sourcesBox->currentIndex() == FILE_OPTIONS)
    {
        if (ui->pathLine->text().length() == 0)
        {
            QMessageBox::critical(0, "Error!", "The path field is empty!", QMessageBox::Ok);
            return;
        }
    }
    else if (ui->sourcesBox->currentIndex() == WEB_OPTIONS)
    {
        if (ui->webHostLine->text().length() == 0)
        {
            QMessageBox::critical(0, "Error!", "The host field is empty!", QMessageBox::Ok);
            return;
        }
        if (ui->webPortLine->text().length() == 0)
        {
            QMessageBox::critical(0, "Error!", "The port field is empty!", QMessageBox::Ok);
            return;
        }
        bool ok = false;
        ui->webPortLine->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::critical(0, "Error!", "In the port field should be a decimal number!", QMessageBox::Ok);
            return;
        }
    }
    else
    {
        QMessageBox::critical(0, "Unknown error!", "Unknown error with data source field!", QMessageBox::Ok);
        return;
    }

    if (ui->webPeriodLine->text().length() == 0)
    {
        QMessageBox::critical(0, "Error!", "In the period field should be a number!", QMessageBox::Ok);
        return;
    }

    bool ok = false;
    int period = ui->webPeriodLine->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(0, "Error!", "In the period field should be a decimal number!", QMessageBox::Ok);
        return;
    }

    m_settings.setValue("/separator", ui->separatorLine->text());
    m_settings.setValue("/source", ui->sourcesBox->currentIndex());
    m_settings.setValue("/path", ui->pathLine->text());
    m_settings.setValue("/period", period);
    m_settings.setValue("/host", ui->webHostLine->text());
    m_settings.setValue("/port", ui->webPortLine->text());

    m_settings.endGroup();
}

void OptionsWindow::readSettings()
{
    m_settings.beginGroup("/Options");

    QString separator = m_settings.value("/separator", ";").toString();
    int source = m_settings.value("/source", FILE_OPTIONS).toInt();
    QString path = m_settings.value("/path", "").toString();
    int period = m_settings.value("/period", 15).toInt();
    QString host = m_settings.value("/host", "54.191.174.225").toString();
    QString port = m_settings.value("/port", "1430").toString();

    ui->separatorLine->setText(separator);
    ui->sourcesBox->setCurrentIndex(source);
    ui->pathLine->setText(path);
    QString tmp = "";
    tmp.setNum(period);
    ui->webPeriodLine->setText(tmp);
    ui->webHostLine->setText(host);
    ui->webPortLine->setText(port);

    m_settings.endGroup();
}
