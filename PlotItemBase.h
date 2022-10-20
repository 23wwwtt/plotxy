/* * @filename: PlotItemBase.h
* @brief: PlotBase Plot����
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef  _PLOT_ITEM_BASE_H_
#define  _PLOT_ITEM_BASE_H_

#include <QtWidgets/QMainWindow>
#include "ui_PlotItemBase.h"


class PlotItemBase : public QWidget
{
    Q_OBJECT

public:
    explicit PlotItemBase(QWidget* parent = 0);
    ~PlotItemBase();

public:
    void init();                        //��ʼ�������������źŲ�

    void setPosition(int x,int y);      //���ð�Χ�����Ͻ�λ��
    void setWidth(int width);           //���ÿ��
    void setHeight(int height);         //���ð�Χ�и߶�

    QPoint currPosition();
    int currWidth();
    int currHeight();

    void setName(const QString&);
    void setTabName(const QString&);
    
    //void setVisible(bool);            //ʼ��ٸ�ߣ����֮�ף�������һ���޷���ʾ��bug

    QString currName();
    QString currTabName();

	void addPlotPairData(QPair<QString, QString>);
	void delPlotPairData(QPair<QString, QString>);
	QList<QPair<QString, QString>> getPlotPairData();

protected:
    //virtual void paintEvent(QPaintEvent* event);

private:
    QPoint  m_position;
    int m_width;
    int m_height;
    QString m_plotItemName;
    QString m_tabName;
    bool    m_bVisible;
	QList<QPair<QString, QString>> m_plotPairData;

    Ui::PlotItemBase ui;
};

#endif // ! 