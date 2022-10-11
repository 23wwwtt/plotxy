#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QFileDialog>

#include "PlotXYDemo.h"
#include "FreeWidgetWraper.h"
#include "PlotItemBase.h"
#include "AdvancedDataManager.h"
#include "PlotManager.h"
#include "AddPlotPair.h"
#include "PlotBar.h"
#include "PlotPolar.h"
#include "DataManager.h"
#include "PlotText.h"

PlotXYDemo::PlotXYDemo(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //m_plotItem = nullptr;
    //m_freeWidgetWraper = nullptr;
    m_plotManager = new PlotManager();
    m_addPlotPair = NULL;
    m_AdvancedDataManager = new AdvancedDataManager();

    setMinimumSize(1600, 1200);
    showMaximized();

    //statusbar���������Ϣ��ʾ
    QLabel* info = new QLabel;
    info->setText(QString::fromLocal8Bit("��ѡ���ͼ�㣺"));
    ui.statusBar->addWidget(info);

    connect(ui.actionAdvanced_Data_Manager, &QAction::triggered, this, &PlotXYDemo::onAdvancedData);
    connect(ui.actionPlot_Manager_Ctrl_M, &QAction::triggered, this, &PlotXYDemo::onPlotManager);
    connect(ui.actionAdd_Plot_Pair_Ctrl_A, &QAction::triggered, this, &PlotXYDemo::onAddPlotPair);
    connect(ui.actionopen, &QAction::triggered, this, &PlotXYDemo::onOpenFile);
    
    //�Ҽ��˵���
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), 
            this, SLOT(onCustomContextMenuRequested(const QPoint&)));

    connect(m_plotManager,SIGNAL(sigAddPlotPair()),this,SLOT(onAddPlotPair()));
}

PlotXYDemo::~PlotXYDemo()
{

}

void PlotXYDemo::onAdvancedData()
{
    m_AdvancedDataManager->show();
}

void PlotXYDemo::onPlotManager()
{
    if (!m_plotManager)
    {
        return;
    }
    m_plotManager->show();
}

void PlotXYDemo::onAddPlotPair()
{
    if (!m_addPlotPair)
    {
        m_addPlotPair = new AddPlotPair();
        connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)), 
            m_plotManager, SLOT(onAddPlotPair(QString, QString)));
        connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)), 
            m_AdvancedDataManager, SLOT(onAdvancedDataManagerAddPair(QString, QString)));

        m_addPlotPair->init();
    }
        
    m_addPlotPair->show();
}

void PlotXYDemo::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, "open File", "/home", tr("Microsoft CSV(*.csv)"));
    DataManager::getInstance()->loadCSV(path);
}

void PlotXYDemo::onCustomContextMenuRequested(const QPoint& point)
{
    QMenu* pMenu = new QMenu(this);

    QAction* addTabPage = new QAction(QString::fromLocal8Bit("���tabҳ��"), this);
    QAction* removeTabPage = new QAction(QString::fromLocal8Bit("ɾ��tabҳ��"), this);
    QAction* renameTabPage = new QAction(QString::fromLocal8Bit("������tabҳ��"), this);

    QAction* addBarPlot = new QAction(QString::fromLocal8Bit("���Bar���"), this);

	QAction* addTextPlot = new QAction(QString::fromLocal8Bit("���Text���"), this);

	QAction* addPolarPlot = new QAction(QString::fromLocal8Bit("��Ӽ��������"), this);


    /* ��Ӳ˵��� */
    pMenu->addAction(addTabPage);
    pMenu->addAction(removeTabPage);
    pMenu->addAction(renameTabPage);

    pMenu->addAction(addBarPlot);

	pMenu->addAction(addTextPlot);

	pMenu->addAction(addPolarPlot);


    /* ���Ӳۺ��� */
    connect(addTabPage, SIGNAL(triggered()), this, SLOT(onAddTabPage()));
    connect(removeTabPage, SIGNAL(triggered()), this, SLOT(onRemoveTabPage()));
    connect(renameTabPage, SIGNAL(triggered()), this, SLOT(onRenameTabPage()));

    connect(addBarPlot, SIGNAL(triggered()), this, SLOT(onAddBarPlot()));

	connect(addTextPlot,SIGNAL(triggered()),this,SLOT(onAddTextPlot()));

	connect(addPolarPlot, SIGNAL(triggered()), this, SLOT(onAddPolarPlot()));


    /* ������Ҽ�����ʾ�˵� */
    pMenu->exec(point);

    /* �ͷ��ڴ� */
    QList<QAction*> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
}

void PlotXYDemo::onAddTabPage()
{
    QWidget* tabWidgetItem = new QWidget();
    int currCount = ui.tabWidget->count();
    QString genTabName = QString("Tab ") + QString::number(currCount+1);
    ui.tabWidget->addTab(tabWidgetItem, genTabName);
}

void PlotXYDemo::onRemoveTabPage()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    ui.tabWidget->removeTab(currTabIndex);
}

void PlotXYDemo::onRenameTabPage()
{
    //todo:
    int currTabIndex = ui.tabWidget->currentIndex();
    ui.tabWidget->setTabText(currTabIndex,QString());
}

void PlotXYDemo::onAddBarPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotBar* plotItem = new PlotBar(ui.tabWidget->currentWidget());
    //bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
    bool res = connect(ui.actionStop, &QAction::triggered, plotItem, &PlotBar::onSwitch);
    res = connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
                 plotItem, &PlotBar::onUpdateColorThresholdMap);

    initWidget(plotItem);

   // �����������ƶ�������
    FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
    m_freeWidgetWraper->setWidget(plotItem);

    m_freeWidgetWraper->setMoveEnable(true);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();

    m_plotManager->addPlot(currTabText, plotItem); //tabҳ���ܱ��������bug
}
void PlotXYDemo::onAddTextPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotText* plotItem = new PlotText(ui.tabWidget->currentWidget());
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	connect(ui.actionStop, &QAction::triggered, plotItem, &PlotText::onSwitch);
	connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
		plotItem, &PlotText::onUpdateColorThresholdMap);
	// ���������ݴ����йصĶ���

	initWidget(plotItem);

	// �����������ƶ�������
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_plotManager->addPlot(currTabText, plotItem); //tabҳ���ܱ��������bug
}

void PlotXYDemo::onAddPolarPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotPolar* plotItem = new PlotPolar(ui.tabWidget->currentWidget());
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	bool res = connect(ui.actionStop, &QAction::triggered, plotItem, &PlotPolar::onSwitch);
	res = connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
		plotItem, &PlotPolar::onUpdateColorThresholdMap);

	initWidget(plotItem);
	plotItem->initPlot();
	// �����������ƶ�������
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_plotManager->addPlot(currTabText, plotItem); 

}

void PlotXYDemo::init()
{
}


void PlotXYDemo::initWidget(QWidget* w)
{
    //�����ޱ߿�����
    w->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Widget);
    //w->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    //w->setAttribute(Qt::WA_ShowModal,true);
    w->setMinimumSize(200, 150);
    w->resize(1600,800);

    //�����±�����ɫ����
    QPalette palette = w->palette();
    palette.setBrush(QPalette::Window, QColor(255,255,255));
    w->setPalette(palette);

    QPushButton* btn = new QPushButton(w);
    btn->setText(QString::fromLocal8Bit("�ر�"));
    btn->setGeometry(10, 10, 130, 25);
    connect(btn, SIGNAL(clicked(bool)), w, SLOT(close()));

}

