#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotAScope :
	public PlotItemBase
{
	Q_OBJECT

public:
	PlotAScope(QWidget* parent = Q_NULLPTR);
	~PlotAScope();

	enum AxisType {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

	void initPlot();
	
	QString getxAxisLabel() const { return m_xAxisLabel; }
	QString getyAxisLabel() const { return m_yAxisLabel; }
	QColor getAxisColor() const { return m_axisColor; }
	QFont getAxisFont() const { return m_axisFont; }

	void setPaddings(double top, double bottom, double left, double right);
	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisColor(QColor& color);
	void setAxisFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);
	void setMinimumMargins(const QMargins &margins);

	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);

	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);
	
	virtual uint getHorzGrids() { return m_horzGrids; }
	virtual uint getVertGrids() { return m_vertGrids; }
	virtual uint getAxisWidth() { return m_axisWidth; }
	virtual uint getGridWidth() { return m_gridWidth; }
	virtual QColor getAxisColor() { return m_axisColor; }
	virtual QColor getGridColor() { return m_gridColor; }

	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
	void setTitle(QString& str);
	void setTitleColor(QColor& color);
	void setTitleFont(QFont& font);
	void setTitleVisible(bool show);
	virtual void setTitleFillColor(QColor& color);
	virtual bool unitsShowX() { return m_showUnits_x; }
	virtual bool unitsShowY() { return m_showUnits_y; }
	virtual QString getUnitsX() { return m_units_x; }
	virtual QString getUnitsY() { return m_units_y; }
	virtual bool getTitleVisible() { return m_titleVisible; }
	virtual QString getTitle() { return m_title; }
	virtual QColor getTitleColor() { return m_titleColor; }
	virtual QColor getTitleFillColor() { return m_titleFillColor; }
	virtual QFont getTitleFont() { return m_titleFont; }


public:
	static int m_instanceCount;			//实体个数

private:
	QCustomPlot *m_customPlot;

	double m_topPadding;				//绘图间隔-top
	double m_bottomPadding;				//绘图间隔-bottom
	double m_leftPadding;				//绘图间隔-left
	double m_rightPadding;				//绘图间隔-right

	QFont	m_axisFont;					//坐标标题字体
	QString m_xAxisLabel;
	QString m_yAxisLabel;

public slots:
	//mouseEvent
	void slot_setMouseEventEnable(bool on);

	void slot_getCurrentSeconds(double secs);

protected:
	virtual void paintEvent(QPaintEvent* event);
	bool eventFilter(QObject *watched, QEvent *event);
};

