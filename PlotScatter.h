#ifndef PLOTSCATTER_H
#define PLOTSCATTER_H

#include "PlotItemBase.h"
#include "BaseData.h"
#include "qcustomplot.h"
#include <QMap>
#include <QString>

class PlotScatter : public PlotItemBase
{
    Q_OBJECT
public:
    PlotScatter(QWidget *parent = nullptr);
    ~PlotScatter();

	enum AxisType {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

    static int m_instanceCount;         //Plot实体个数

	QBrush getBackground() const { return m_backgroundBrush; }
	QString getxAxisLabel() const { return m_xAxisLabel; }
	QString getyAxisLabel() const { return m_yAxisLabel; }
	QFont getAxisFont() const { return m_axisFont; }

	void setBackground(QBrush brush);
	void setPaddings(double top, double bottom, double left, double right);
	
	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisColor(QColor& color);
	void setAxisFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

	void rescale_xAxis(bool);
	void rescale_yAxis(bool);
	void rescaleAxis(bool);

	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);
	virtual void setHorzGrids(uint);
	virtual void setVertGrids(uint);
	virtual uint getHorzGrids();
	virtual uint getVertGrids();
	virtual void setAxisColorWidth(QColor, uint);
	virtual void setGridColorWidth(QColor, uint);
	virtual uint getAxisWidth();
	virtual uint getGridWidth();
	virtual QColor getAxisColor();
	virtual QColor getGridColor();


	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
	virtual void setTitle(QString& str);
	virtual void setTitleColor(QColor& color);
	virtual void setTitleFillColor(QColor& color);
	virtual void setTitleFont(QFont& font);
	virtual void setTitleVisible(bool show);
	virtual bool unitsShowX() { return m_showUnits_x; }
	virtual bool unitsShowY() { return m_showUnits_y; }
	virtual QString getUnitsX() { return m_units_x; }
	virtual QString getUnitsY() { return m_units_y; }
	virtual bool getTitleVisible() { return m_titleVisible; }
	virtual QString getTitle() { return m_title; }
	virtual QColor getTitleColor() { return m_titleColor; }
	virtual QColor getTitleFillColor() { return m_titleFillColor; }
	virtual QFont getTitleFont() { return m_titleFont; }


public slots:
    void onGetCurrentSeconds(double secs);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
	void initPlot();
    void getDataInfo(double secs);
    void updateData(QString xEntityType, QString yEntityType, double secs, int index, QColor color = Qt::white);

private:
    double m_curSeconds;
	QBrush m_backgroundBrush;			//背景刷
	double m_topPadding;				//绘图间隔-top
	double m_bottomPadding;				//绘图间隔-bottom
	double m_leftPadding;				//绘图间隔-left
	double m_rightPadding;				//绘图间隔-right

	QCustomPlot *m_customPlot;

	QColor	m_axisLabelColor;			//坐标标题颜色
	QFont	m_axisFont;					//坐标标题字体
	QString m_xAxisLabel;
	QString m_yAxisLabel;

//    QList<QColor> m_clrList;
};

#endif // PLOTSCATTER_H
