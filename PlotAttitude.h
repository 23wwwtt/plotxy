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
	QTimer* m_timer;
	int m_currTimeIndex;
	bool m_started;


	QColor m_titleColor;				//������ɫ
	QColor m_border_ColorStart;			//�߿򽥱俪ʼ��ɫ
	QColor m_border_ColorEnd;			//�߿򽥱������ɫ
	QColor m_bgColor;					//������ɫ
	QColor m_scaleColor_roll;			//�̶ȳ���ɫroll
	QColor m_scaleColor_pitch;			//�̶ȳ���ɫpitch
	QColor m_lineColor_roll;			//������ɫroll
	QColor m_lineColor_pitch;			//������ɫpitch
	QColor m_textColor_roll;			//������ɫroll
	QColor m_textColor_pitch;			//������ɫpitch

	QFont m_titleFont;					//��������
	QFont m_scaleFont;					//�̶�����
	QFont m_textFont;					//�ı�����

	QString m_title;					//����
	double m_pitchValue;				//��ת�Ƕ�
	double m_rollValue;					//����ֵ
	int    m_coordNum_roll;				//roll�������
	double m_startAngle_roll;			//roll��ʼ�Ƕ�
	double m_endAngle_roll;				//roll�����Ƕ�
	QString m_unit_roll;				//roll��λ
	int		m_decision_roll;			//rollС���㾫��

	int    m_coordNum_pitch;			//pitch�������
	double m_startAngle_pitch;			//pitch��ʼ�Ƕ�
	double m_endAngle_pitch;			//pitch�����Ƕ�
	QString m_unit_pitch;				//pitch��λ
	int		m_decision_pitch;			//pitchС���㾫��

	double m_topPadding;				//��ͼ���-top
	double m_bottomPadding;				//��ͼ���-bottom
	double m_leftPadding;				//��ͼ���-left
	double m_rightPadding;				//��ͼ���-right
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

	//���ñ�����ɫ
	void slot_setTitleColor(const QColor &titleColor);

	//���ñ�������
	void slot_setTitleFont(const QFont &font);

	//���ÿ̶�����
	void slot_setScaleFont(const QFont &font);

	//�����ı�����
	void slot_setTextFont(const QFont &font);

	//���ñ���
	void slot_setTitle(QString title);

	//������ת�Ƕ�ֵ
	void slot_setPitchValue(double pitchValue);

	//����ǰ����תֵ
	void slot_setRollValue(double rollValue);

	//
	void onTimerout();
	void onSwitch(bool);
};

