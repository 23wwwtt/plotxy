#ifndef PLOTDIAL_H
#define PLOTDIAL_H

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QTimer>
#include <QMap>
#include <QString>

class PlotDial : public PlotItemBase
{
    Q_OBJECT

public:
    PlotDial(QWidget *parent = Q_NULLPTR);
    ~PlotDial();

    //更新指针
    void updatePointer(double secs);
    static int m_instanceCount;         //实体个数
public slots:
    void onGetCurrentSeconds(double secs);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    bool m_bThinStyle;

    QString m_currEntityType;
    QString m_currEntityAttr;

    double m_seconds;

    QColor m_dialColor;
    QColor m_pointColor;

    QPoint m_centerPoint;
    int m_circleRadius;
    int m_startAngle;
    int m_endAngle;
    int m_contentPadding;
};

#endif // PLOTDIAL_H
