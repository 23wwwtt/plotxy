#include "PlotRTI.h"

int PlotRTI::m_instanceCount = 1;
PlotRTI::PlotRTI(QWidget* parent)
	:PlotItemBase(parent)
{
	QString name = QString("Attitude%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
}


PlotRTI::~PlotRTI()
{
}
