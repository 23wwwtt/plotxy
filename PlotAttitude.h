/* * @filename: PlotAttitude.h
* @brief: PlotAttitude�ؼ���
* @author: wangzhen
* @create time: 2022-10-10
*  */

#pragma once
#include "PlotItemBase.h"
#include <QTimer>
class PlotAttitude :
	public PlotItemBase
{
	Q_OBJECT
public:
	PlotAttitude(QWidget* parent = Q_NULLPTR);
	~PlotAttitude();

protected:
	virtual void paintEvent(QPaintEvent* event);
public:
	void drawTitle(QPainter *painter, int radius);
	void drawBorder(QPainter* painted, int radius);
	void drawBg(QPainter *painter, int radius);
	void drawScale_roll(QPainter *painter, int radius);
	void drawScale_pitch(QPainter *painter, int radius);
	void drawLine_roll(QPainter *painter, int radius);
	void drawLine_pitch(QPainter *painter, int radius);
	void drawText_roll(QPainter *painter, int radius);
	void drawText_pitch(QPainter *painter, int radius);
private:
	QColor m_border_ColorStart;			//�߿򽥱俪ʼ��ɫ
	QColor m_border_ColorEnd;			//�߿򽥱������ɫ
	QColor m_bgColor;					//������ɫ
	QColor m_scaleColor_roll;			//�̶ȳ���ɫroll
	QColor m_scaleColor_pitch;			//�̶ȳ���ɫpitch
	QColor m_lineColor_roll;			//������ɫroll
	QColor m_lineColor_pitch;			//������ɫpitch
	QColor m_textColor_roll;			//������ɫroll
	QColor m_textColor_pitch;			//������ɫpitch

	QFont m_scaleFont;					//�̶�����
	QFont m_textFont;					//�ı�����

	double m_pitchValue;				//��ת�Ƕ�
	double m_rollValue;					//����ֵ
	int    m_horzGrids;					//roll�������
	double m_coordBgn_x;				//roll��ʼ�Ƕ�
	double m_coordEnd_x;				//roll�����Ƕ�
	int	   m_decision_roll;				//rollС���㾫��

	int    m_vertGrids;					//pitch�������
	double m_coordBgn_y;				//pitch��ʼ�Ƕ�
	double m_coordEnd_y;				//pitch�����Ƕ�
	int	   m_decision_pitch;			//pitchС���㾫��

	double m_topPadding;				//��ͼ���-top
	double m_bottomPadding;				//��ͼ���-bottom
	double m_leftPadding;				//��ͼ���-left
	double m_rightPadding;				//��ͼ���-right

	QList<double> m_xValueList;
	QList<double> m_yValueList;

public:
	static int m_instanceCount;			//ʵ�����
public:
	QColor getBorderOutColorStart() const;
	QColor getBorderOutColorEnd()   const;
	QColor getBgColor()             const;
	QColor getScaleColor_roll()     const;
	QColor getScaleColor_pitch()    const;
	QColor getLineColor_roll()      const;
	QColor getLineColor_pitch()     const;
	QColor getTextColor_roll()      const;
	QColor getTextColor_pitch()     const;

	float getPitchValue()           const;
	float getRollValue()            const;

	QSize sizeHint()                const;
	QSize minimumSizeHint()         const;

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
	virtual void setTitle(QString& title);
	virtual void setTitleVisible(bool show);
	virtual void setTitleColor(QColor &titleColor);
	virtual void setTitleFillColor(QColor& color);
	virtual void setTitleFont(QFont &font);
	virtual bool unitsShowX() { return m_showUnits_x; }
	virtual bool unitsShowY() { return m_showUnits_y; }
	virtual QString getUnitsX() { return m_units_x; }
	virtual QString getUnitsY() { return m_units_y; }
	virtual bool getTitleVisible() { return m_titleVisible; }
	virtual QString getTitle() { return m_title; }
	virtual QColor getTitleColor() { return m_titleColor; }
	virtual QColor getTitleFillColor() { return m_titleFillColor; }
	virtual QFont getTitleFont() { return m_titleFont; }

	void updateItems();

public slots:
	//���ñ߿򽥱���ɫ
	void slot_setBorderColorStart(const QColor &borderOutColorStart);
	void slot_setBorderColorEnd(const QColor &borderOutColorEnd);

	//���ñ���ɫ
	void slot_setBgColor(const QColor &bgColor);

	//���ÿ̶ȳ���ɫ
	void slot_setScaleColor_roll(const QColor &scaleColor);
	void slot_setScaleColor_pitch(const QColor &scaleColor);

	//����������ɫ
	void slot_setLineColor_roll(const QColor &lineColor);
	void slot_setLineColor_pitch(const QColor &lineColor);

	//����������ɫ
	void slot_setTextColor_roll(const QColor &textColor);
	void slot_setTextColor_pitch(const QColor &textColor);

	//���ÿ̶�����
	void slot_setScaleFont(const QFont &font);

	//�����ı�����
	void slot_setTextFont(const QFont &font);

	//������ת�Ƕ�ֵ
	void slot_setPitchValue(double pitchValue);

	//����ǰ����תֵ
	void slot_setRollValue(double rollValue);

	//���յ�ǰʱ��
	void slot_getCurrentSeconds(double);
};

