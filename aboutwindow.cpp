#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(clicked()), SLOT(close()));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}
