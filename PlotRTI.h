#pragma once
#include "PlotItemBase.h"
class PlotRTI :
	public PlotItemBase
{
	Q_OBJECT

public:
	PlotRTI(QWidget* parent = Q_NULLPTR);
	~PlotRTI();

public:
	static int m_instanceCount;			//实体个数
};

