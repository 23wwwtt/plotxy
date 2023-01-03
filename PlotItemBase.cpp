/* * @filename: PlotItemBase.cpp
* @brief: PlotBase Plot»ùÀà
* @author: zhameng
* @create time: 2022-09-21
*  */

#include "PlotItemBase.h"


PlotItemBase::PlotItemBase(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    m_position = QPoint(0,0);
    m_width = 0;
    m_height = 0;
    m_bVisible = true; 
	m_coordBgn_x = 0;
	m_coordEnd_x = 100;
	m_coordBgn_y = 0;
	m_coordEnd_y = 100;

	m_horzGrids = 4;
	m_vertGrids = 4;
	m_axisWidth = 1;
	m_gridWidth = 1;
	m_axisColor = Qt::white;
	m_gridColor = QColor(200, 200, 200);


	m_units_x = nullptr;
	m_units_y = nullptr;
	m_showUnits_x = true;
	m_showUnits_y = true;
	m_title = nullptr;
	m_titleVisible = true;
	m_titleColor = Qt::white;
	m_titleFillColor = Qt::black;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);
}
   
PlotItemBase::~PlotItemBase()
{

}


void PlotItemBase::init()
{

}

void PlotItemBase::setPosition(int x, int y)
{
    m_position.setX(x);
    m_position.setY(y);
	this->setGeometry(x, y, currWidth(), currHeight());
	update();
}

void PlotItemBase::setWidth(int width)
{
    m_width = width;
	update();
}

void PlotItemBase::setHeight(int height)
{
    m_height = height;
	update();
}

void PlotItemBase::setRect(QRect rect)
{
	m_position.setX(rect.x());
	m_position.setY(rect.y());
	m_width = rect.width();
	m_height = rect.height();
	this->setGeometry(rect);
	update();
}

QPoint PlotItemBase::currPosition()
{
	m_position = this->pos();
    return m_position;
}

int PlotItemBase::currWidth()
{
	m_width = this->width();
    return m_width;
}

int PlotItemBase::currHeight()
{
	m_height = this->height();
    return m_height;
}

QRect PlotItemBase::currRect()
{
	return this->rect();
}

void PlotItemBase::setName(const QString& name)
{
    m_plotItemName = name;
}

void PlotItemBase::setTabName(const QString& tabName)
{
    m_tabName = tabName;
}


QString PlotItemBase::currName()
{
    return m_plotItemName;
}

QString PlotItemBase::currTabName()
{
    return m_tabName;
}

void PlotItemBase::setCoordRangeX(double lower, double upper)
{
	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
}

void PlotItemBase::setCoordRangeY(double lower, double upper)
{
	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
}

void PlotItemBase::getCoordRangeX(double & lower, double & upper)
{
	lower = m_coordBgn_x;
	upper = m_coordEnd_x;
}

void PlotItemBase::getCoordRangeY(double & lower, double & upper)
{
	lower = m_coordBgn_y;
	upper = m_coordEnd_y;
}

void PlotItemBase::setHorzGrids(uint count)
{
	m_horzGrids = count;
}

void PlotItemBase::setVertGrids(uint count)
{
	m_vertGrids = count;
}

void PlotItemBase::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
}

void PlotItemBase::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
}

void PlotItemBase::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
}

void PlotItemBase::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
}

void PlotItemBase::setUnitsX(const QString & units)
{
	m_units_x = units;
}

void PlotItemBase::setUnitsY(const QString & units)
{
	m_units_y = units;
}

void PlotItemBase::setTitleVisible(bool on)
{
	m_titleVisible = on;
}

void PlotItemBase::setTitle(QString & title)
{
	m_title = title;
}

void PlotItemBase::setTitleColor(QColor& color)
{
	m_titleColor = color;
}

void PlotItemBase::setTitleFillColor(QColor& color)
{
	m_titleFillColor = color;
}

void PlotItemBase::setTitleFont(QFont& font)
{
	m_titleFont = font;
}

void PlotItemBase::addPlotPairData(QPair<QString, QString> pair)
{
	m_plotPairData.append(pair);
	DataPair* data = new DataPair(pair);
	m_dataPair.append(data);
}

void PlotItemBase::delPlotPairData(QPair<QString, QString> pair)
{
	if (m_plotPairData.isEmpty())
		return;

	for (auto &i : m_plotPairData)
	{
		if (QPair<QString, QString>(i) == pair)
		{
			m_plotPairData.removeOne(i);
			break;
		}
	}

	for (int i = 0; i < m_dataPair.size(); ++i)
	{
		if (m_dataPair.at(i)->getDataPair() == pair)
		{
			m_dataPair.remove(i);
			break;
		}
	}
}

void PlotItemBase::updatePlotPairData(QPair<QString, QString> oldPair, QPair<QString, QString> newPair)
{
	if (m_plotPairData.isEmpty())
		return;

	for (int i = 0; i < m_plotPairData.size(); ++i)
	{
		if (m_plotPairData.at(i) == oldPair)
		{
			m_plotPairData.replace(i, newPair);
			break;
		}
	}

	for (int i = 0; i < m_dataPair.size(); ++i)
	{
		if (m_dataPair.at(i)->getDataPair() == oldPair)
		{
			m_dataPair.at(i)->setDataPair(newPair);
			break;
		}
	}
}

QList<QPair<QString, QString>> PlotItemBase::getPlotPairData()
{
	return m_plotPairData;
}

void PlotItemBase::slot_setVisible(bool on)
{
	m_bVisible = on;
	this->setVisible(on);
}

void PlotItemBase::slot_updateRect(QRect rect)
{
	setRect(rect);
}

//void PlotItemBase::setVisible(bool bVisible)
//{
//    m_bVisible = bVisible;
//}
// 
//bool PlotItemBase::bVisible()
//{
//    return m_bVisible;
//}