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
    m_verPadding = 50;

    m_horiGridNum = 4;
    m_verGridNum = 5;

    QString name = QString("Track%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColor = Qt::gray;
    m_gridColor = Qt::darkGray;
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

void PlotTrack::drawRect(int itemIndex, TrackStatus status, int leftBoundary, int rightBoundary, QColor color)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(color);
    QBrush brush;
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);

    painter.setPen(pen);
    painter.setBrush(brush);

    QRect rect;
    int itemHeight;
    switch (status) {
    case TrackStatus::Available:
        itemHeight = this->height() / 25 * 1.5;
        break;
    case TrackStatus::Invalid:
    case TrackStatus::Unavailable:
        itemHeight = this->height() / 25;
        break;
    }
    int xPos = leftBoundary;
    int yPos = height() - m_verPadding - ((itemIndex + 1) * (itemHeight + m_interPadding));
    int width = rightBoundary - leftBoundary;
    int height = itemHeight;
    rect.setRect(xPos, yPos, width, height);
    painter.drawRect(rect);
}

void PlotTrack::paintEvent(QPaintEvent *event)
{
    //绘制x轴和y轴
    QPainter painter(this);
    QPen pen;
    pen.setColor(m_axisColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(QPoint(0, height()), QPoint(width(), height()));   //x轴
    painter.drawLine(QPoint(0, height()), QPoint(0, 0));    //y轴

    //绘制网格
    pen.setColor(m_gridColor);
    painter.setPen(pen);

    QBrush brush;   //画刷。填充几何图形的调色板，由颜色和填充风格组成
    brush.setColor(m_gridFillColor);
    brush.setStyle(Qt::SolidPattern);
    //painter.setBrush(brush);

    int horiGridWidth = 0;
    if (m_horiGridNum) {    //item水平方向延展
        horiGridWidth = width() / m_horiGridNum;
    }

    for (int i = 0; i < m_horiGridNum; i++) {
        QRect gridRect;
        gridRect.setRect(i * horiGridWidth, 0, horiGridWidth, height());
        painter.drawRect(gridRect);
    }

    int verGridWidth = 0;
    if (m_verGridNum) {     //item水平方向延展
        verGridWidth = height() / m_verGridNum;
    }

    for (int i = 0; i < m_verGridNum; i++) {
        QRect gridRect;
        gridRect.setRect(0, (i + 1) * verGridWidth, width(), verGridWidth);
        painter.drawRect(gridRect);
    }

    getDataInfo(m_seconds);

    return PlotItemBase::paintEvent(event);
}

void PlotTrack::getDataInfo(double secs)
{
    m_itemCnt = getPlotPairData().size();
    setInterPadding((height() - 2 * m_verPadding - this->height() / 25 * 1.5 * m_itemCnt) / (m_itemCnt + 1));

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

    //*获取当前Attr值
    double currValue = valueList.last();

    QString currKey = entityList.at(0) + '_' + entityList.at(1);
    if (!m_thresholdColorMap.contains(currKey) || m_thresholdColorMap.value(currKey).keys().isEmpty()) {
        //没有设置阈值，则无需分开绘制多个矩形，以默认颜色绘制一个即可
        drawRect(itemIndex, TrackStatus::Available, 0, currValue, m_defaultColor);
        update();
        return;
    }

    //根据颜色阈值来分开绘制
    auto colorMap = m_thresholdColorMap.value(currKey);
    QList<int> thresholdList = colorMap.keys();
    if (currValue < thresholdList.first()) {
        drawRect(itemIndex, TrackStatus::Available, 0, currValue, m_defaultColor);
    }

    //以默认颜色绘制第一个矩形
    drawRect(itemIndex, TrackStatus::Available, 0, thresholdList.first(), m_defaultColor);

    if (thresholdList.size() == 1) {
        drawRect(itemIndex, TrackStatus::Available, thresholdList.first(), currValue, colorMap.value(thresholdList.first()));
    }

    for (int i = 0; i < thresholdList.size() - 1; i++) {
        QColor currColor = colorMap.value(thresholdList.at(i));

        if (currValue < thresholdList.at(i + 1)) {
            drawRect(itemIndex, TrackStatus::Available, thresholdList.at(i), currValue, colorMap.value(thresholdList.at(i)));
            break;
        } else {
            drawRect(itemIndex, TrackStatus::Available, thresholdList.at(i), thresholdList.at(i + 1), colorMap.value(thresholdList.at(i)));
        }
    }

    if (currValue > thresholdList.last()) {
        drawRect(itemIndex, TrackStatus::Available, thresholdList.last(), currValue, colorMap.value(thresholdList.last()));
    }
    update();
}

void PlotTrack::onGetCurrentSeconds(double secs)
{
    if (getPlotPairData().isEmpty())
        return;

    m_seconds = secs;
    update();
}

