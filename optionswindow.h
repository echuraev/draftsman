#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QWidget>
#include <QMutex>
#include <QSettings>

#define FILE_OPTIONS 0
#define WEB_OPTIONS 1

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QWidget
{
    Q_OBJECT

public:
    static OptionsWindow * getInstance(QWidget *parent = 0);
    ~OptionsWindow();

private slots:
    void changeSource(int val);
    void onPathBtowseButton();
    void onSaveButton();

private:
    explicit OptionsWindow(QWidget *parent = 0);
    void moveToCenter();
    static OptionsWindow * m_instance;
    static QMutex * m_mutex;
    QSettings m_settings;
    Ui::OptionsWindow *ui;
    void writeSettings();
    void readSettings();
};

#endif // OPTIONSWINDOW_H
