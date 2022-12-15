/* * @filename: PlotItemBase.h
* @brief: PlotBase Plot基类
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef  _PLOT_ITEM_BASE_H_
#define  _PLOT_ITEM_BASE_H_

#include <QtWidgets/QMainWindow>
#include "ui_PlotItemBase.h"
#include "DataPair.h"

class PlotItemBase : public QWidget
{
    Q_OBJECT

public:
    explicit PlotItemBase(QWidget* parent = 0);
    ~PlotItemBase();

public:
    void init();                        //初始化函数，连接信号槽

    void setPosition(int x,int y);      //设置包围盒左上角位置
    void setWidth(int width);           //设置宽度
    void setHeight(int height);         //设置包围盒高度
	void setRect(QRect rect);

    QPoint currPosition();
    int currWidth();
    int currHeight();
	QRect currRect();

    void setName(const QString&);
    void setTabName(const QString&);
    
    //void setVisible(bool);            //始作俑者，万恶之首！跟踪了一天无法显示的bug

    QString currName();
    QString currTabName();

	virtual void addPlotPairData(QPair<QString, QString>);
	virtual void delPlotPairData(QPair<QString, QString>);
	virtual void updatePlotPairData(QPair<QString, QString> oldPair, QPair<QString, QString> newPair);
	QList<QPair<QString, QString>> getPlotPairData();
	QVector<DataPair*> getDataPair() { return m_dataPair; }

protected:
    //virtual void paintEvent(QPaintEvent* event);
	QList<QPair<QString, QString>> m_plotPairData;
	QVector<DataPair*> m_dataPair;

public slots:
	void slot_updateRect(QRect);
	void slot_setVisible(bool);

private:
    QPoint  m_position;
    int m_width;
    int m_height;
    QString m_plotItemName;
    QString m_tabName;
    bool    m_bVisible;

    Ui::PlotItemBase ui;
};

#endif // ! 