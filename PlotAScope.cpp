#include "PlotAScope.h"

int PlotAScope::m_instanceCount = 1;
PlotAScope::PlotAScope(QWidget* parent)
	:PlotItemBase(parent)
{
	QString name = QString("Attitude%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
}


PlotAScope::~PlotAScope()
{
}
