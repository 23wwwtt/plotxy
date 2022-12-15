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

	initPlot();
}

PlotScatter::~PlotScatter()
{
	
}

void PlotScatter::initPlot()
{
	m_customPlot = new QCustomPlot(this);
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));

	m_customPlot->xAxis->setLabel(m_xAxisLabel);
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->xAxis->setRange(0, 2000);
	m_customPlot->yAxis->setRange(0, 2000);
//  	m_customPlot->xAxis->setNumberPrecision(3);
//  	m_customPlot->yAxis->setNumberPrecision(3);

	m_customPlot->setBackground(m_backgroundBrush);
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor));
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->xAxis->setLabelFont(m_axisFont);
	m_customPlot->yAxis->setLabelFont(m_axisFont);
	m_customPlot->xAxis->setTickLabelColor(QColor(255, 255, 255));
	m_customPlot->yAxis->setTickLabelColor(QColor(255, 255, 255));
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
	//	m_customPlot->replot();
}

void PlotScatter::setAxisColor(QColor & color)
{
	m_axisLabelColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	//	m_customPlot->replot();
}

void PlotScatter::setAxisFont(QFont & font)
{
	m_axisFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisFont);
	m_customPlot->yAxis->setLabelFont(m_axisFont);
	//	m_customPlot->replot();
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
	//	m_customPlot->replot();
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
	//	m_customPlot->replot();
}

void PlotScatter::setRange_xAxis(double lower, double upper)
{
	m_customPlot->xAxis->setRange(lower, upper);
	//	m_customPlot->replot();
}

void PlotScatter::setRange_yAxis(double lower, double upper)
{
	m_customPlot->yAxis->setRange(lower, upper);
	//	m_customPlot->replot();
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
