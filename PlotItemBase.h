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
#include "constdef.h"

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

	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);
	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual uint getHorzGrids() { return m_horzGrids; }
	virtual uint getVertGrids() { return m_vertGrids; }
	virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);
	virtual uint getAxisWidth() { return m_axisWidth; }
	virtual uint getGridWidth() { return m_gridWidth; }
	virtual QColor getAxisColor() { return m_axisColor; }
	virtual QColor getGridColor() { return m_gridColor; }
	virtual void setGridVisible(bool enable);
	virtual void setTickLabelColor(QColor& color);
	virtual void setTickLabelFont(QFont& font);
	virtual void setTickLabelFontSize(int size);
	virtual void setGridStyle(GridStyle style);
	virtual void setGridDensity(GridDensity density);
	virtual bool getGridVisible() { return m_gridVisible; }
	virtual QColor getTickLabelColor() { return m_tickLabelColor; }
	virtual QFont getTickLabelFont() { return m_tickLabelFont; }
	virtual int getTickLabelFontSize() { return m_tickLabelFontSize; }
	virtual Qt::PenStyle getGridStyle() { return m_gridStyle; }
	virtual GridDensity getGridDensity() { return m_gridDensity; }

	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
	virtual void setTitleVisible(bool on);
	virtual void setTitle(QString& title);
	virtual void setTitleColor(QColor& color);
	virtual void setTitleFillColor(QColor& color);
	virtual void setTitleFont(QFont& font);
	virtual void setTitleFontSize(int size);
	virtual bool unitsShowX() { return m_showUnits_x; }
	virtual bool unitsShowY() { return m_showUnits_y; }
	virtual QString getUnitsX() { return m_units_x; }
	virtual QString getUnitsY() { return m_units_y; }
	virtual bool getTitleVisible() { return m_titleVisible; }
	virtual QString getTitle() { return m_title; }
	virtual QColor getTitleColor() { return m_titleColor; }
	virtual QColor getTitleFillColor() { return m_titleFillColor; }
	virtual QFont getTitleFont() { return m_titleFont; }
	virtual int getTitleFontSize() { return m_titleFontSize; }

	virtual void addPlotPairData(QPair<QString, QString>);
	virtual void delPlotPairData(QPair<QString, QString>);
	virtual void updatePlotPairData(QPair<QString, QString> oldPair, QPair<QString, QString> newPair);
	QList<QPair<QString, QString>> getPlotPairData();
	QVector<DataPair*> getDataPair() { return m_dataPair; }

protected:
    //virtual void paintEvent(QPaintEvent* event);
	QList<QPair<QString, QString>> m_plotPairData;
	QVector<DataPair*> m_dataPair;

	//Axis and Grid
	double  m_coordBgn_x;	//x坐标起始值
	double  m_coordEnd_x;	//x坐标结束值
	double  m_coordBgn_y;	//y坐标起始值
	double  m_coordEnd_y;	//y坐标结束值
	uint	m_horzGrids;
	uint	m_vertGrids;
	uint	m_axisWidth;
	uint	m_gridWidth;
	QColor	m_axisColor;
	QColor	m_gridColor;
	bool	m_gridVisible;
	QColor	m_tickLabelColor;
	QFont	m_tickLabelFont;
	int		m_tickLabelFontSize;
	Qt::PenStyle m_gridStyle;
	GridDensity m_gridDensity;

	//Text
	QString m_units_x;
	QString m_units_y;
	bool	m_showUnits_x;
	bool	m_showUnits_y;
	QString m_title;
	bool	m_titleVisible;
	QColor	m_titleColor;
	QFont	m_titleFont;
	int		m_titleFontSize;
	QColor	m_titleFillColor;

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