#pragma once
#include "PlotItemBase.h"
class PlotAScope :
	public PlotItemBase
{
	Q_OBJECT

public:
	PlotAScope(QWidget* parent = Q_NULLPTR);
	~PlotAScope();

public:
	static int m_instanceCount;			//实体个数
};

