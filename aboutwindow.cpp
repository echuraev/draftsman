#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QDesktopWidget>
#include <QRect>
#include <QPoint>

AboutWindow::AboutWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    this->moveToCenter();
    connect(ui->okButton, SIGNAL(clicked()), SLOT(close()));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::moveToCenter()
{
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    QPoint center = rect.center();
    center.setX(center.x() - (this->width()/2));
    center.setY(center.y() - (this->height()/2));
    move(center);
}
