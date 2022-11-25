#include "PlotRTI.h"

int PlotRTI::m_instanceCount = 1;
PlotRTI::PlotRTI(QWidget* parent)
	:PlotItemBase(parent)
{
	QString name = QString("RTI%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_title = "RTI Display";
	m_titleColor = Qt::white;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);
	m_titleShow = true;

	m_axisColor = Qt::white;
	m_axisFont.setFamily("Microsoft YaHei");
	m_axisFont.setPointSizeF(10.0);
	m_xAxisLabel = "Range(m)";
	m_yAxisLabel = "Time(s)";

	m_leftPadding = 10;
	m_rightPadding = 30;
	m_topPadding = 20;
	m_bottomPadding = 10;

	initPlot();
}


PlotRTI::~PlotRTI()
{
}

void PlotRTI::initPlot()
{
	m_customPlot = new QCustomPlot(this);
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	connect(m_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(m_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_customPlot->yAxis2, SLOT(setRange(QCPRange)));

	m_customPlot->xAxis->setLabel(m_xAxisLabel);
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->xAxis->setRange(-1, 1);
	m_customPlot->yAxis->setRange(0, 1);

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
	//m_customPlot->plotLayout()->addElement(0, 1, m_colorScale);
	//m_customPlot->plotLayout()->setColumnSpacing(0);
	//m_colorScale->setType(QCPAxis::atRight);
	m_colorMap->setColorScale(m_colorScale);
	m_colorMap->setGradient(QCPColorGradient::gpJet);
	m_colorMap->rescaleDataRange();

	m_marginGroup = new QCPMarginGroup(m_customPlot);
//	m_customPlot->axisRect()->setMinimumMargins(QMargins(2, 2, 2, 2));
	m_customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);
	m_colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);

	m_customPlot->rescaleAxes();
}

void PlotRTI::paintEvent(QPaintEvent * event)
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
	QRectF rect = fm.boundingRect(m_title);

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

	m_customPlot->setGeometry(m_leftPadding, h + m_topPadding,
		width - m_leftPadding - m_rightPadding, height - h - m_topPadding - m_bottomPadding);
}

void PlotRTI::slot_setMouseEventEnable(bool on)
{
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotRTI::setTitle(QString & str)
{
	m_title = str;
	update();
}

void PlotRTI::setTitleColor(QColor & color)
{
	m_titleColor = color;
	update();
}

void PlotRTI::setTitleFont(QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotRTI::setTitleShow(bool show)
{
	m_titleShow = show;
	update();
}

void PlotRTI::setxAxisLabel(QString & str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
	m_customPlot->replot();
}

void PlotRTI::setyAxisLabel(QString & str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->replot();
}

void PlotRTI::setAxisColor(QColor & color)
{
	m_axisColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisColor);
	m_customPlot->yAxis->setLabelColor(m_axisColor);
	m_customPlot->replot();
}

void PlotRTI::setAxisFont(QFont & font)
{
	m_axisFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisFont);
	m_customPlot->yAxis->setLabelFont(m_axisFont);
	m_customPlot->replot();
}

void PlotRTI::setAxisVisible(bool on, AxisType type)
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

void PlotRTI::setAxisTickLabelShow(bool on, AxisType type)
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

void PlotRTI::setRange_xAxis(double lower, double upper)
{
	m_customPlot->xAxis->setRange(lower, upper);
	m_customPlot->replot();
}

void PlotRTI::setRange_yAxis(double lower, double upper)
{
	m_customPlot->yAxis->setRange(lower, upper);
	m_customPlot->replot();
}
