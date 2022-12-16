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
	QString getTitle() const { return m_title; }
	QColor getTitleColor() const { return m_titleColor; }
	QFont getTitleFont() const { return m_titleFont; }
	bool getTitleShow() const { return m_titleShow; }
	QString getxAxisLabel() const { return m_xAxisLabel; }
	QString getyAxisLabel() const { return m_yAxisLabel; }
	QColor getAxisColor() const { return m_axisColor; }
	QFont getAxisFont() const { return m_axisFont; }

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
	void setMinimumMargins(const QMargins &margins);

	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);

public:
	static int m_instanceCount;			//ʵ�����

private:
	QCustomPlot *m_customPlot;

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

	void slot_getCurrentSeconds(double secs);

protected:
	virtual void paintEvent(QPaintEvent* event);
	bool eventFilter(QObject *watched, QEvent *event);
};

