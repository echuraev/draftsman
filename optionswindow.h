#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QWidget>
#include <QMutex>

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QWidget
{
    Q_OBJECT

public:
    static OptionsWindow * getInstance(QWidget *parent = 0);
    ~OptionsWindow();

private:
    explicit OptionsWindow(QWidget *parent = 0);
    void moveToCenter();
    static OptionsWindow * m_instance;
    static QMutex * m_mutex;
    Ui::OptionsWindow *ui;
};

#endif // OPTIONSWINDOW_H
