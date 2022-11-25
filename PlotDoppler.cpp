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
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);
	m_titleShow = true;

	m_axisColor = Qt::white;
	m_axisFont.setFamily("Microsoft YaHei");
	m_axisFont.setPointSizeF(10.0);
	m_xAxisLabel = "Range(m)";
	m_yAxisLabel = "Voltage(V)";

	m_leftPadding = 10;
	m_rightPadding = 20;
	m_topPadding = 0;
	m_bottomPadding = 10;

	initPlot();
}


PlotDoppler::~PlotDoppler()
{
}

void PlotDoppler::initPlot()
{
	m_customPlot = new QCustomPlot(this);
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));

// 	m_customPlot->xAxis->setRange(-1, 1);
// 	m_customPlot->yAxis->setRange(0, 1);
	
	m_customPlot->xAxis->setTickLabels(false);
	m_customPlot->yAxis->setTickLabels(false);
	m_customPlot->xAxis->setVisible(false);
	m_customPlot->yAxis->setVisible(false);

	m_customPlot->setBackground(QBrush(QColor(0, 0, 0)));
	m_customPlot->xAxis->setBasePen(QPen(QColor(255, 255, 255)));
	m_customPlot->yAxis->setBasePen(QPen(QColor(255, 255, 255)));
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

	m_customPlot->rescaleAxes();

	m_horizon_AScope = new PlotAScope(this);
	m_horizon_AScope->setPaddings(0, 0, 0, 0);
	m_horizon_AScope->setTitleShow(false);
	m_horizon_AScope->setxAxisLabel(QString(""));
	m_horizon_AScope->setyAxisLabel(QString(""));
	m_horizon_AScope->setAxisTickLabelShow(false, PlotAScope::yAxis);
	m_horizon_AScope->setMinimumMargins(QMargins(15,1,15,15));

	m_vertical_AScope = new PlotAScope(this);
	m_vertical_AScope->setPaddings(0, 0, 0, 0);
	m_vertical_AScope->setTitleShow(false);
	m_vertical_AScope->setxAxisLabel(QString(""));
	m_vertical_AScope->setyAxisLabel(QString(""));
	m_vertical_AScope->setAxisTickLabelShow(false, PlotAScope::xAxis);
	m_vertical_AScope->setMinimumMargins(QMargins(15, 15, 3, 15));

	setRange_xAxis(0, 100);
	setRange_yAxis(0, 100);
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

	if (!m_titleShow)
	{
		w = 0.0;
		h = 0.0;
		as = 0.0;
	}
	else
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
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

void PlotDoppler::setTitleFont(QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotDoppler::setTitleShow(bool show)
{
	m_titleShow = show;
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

void PlotDoppler::setRange_xAxis(double lower, double upper)
{
	m_customPlot->xAxis->setRange(lower, upper);
	m_horizon_AScope->setRange_xAxis(lower, upper);

	m_customPlot->rescaleAxes();
 	m_customPlot->replot();
}

void PlotDoppler::setRange_yAxis(double lower, double upper)
{
	m_customPlot->yAxis->setRange(lower, upper);
	m_vertical_AScope->setRange_yAxis(lower, upper);

	m_customPlot->rescaleAxes();
	m_customPlot->replot();
}