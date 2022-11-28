#ifndef PLOTSCATTER_H
#define PLOTSCATTER_H

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QMap>
#include <QString>

class PlotScatter : public PlotItemBase
{
    Q_OBJECT
public:
    PlotScatter(QWidget *parent = nullptr);
    ~PlotScatter();

    static int m_instanceCount;         //Plot实体个数

public slots:
    void onGetCurrentSeconds(double secs);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    void getDataInfo(double secs);
    void updateData(QString xEntityType, QString yEntityType, double secs, QColor color); // 实现核心绘制逻辑
    void drawTargetPoint(double x, double y, QColor color);

private:
    double m_curSeconds;
    QColor m_axisColor{Qt::lightGray};
    QColor m_gridColor{Qt::darkGray};
    double m_contextPadding;
    int m_horTableCnt;  // 横向网格数量
    int m_verTableCnt;  // 纵向网格数量
    int m_itemCnt;

    QList<QColor> m_clrList;
};

#endif // PLOTSCATTER_H
