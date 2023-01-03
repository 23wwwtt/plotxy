#include "PlotText.h"
#include "DataManager.h"
#include "AddPlotPair.h"
#include "PlotItemBase.h"
#include "PlotManager.h"
#include "colorbutton.h"
#include <QPainter>
#include <QStringList>
#include <QPen>
#include <QFont>
#include <QList>
#include <QDebug>
#include <QFont>


int PlotText::m_instanceCount = 1;
PlotText::PlotText(QWidget* parent)
	:PlotItemBase(parent)
{
	m_bHorizontal = true;

	m_leftPadding = 50;
	m_rightPadding = 50;
	m_interPadding = 20;
	m_horiGridNum = m_verGridNum = 1;
	m_currTimeIndex = 0;

	m_started = false;

	QString name = QString("Text%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_defaultColor = Qt::gray;
	//connect(m_timer, &QTimer::timeout, this, &PlotText::onTimeout);
}

PlotText::~PlotText()
{

}

void PlotText::paintEvent(QPaintEvent* event)
{
	//以下绘制标题“Text Plot”
	QPainter painter(this);
	QPen pen;
	QFont font;
	QRect rect;
	QList<QPair<QString, QString>> dataList;
	QSet<QString> xset, yset;
	int i = 0, j = 0;
	int horGT = 0;
	int verGT = 0;
	pen.setColor(Qt::white);
	painter.setPen(pen);
	font.setPointSize(20);
	painter.setFont(font);
	QFontMetricsF fm(font);
	double as = fm.ascent();
	rect.setRect(0, 0, width(), as);
	int horiGridWidth = 0;
	if (m_horiGridNum)		//item水平方向延展
	{
		horiGridWidth = 0.9*width() / m_horiGridNum;
	}
	int verGridWidth = 0;
	if (m_verGridNum)		//item水平方向延展
	{
		verGridWidth = (0.93*height()-as) / m_verGridNum;
	}
	//以下为绘制表格title名字
	setTitle(painter, rect);
	//以下绘制n×m的格子

	pen.setColor(m_gridColor);
	pen.setWidth(m_gridWidth);
	painter.setPen(pen);
	drawNMCell(painter, xset, yset, dataList, horiGridWidth, verGridWidth,as);
	//以下为绘制X/Y轴item名字
	pen.setColor(Qt::white);
	pen.setWidth(3);
	painter.setPen(pen);
	drawXYTitle(painter, horiGridWidth, verGridWidth, dataList,as);
	//以下为绘制对应的数据
	if (m_temValueList.isEmpty())
		return;
	else
	{
		painter.drawText(0.05*width(), as + 0.02*height(), horiGridWidth, verGridWidth, Qt::AlignCenter | Qt::TextWrapAnywhere, QString::fromLocal8Bit("0表示0或无数据"));
		for (int i = m_entityName.size() - 1; i != -1; i--)
		{
			for (int j = m_attriName.size() - 1; j != -1; j--)
			{
				rect.setRect(0.05*width() + (1 + horGT)*horiGridWidth, as+0.02*height() + (1 + verGT)*verGridWidth, horiGridWidth, verGridWidth);
				//painter.drawText(rect, QString::number(*(m_valueListVector.at(j).end()),'f',2));
				painter.drawText(rect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString::number(m_temValueList.at(m_temValueList.size() - 1 - j - i*m_attriName.size()).back(), 'f', 6));
				update();
				verGT++;
			}
			horGT++;
			verGT = 0;
		}
	}
	//drawData(xset, yset, horiGridWidth, verGridWidth);
}

//以下为用户自定义数据
//QList<textUserData> list1;
//QRect rect, rectErase;
//list1 = addPP->getUserText();
//for (int i = 0; i < list1.size(); i++)
//{
//	int x = list1.at(i).row;
//	int y = list1.at(i).column;
//	if (x <= m_verGridNum || y <= m_horiGridNum)
//	{
//		rect.setRect(y*horiGridWidth, x*verGridWidth, horiGridWidth, verGridWidth);
//		rectErase.setRect(y*horiGridWidth + 3, x*verGridWidth + 3, horiGridWidth - 3, verGridWidth - 3);
//		painter.eraseRect(rectErase);
//		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, list1.at(i).str);
//	}
//	else
//	{
//		m_verGridNum = x;
//		m_horiGridNum = y;
//		rect.setRect(y*horiGridWidth, x*verGridWidth, horiGridWidth, verGridWidth);
//		painter.eraseRect(rect);
//		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, list1.at(i).str);
//	}
//}


void PlotText::setTitle(QPainter& painter, QRect& rect)
{
	painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "Text Plot");
}

void PlotText::drawData(QSet<QString>& xset, QSet<QString>& yset, int& horiGridWidth, int& verGridWidth)
{
	if (getPlotPairData().isEmpty())
	{
		return;
	}
	if (m_temValueList.isEmpty())
	{
		return;
	}
	int i = 0;
	int j = 0;
	QRect rect;
	QPainter painter;
	for (auto it = xset.begin(); it != xset.end(); it++)
	{
		for (auto it2 = yset.begin(); it2 != yset.end(); it2++)
		{
			j++;
			//动态数据
		/*	QString currEntityType = *it;
			QString currEntityAttr = *it2;*/
			//auto dataMap = DataManager::getInstance()->getDataMap();
			//if (!dataMap.contains(currEntityType))
			//{
			//	continue;
			//}
			//if (!dataMap.value(currEntityType).contains(currEntityAttr))
			//{
			//	continue;
			//}
			//if (m_currTimeIndex >= dataMap.value(currEntityType).value(currEntityAttr).size())
			//{
			//	continue;
			//}
			//*获取当前Attr值

			rect.setRect(0.05*width() + (1 + i)*horiGridWidth, 0.1*height() + j*verGridWidth, horiGridWidth, verGridWidth);
			painter.drawText(rect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString::number((m_temValueList.at(j).back()), 'f', 2));
		}
		i++;
		j = 0;
	}
}



void PlotText::slot_getCurrentSeconds(double secs)
{
	if (getPlotPairData().isEmpty())
		return;
	int isize = getPlotPairData().size();
	int entityNum = 0;
	int attriNum = 0;
	//m_entityName.clear();
	//m_attriName.clear();
	for (int i = 0; i < isize; i++)
	{
		QString getTextData = getPlotPairData().at(i).first;
		QList<QString> textValueList = getTextData.split("+");
		if (m_entityName.isEmpty())
			m_entityName.push_back(textValueList.front());
		for (int i = 0; i < m_entityName.size(); i++)
		{
			if (textValueList.front() == m_entityName.at(i))
				entityNum++;
		}
		if (entityNum == 0)
			m_entityName.push_back(textValueList.front());
		entityNum = 0;

		if (m_attriName.isEmpty())
			m_attriName.push_back(textValueList.back());
		for (int i = 0; i < m_attriName.size(); i++)
		{
			if (textValueList.back() == m_attriName.at(i))
				attriNum++;
		}
		if (attriNum == 0)
			m_attriName.push_back(textValueList.back());
		attriNum = 0;
	}
	for (auto ite = m_entityName.begin(); ite != m_entityName.end(); ite++)
	{
		for (auto ita = m_attriName.begin(); ita != m_attriName.end(); ita++)
		{
			m_valueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(*ite, *ita, secs);
			if (m_valueList.isEmpty())
				m_valueList.push_back(0);
			m_temValueList.push_back(m_valueList);

		}
	}
	update();
}

void PlotText::drawXYTitle(QPainter& painter, int& horiGridWidth, int& verGridWidth, QList<QPair<QString, QString>>& dataList,double &as)
{
	QRect rectXName, rectYName;
	int i = 0, j = 0;
	dataList = getPlotPairData();
	int icount = 0;
	for (int i = 0; i < dataList.size(); i++)
	{
		QString xIncludePlus = dataList.at(i).first;
		int pos = xIncludePlus.indexOf("+");
		QString xColumn = xIncludePlus.mid(0, pos);
		QString yColumn = xIncludePlus.mid(pos + 1);
		if (m_xColumnList.isEmpty())
			m_xColumnList.push_back(xColumn);
		else
		{
			for (int j = 0; j < m_xColumnList.size(); j++)
			{
				if (QString::compare(xColumn, m_xColumnList.at(j)) == 0)
					icount++;
			}
			if (icount == 0)
				m_xColumnList.push_back(xColumn);
			icount = 0;
		}
		if (m_yColumnList.isEmpty())
			m_yColumnList.push_back(yColumn);
		else
		{
			for (int j = 0; j < m_yColumnList.size(); j++)
			{
				if (QString::compare(yColumn, m_yColumnList.at(j)) == 0)
					icount++;
			}
			if (icount == 0)
				m_yColumnList.push_back(yColumn);
			icount = 0;
		}
	}
	for (int i = 0; i < m_xColumnList.size(); i++)
	{
		rectXName.setRect(0.05*width() + (i + 1)* horiGridWidth, as+0.02*height(), horiGridWidth, verGridWidth);
		painter.drawText(rectXName, Qt::AlignCenter | Qt::TextWordWrap, m_xColumnList.at(i));
	}
	for (int i = 0; i < m_yColumnList.size(); i++)
	{
		rectXName.setRect(0.05*width(), as + 0.02*height() + (1 + i)*verGridWidth, horiGridWidth, verGridWidth);
		painter.drawText(rectXName, Qt::AlignCenter | Qt::TextWordWrap, m_yColumnList.at(i));
	}
	//for (auto it = m_entityName.begin(); it != m_entityName.end(); it++)
	//{
	//	rectXName.setRect(0.05*width() + (i + 1)* horiGridWidth, 0.1*height(), horiGridWidth, verGridWidth);
	//	painter.drawText(rectXName, Qt::AlignCenter | Qt::TextWordWrap, *it);
	//	i++;
	//}
	//for (auto it = m_attriName.begin(); it != m_attriName.end(); it++)
	//{
	//	rectXName.setRect(0.05*width(), 0.1*height() + (1 + j)*verGridWidth, horiGridWidth, verGridWidth);
	//	painter.drawText(rectXName, Qt::AlignCenter | Qt::TextWordWrap, *it);
	//	j++;
	//}
	update();
}

void PlotText::drawNMCell(QPainter& painter, QSet<QString>& xset, QSet<QString>& yset, QList<QPair<QString, QString>> dataList,
	int& horiGridWidth, int& verGridWidth,double &as)
{
	dataList = getPlotPairData();
	for (int i = 0; i < dataList.size(); i++)
	{
		QString xIncludePlus = dataList.at(i).first;
		int pos = xIncludePlus.indexOf("+");
		QString xColumn = xIncludePlus.mid(0, pos);
		QString yColumn = xIncludePlus.mid(pos + 1);
		xset.insert(xColumn);
		yset.insert(yColumn);
		m_horiGridNum = xset.size() + 1;
		m_verGridNum = yset.size() + 1;
	}
	if (!dataList.empty())
	{
		for (int i = 0; i < m_horiGridNum; i++)
		{
			QRect gridRect;
			//gridRect.setRect(  0.05*width()+i* horiGridWidth, 0.1*height(), horiGridWidth, 0.85*height());
			//painter.drawRect(gridRect);
			painter.drawLine(0.05*width() + i* horiGridWidth, 0.02*height() + as, 0.05*width() + i* horiGridWidth, 0.02*height() + as+verGridWidth*m_verGridNum);
		}
		for (int i = 0; i < m_verGridNum; i++)
		{
			QRect gridRect;
			gridRect.setRect(0.05*width(), i * verGridWidth + 0.02*height()+as, 0.9*width(), verGridWidth);
			painter.drawRect(gridRect);
		}
	}
	update();
}

void PlotText::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
}

void PlotText::dataPairOrder()
{
	//flag为ture，则对x轴数据进行调整
	if (m_flag)
	{
		if (m_xColumnList.isEmpty())
			return;
		else
		{

		}
	}
}