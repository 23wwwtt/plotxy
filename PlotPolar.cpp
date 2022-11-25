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

	m_angularUnit = QString::fromLocal8Bit("¡ã");
	m_radialUnit = QString::fromLocal8Bit("¡ã");

	m_angularRange_lower = 0.0;
	m_angularRange_upper = 360.0;
	m_radialRange_lower = 0.0;
	m_radialRange_upper = 90.0;

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
	m_angularAxis->setBasePen(QPen(QColor(255, 255, 255), 2));
	m_customPlot->plotLayout()->addElement(0, 0, m_angularAxis);
	
//	m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	m_angularAxis->setRangeDrag(false);
	m_angularAxis->setTickLabelMode(QCPPolarAxisAngular::lmUpright);
	m_angularAxis->setFormat(m_angularUnit);
	m_angularAxis->setTickLabelColor(QColor(255, 255, 255));
	m_angularAxis->setTickPen(QPen(QColor(255, 255, 255),2));

	m_angularAxis->radialAxis()->setTickLabelColor(QColor(255, 255, 255));
	m_angularAxis->radialAxis()->setFormat(m_radialUnit);
	m_angularAxis->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);
	m_angularAxis->radialAxis()->setTickLabelRotation(0);
	m_angularAxis->radialAxis()->setBasePen(QPen(QColor(255, 255, 255), 2));
	m_angularAxis->radialAxis()->setAngle(0);

	//angularAxis->radialAxis()->setNumberFormat("e");
	m_angularAxis->grid()->setAngularPen(QPen(QColor(255, 255, 255), 0, Qt::SolidLine));
	m_angularAxis->grid()->setSubGridType(QCPPolarGrid::gtNone);

	m_angularAxis->setRange(m_angularRange_lower, m_angularRange_upper);
	m_angularAxis->radialAxis()->setRange(m_radialRange_lower, m_radialRange_upper);
	m_angularAxis->radialAxis()->setRangeReversed(false);

//	m_customPlot->replot();
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
		painter.drawText(QPoint((width - w) / 2, h), m_title);
	} 
	else
	{
		m_customPlot->setGeometry(0, (height + h - radius)/2, radius, radius);
		painter.drawText(QPoint((width - w) / 2, (height + h - radius) / 2), m_title);
	}
}

void PlotPolar::slot_setTitle(QString title)
{
	m_title = title;
	update();
}

void PlotPolar::slot_setTitleColor(const QColor & color)
{
	m_titleColor = color;
	update();
}

void PlotPolar::slot_setTitleFont(const QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotPolar::slot_setAngularUnit(QString unit)
{
	m_angularAxis->setFormat(unit);
}

void PlotPolar::slot_setRadialUnit(QString unit)
{
	m_angularAxis->radialAxis()->setFormat(unit);
}

void PlotPolar::slot_setAngularRange(double lower, double upper)
{
	m_angularAxis->setRange(lower, upper);
}

void PlotPolar::slot_setRadialRange(double lower, double upper)
{
	m_angularAxis->radialAxis()->setRange(lower, upper);
}

void PlotPolar::slot_setRangeDrag(bool enabled)
{
	m_customPlot->setInteraction(QCP::iRangeDrag, enabled);
}

void PlotPolar::slot_setRangeZoom(bool enabled)
{
	m_customPlot->setInteraction(QCP::iRangeZoom, enabled);
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
