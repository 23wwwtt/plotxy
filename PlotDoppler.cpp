#include "PlotDoppler.h"

int PlotDoppler::m_instanceCount = 1;
PlotDoppler::PlotDoppler(QWidget* parent)
	:PlotItemBase(parent)
{
	QString name = QString("Doppler%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_title = "Range Doppler";
	m_titleColor = Qt::white;
	m_titleFillColor = Qt::black;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);
	m_titleVisible = true;

	m_axisFont.setFamily("Microsoft YaHei");
	m_axisFont.setPointSizeF(10.0);
	m_xAxisLabel = "Range(m)";
	m_yAxisLabel = "Voltage(V)";

	m_leftPadding = 10;
	m_rightPadding = 20;
	m_topPadding = 0;
	m_bottomPadding = 10;

	m_coordBgn_x = 0;
	m_coordEnd_x = 100;
	m_coordBgn_y = 0;
	m_coordEnd_y = 100;

	m_horzGrids = 5;
	m_vertGrids = 5;
	m_axisWidth = 1;
	m_gridWidth = 1;
	m_axisColor = Qt::white;
	m_gridColor = QColor(200, 200, 200);

	m_showUnits_x = false;
	m_showUnits_y = false;

	initPlot();
}


PlotDoppler::~PlotDoppler()
{
}

void PlotDoppler::initPlot()
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
	
	m_customPlot->xAxis->setTickLabels(false);
	m_customPlot->yAxis->setTickLabels(false);
	m_customPlot->xAxis->setVisible(false);
	m_customPlot->yAxis->setVisible(false);

	m_customPlot->setBackground(QBrush(QColor(0, 0, 0)));
	m_customPlot->xAxis->setLabelColor(m_axisColor);
	m_customPlot->yAxis->setLabelColor(m_axisColor);
	m_customPlot->xAxis->setLabelFont(m_axisFont);
	m_customPlot->yAxis->setLabelFont(m_axisFont);
	m_customPlot->xAxis->setTickLabelColor(QColor(255, 255, 255));
	m_customPlot->yAxis->setTickLabelColor(QColor(255, 255, 255));

	m_colorMap = new QCPColorMap(m_customPlot->xAxis, m_customPlot->yAxis);
	m_colorScale = new QCPColorScale(m_customPlot);
	m_customPlot->plotLayout()->addElement(0, 1, m_colorScale);
	m_customPlot->plotLayout()->setColumnSpacing(0);
	m_colorScale->setType(QCPAxis::atRight);
	m_colorMap->setColorScale(m_colorScale);
	m_colorMap->setGradient(QCPColorGradient::gpJet);
	m_colorMap->rescaleDataRange();
	
	m_marginGroup = new QCPMarginGroup(m_customPlot);
	m_customPlot->axisRect()->setMinimumMargins(QMargins(0, 15, 0, 0));
	m_customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);
	m_colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);

//	m_customPlot->rescaleAxes();

	m_horizon_AScope = new PlotAScope(this);
	m_horizon_AScope->setTitle(QString(""));
	m_horizon_AScope->setTitleFont(QFont("Microsoft YaHei", 1));
	m_horizon_AScope->setPaddings(0, 0, 0, 0);
	m_horizon_AScope->setTitleVisible(false);
	m_horizon_AScope->setxAxisLabel(QString(""));
	m_horizon_AScope->setyAxisLabel(QString(""));
	m_horizon_AScope->setAxisTickLabelShow(false, PlotAScope::yAxis);
	m_horizon_AScope->setMinimumMargins(QMargins(15,1,15,15));
	m_horizon_AScope->setVertGrids(m_vertGrids);
//	m_horizon_AScope->setHorzGrids(m_horzGrids);
	m_horizon_AScope->setAxisColorWidth(m_axisColor, m_axisWidth);
	m_horizon_AScope->setGridColorWidth(m_gridColor, m_gridWidth);

	m_vertical_AScope = new PlotAScope(this);
	m_vertical_AScope->setTitle(QString(""));
	m_vertical_AScope->setTitleFont(QFont("Microsoft YaHei", 1));
	m_vertical_AScope->setPaddings(0, 0, 0, 0);
	m_vertical_AScope->setTitleVisible(false);
	m_vertical_AScope->setxAxisLabel(QString(""));
	m_vertical_AScope->setyAxisLabel(QString(""));
	m_vertical_AScope->setAxisTickLabelShow(false, PlotAScope::xAxis);
	m_vertical_AScope->setMinimumMargins(QMargins(15, 15, 3, 15));
//	m_vertical_AScope->setVertGrids(m_vertGrids);
	m_vertical_AScope->setHorzGrids(m_horzGrids);
	m_vertical_AScope->setAxisColorWidth(m_axisColor, m_axisWidth);
	m_vertical_AScope->setGridColorWidth(m_gridColor, m_gridWidth);

	setCoordRangeX(m_coordBgn_x, m_coordEnd_x);
	setCoordRangeY(m_coordBgn_y, m_coordEnd_y);
}

void PlotDoppler::paintEvent(QPaintEvent * event)
{
	int width = this->width();
	int height = this->height();
	//»­±Ê
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();

	if (m_titleVisible)
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
		painter.fillRect((width - w + m_leftPadding - m_rightPadding) / 2, m_topPadding, w, h, m_titleFillColor);
		painter.drawText(QPoint((width + m_leftPadding - m_rightPadding - w) / 2, as + m_topPadding), m_title);
	}

	int verWidth = (width - m_leftPadding - m_rightPadding)*0.2;
	int horHeight = (height - h - m_topPadding - m_bottomPadding)*0.2;
	int plotWidth = width - m_leftPadding - m_rightPadding - verWidth;
	int plotHeight = height - h - m_topPadding - m_bottomPadding - horHeight;

	m_vertical_AScope->resize(verWidth, plotHeight + 15);
	m_vertical_AScope->setGeometry(m_leftPadding,
		h + m_topPadding,
		verWidth,
		plotHeight + 15);

	m_horizon_AScope->resize(plotWidth - m_colorScale->barWidth() + 15, horHeight);
	m_horizon_AScope->setGeometry(m_leftPadding + verWidth -15,
		h + m_topPadding + plotHeight,
		plotWidth - m_colorScale->barWidth() + 15,
		horHeight);

	m_customPlot->setGeometry(m_leftPadding + verWidth,
		h + m_topPadding,
		plotWidth,
		plotHeight);
}

void PlotDoppler::slot_setMouseEventEnable(bool on)
{
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotDoppler::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	m_horizon_AScope->setUnitsShowX(on);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	m_vertical_AScope->setUnitsShowY(on);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsX(const QString & units)
{
	m_units_x = units;
	m_horizon_AScope->setUnitsX(units);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsY(const QString & units)
{
	m_units_y = units;
	m_vertical_AScope->setUnitsY(units);
	m_customPlot->replot();
}

void PlotDoppler::setTitle(QString & str)
{
	m_title = str;
	update();
}

void PlotDoppler::setTitleColor(QColor & color)
{
	m_titleColor = color;
	update();
}

void PlotDoppler::setTitleFillColor(QColor & color)
{
	m_titleFillColor = color;
	update();
}

void PlotDoppler::setTitleFont(QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotDoppler::setTitleVisible(bool show)
{
	m_titleVisible = show;
	update();
}

void PlotDoppler::setxAxisLabel(QString & str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
	m_customPlot->replot();
}

void PlotDoppler::setyAxisLabel(QString & str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->replot();
}

void PlotDoppler::setAxisColor(QColor & color)
{
	m_axisColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisColor);
	m_customPlot->yAxis->setLabelColor(m_axisColor);
	m_customPlot->replot();
}

void PlotDoppler::setAxisFont(QFont & font)
{
	m_axisFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisFont);
	m_customPlot->yAxis->setLabelFont(m_axisFont);
	m_customPlot->replot();
}

void PlotDoppler::setAxisVisible(bool on, AxisType type)
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

void PlotDoppler::setAxisTickLabelShow(bool on, AxisType type)
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

void PlotDoppler::setCoordRangeX(double lower, double upper)
{
	if (m_coordBgn_x == lower && m_coordEnd_x == upper)
	{
		return;
	}


	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
	m_customPlot->xAxis->setRange(lower, upper);
	m_horizon_AScope->setCoordRangeX(lower, upper);


	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
	m_customPlot->xAxis->setRange(lower, upper);
	m_horizon_AScope->setCoordRangeX(lower, upper);

//	m_customPlot->rescaleAxes();
 	m_customPlot->replot();
}

void PlotDoppler::setCoordRangeY(double lower, double upper)
{
	if (m_coordBgn_y == lower && m_coordEnd_y == upper)
	{
		return;
	}

	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
	m_customPlot->yAxis->setRange(lower, upper);
	m_vertical_AScope->setCoordRangeY(lower, upper);


	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
	m_customPlot->yAxis->setRange(lower, upper);
	m_vertical_AScope->setCoordRangeY(lower, upper);

//	m_customPlot->rescaleAxes();
	m_customPlot->replot();
}

void PlotDoppler::getCoordRangeX(double & lower, double & upper)
{
	lower = m_coordBgn_x;
	upper = m_coordEnd_x;
}

void PlotDoppler::getCoordRangeY(double & lower, double & upper)
{
	lower = m_coordBgn_y;
	upper = m_coordEnd_y;
}


void PlotDoppler::setHorzGrids(uint count)
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
//	m_horizon_AScope->setHorzGrids(m_horzGrids);
	m_vertical_AScope->setHorzGrids(m_horzGrids);
	m_customPlot->replot();
}

void PlotDoppler::setVertGrids(uint count)
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
	m_horizon_AScope->setVertGrids(m_vertGrids);
//	m_vertical_AScope->setVertGrids(m_vertGrids);
	m_customPlot->replot();
}

void PlotDoppler::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));

	m_horizon_AScope->setAxisColorWidth(m_axisColor, m_axisWidth);
	m_vertical_AScope->setAxisColorWidth(m_axisColor, m_axisWidth);
	m_customPlot->replot();
}

void PlotDoppler::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
	
	m_horizon_AScope->setGridColorWidth(m_gridColor, m_gridWidth);
	m_vertical_AScope->setGridColorWidth(m_gridColor, m_gridWidth);
	m_customPlot->replot();
}
