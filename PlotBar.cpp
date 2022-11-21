#include "PlotBar.h"
#include <QDebug>
#include <QPainter>
#include "DataManager.h"
int PlotBar::m_instanceCount = 1;
PlotBar::PlotBar(QWidget *parent)
    : PlotItemBase(parent)
{
    m_bHorizontal = true;

    m_leftPadding = 50;
    m_rightPadding = 50;
    m_interPadding = 20;

    m_currTimeIndex = 0;

    m_horiGridNum = 4;
    m_verGridNum = 5;

    QString name = QString("Bar%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColor = Qt::gray;
    m_gridColor = Qt::darkGray;
}

PlotBar::~PlotBar()
{

}

void PlotBar::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
    m_thresholdColorMap = targetMap;
}

void PlotBar::onGetCurrentSecond(double secs)
{
    if (getPlotPairData().isEmpty())
        return;

    m_seconds = secs;
}

void PlotBar::getDataInfo(double secs)
{
    int isize = getPlotPairData().size();

    for (int i = 0; i < isize; i++) {
        QString xColumn = getPlotPairData().at(i).first;
        QString yColumn = getPlotPairData().at(i).first;
        updateData(i, xColumn, yColumn, secs);
    }
}

void PlotBar::setPlotItemExtentionDirec(bool horizontal)
{
    m_bHorizontal = horizontal;
}

void PlotBar::setLeftPadding(int leftPadding)
{
    m_leftPadding = leftPadding;
}

void PlotBar::setRightPadding(int rightPadding)
{
    m_rightPadding = rightPadding;
}

void PlotBar::setInterPadding(int interPadding)
{
    m_interPadding = interPadding;
}

void PlotBar::addPlotDataPair(const QString &entityType, const QString &entityAttr)
{
    if (entityType.isEmpty() || entityAttr.isEmpty())
        return;

    m_entityTypeList.append(entityType);
    m_entityAttrList.append(entityAttr);
}

void PlotBar::drawRect(int itemIndex, bool bHorizontal, int itemLength, int leftBoundary, int rightBoundary, QColor color)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(color);
    QBrush brush;
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);

    painter.setPen(pen);
    painter.setBrush(brush);

    int height = this->height();
    int width = this->width();

    QRect rect;
    if (bHorizontal) {
        rect.setRect(leftBoundary, height - (m_leftPadding + itemIndex * (itemLength + m_interPadding) + itemLength), rightBoundary - leftBoundary, itemLength);
    } else {
        rect.setRect(m_leftPadding + itemIndex * (itemLength + m_interPadding), height - rightBoundary, itemLength, rightBoundary - leftBoundary);
    }
    painter.drawRect(rect);
}

//ʵ�ֺ��Ļ����߼�
void PlotBar::updateItems()
{
    if (m_entityTypeList.isEmpty() || m_entityAttrList.isEmpty()) {
        qDebug() << "PlotBar::updateItems either m_entityTypeList or m_entityAttrList is empty." << endl;
        return;
    }

    if (m_entityTypeList.size() != m_entityAttrList.size()) {
        qDebug() << "PlotBar::updateItems  m_entityTypeList and m_entityAttrList don't match." << endl;
        return;
    }

    //���ȼ���ÿ��item�Ŀ��/�߶�
    int perItemLength = 0;
    int width = this->width();
    int height = this->height();
    if (m_bHorizontal) {    //itemˮƽ������չ
        perItemLength = (height - m_leftPadding - m_rightPadding - (m_entityTypeList.size() - 1) * m_interPadding) / m_entityTypeList.size();
    } else {                //item��ֱ������չ
        perItemLength = (width - m_leftPadding - m_rightPadding - (m_entityTypeList.size() - 1) * m_interPadding) / m_entityTypeList.size();
    }

    //��item����
    for (int itemIndex = 0; itemIndex < m_entityTypeList.size(); itemIndex++) {
        QString currEntityType = m_entityTypeList.at(itemIndex);
        QString currEntityAttr = m_entityAttrList.at(itemIndex);

        auto dataMap = DataManager::getInstance()->getDataMap();
        if (!dataMap.contains(currEntityType)) {
            continue;
        }
        if (!dataMap.value(currEntityType).contains(currEntityAttr)) {
            continue;
        }

        if (m_currTimeIndex >= dataMap.value(currEntityType).value(currEntityAttr).size()) {
            continue;
        }

        //*��ȡ��ǰAttrֵ
        double currValue = dataMap.value(currEntityType).value(currEntityAttr).at(m_currTimeIndex);

        QString currKey = currEntityType + '_' + currEntityAttr;
        if (!m_thresholdColorMap.contains(currKey) || m_thresholdColorMap.value(currKey).keys().isEmpty()) {
            //û��������ֵ��������ֿ����ƶ�����Σ���Ĭ����ɫ����һ������
            drawRect(itemIndex, m_bHorizontal, perItemLength, 0, currValue, m_defaultColor);
            continue;
        }

        //������ɫ��ֵ���ֿ�����
        auto colorMap = m_thresholdColorMap.value(currKey);
        QList<int> thresholdList = colorMap.keys();
        if (currValue < thresholdList.first()) {
            drawRect(itemIndex, m_bHorizontal, perItemLength, 0, currValue, m_defaultColor);
            continue;
        }

        //��Ĭ����ɫ���Ƶ�һ������
        drawRect(itemIndex, m_bHorizontal, perItemLength, 0, thresholdList.first(), m_defaultColor);

        if (thresholdList.size() == 1) {
            drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.first(), currValue, colorMap.value(thresholdList.first()));
            continue;
        }

        for (int i = 0; i < thresholdList.size() - 1; i++) {
            QColor currColor = colorMap.value(thresholdList.at(i));

            if (currValue < thresholdList.at(i + 1)) {
                drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), currValue, colorMap.value(thresholdList.at(i)));
                break;
            } else {
                drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), thresholdList.at(i + 1), colorMap.value(thresholdList.at(i)));
            }
        }

        if (currValue > thresholdList.last()) {
            drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.last(), currValue, colorMap.value(thresholdList.last()));
            continue;
        }
    }
    update();
}

void PlotBar::paintEvent(QPaintEvent *event)
{
    //����x���y��
    QPainter painter(this);
    QPen pen;
    pen.setColor(m_axisColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(QPoint(0, height()), QPoint(width(), height()));   //x��
    painter.drawLine(QPoint(0, height()), QPoint(0, 0));    //y��

    //��������
    pen.setColor(m_gridColor);
    painter.setPen(pen);

    QBrush brush;   //��ˢ����伸��ͼ�εĵ�ɫ�壬����ɫ����������
    brush.setColor(m_gridFillColor);
    brush.setStyle(Qt::SolidPattern);
    //painter.setBrush(brush);

    int horiGridWidth = 0;
    if (m_horiGridNum) {    //itemˮƽ������չ
        horiGridWidth = width() / m_horiGridNum;
    }

    for (int i = 0; i < m_horiGridNum; i++) {
        QRect gridRect;
        gridRect.setRect(i * horiGridWidth, 0, horiGridWidth, height());
        painter.drawRect(gridRect);
    }

    int verGridWidth = 0;
    if (m_verGridNum) {     //itemˮƽ������չ
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

void PlotBar::updateData(int itemIndex, QString x, QString y, double secs)
{
    QStringList xlist = x.split("+");
    QStringList ylist = y.split("+");
    QList<double> xSecList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs);
    QList<double> ySecList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs);
    int cnt = m_entityTypeList.size();

    if (xSecList.isEmpty())
        return;

    //���ȼ���ÿ��item�Ŀ��/�߶�
    int perItemLength = 0;
    int width = this->width();
    int height = this->height();
    if (m_bHorizontal) {    //itemˮƽ������չ
        perItemLength = (height - m_leftPadding - m_rightPadding - (m_entityTypeList.size() - 1) * m_interPadding) / m_entityTypeList.size();
    } else {                //item��ֱ������չ
        perItemLength = (width - m_leftPadding - m_rightPadding - (m_entityTypeList.size() - 1) * m_interPadding) / m_entityTypeList.size();
    }

    //*��ȡ��ǰAttrֵ
    double currValue = xSecList.last();

    QString currKey = xlist.at(0) + '_' + xlist.at(1);
    if (!m_thresholdColorMap.contains(currKey) || m_thresholdColorMap.value(currKey).keys().isEmpty()) {
        //û��������ֵ��������ֿ����ƶ�����Σ���Ĭ����ɫ����һ������
        drawRect(itemIndex, m_bHorizontal, perItemLength, 0, currValue, m_defaultColor);
        update();
        return;
    }

    //������ɫ��ֵ���ֿ�����
    auto colorMap = m_thresholdColorMap.value(currKey);
    QList<int> thresholdList = colorMap.keys();
    if (currValue < thresholdList.first()) {
        drawRect(itemIndex, m_bHorizontal, perItemLength, 0, currValue, m_defaultColor);
    }

    //��Ĭ����ɫ���Ƶ�һ������
    drawRect(itemIndex, m_bHorizontal, perItemLength, 0, thresholdList.first(), m_defaultColor);

    if (thresholdList.size() == 1) {
        drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.first(), currValue, colorMap.value(thresholdList.first()));
    }

    for (int i = 0; i < thresholdList.size() - 1; i++) {
        QColor currColor = colorMap.value(thresholdList.at(i));

        if (currValue < thresholdList.at(i + 1)) {
            drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), currValue, colorMap.value(thresholdList.at(i)));
            break;
        } else {
            drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), thresholdList.at(i + 1), colorMap.value(thresholdList.at(i)));
        }
    }

    if (currValue > thresholdList.last()) {
        drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.last(), currValue, colorMap.value(thresholdList.last()));
    }
    update();
}
