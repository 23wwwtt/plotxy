#include "PlotScatter.h"
#include "DataManager.h"

#include <QDebug>
#include <QPainter>
#include <QTime>

int PlotScatter::m_instanceCount = 1;

PlotScatter::PlotScatter(QWidget *parent)
    : PlotItemBase(parent)
{
    QString name = QString("Scatter%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for (int i = 0; i < 20; i++) {
        m_clrList << QColor::fromRgb(qrand() % 255, qrand() % 255, qrand() % 255);
    }
	m_backgroundBrush = QBrush(QColor(0,0,0));
	m_title = "Scatter Plot";
	m_titleColor = Qt::white;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);
	m_titleShow = true;

	m_axisLabelColor = Qt::white;
	m_axisFont.setFamily("Microsoft YaHei");
	m_axisFont.setPointSizeF(10.0);
	m_xAxisLabel = "X Axis";
	m_yAxisLabel = "Y Axis";

	m_leftPadding = 10;
	m_rightPadding = 10;
	m_topPadding = 10;
	m_bottomPadding = 20;

	m_coordBgn_x = 0;
	m_coordEnd_x = 2000;
	m_coordBgn_y = 0;
	m_coordEnd_y = 2000;

	m_horzGrids = 5;
	m_vertGrids = 5;
	m_axisWidth = 1;
	m_gridWidth = 1;
	m_axisColor = Qt::white;
	m_gridColor = QColor(200, 200, 200);

	initPlot();
}

PlotScatter::~PlotScatter()
{
	
}

void PlotScatter::initPlot()
{
	m_customPlot = new QCustomPlot(this);
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
 	m_customPlot->axisRect()->setupFullAxesBox(true);

	m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
	m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
	m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
	m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
	m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);

//  	m_customPlot->xAxis->setNumberPrecision(3);
//  	m_customPlot->yAxis->setNumberPrecision(3);

	m_customPlot->setBackground(m_backgroundBrush);
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->xAxis->setLabelFont(m_axisFont);
	m_customPlot->yAxis->setLabelFont(m_axisFont);
	m_customPlot->xAxis->setTickLabelColor(QColor(255, 255, 255));
	m_customPlot->yAxis->setTickLabelColor(QColor(255, 255, 255));

	m_customPlot->replot();
}

void PlotScatter::getDataInfo(double secs)
{
	if (m_plotPairData.isEmpty())
	{
		return;
	}

	m_customPlot->clearGraphs();

	int itemCnt = m_dataPair.size();
	for (int i = 0; i < itemCnt; ++i)
	{
		QColor color = m_dataPair.at(i)->dataColor();
		QString xcolumn = m_dataPair.at(i)->getDataPair().first;
		QString ycolumn = m_dataPair.at(i)->getDataPair().second;
		updateData(xcolumn, ycolumn, secs, i, color);
	}
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
//	update();
}

void PlotScatter::updateData(QString xEntityType, QString yEntityType, double secs, int index, QColor color)
{
    QStringList xlist = xEntityType.split("+");
    QStringList ylist = yEntityType.split("+");
	QVector<double> x = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs).toVector();
	QVector<double> y = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs).toVector();

    if (x.isEmpty() || y.isEmpty())
        return;

	m_customPlot->addGraph();
	m_customPlot->graph(index)->setPen(QPen(color));
	m_customPlot->graph()->setLineStyle(QCPGraph::lsNone);
	m_customPlot->graph(index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
	m_customPlot->graph(index)->addData(x, y);
}


void PlotScatter::onGetCurrentSeconds(double secs)
{
    m_curSeconds = secs;
//    update();
	getDataInfo(m_curSeconds);
}

void PlotScatter::paintEvent(QPaintEvent *event)
{
	int width = this->width();
	int height = this->height();

    QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();
	//绘制标题
	if (m_titleShow)
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
		painter.drawText(QPoint((width - w + m_leftPadding - m_rightPadding) / 2, as + m_topPadding), m_title);
	}

	m_customPlot->setGeometry(m_leftPadding, h + m_topPadding,
		width - m_leftPadding - m_rightPadding, height - h - m_topPadding - m_bottomPadding);

//    getDataInfo(m_curSeconds);
}

void PlotScatter::setBackground(QBrush brush)
{
	m_backgroundBrush = brush;
	m_customPlot->setBackground(m_backgroundBrush);
}

void PlotScatter::setPaddings(double top, double bottom, double left, double right)
{
	m_topPadding = top;
	m_bottomPadding = bottom;
	m_leftPadding = left;
	m_rightPadding = right;
	update();
}

void PlotScatter::setTitle(QString & str)
{
	m_title = str;
	update();
}

void PlotScatter::setTitleColor(QColor & color)
{
	m_titleColor = color;
	update();
}

void PlotScatter::setTitleFont(QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotScatter::setTitleShow(bool show)
{
	m_titleShow = show;
	update();
}

void PlotScatter::setxAxisLabel(QString & str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
}

void PlotScatter::setyAxisLabel(QString & str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->replot();
}

void PlotScatter::setAxisColor(QColor & color)
{
	m_axisLabelColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->replot();
}

void PlotScatter::setAxisFont(QFont & font)
{
	m_axisFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisFont);
	m_customPlot->yAxis->setLabelFont(m_axisFont);
	m_customPlot->replot();
}

void PlotScatter::setAxisVisible(bool on, AxisType type)
{
	switch (type)
	{
	case AxisType::xAxis:
		m_customPlot->xAxis->setVisible(on);
		break;
	case AxisType::yAxis:
		m_customPlot->yAxis->setVisible(on);
		break;
	case AxisType::xAxis2:
		m_customPlot->xAxis2->setVisible(on);
		break;
	case AxisType::yAxis2:
		m_customPlot->yAxis2->setVisible(on);
		break;
	default:
		break;
	}
	m_customPlot->replot();
}

void PlotScatter::setAxisTickLabelShow(bool on, AxisType type)
{
	switch (type)
	{
	case AxisType::xAxis:
		m_customPlot->xAxis->setTickLabels(on);
		break;
	case AxisType::yAxis:
		m_customPlot->yAxis->setTickLabels(on);
		break;
	case AxisType::xAxis2:
		m_customPlot->xAxis2->setTickLabels(on);
		break;
	case AxisType::yAxis2:
		m_customPlot->yAxis2->setTickLabels(on);
		break;
	default:
		break;
	}
	m_customPlot->replot();
}

void PlotScatter::rescale_xAxis(bool on)
{
	m_customPlot->xAxis->rescale(on);
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::rescale_yAxis(bool on)
{
	m_customPlot->yAxis->rescale(on);
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::rescaleAxis(bool on)
{
	m_customPlot->rescaleAxes(on);
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::setCoordRangeX(double lower, double upper)
{
	if (m_coordBgn_x == lower && m_coordEnd_x == upper)
	{
		return;
	}

	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
	m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
	m_customPlot->replot();
}

void PlotScatter::setCoordRangeY(double lower, double upper)
{
	if (m_coordBgn_y == lower && m_coordEnd_y == upper)
	{
		return;
	}

	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
	m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);
	m_customPlot->replot();
}

void PlotScatter::getCoordRangeX(double & lower, double & upper)
{
	lower = m_coordBgn_x;
	upper = m_coordEnd_x;
}

void PlotScatter::getCoordRangeY(double & lower, double & upper)
{
	lower = m_coordBgn_y;
	upper = m_coordEnd_y;
}

void PlotScatter::setHorzGrids(uint count)
{
	if (m_horzGrids == count || count < 0)
	{
		return;
	}
	m_horzGrids = count;
	if (count == 0)
	{
		m_customPlot->yAxis->grid()->setVisible(false);
	}
	else
	{
		m_customPlot->yAxis->grid()->setVisible(true);
		m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
	}
	m_customPlot->replot();
}

void PlotScatter::setVertGrids(uint count)
{
	if (m_vertGrids == count || count < 0)
	{
		return;
	}
	m_vertGrids = count;
	if (count == 0)
	{
		m_customPlot->xAxis->grid()->setVisible(false);
	}
	else
	{
		m_customPlot->xAxis->grid()->setVisible(true);
		m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
	}
	m_customPlot->replot();
}

uint PlotScatter::getHorzGrids()
{
	return m_horzGrids;
}

uint PlotScatter::getVertGrids()
{
	return m_vertGrids;
}

void PlotScatter::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->replot();
}

void PlotScatter::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
	m_customPlot->replot();
}

uint PlotScatter::getAxisWidth()
{
	return m_axisWidth;
}

uint PlotScatter::getGridWidth()
{
	return m_gridWidth;
}

QColor PlotScatter::getAxisColor()
{
	return m_axisColor;
}

QColor PlotScatter::getGridColor()
{
	return m_gridColor;
}
