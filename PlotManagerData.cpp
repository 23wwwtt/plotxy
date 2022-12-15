#include "PlotManagerData.h"

PlotManagerData* PlotManagerData::m_pInstance = new PlotManagerData();
PlotManagerData::PlotManagerDataRelease PlotManagerData::m_release;

PlotManagerData::PlotManagerData(QObject *parent)
	: QObject(parent)
{
}

PlotManagerData::~PlotManagerData()
{
}

void PlotManagerData::addPlotManagerData(const QString& tabName, PlotItemBase* plotItem)
{
	m_plotManager[tabName].append(plotItem);

	emit sgnUpdatePlotManager();
}

QMap<QString, QList<PlotItemBase*>> PlotManagerData::getPlotManagerData()
{
	return m_plotManager;
}

void PlotManagerData::slotChangePlotName()
{
	emit sgnUpdatePlotManager();
}
