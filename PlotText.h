#pragma once
/* * @filename: PlotText.h
* @brief: PlotText�ؼ���
* @author: zhangdingwen
* @create time: 2022-10-10
*  */

#ifndef  _PLOT_TEXT_H_
#define  _PLOT_TEXT_H_ 

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QTimer>
#include <QColor>
#include <QDebug>

class PlotText :public PlotItemBase
{
	Q_OBJECT
public:
	PlotText(QWidget* parent = Q_NULLPTR);
	~PlotText();


	


	void onSwitch(bool bOn);
	//void updateItems();
	void drawRect(double itemIndex, bool bHorizontal, double itemLength, double leftBoundary, double rightBoundary, QColor color);
public:
	static int m_instanceCount;			//ʵ�����
public slots:


private:

	bool m_bHorizontal;
	bool m_started;

	QTimer* m_timer;


	QColor m_gridColor;
	QColor m_gridFillColor;
	QColor m_axisColor;


	int m_horiGridNum;
	int m_verGridNum;


	QStringList m_entityTypeList;
	QStringList m_entityAttrList;

	int m_leftPadding;
	int m_rightPadding;
	int m_interPadding;

	int m_currTimeIndex;


	QMap<QString, QMap<int, QColor>>m_thresholdColorMap;
	QColor m_defaultColor;

protected:
	virtual void paintEvent(QPaintEvent* event);


};
#endif // _PLOT_TEXT_H_ 