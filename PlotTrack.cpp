#include "PlotTrack.h"
#include "DataManager.h"

#include <QDebug>
#include <QPainter>

int PlotTrack::m_instanceCount = 1;

PlotTrack::PlotTrack(QWidget *parent)
    : PlotItemBase(parent)
{
    m_leftPadding = 50;
    m_rightPadding = 50;
    m_interPadding = 20;
    m_contextPadding = 50;

    m_horiGridNum = 4;
    m_verGridNum = 5;

    QString name = QString("Track%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColor = Qt::gray;
    m_gridColor = Qt::darkGray;
    m_axisColor = Qt::lightGray;
}

PlotTrack::~PlotTrack()
{

}

void PlotTrack::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
    m_thresholdColorMap = targetMap;
}

void PlotTrack::setLeftPadding(int leftPadding)
{
    m_leftPadding = leftPadding;
}

void PlotTrack::setRightPadding(int rightPadding)
{
    m_rightPadding = rightPadding;
}

void PlotTrack::setInterPadding(int interPadding)
{
    m_interPadding = interPadding;
}

void PlotTrack::drawRect(int itemIndex, QList<QColor> dataList)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(1);
    int xPos = m_contextPadding;
    int yPos;
    int itemHeight;
    for (int i = 0; i < dataList.size(); i++) {
        pen.setColor(dataList.at(i));
        painter.setPen(pen);
        if (dataList.at(i) != Qt::gray) {
            itemHeight = this->height() / 25 * 1.5;
        } else {
            itemHeight = this->height() / 25;
        }
        yPos = this->height() - m_contextPadding - ((itemIndex + 1) * (itemHeight + m_interPadding));
        painter.drawLine(QPoint(xPos + i, yPos), QPoint(xPos + i, yPos + itemHeight));
    }
}

void PlotTrack::paintEvent(QPaintEvent *event)
{
    //绘制x轴和y轴
    QPainter painter(this);
    QPen pen;
    pen.setColor(m_axisColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(QPointF(m_contextPadding, height() - m_contextPadding), QPointF(width() - m_contextPadding, height() - m_contextPadding)); //x轴
    painter.drawLine(QPointF(m_contextPadding, height() - m_contextPadding), QPointF(m_contextPadding, m_contextPadding));   //y轴

    //绘制网格
    pen.setColor(m_gridColor);
    pen.setWidth(1);
    painter.setPen(pen);
    double verTablePadding = (height() - 2 * m_contextPadding) / m_verGridNum;
    double horTablePadding = (width() - 2 * m_contextPadding) / m_horiGridNum;
    for (int i = 0; i < m_verGridNum; i++) {
        painter.drawLine(QPointF(m_contextPadding, m_contextPadding + verTablePadding * i),
                         QPointF(width() - m_contextPadding, m_contextPadding + verTablePadding * i));
    }
    for (int i = 0; i < m_horiGridNum; i++) {
        painter.drawLine(QPointF(m_contextPadding + horTablePadding * (i + 1), m_contextPadding),
                         QPointF(m_contextPadding + horTablePadding * (i + 1), height() - m_contextPadding));
    }

    getDataInfo(m_seconds);

    return PlotItemBase::paintEvent(event);
}

void PlotTrack::getDataInfo(double secs)
{
    m_itemCnt = getPlotPairData().size();
    setInterPadding((height() - 2 * m_contextPadding - this->height() / 25 * 1.5 * m_itemCnt) / (m_itemCnt + 1));

    for (int i = 0; i < m_itemCnt; i++) {
        QString entityType = getPlotPairData().at(i).first;
        updateData(i, entityType, secs);
    }
}

void PlotTrack::updateData(int itemIndex, QString entityType, double secs)
{
    QStringList entityList = entityType.split("+");
    QList<double> valueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(entityList.at(0), entityList.at(1), secs);

    if (valueList.isEmpty())
        return;

    QList<QColor> dataList;
    for (int i = 0; i < valueList.size(); i++) {
        if (valueList.at(i) == 0) { // 当前时间有跟踪数据
            dataList.push_back(Qt::gray);
        } else {
            dataList.push_back(Qt::green);
        }
    }
    drawRect(itemIndex, dataList);
    return;
}

void PlotTrack::onGetCurrentSeconds(double secs)
{
    if (getPlotPairData().isEmpty())
        return;

    m_seconds = secs;
    update();
}

