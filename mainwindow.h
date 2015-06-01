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
    void onActionAbout();
    void onPathBrowseButton();
    void onPdfBrowseButton();
    void onCreatePDFButton();

private:
    void createPDF(QPixmap *image, QString pdfName);
    Ui::MainWindow *ui;
    void moveToCenter();
    DrawData * m_data;
};

#endif // MAINWINDOW_H
