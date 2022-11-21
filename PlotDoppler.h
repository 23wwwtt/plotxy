#pragma once
#include "PlotItemBase.h"
class PlotDoppler :
	public PlotItemBase
{
	Q_OBJECT

public:
	PlotDoppler(QWidget* parent = Q_NULLPTR);
	~PlotDoppler();

public:
	static int m_instanceCount;			//实体个数
};

