#ifndef  _PLOT_TRACK_H_
#define  _PLOT_TRACK_H_

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QTimer>
#include <QMap>
#include <QString>

class PlotTrack : public PlotItemBase
{
    Q_OBJECT
public:
    enum TrackStatus {
        Available,      // ��Ч�ҿ���
        Invalid,        // ��Ч����
        Unavailable     // ��Ч������
    };

    PlotTrack(QWidget *parent = Q_NULLPTR);
    ~PlotTrack();

    void setLeftPadding(int);
    void setRightPadding(int);
    void setInterPadding(int);

    void updateItems(); //ʵ�ֺ��Ļ����߼�

    void drawRect(int itemIndex, TrackStatus status, int leftBoundary, int rightBoundary, QColor color);
public:
    static int m_instanceCount;         //ʵ�����

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
    void onGetCurrentSeconds(double secs);

protected:
    virtual void paintEvent(QPaintEvent *event);
    void getDataInfo(double secs);
    void updateData(int itemIndex, QString entityType, double secs);

private:
    QLine   m_xAxis;
    QLine   m_yAxis;

    int m_leftPadding;
    int m_rightPadding;
    int m_interPadding;
    int m_verPadding;

    //QMap<QString,QString> m_plotDataPair;   //ע��ʵ�����ͣ�ʵ������   //���ﲻ����map
    QStringList m_entityTypeList;
    QStringList m_entityAttrList;

    QMap<QString, QMap<int, QColor>> m_thresholdColorMap;    //key:entityType+entityAttr, threshold,QColor
    QColor m_defaultColor;

    QColor m_gridColor;
    QColor m_gridFillColor;
    QColor m_axisColor;

    int m_horiGridNum;
    int m_verGridNum;
    double m_seconds;
    int m_itemCnt;
};

#endif // _PLOT_TRACK_H_
