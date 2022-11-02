#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QFileDialog>
#include <QMetaType>

#include "PlotXYDemo.h"
#include "FreeWidgetWraper.h"
#include "PlotItemBase.h"
#include "AdvancedDataManager.h"
#include "PlotManager.h"
#include "AddPlotPair.h"
#include "PlotBar.h"
#include "PlotPolar.h"
#include "DataManager.h"
#include "PlotAttitude.h"
#include "PlotText.h"
#include "PlotLight.h"


PlotXYDemo::PlotXYDemo(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //m_plotItem = nullptr;
    //m_freeWidgetWraper = nullptr;
	m_AdvancedDataManager = new AdvancedDataManager();
    m_plotManager = new PlotManager();
	m_addPlotPair = AddPlotPair::m_getInstance();
	m_timeCtrl = new TimeControls();
	connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)),m_plotManager, SLOT(onAddPlotPair(QString, QString)));
	connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)),m_AdvancedDataManager, SLOT(onAdvancedDataManagerAddPair(QString, QString)));
	connect(this, SIGNAL(sgn_loadDataReady()), m_addPlotPair, SLOT(onUpdateData()));

	m_curBaseInfo.Base_TabName = nullptr;
	m_curBaseInfo.Base_PlotName = nullptr;
	qRegisterMetaType<BaseInfo>("BaseInfo");

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
	connect(ui.actionTime_Controls, &QAction::triggered, this, &PlotXYDemo::onTimeControl);

	connect(m_plotManager, SIGNAL(sigAddPlotPair()), this, SLOT(onAddPlotPair()));
    
    //�Ҽ��˵���
	ui.tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tabWidget->tabBar(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenuRequested(const QPoint&)));
	ui.tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tabWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
    
	m_nowFocusWidget = new QWidget(this);
	connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(onFocusChanged(QWidget*, QWidget*)));

	m_lastSelectedType = PlotType::Type_PlotScatter;
}

PlotXYDemo::~PlotXYDemo()
{
	if(m_plotManager)
		m_plotManager->deleteLater();
	if(m_addPlotPair)
		m_addPlotPair->deleteLater();
	if(m_AdvancedDataManager)
		m_AdvancedDataManager->deleteLater();
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
		m_addPlotPair = AddPlotPair::m_getInstance();
        connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)), 
            m_plotManager, SLOT(onAddPlotPair(QString, QString)));
        connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)), 
            m_AdvancedDataManager, SLOT(onAdvancedDataManagerAddPair(QString, QString)));
		connect(this, SIGNAL(sgn_loadDataReady()), m_addPlotPair, SLOT(onUpdateData()));
        m_addPlotPair->init(getCurrentFocusPlot());
    }
	m_addPlotPair->onChangeStackIndex(m_lastSelectedType);
	m_addPlotPair->setPlotBaseInfo(m_curBaseInfo);
    m_addPlotPair->show();
}

void PlotXYDemo::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, "open File", "/home", tr("Microsoft CSV(*.csv)"));
    DataManager::getInstance()->loadCSV(path);

	sgn_loadDataReady();
}

void PlotXYDemo::onTimeControl()
{
	m_timeCtrl->show();
}

void PlotXYDemo::onCustomContextMenuRequested(const QPoint& point)
{
	qDebug() << sender();
    QMenu* pMenu = new QMenu(this);

    QAction* addTabPage = new QAction(QString::fromLocal8Bit("���tabҳ��"), this);
    QAction* removeTabPage = new QAction(QString::fromLocal8Bit("ɾ��tabҳ��"), this);
    QAction* renameTabPage = new QAction(QString::fromLocal8Bit("������tabҳ��"), this);

    /* ��Ӳ˵��� */
    pMenu->addAction(addTabPage);
    pMenu->addAction(removeTabPage);
    pMenu->addAction(renameTabPage);

    /* ���Ӳۺ��� */
    connect(addTabPage, SIGNAL(triggered()), this, SLOT(onAddTabPage()));
    connect(removeTabPage, SIGNAL(triggered()), this, SLOT(onRemoveTabPage()));
    connect(renameTabPage, SIGNAL(triggered()), this, SLOT(onRenameTabPage()));

    /* ������Ҽ�����ʾ�˵� */
    pMenu->exec(QCursor::pos());

    /* �ͷ��ڴ� */
    QList<QAction*> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
}

void PlotXYDemo::onContextMenu(const QPoint& point)
{
	getCurrentFocusPlot();

	QWidget* subWidget = QApplication::widgetAt(QCursor::pos().x(), QCursor::pos().y());
	QString name = subWidget->objectName();
	if (name == "PlotItemBase")
	{
		name = dynamic_cast<PlotItemBase*>(subWidget)->currName();
	}
	else
	{
		name = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
	}
		
	QMenu* pMenu = new QMenu(this);
	//QWidgetAction
	QWidgetAction* object_action = new QWidgetAction(this);
	QLabel* label = new QLabel(name);
	label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	label->setStyleSheet("color:rgb(0,128,0);font:Bold");
	object_action->setDefaultWidget(label);

	//createPlot�˵�
	QMenu* createPlotMenu = new QMenu(QString::fromLocal8Bit("������ͼ���"));
	QAction* addBarPlot = new QAction(QString::fromLocal8Bit("���Bar���"), this);
	QAction* addAttitudePlot = new QAction(QString::fromLocal8Bit("���Attitude���"), this);
	QAction* addTextPlot = new QAction(QString::fromLocal8Bit("���Text���"), this);
	QAction* addPolarPlot = new QAction(QString::fromLocal8Bit("���Polar���"), this);
	QAction* addLightPlot = new QAction(QString::fromLocal8Bit("���Light���"), this);
	/* ��Ӳ˵��� */
	createPlotMenu->addAction(addBarPlot);
	createPlotMenu->addAction(addAttitudePlot);
	createPlotMenu->addAction(addTextPlot);
	createPlotMenu->addAction(addPolarPlot);
	createPlotMenu->addAction(addLightPlot);
	/* ���Ӳۺ��� */
	connect(addBarPlot, SIGNAL(triggered()), this, SLOT(onAddBarPlot()));
	connect(addAttitudePlot, SIGNAL(triggered()), this, SLOT(onAddAttitudePlot()));
	connect(addTextPlot, SIGNAL(triggered()), this, SLOT(onAddTextPlot()));
	connect(addPolarPlot, SIGNAL(triggered()), this, SLOT(onAddPolarPlot()));
	connect(addLightPlot, SIGNAL(triggered()), this, SLOT(onAddLightPlot()));

	//QAction
	QAction* Undo_Action = new QAction(QString::fromLocal8Bit("����"), this);
	QAction* Cut_Action = new QAction(QString::fromLocal8Bit("����"), this);
	QAction* Copy_Action = new QAction(QString::fromLocal8Bit("����"), this);
	QAction* Paste_Action = new QAction(QString::fromLocal8Bit("ճ��"), this);
	QAction* Delete_Action = new QAction(QString::fromLocal8Bit("ɾ��"), this);

	QMenu* autofitMenu = new QMenu(QString::fromLocal8Bit("�Զ���Ӧ��С"));
	QAction* autofit_x = new QAction(QString::fromLocal8Bit("X"), this);
	QAction* autofit_y = new QAction(QString::fromLocal8Bit("Y"), this);
	autofitMenu->addAction(autofit_x);
	autofitMenu->addAction(autofit_y);

	QAction* One2One_Action = new QAction(QString::fromLocal8Bit("One-To-One"), this);
	QAction* RoundRanges_Action = new QAction(QString::fromLocal8Bit("Round Ranges"), this);

	QAction* PlotManager_Action = new QAction(QString::fromLocal8Bit("��ͼ������"), this);
	QAction* WidgetEditor_Action = new QAction(QString::fromLocal8Bit("�ؼ��༭"), this);
	QAction* DataManager_Action = new QAction(QString::fromLocal8Bit("�߼����ݹ�����"), this);
	QAction* PlotPair_Action = new QAction(QString::fromLocal8Bit("������ݶ�"), this);
	QAction* Screenshot_Action = new QAction(QString::fromLocal8Bit("�����ͼ"), this);

	connect(PlotManager_Action, SIGNAL(triggered()), this, SLOT(onPlotManager()));
	connect(DataManager_Action, SIGNAL(triggered()), this, SLOT(onAdvancedData()));
	connect(PlotPair_Action, SIGNAL(triggered()), this, SLOT(onAddPlotPair()));
	

	QAction* GOG_Action = new QAction(QString::fromLocal8Bit("����GOG"), this);
	QAction* HDF5_Action = new QAction(QString::fromLocal8Bit("����HDF5"), this);

	QMenu* OrderMenu = new QMenu(QString::fromLocal8Bit("Order"));
	QMenu* ViewMenu = new QMenu(QString::fromLocal8Bit("View"));
	QMenu* SelectPlotMenu = new QMenu(QString::fromLocal8Bit("Select Plot"));
	//���˵�
	pMenu->addAction(object_action);
	pMenu->addSeparator();
	pMenu->addMenu(createPlotMenu);
	pMenu->addSeparator();
	pMenu->addAction(Undo_Action);
	pMenu->addSeparator();
	pMenu->addAction(Cut_Action);
	pMenu->addAction(Copy_Action);
	pMenu->addAction(Paste_Action);
	pMenu->addAction(Delete_Action);
	pMenu->addSeparator();
	pMenu->addMenu(autofitMenu);
	pMenu->addAction(One2One_Action);
	pMenu->addAction(RoundRanges_Action);
	pMenu->addSeparator();
	pMenu->addAction(PlotManager_Action);
	pMenu->addAction(WidgetEditor_Action);
	pMenu->addAction(DataManager_Action);
	pMenu->addAction(PlotPair_Action);
	pMenu->addAction(Screenshot_Action);
	pMenu->addSeparator();
	pMenu->addAction(GOG_Action);
	pMenu->addAction(HDF5_Action);
	pMenu->addSeparator();
	pMenu->addMenu(OrderMenu);
	pMenu->addMenu(ViewMenu);
	pMenu->addMenu(SelectPlotMenu);

	/* ������Ҽ�����ʾ�˵� */
	pMenu->exec(QCursor::pos());
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
	plotItem->setTabName(currTabText);
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

	m_lastSelectedType = PlotType::Type_PlotBar;
    m_plotManager->addPlot(currTabText, plotItem);
	m_addPlotPair->onAddPlot(currTabText, plotItem);
}
void PlotXYDemo::onAddTextPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotText* plotItem = new PlotText(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	connect(ui.actionStop, &QAction::triggered, plotItem, &PlotText::onSwitch);
	//connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
	//	plotItem, &PlotText::onUpdateColorThresholdMap);
	// ���������ݴ����йصĶ���

	initWidget(plotItem);

	// �����������ƶ�������
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_lastSelectedType = PlotType::Type_PlotText;
	m_plotManager->addPlot(currTabText, plotItem);
	m_addPlotPair->onAddPlot(currTabText, plotItem);
}

void PlotXYDemo::onAddLightPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotLight* plotItem = new PlotLight(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	//connect(ui.actionStop, &QAction::triggered, plotItem, &PlotText::onSwitch);
	//connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
	//	plotItem, &PlotText::onUpdateColorThresholdMap);


	initWidget(plotItem);

	// �����������ƶ�������
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_lastSelectedType = PlotType::Type_PlotLight;
	m_plotManager->addPlot(currTabText, plotItem);
	m_addPlotPair->onAddPlot(currTabText, plotItem);
}

void PlotXYDemo::onAddPolarPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotPolar* plotItem = new PlotPolar(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	bool res = connect(ui.actionStop, &QAction::triggered, plotItem, &PlotPolar::onSwitch);
	res = connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
		plotItem, &PlotPolar::onUpdateColorThresholdMap);

	initWidget(plotItem);
	// �����������ƶ�������
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_lastSelectedType = PlotType::Type_PlotPolar;
	m_plotManager->addPlot(currTabText, plotItem); 
	m_addPlotPair->onAddPlot(currTabText, plotItem);

}

void PlotXYDemo::onFocusChanged(QWidget * oldWidget, QWidget * newWidget)
{
	m_nowFocusWidget = newWidget;
}

void PlotXYDemo::onAddAttitudePlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotAttitude* plotItem = new PlotAttitude(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
	connect(ui.actionStop, &QAction::triggered, plotItem, &PlotAttitude::onSwitch);
	initWidget(plotItem);

	// �����������ƶ�������
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();
	plotItem->update();

	m_lastSelectedType = PlotType::Type_PlotAttitude;
	m_plotManager->addPlot(currTabText, plotItem);
	m_addPlotPair->onAddPlot(currTabText, plotItem);	
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
	w->setAutoFillBackground(true);
    w->setMinimumSize(200, 150);
    w->resize(1600,800);

    //�����±�����ɫ����
    QPalette palette = w->palette();
    palette.setColor(QPalette::Window, QColor(0,0,0));
    w->setPalette(palette);

    QPushButton* btn = new QPushButton(w);
    btn->setText(QString::fromLocal8Bit("�ر�"));
    btn->setGeometry(10, 10, 130, 25);
    connect(btn, SIGNAL(clicked(bool)), w, SLOT(close()));
}

PlotType PlotXYDemo::getCurrentFocusPlot()
{
	QWidget* subWidget = QApplication::widgetAt(QCursor::pos().x(), QCursor::pos().y());
	QString name = subWidget->metaObject()->className();

	QString objname = subWidget->objectName();
	if (objname == "PlotItemBase")
	{
		m_curBaseInfo.Base_PlotName = dynamic_cast<PlotItemBase*>(subWidget)->currName();
		m_curBaseInfo.Base_TabName = dynamic_cast<PlotItemBase*>(subWidget)->currTabName();
	}
	else
	{
		m_curBaseInfo.Base_PlotName = nullptr;
		m_curBaseInfo.Base_TabName = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
	}

	if (name.compare("PlotPlotScatter") == 0)
	{
		m_lastSelectedType =  PlotType::Type_PlotScatter;
	}
	else if (name.compare("PlotAScope") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotAScope;
	} 
	else if (name.compare("PlotRTI") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotRTI;
	}
	else if (name.compare("PlotText") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotText;
	}
	else if (name.compare("PlotLight") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotLight;
	}
	else if (name.compare("PlotBar") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotBar;
	}
	else if (name.compare("PlotDial") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotDial;
	}
	else if (name.compare("PlotAttitude") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotAttitude;
	}
	else if (name.compare("PlotPolar") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotPolar;
	}
	else if (name.compare("PlotTrack") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotTrack;
	}
	else if (name.compare("PlotDoppler") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotDoppler;
	}
	else
		m_lastSelectedType = PlotType::Type_PlotScatter;

	return m_lastSelectedType;
}


