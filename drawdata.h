#ifndef DRAWDATA_H
#define DRAWDATA_H

#include <QVector>
#include <QtSql>

#define MAX_DATA 1
#define MIN_DATA 0

class DrawData
{
public:
    DrawData();
    ~DrawData();
    void addValue(QString time, double value);
    void clear();
    void getMaxDataFromDBFile(QString file, int period);
    void getMinDataFromDBFile(QString file, int period);
    QString getTime(int index);
    QVector<QString> getTimes();
    QVector<double> getItems();
    QVector<double> getValue();
private:
    void getDataFromDBFile(QString file, int oper);
    void getMaxAndMinTime();
    int getNumberOfDays();
    QString getDateTimeFromMinTime(int day, int periodNumber);
    QString getDateFromMinTime(int day);
    static int m_itemsCounter;
    QVector<QString> m_time;
    QVector<double> m_itemIndex;
    QVector<double> m_value;
    QSqlDatabase m_db;
    QString m_minTime;
    QString m_maxTime;
    int m_period;
    int m_numberOfDays;
    int *m_timesPerDay;
};

#endif // DRAWDATA_H
