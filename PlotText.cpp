#include "PlotText.h"
#include "DataManager.h"
#include <QPainter>
#include <QPen>


int PlotText::m_instanceCount = 1;
PlotText::PlotText(QWidget* parent)
	:PlotItemBase(parent)
{
	m_bHorizontal = true;

	m_leftPadding = 50;
	m_rightPadding = 50;
	m_interPadding = 20;

	m_currTimeIndex = 0;

	m_horiGridNum = 4;
	m_verGridNum = 5;

	m_started = false;

	QString name = QString("Text%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_defaultColor = Qt::gray;
	m_timer = new QTimer(this);
	//connect(m_timer, &QTimer::timeout, this, &PlotText::onTimeout);
}

PlotText::~PlotText()
{

}

void PlotText::onSwitch(bool bOn)
{
	if (!m_started)
	{
		m_timer->start(500);
		m_started = true;
	}
	else
	{
		m_timer->stop();
		m_started = false;
	}
}
void PlotText::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>targetMap)
{
	m_thresholdColorMap = targetMap;
}
void PlotText::paintEvent(QPaintEvent* event)
{
	//����x���y��
	QPainter painter(this);
	QPen pen;
	m_axisColor = Qt::white;
	pen.setColor(m_axisColor);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.drawLine(QPoint(0, height()), QPoint(width(), height()));	//x��
	painter.drawLine(QPoint(0, height()), QPoint(0, 0));		//y��

																//��������
	m_gridColor = Qt::white;
	pen.setColor(m_gridColor);
	painter.setPen(pen);

	QBrush brush;   //��ˢ����伸��ͼ�εĵ�ɫ�壬����ɫ����������
	m_gridFillColor = Qt::white;
	brush.setColor(m_gridFillColor);
	brush.setStyle(Qt::SolidPattern);
	//painter.setBrush(brush);

	int horiGridWidth = 0;
	if (m_horiGridNum)		//itemˮƽ������չ
	{
		horiGridWidth = width() / m_horiGridNum;
	}

	for (int i = 0; i < m_horiGridNum; i++)
	{
		QRect gridRect;
		gridRect.setRect(i* horiGridWidth, 0, horiGridWidth, height());
		painter.drawRect(gridRect);
	}

	int verGridWidth = 0;
	if (m_verGridNum)		//itemˮƽ������չ
	{
		verGridWidth = height() / m_verGridNum;
	}

	for (int i = 0; i < m_verGridNum; i++)
	{
		QRect gridRect;
		gridRect.setRect(0, (i + 1) * verGridWidth, width(), verGridWidth);
		painter.drawRect(gridRect);
	}
}

void PlotText::drawRect(int itemIndex, bool bHorizontal, int itemLength, int leftBoundary, int rightBoundary, QColor color)
{
	QPainter painter(this);
	QPen pen;
	color = Qt::white;
	pen.setColor(color);
	QBrush brush;
	brush.setColor(color);
	brush.setStyle(Qt::SolidPattern);

	painter.setPen(pen);
	painter.setBrush(brush);

	int height = this->height();
	int width = this->width();

	QRect rect;
	if (bHorizontal)
	{
		rect.setRect(leftBoundary, height - (m_leftPadding + itemIndex * (itemLength + m_interPadding) + itemLength), rightBoundary - leftBoundary, itemLength);
	}
	else
	{
		rect.setRect(m_leftPadding + itemIndex * (itemLength + m_interPadding), height - rightBoundary, itemLength, rightBoundary - leftBoundary);
	}
	painter.drawRect(rect);
}
