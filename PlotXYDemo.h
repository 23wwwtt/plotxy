#ifndef  _PLOTXY_DEMO_H_
#define  _PLOTXY_DEMO_H_

#include <QtWidgets/QMainWindow>

#include "ui_PlotXYDemo.h"

class FreeWidgetWraper;
class PlotItemBase;
class PlotManager;
class AddPlotPair;
class AdvancedDataManager;

class PlotXYDemo : public QMainWindow
{
    Q_OBJECT

public:
    PlotXYDemo(QWidget* parent = Q_NULLPTR);
    ~PlotXYDemo();

    void init();            //��ʼ�������������źŲ�
    void initWidget(QWidget* w);

public slots:
    void onAdvancedData();
    void onPlotManager();
    void onAddPlotPair();
    void onOpenFile();

    void onCustomContextMenuRequested(const QPoint&);

    void onAddTabPage();
    void onRemoveTabPage();
    void onRenameTabPage();

    void onAddBarPlot();

	void onAddTextPlot();

	void onAddPolarPlot();


private:
    Ui::PlotXYDemo ui;

    //FreeWidgetWraper* m_freeWidgetWraper;
    PlotManager* m_plotManager;
    AddPlotPair* m_addPlotPair;
    AdvancedDataManager* m_AdvancedDataManager;
};

#endif // ! 