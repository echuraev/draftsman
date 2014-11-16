#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QVector>

namespace Ui {
class MainWindow;
}

typedef struct SData
{
    QVector<double> time;
    QVector<double> population;
} Data;

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
    Data m_data;
    QString m_xLabel;
    void parseData(QVector<QString> lines, QString separator);
    void clearData();
};

#endif // MAINWINDOW_H
