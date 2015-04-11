#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "drawdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();

private slots:
    void onActionStart();
    void onActionOptions();
    void onActionAbout();

private:
    Ui::MainWindow *ui;
    void moveToCenter();
    DrawData * m_data;
};

#endif // MAINWINDOW_H
