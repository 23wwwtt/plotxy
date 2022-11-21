#include "PlotDial.h"
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <QVector2D>
#include "DataManager.h"

int PlotDial::m_instanceCount = 1;

const double PI = 3.141592653589793;

PlotDial::PlotDial(QWidget *parent)
    : PlotItemBase(parent)
    , m_contentPadding(20)
{
    m_bThinStyle = true;

    m_dialColor = Qt::white;
    m_pointColor = Qt::green;

    QString name = QString("Dial%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;
}

PlotDial::~PlotDial()
{

}

//更新表盘指针
void PlotDial::updatePointer(double secs)
{
    if (getPlotPairData().isEmpty())
        return;

    int isize = getPlotPairData().size();
    QString xcolumn = getPlotPairData().at(isize - 1).first;
    QStringList list = xcolumn.split("+");
    QList<double> m_valueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(list.at(0), list.at(1), secs);

    if (m_valueList.isEmpty())
        return;

    double currValue = m_valueList.last();

    //根据当前值计算指针终点
    double angle = (int)currValue % 360;

    QPoint endPoint;
    endPoint.setX(m_circleRadius * qCos(angle * PI / 180.0) + m_centerPoint.x());
    endPoint.setY(-m_circleRadius * sin(angle * PI / 180.0) + m_centerPoint.y());

    QVector2D vec(endPoint.x() - m_centerPoint.x(),
                  endPoint.y() - m_centerPoint.y());
    QPoint midPoint;
    midPoint.setX(m_centerPoint.x() + vec.x() * 0.1);
    midPoint.setY(m_centerPoint.y() + vec.y() * 0.1);

    QVector2D normalVec(-0.1 * ((double)endPoint.y() - (double)m_centerPoint.y()),
                        0.1 * ((double)endPoint.x() - (double)m_centerPoint.x()));

    QPoint endPoint1(midPoint.x() + normalVec.x(),
                     midPoint.y() + normalVec.y());
    QPoint endPoint2(midPoint.x() - normalVec.x(),
                     midPoint.y() - normalVec.y());

    QPoint points[4] = {
        m_centerPoint,
        endPoint1,
        endPoint,
        endPoint2
    };

    QPainter painter(this);
    QBrush pointerBrush(m_pointColor, Qt::SolidPattern);
    painter.setBrush(pointerBrush);
    painter.drawPolygon(points, 4);
}

void PlotDial::onGetCurrentSeconds(double secs)
{
    m_seconds = secs;
    update();
}

void PlotDial::paintEvent(QPaintEvent *event)
{
    //绘制表盘圆圈
    QPainter painter(this);
    QPen pen;
    pen.setColor(m_dialColor);
    pen.setWidth(2);
    painter.setPen(pen);
    m_circleRadius = width() < height() ? width() / 2 - m_contentPadding : height() / 2 - m_contentPadding;
    m_centerPoint = QPoint(width() / 2, height() / 2);
    painter.drawEllipse(m_centerPoint, m_circleRadius, m_circleRadius);

    // 绘制表盘圆弧
    double arcHeight = m_circleRadius / 20;
    painter.translate(m_centerPoint);
    QRectF rect(-m_circleRadius, -m_circleRadius, m_circleRadius << 1, m_circleRadius << 1);
    QPainterPath pathGreen;
    pathGreen.arcTo(rect, 30, 180);
    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));
    pathGreen -= subPath;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 128, 0));
    painter.drawPath(pathGreen);

    QPainterPath pathYellow;
    pathYellow.arcTo(rect, 30, -45);
    pathYellow -= subPath;
    painter.setBrush(QColor(128, 128, 0));
    painter.drawPath(pathYellow);

    QPainterPath pathRed;
    pathRed.arcTo(rect, -15, -75);
    pathRed -= subPath;
    painter.setBrush(QColor(128, 0, 0));
    painter.drawPath(pathRed);

    // 绘制刻度
    pen.setWidth(2);
    pen.setColor(m_dialColor);
    painter.setPen(pen);
    painter.drawLine(QPointF(-m_circleRadius, 0), QPointF(-m_circleRadius * 0.95, 0));
    painter.drawLine(QPointF(m_circleRadius, 0), QPointF(m_circleRadius * 0.95, 0));
    painter.drawLine(QPointF(0, -m_circleRadius * 0.95), QPointF(0, -m_circleRadius));
    painter.drawLine(QPointF(0, m_circleRadius * 0.95), QPointF(0, m_circleRadius));

    // 绘制表盘文字
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(14);
    painter.drawText(-m_circleRadius * 0.9, 0, QString("-50°"));
    painter.drawText(0, -m_circleRadius * 0.9, QString("0°"));
    painter.drawText(m_circleRadius * 0.9, 0, QString("50°"));
    painter.drawText(0, m_circleRadius * 0.9, QString("100°"));

    updatePointer(m_seconds);

    return PlotItemBase::paintEvent(event);
}
