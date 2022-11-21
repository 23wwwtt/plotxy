#include "PlotDoppler.h"

int PlotDoppler::m_instanceCount = 1;
PlotDoppler::PlotDoppler(QWidget* parent)
	:PlotItemBase(parent)
{
	QString name = QString("Attitude%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
}


PlotDoppler::~PlotDoppler()
{
}
