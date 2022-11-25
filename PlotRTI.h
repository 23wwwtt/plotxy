#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotRTI :
	public PlotItemBase
{
	Q_OBJECT

public:
	PlotRTI(QWidget* parent = Q_NULLPTR);
	~PlotRTI();

	enum AxisType {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

	void initPlot();
	QString getTitle() const { return m_title; }
	QColor getTitleColor() const { return m_titleColor; }
	QFont getTitleFont() const { return m_titleFont; }
	bool getTitleShow() const { return m_titleShow; }
	QString getxAxisLabel() const { return m_xAxisLabel; }
	QString getyAxisLabel() const { return m_yAxisLabel; }
	QColor getAxisColor() const { return m_axisColor; }
	QFont getAxisFont() const { return m_axisFont; }

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

public:
	static int m_instanceCount;			//ʵ�����

private:
	QCustomPlot *m_customPlot;
	QCPColorMap *m_colorMap;
	QCPColorScale *m_colorScale;
	QCPMarginGroup *m_marginGroup;

	QString m_title;
	QColor	m_titleColor;				//������ɫ
	QFont	m_titleFont;				//��������
	bool	m_titleShow;				//�����Ƿ���ʾ
	double m_topPadding;				//��ͼ���-top
	double m_bottomPadding;				//��ͼ���-bottom
	double m_leftPadding;				//��ͼ���-left
	double m_rightPadding;				//��ͼ���-right

	QColor	m_axisColor;				//���������ɫ
	QFont	m_axisFont;					//�����������
	QString m_xAxisLabel;
	QString m_yAxisLabel;

public slots:
	//mouseEvent
	void slot_setMouseEventEnable(bool on);

protected:
	virtual void paintEvent(QPaintEvent* event);
};

