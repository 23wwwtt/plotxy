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
	QString getTitle() const { return m_title; }
	QColor getTitleColor() const { return m_titleColor; }
	QFont getTitleFont() const { return m_titleFont; }
	bool getTitleShow() const { return m_titleShow; }
	QString getxAxisLabel() const { return m_xAxisLabel; }
	QString getyAxisLabel() const { return m_yAxisLabel; }
	QColor getAxisColor() const { return m_axisColor; }
	QFont getAxisFont() const { return m_axisFont; }

	void setBackground(QBrush brush);
	void setPaddings(double top, double bottom, double left, double right);
	void setTitle(QString& str);
	void setTitleColor(QColor& color);
	void setTitleFont(QFont& font);
	void setTitleShow(bool show);
	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisColor(QColor& color);
	void setAxisFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);
	void setRange_xAxis(double lower, double upper);
	void setRange_yAxis(double lower, double upper);

	void rescale_xAxis(bool);
	void rescale_yAxis(bool);
	void rescaleAxis(bool);

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
    QColor m_axisColor{Qt::lightGray};
	QBrush m_backgroundBrush;			//背景刷
	double m_topPadding;				//绘图间隔-top
	double m_bottomPadding;				//绘图间隔-bottom
	double m_leftPadding;				//绘图间隔-left
	double m_rightPadding;				//绘图间隔-right

	QCustomPlot *m_customPlot;

	QString m_title;
	QColor	m_titleColor;				//标题颜色
	QFont	m_titleFont;				//标题字体
	bool	m_titleShow;				//标题是否显示

	QColor	m_axisLabelColor;			//坐标标题颜色
	QFont	m_axisFont;					//坐标标题字体
	QString m_xAxisLabel;
	QString m_yAxisLabel;

    QList<QColor> m_clrList;
};

#endif // PLOTSCATTER_H
