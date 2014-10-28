#include "optionswindow.h"
#include "ui_optionswindow.h"

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
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}
