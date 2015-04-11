#include "drawdata.h"

DrawData::DrawData()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

DrawData::~DrawData()
{

}

void DrawData::addValue(QString time, double value)
{
    m_time.append(time);
    m_itemIndex.append(++m_itemsCounter);
    m_value.append(value);
}

void DrawData::clear()
{
    m_itemIndex.clear();
    m_time.clear();
    m_value.clear();
    m_itemsCounter = 0;
}

QString DrawData::getTime(int index)
{
    return m_time.at(index);
}

QVector<QString> DrawData::getTimes()
{
    return m_time;
}

QVector<double> DrawData::getItems()
{
    return m_itemIndex;
}

QVector<double> DrawData::getValue()
{
    return m_value;
}

void DrawData::getMaxDataFromDBFile(QString file)
{
    getDataFromDBFile(file, MAX_DATA);
}

void DrawData::getMinDataFromDBFile(QString file)
{
    getDataFromDBFile(file, MIN_DATA);
}

void DrawData::getDataFromDBFile(QString file, int oper) //throw (QString)
{
    m_db.setDatabaseName(file);
    if (!m_db.open())
    {
        throw QString("Cannot open db file!");
    }

    getMaxAndMinTime();
    m_numberOfDays = getNumberOfDays();

    QSqlQuery query;
    QString request;
    for (int i(0); i < m_numberOfDays; ++i)
    {
        if (oper == MAX_DATA)
            request = "SELECT MAX(value) FROM raw_data WHERE time>=\"" + getDateFromMinTime(i) + "\" AND time<\"" + getDateFromMinTime(i+1) + "\"";
        else
            request = "SELECT MIN(value) FROM raw_data WHERE time>=\"" + getDateFromMinTime(i) + "\" AND time<\"" + getDateFromMinTime(i+1) + "\"";
        query.exec(request);
        double val;
        while (query.next())
        {
            val = query.value(0).toDouble();
        }
        this->addValue(getDateFromMinTime(i), val);
    }

    m_db.close();
}

void DrawData::getMaxAndMinTime()
{
    QSqlQuery query("SELECT MIN(time) FROM raw_data");

    while (query.next())
    {
        m_minTime = query.value(0).toString();
    }

    query.exec("SELECT MAX(time) FROM raw_data");

    while (query.next())
    {
        m_maxTime = query.value(0).toString();
    }
}

int DrawData::getNumberOfDays()
{
    int minDay = m_minTime.split("-").at(2).split(" ").at(0).toInt();
    int maxDay = m_maxTime.split("-").at(2).split(" ").at(0).toInt();
    return maxDay - minDay + 1;
}

QString DrawData::getDateFromMinTime(int day)
{
    QString date = m_minTime.mid(0, m_minTime.lastIndexOf("-") + 1);
    int targetDay = m_minTime.split("-").at(2).split(" ").at(0).toInt() + day;
    date += QString::number(targetDay);
    return date;
}

int DrawData::m_itemsCounter = 0;
