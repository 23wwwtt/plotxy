#include "PlotAScope.h"

int PlotAScope::m_instanceCount = 1;
PlotAScope::PlotAScope(QWidget* parent)
	:PlotItemBase(parent)
{
	QString name = QString("A-Scope%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_title = "A-Scope";
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
	m_bottomPadding = 20;

	initPlot();

	parent->installEventFilter(this);
}


PlotAScope::~PlotAScope()
{
}

void PlotAScope::initPlot()
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
}

void PlotAScope::paintEvent(QPaintEvent * event)
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
	
	m_customPlot->setGeometry(m_leftPadding, h + m_topPadding, 
		width - m_leftPadding - m_rightPadding, height - h - m_topPadding - m_bottomPadding);	
}

bool PlotAScope::eventFilter(QObject * watched, QEvent * event)
{
	if (watched == parent() && event->type() == QEvent::Paint)
	{
		this->paintEvent((QPaintEvent*)event);
	}
	return false;
}

void PlotAScope::slot_setMouseEventEnable(bool on)
{
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotAScope::setPaddings(double top, double bottom, double left, double right)
{
	m_topPadding = top;
	m_bottomPadding = bottom;
	m_leftPadding = left;
	m_rightPadding = right;
	update();
}

void PlotAScope::setTitle(QString & str)
{
	m_title = str;
	update();
}

void PlotAScope::setTitleColor(QColor & color)
{
	m_titleColor = color;
	update();
}

void PlotAScope::setTitleFont(QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotAScope::setTitleShow(bool show)
{
	m_titleShow = show;
	update();
}

void PlotAScope::setxAxisLabel(QString & str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
//	m_customPlot->replot();
}

void PlotAScope::setyAxisLabel(QString & str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
//	m_customPlot->replot();
}

void PlotAScope::setAxisColor(QColor & color)
{
	m_axisColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisColor);
	m_customPlot->yAxis->setLabelColor(m_axisColor);
//	m_customPlot->replot();
}

void PlotAScope::setAxisFont(QFont & font)
{
	m_axisFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisFont);
	m_customPlot->yAxis->setLabelFont(m_axisFont);
//	m_customPlot->replot();
}

void PlotAScope::setAxisVisible(bool on, AxisType type)
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

void PlotAScope::setAxisTickLabelShow(bool on, AxisType type)
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

void PlotAScope::setRange_xAxis(double lower, double upper)
{
	m_customPlot->xAxis->setRange(lower, upper);
//	m_customPlot->replot();
}

void PlotAScope::setRange_yAxis(double lower, double upper)
{
	m_customPlot->yAxis->setRange(lower, upper);
//	m_customPlot->replot();
}

void PlotAScope::setMinimumMargins(const QMargins & margins)
{
	m_customPlot->axisRect()->setMinimumMargins(margins);
}

