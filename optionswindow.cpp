#include "optionswindow.h"
#include "ui_optionswindow.h"
#include <QDesktopWidget>
#include <QRect>
#include <QPoint>

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
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    this->moveToCenter();
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
