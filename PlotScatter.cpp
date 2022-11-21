#include "PlotScatter.h"
#include "DataManager.h"

#include <QDebug>
#include <QPainter>
#include <QTime>

int PlotScatter::m_instanceCount = 1;

PlotScatter::PlotScatter(QWidget *parent)
    : PlotItemBase(parent)
    , m_contextPadding(50)
    , m_horTableCnt(4)
    , m_verTableCnt(4)
{
    QString name = QString("Scatter%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for (int i = 0; i < 20; i++) {
        m_clrList << QColor::fromRgb(qrand() % 255, qrand() % 255, qrand() % 255);
    }
}

PlotScatter::~PlotScatter()
{

}

void PlotScatter::getDataInfo(double secs)
{
    m_itemCnt = getPlotPairData().size();

    for (int i = 0; i < m_itemCnt; i++) {
        QString xcolumn = getPlotPairData().at(i).first;
        QString ycolumn = getPlotPairData().at(i).second;
        updateData(xcolumn, ycolumn, secs, m_clrList.at(i % 20));
    }
}

void PlotScatter::updateData(QString xEntityType, QString yEntityType, double secs, QColor color)
{
    QStringList xlist = xEntityType.split("+");
    QStringList ylist = yEntityType.split("+");
    QList<double> m_xValueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs);
    QList<double> m_yValueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs);

    if (m_xValueList.isEmpty() || m_yValueList.isEmpty())
        return;

    int xCnt = m_xValueList.size();
    int yCnt = m_yValueList.size();
    double yMax = *std::max_element(m_yValueList.begin(), m_yValueList.end());
    double xMax = *std::max_element(m_xValueList.begin(), m_xValueList.end());

    // 临时设置最大值为2000，待拿到真实数据再确定这个最大值的范围
    if (xMax <= 2000) {
        xMax = 2000;
    }
    if (yMax <= 2000) {
        yMax = 2000;
    }

    if (xCnt >= yCnt) {
        // 横坐标数据较多，多余的纵坐标数据显示为0
        for (int i = 0; i < yCnt; i++) {
            drawTargetPoint(m_xValueList.at(i) / xMax, m_yValueList.at(i) / yMax, color);
        }
        for (int i = yCnt; i < xCnt; i++) {
            drawTargetPoint(m_xValueList.at(i) / xMax, 0, color);
        }
    } else {
        // 纵坐标数据较多，多余的横坐标数据显示为0
        for (int i = 0; i < xCnt; i++) {
            drawTargetPoint(m_xValueList.at(i) / xMax, m_yValueList.at(i) / yMax, color);
        }
        for (int i = xCnt; i < yCnt; i++) {
            drawTargetPoint(0, m_yValueList.at(i) / yMax, color);
        }
    }
    update();
}

void PlotScatter::drawTargetPoint(double x, double y, QColor color)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(color);
    painter.setPen(pen);
    painter.setBrush(QBrush(color));

    painter.drawEllipse(QPointF(m_contextPadding + x * (width() - 2 * m_contextPadding),
                                height() - m_contextPadding - y * (height() - 2 * m_contextPadding)),
                        3, 3);
}

void PlotScatter::onGetCurrentSeconds(double secs)
{
    m_curSeconds = secs;
    update();
}

void PlotScatter::paintEvent(QPaintEvent *event)
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
    double verTablePadding = (height() - 2 * m_contextPadding) / m_verTableCnt;
    double horTablePadding = (width() - 2 * m_contextPadding) / m_horTableCnt;
    for (int i = 0; i < m_verTableCnt; i++) {
        painter.drawLine(QPointF(m_contextPadding, m_contextPadding + verTablePadding * i),
                         QPointF(width() - m_contextPadding, m_contextPadding + verTablePadding * i));
    }
    for (int i = 0; i < m_horTableCnt; i++) {
        painter.drawLine(QPointF(m_contextPadding + horTablePadding * (i + 1), m_contextPadding),
                         QPointF(m_contextPadding + horTablePadding * (i + 1), height() - m_contextPadding));
    }

    getDataInfo(m_curSeconds);

    return PlotItemBase::paintEvent(event);
}
