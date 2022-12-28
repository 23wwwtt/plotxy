#include "PlotPolar.h"
#include <qlabel.h>
#include "DataManager.h"

int PlotPolar::m_instanceCount = 1;
PlotPolar::PlotPolar(QWidget * parent)
	:PlotItemBase(parent)
{
	QString name = QString("Polar%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
	m_title = "Polar";
	m_titleColor = Qt::white;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);
	m_titleVisible = true;

	m_units_x = QString::fromLocal8Bit("¡ã");
	m_units_y = QString::fromLocal8Bit("¡ã");
	m_showUnits_x = true;
	m_showUnits_y = true;

	m_angularRange_lower = 0.0;
	m_angularRange_upper = 360.0;
	m_radialRange_lower = 0.0;
	m_radialRange_upper = 2000.0;

	m_horzGrids = (m_angularRange_upper - m_angularRange_lower) / 30.0;
	m_axisWidth = 1;
	m_gridWidth = 1;
	m_axisColor = Qt::white;
	m_gridColor = QColor(200, 200, 200);

	initPlot();
}

PlotPolar::~PlotPolar()
{
}

void PlotPolar::initPlot()
{
	m_customPlot = new QCustomPlot(this);
	m_customPlot->setSelectionRectMode(QCP::srmNone);
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);

	m_customPlot->setBackground(QBrush(QColor(0, 0, 0)));
	m_customPlot->plotLayout()->clear();

	m_angularAxis = new QCPPolarAxisAngular(m_customPlot);
	m_angularAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->plotLayout()->addElement(0, 0, m_angularAxis);
	
//	m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	m_angularAxis->setRangeDrag(false);
	m_angularAxis->setTickLabelMode(QCPPolarAxisAngular::lmUpright);
	m_angularAxis->setFormat(m_units_x);
	m_angularAxis->setTickLabelColor(QColor(255, 255, 255));
	m_angularAxis->setTickPen(QPen(QColor(255, 255, 255),1));

	m_angularAxis->radialAxis()->setTickLabelColor(QColor(255, 255, 255));
	m_angularAxis->radialAxis()->setFormat(m_units_y);
	m_angularAxis->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);
	m_angularAxis->radialAxis()->setTickLabelRotation(0);
	m_angularAxis->radialAxis()->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_angularAxis->radialAxis()->setAngle(0);

	//angularAxis->radialAxis()->setNumberFormat("e");
	m_angularAxis->grid()->setAngularPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
	m_angularAxis->grid()->setRadialPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
	m_angularAxis->grid()->setSubGridType(QCPPolarGrid::gtNone);

	m_angularAxis->setRange(m_angularRange_lower, m_angularRange_upper);
	m_angularAxis->radialAxis()->setRange(m_radialRange_lower, m_radialRange_upper);
	m_angularAxis->radialAxis()->setRangeReversed(false);

	m_vertGrids = m_angularAxis->radialAxis()->ticker()->tickCount();
	m_customPlot->replot();
}

void PlotPolar::getCoordRangeX(double & lower, double & upper)
{
	lower = m_angularRange_lower;
	upper = m_angularRange_upper;
}

void PlotPolar::getCoordRangeY(double & lower, double & upper)
{
	lower = m_radialRange_lower;
	upper = m_radialRange_upper;
}

void PlotPolar::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	m_angularAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_angularAxis->radialAxis()->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->replot();
}

void PlotPolar::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_angularAxis->grid()->setAngularPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
	m_angularAxis->grid()->setRadialPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
	m_customPlot->replot();
}

void PlotPolar::setHorzGrids(uint count)
{
	if (m_horzGrids == count || count <= 0)
	{
		return;
	}
	m_horzGrids = count;
	double range = (m_angularRange_upper - m_angularRange_lower) / count;
	m_angularAxis->ticker().dynamicCast<QCPAxisTickerFixed>().data()->setTickStep(range);
	m_customPlot->replot();
}

void PlotPolar::setVertGrids(uint count)
{
	if (m_vertGrids == count || count < 0)
	{
		return;
	}
	m_vertGrids = count;
	if (count == 0)
	{
		m_angularAxis->grid()->setVisible(false);
	}
	else
	{
		m_angularAxis->grid()->setVisible(true);
		m_angularAxis->radialAxis()->ticker()->setTickCount(count);
	}
	m_customPlot->replot();
}


void PlotPolar::paintEvent(QPaintEvent * event)
{
	int width = this->width();
	int height = this->height();
	//»­±Ê
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();

	int radius = qMin(width, int(height - h));

	painter.setFont(m_titleFont);
	painter.setPen(m_titleColor);
	if (width > (height - h))
	{
		m_customPlot->setGeometry((width - radius)/2, h, radius, radius);
		if(m_titleVisible)
			painter.drawText(QPoint((width - w) / 2, h), m_title);
	} 
	else
	{
		m_customPlot->setGeometry(0, (height + h - radius)/2, radius, radius);
		if (m_titleVisible)
			painter.drawText(QPoint((width - w) / 2, (height + h - radius) / 2), m_title);
	}
}

void PlotPolar::setTitle(QString& title)
{
	m_title = title;
	update();
}

void PlotPolar::setTitleColor(QColor & color)
{
	m_titleColor = color;
	update();
}

void PlotPolar::setTitleFillColor(QColor & color)
{
	m_titleFillColor = color;
	update();
}

void PlotPolar::setTitleFont(QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotPolar::setTitleVisible(bool show)
{
	m_titleVisible = show;
	update();
}

void PlotPolar::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	m_angularAxis->setFormatShow(on);
	m_customPlot->replot();
}

void PlotPolar::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	m_angularAxis->radialAxis()->setFormatShow(on);
	m_customPlot->replot();
}

void PlotPolar::setUnitsX(const QString& units)
{
	m_units_x = units;
	m_angularAxis->setFormat(units);
	m_customPlot->replot();
}

void PlotPolar::setUnitsY(const QString& units)
{
	m_units_y = units;
	m_angularAxis->radialAxis()->setFormat(units);
	m_customPlot->replot();
}

void PlotPolar::setCoordRangeX(double lower, double upper)
{
	m_angularAxis->setRange(lower, upper);
	m_customPlot->replot();
}

void PlotPolar::setCoordRangeY(double lower, double upper)
{
	m_angularAxis->radialAxis()->setRange(lower, upper);
	m_customPlot->replot();
}

void PlotPolar::slot_setRangeDrag(bool enabled)
{
	m_customPlot->setInteraction(QCP::iRangeDrag, enabled);
	m_customPlot->replot();
}

void PlotPolar::slot_setRangeZoom(bool enabled)
{
	m_customPlot->setInteraction(QCP::iRangeZoom, enabled);
	m_customPlot->replot();
}

void PlotPolar::slot_getCurrentSeconds(double secs)
{
	if (getPlotPairData().isEmpty())
		return;

	int isize = getPlotPairData().size();
	QVector<QCPPolarGraph*> graph;

	for (int i = 0; i < isize; ++i)
	{
		QString xcolumn = getPlotPairData().at(i).first;
		QString ycolumn = getPlotPairData().at(i).second;
		QStringList xlist = xcolumn.split("+");
		QStringList ylist = ycolumn.split("+");

		QVector<double> x = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs).toVector();
		QVector<double> y = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs).toVector();
		
		QCPPolarGraph* g = new QCPPolarGraph(m_angularAxis, m_angularAxis->radialAxis());
		g->setScatterStyle(QCPScatterStyle::ssDisc);
		g->setLineStyle(QCPPolarGraph::lsNone);
		graph.append(g);
		graph[i]->setPen(QPen(QColor(255, 0, 0), 2));
		graph[i]->addData(x, y);
	}
	m_customPlot->replot();

	qDeleteAll(graph);
	graph.clear();
}

void PlotPolar::slot_setMouseEventEnable(bool on)
{
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotPolar::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
	m_thresholdColorMap = targetMap;
}
