#include "PlotText.h"
#include "DataManager.h"
#include <QPainter>
#include <QPen>
#include <QDebug>


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
	pen.setColor(m_axisColor);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.drawLine(QPoint(0, height()), QPoint(width(), height()));	//x��
	painter.drawLine(QPoint(0, height()), QPoint(0, 0));		//y��

																//��������
	pen.setColor(m_gridColor);
	painter.setPen(pen);

	QBrush brush;   //��ˢ����伸��ͼ�εĵ�ɫ�壬����ɫ����������
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

	if (m_started)
		updateItems();
}

void PlotText::drawRect(int itemIndex, bool bHorizontal, int itemLength, int leftBoundary, int rightBoundary, QColor color)
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

void PlotText::updateItems()
{
	if (m_entityTypeList.isEmpty() || m_entityAttrList.isEmpty())
	{
		qDebug() << "PlotBar::updateItems either m_entityTypeList or m_entityAttrList is empty." << endl;
		return;
	}

	if (m_entityTypeList.size() != m_entityAttrList.size())
	{
		qDebug() << "PlotBar::updateItems  m_entityTypeList and m_entityAttrList don't match." << endl;
		return;
	}

	//���ȼ���ÿ��item�Ŀ��/�߶�
	int perItemLength = 0;
	int width = this->width();
	int height = this->height();
	if (m_bHorizontal)		//itemˮƽ������չ
	{
		perItemLength = (height - m_leftPadding - m_rightPadding - (m_entityTypeList.size() - 1) * m_interPadding) / m_entityTypeList.size();
	}
	else					//item��ֱ������չ
	{
		perItemLength = (width - m_leftPadding - m_rightPadding - (m_entityTypeList.size() - 1) * m_interPadding) / m_entityTypeList.size();
	}

	//��item����
	for (int itemIndex = 0; itemIndex < m_entityTypeList.size(); itemIndex++)
	{
		QString currEntityType = m_entityTypeList.at(itemIndex);
		QString currEntityAttr = m_entityAttrList.at(itemIndex);

		auto dataMap = DataManager::getInstance()->getDataMap();
		if (!dataMap.contains(currEntityType))
		{
			continue;
		}
		if (!dataMap.value(currEntityType).contains(currEntityAttr))
		{
			continue;
		}

		if (m_currTimeIndex >= dataMap.value(currEntityType).value(currEntityAttr).size())
		{
			continue;
		}

		//*��ȡ��ǰAttrֵ
		double currValue = dataMap.value(currEntityType).value(currEntityAttr).at(m_currTimeIndex);

		QString currKey = currEntityType + '_' + currEntityAttr;
		if (!m_thresholdColorMap.contains(currKey) || m_thresholdColorMap.value(currKey).keys().isEmpty())
		{
			//û��������ֵ��������ֿ����ƶ�����Σ���Ĭ����ɫ����һ������
			drawRect(itemIndex, m_bHorizontal, perItemLength, 0, currValue, m_defaultColor);
			continue;
		}

		//������ɫ��ֵ���ֿ�����
		auto colorMap = m_thresholdColorMap.value(currKey);
		QList<int> thresholdList = colorMap.keys();
		if (currValue < thresholdList.first())
		{
			drawRect(itemIndex, m_bHorizontal, perItemLength, 0, currValue, m_defaultColor);
			continue;
		}

		//��Ĭ����ɫ���Ƶ�һ������
		drawRect(itemIndex, m_bHorizontal, perItemLength, 0, thresholdList.first(), m_defaultColor);

		if (thresholdList.size() == 1)
		{
			drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.first(), currValue, colorMap.value(thresholdList.first()));
			continue;
		}

		for (int i = 0; i < thresholdList.size() - 1; i++)
		{
			QColor currColor = colorMap.value(thresholdList.at(i));

			if (currValue < thresholdList.at(i + 1))
			{
				drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), currValue, colorMap.value(thresholdList.at(i)));
				break;
			}
			else
			{
				drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), thresholdList.at(i + 1), colorMap.value(thresholdList.at(i)));
			}
		}

		if (currValue > thresholdList.last())
		{
			drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.last(), currValue, colorMap.value(thresholdList.last()));
			continue;
		}
	}
	update();
}