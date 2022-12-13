#include "PlotManager.h"
#include "addplotpair.h"
#include <QColorDialog>
#include <QTreeWidget>
#include <QDebug>
#include <QAction>
#include <QString>
#include <QStyleFactory>
#include "PlotItemBase.h"

//#include "PlotBar.h"

PlotManager::PlotManager(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	connect(ui.pushButton_close, &QPushButton::clicked, this, &PlotManager::onBtnCloseClicked);
	this->setWindowTitle(QString::fromLocal8Bit("图表管理器"));
	init();
	ui.treeWidget_selectedPlots->setStyle(QStyleFactory::create("windows"));
	ui.treeWidget_selectedPlots->setHeaderHidden(true);
	ui.treeWidget_selectedPlots->expandAll();
	
// 	m_itemGOG->setDisabled(true); 
// 	m_itemScatterPlot->setDisabled(true);
// 	m_itemAScope->setDisabled(true);
// 	m_itemRTI->setDisabled(true);
// 	m_itemTextLight->setDisabled(true);
// 	m_itemBar->setDisabled(true);
// 	m_itemDial->setDisabled(true);
// 	m_itemAttitude->setDisabled(true);
// 	m_itemTrackStatus->setDisabled(true);
// 	m_itemRangeDoppler->setDisabled(true);

	ui.stackedWidget->setCurrentIndex(0);
	ui.treeWidget_settings->setCurrentItem(m_itemGeneral);
	m_itemScatterPlot->setExpanded(true);

	/*ui.textEdit->setFixedHeight(24);
	ui.textEdit_2->setFixedHeight(24);
	ui.pushButton_10->setFixedSize(21, 21);
	ui.pushButton_21->setFixedSize(21, 21);
	ui.pushButton_22->setFixedSize(21, 21);
	ui.pushButton_23->setFixedSize(21, 21);*/

	connect(ui.treeWidget_settings, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTWSclicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget_selectedPlots, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTWSPclicked(QTreeWidgetItem*, int)));
	
	connect(ui.pushButton_addNew, SIGNAL(clicked()), this, SLOT(onAddNewClicked()));

	connect(ui.spinBox_between, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxBetweenChanged);
	connect(ui.spinBox_left, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxLeftChanged);
	connect(ui.spinBox_right, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxRightChanged);

	connect(ui.lineEdit_hrozGrids, &QLineEdit::textChanged, this, [=]() {
		m_hrozGrids = ui.lineEdit_hrozGrids->text().toInt();
		});
	connect(ui.lineEdit_vertGrids, &QLineEdit::textChanged, this, [=]() {
		m_vertGrids = ui.lineEdit_vertGrids->text().toInt();
		});

// 	ui.pushButton_axisColor->setFixedSize(21, 21);
// 	ui.pushButton_gridColor->setFixedSize(21, 21);
// 	ui.pushButton_gridFill->setFixedSize(21, 21);
// 	ui.pushButton_10->setFixedSize(21, 21);
// 	ui.pushButton_21->setFixedSize(21, 21);
// 	ui.pushButton_22->setFixedSize(21, 21);
// 	ui.pushButton_23->setFixedSize(21, 21);

	

// 	QFontDatabase FontDb;
// 	foreach(int size, FontDb.standardSizes()) {
// 		ui.comboBox_AxisGrid_FontSize->addItem(QString::number(size));
// 	}
}

PlotManager::~PlotManager()
{

}


void PlotManager::init()
{
	initTreeWidgetSettings();
	initGeneralUI();
}

void PlotManager::addPlot(const QString& tabName, PlotItemBase* plotItem)
{
	//显示层更新
	if (m_plotManager.contains(tabName))
	{
		QList<QTreeWidgetItem*> topWidget = ui.treeWidget_selectedPlots->findItems(tabName, Qt::MatchCaseSensitive, 0);
		if (topWidget.size() != 0)
		{
			QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotItem->currName());
			topWidget[0]->addChild(itemselPlotI);
		}
	} 
	else
	{
		QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList()<<tabName);
		ui.treeWidget_selectedPlots->addTopLevelItem(itemselPlotH);
		ui.treeWidget_selectedPlots->expandAll();

		QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotItem->currName());
		itemselPlotH->addChild(itemselPlotI);

		//comboBox_tabName
		ui.comboBox_tabName->addItem(tabName);
	}

	//数据层更新
	m_plotManager[tabName].append(plotItem);
}

void PlotManager::initTreeWidgetSettings()
{
	ui.treeWidget_settings->setHeaderHidden(false);
	ui.treeWidget_settings->setHeaderLabel(QString::fromLocal8Bit("设置"));
	ui.treeWidget_settings->setIndentation(15);

	m_itemGeneral = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("总体设置")));
	m_itemAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("坐标轴和网格设置")));
	m_itemPlotData = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("数据设置")));
	m_itemText = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("文本信息")));
	m_itemGOG = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("GOG曲线")));
	m_itemLinkedAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("相关的轴")));
	m_itemScatterPlot = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Scatter设置")));
	m_itemAScope = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("A-Scope设置")));
	m_itemRTI = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("RTI设置")));
	m_itemTextLight = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Text/Light设置")));

	m_itemBar = new QTreeWidgetItem;
	m_itemBar->setText(0, QString::fromLocal8Bit("Bar设置"));
	ui.treeWidget_settings->addTopLevelItem(m_itemBar);

	m_itemDial = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Dials设置")));
	m_itemAttitude = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Attitude设置")));
	m_itemTrackStatus = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Track Status设置")));
	m_itemRangeDoppler = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Range Doppler设置")));

	m_itemLimits = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("限制")));
	m_itemPlotMarkers = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("标记")));
	m_itemTimeLine = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Time Line"));
	m_itemHandsOff = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Hands-Off"));
}

void PlotManager::initGeneralUI()
{
	ui.radioButton_percent->setChecked(true);
	m_radioPixelChecked = false;
	connect(ui.radioButton_percent, &QRadioButton::clicked, this, &PlotManager::onRadioPercentClicked);
	connect(ui.radioButton_pixel, &QRadioButton::clicked, this, &PlotManager::onRadioPixelClicked);
	connect(ui.lineEdit_plotPositionX, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinishing);
	connect(ui.lineEdit_plotPositionY, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinishing);
	connect(ui.lineEdit_plotWidth, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinishing);
	connect(ui.lineEdit_plotHeight, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinishing);
}

void PlotManager::refreshGeneralUI(PlotItemBase * plot)
{
	ui.lineEdit_plotName->setText(plot->currName());
	ui.comboBox_tabName->setCurrentText(plot->currTabName());
	if (ui.radioButton_pixel->isChecked())
	{
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(plot->currPosition().x()));
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(plot->currPosition().y()));
		ui.lineEdit_plotWidth->setText(QString("%1").arg(plot->currWidth()));
		ui.lineEdit_plotHeight->setText(QString("%1").arg(plot->currHeight()));

	}
	else if (ui.radioButton_percent->isChecked())
	{
		ui.lineEdit_plotPositionX->setText(QString("%1").arg((float)plot->currPosition().x() / m_tabWidgetRect.width()));
		ui.lineEdit_plotPositionY->setText(QString("%1").arg((float)plot->currPosition().y() / m_tabWidgetRect.height()));
		ui.lineEdit_plotWidth->setText(QString("%1").arg((float)plot->currWidth() / m_tabWidgetRect.width()));
		ui.lineEdit_plotHeight->setText(QString("%1").arg((float)plot->currHeight() / m_tabWidgetRect.height()));
	}
}

void PlotManager::onTWSPclicked(QTreeWidgetItem* item, int i)
{
	QTreeWidgetItem *parent = item->parent();
	if (NULL == parent)
		return;

	QString parent_text = parent->text(0);
	QString child_text = item->text(i);

	if (m_plotManager.contains(parent_text))
	{
		for (int i = 0; i < m_plotManager[parent_text].size(); ++i)
		{
			PlotItemBase *tempPlot = m_plotManager[parent_text].at(i);
			if (child_text == tempPlot->currName())
			{
				//general界面
				refreshGeneralUI(tempPlot);

				//plotPair界面
				ui.treeWidget_4->clear();
				QList<QPair<QString, QString>> plotPairData = tempPlot->getPlotPairData();
				for (int k = 0; k < plotPairData.size(); ++k)
				{
					//界面更新
					QTreeWidgetItem* addplotItem = new QTreeWidgetItem;
					addplotItem->setText(0, plotPairData[k].first);
					addplotItem->setText(1, plotPairData[k].second);

					ui.treeWidget_4->addTopLevelItem(addplotItem);
				}

				break;
			}
		}
	}

	/*if (child_text == "Bar")
	{
		m_itemBar->setDisabled(false);
	}*/
}

void PlotManager::onAddNewClicked()
{
	/*AddPlotPair* addPlotPair = new AddPlotPair();
	addPlotPair->show();*/

	emit sigAddPlotPair();
}


void PlotManager::onTWSclicked(QTreeWidgetItem* item, int i)
{
	QString compare;
	compare = item->text(i);

	if (item->isDisabled() == true)
	{
		return;
	}
	else
	{
		if (compare == QString::fromLocal8Bit("总体设置"))
		{
			ui.stackedWidget->setCurrentIndex(0);
		}
		else if (compare == QString::fromLocal8Bit("坐标轴和网格设置"))
		{
			ui.stackedWidget->setCurrentIndex(1);
		}
		else if (compare == QString::fromLocal8Bit("数据设置"))
		{
			ui.stackedWidget->setCurrentIndex(2);
		}
		else if (compare == QString::fromLocal8Bit("文本信息"))
		{
			ui.stackedWidget->setCurrentIndex(3);
		}
		else if (compare == QString::fromLocal8Bit("GOG曲线"))
		{
			ui.stackedWidget->setCurrentIndex(4);
		}
		else if (compare == QString::fromLocal8Bit("相关的轴"))
		{
			ui.stackedWidget->setCurrentIndex(5);
		}
		else if (compare == QString::fromLocal8Bit("Scatter设置"))
		{
			ui.stackedWidget->setCurrentIndex(6);
		}
		else if (compare == QString::fromLocal8Bit("限制"))
		{
			ui.stackedWidget->setCurrentIndex(6);
		}
		else if (compare == QString::fromLocal8Bit("标记"))
		{
			ui.stackedWidget->setCurrentIndex(7);
		}
		else if (compare == QString::fromLocal8Bit("Time Line"))
		{
			ui.stackedWidget->setCurrentIndex(8);
		}
		else if (compare == QString::fromLocal8Bit("Hands-Off"))
		{
			ui.stackedWidget->setCurrentIndex(9);
		}
		else if (compare == QString::fromLocal8Bit("A-Scope设置"))
		{
			ui.stackedWidget->setCurrentIndex(10);
		}
		else if (compare == QString::fromLocal8Bit("RTI设置"))
		{
			ui.stackedWidget->setCurrentIndex(11);
		}
		else if (compare == QString::fromLocal8Bit("Text/Light设置"))
		{
			ui.stackedWidget->setCurrentIndex(12);
		}
		else if (compare == QString::fromLocal8Bit("Bar设置"))
		{
			ui.stackedWidget->setCurrentIndex(13);
		}
		else if (compare == QString::fromLocal8Bit("Dials设置"))
		{
			ui.stackedWidget->setCurrentIndex(14);
		}
		else if (compare == QString::fromLocal8Bit("Attitude设置"))
		{
			ui.stackedWidget->setCurrentIndex(15);
		}
		else if (compare == QString::fromLocal8Bit("Track Status设置"))
		{
			ui.stackedWidget->setCurrentIndex(16);
		}
		else if (compare == QString::fromLocal8Bit("Range Doppler设置"))
		{
			ui.stackedWidget->setCurrentIndex(17);
		}
	}
}

void PlotManager::spinboxBetweenChanged()
{
	m_spinBoxBetween = ui.spinBox_between->value();
}
void PlotManager::spinboxLeftChanged()
{
	m_spinBoxLeft = ui.spinBox_left->value();
}
void PlotManager::spinboxRightChanged()
{
	m_spinBoxRight = ui.spinBox_right->value();
}


// void PlotManager::onAddPlotPair(QString entityType, QString entityAttr)
// {
// 	//界面更新
// 	QTreeWidgetItem* addplotItem = new QTreeWidgetItem;
// 	addplotItem->setText(0, entityType + " " + entityAttr);
// 	addplotItem->setText(1, "Time");
// 
// 	ui.treeWidget_4->addTopLevelItem(addplotItem);
// 
// 	//数据层更新 ->更新对应Item数据
// 	// tmpCode
// 	//获取当前plotBar控件
// 	if (m_plotManager.isEmpty())
// 		return;
// 
// 	auto it = m_plotManager.begin();
// 	PlotItemBase* currItem = it.value().at(0);
// 	if (currItem)
// 	{
// 		PlotBar* currBarItem = dynamic_cast<PlotBar*>(currItem);
// //		currBarItem->addPlotDataPair(entityType, entityAttr);
// 	}
// }
// 
// void PlotManager::onAddPlotPair(QString tabName, QString plotName, QString xColumn, QString yColumn)
// {
// 	//界面更新
// 	QTreeWidgetItem* addplotItem = new QTreeWidgetItem;
// 	addplotItem->setText(0, xColumn);
// 	addplotItem->setText(1, yColumn);
// 	ui.treeWidget_4->addTopLevelItem(addplotItem);
// 
// 	//获取当前plotBar控件
// 	if (m_plotManager[tabName].isEmpty())
// 		return;
// 
// 	for (int i = 0; i < m_plotManager[tabName].size(); ++i)
// 	{
// 		PlotItemBase* currItem = m_plotManager[tabName].at(i);
// 		QString name = currItem->metaObject()->className();
// 		if (name.compare("PlotPlotScatter") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotAScope") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotRTI") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotText") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotLight") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotBar") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotDial") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotAttitude") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotPolar") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotTrack") == 0)
// 		{
// 			
// 		}
// 		else if (name.compare("PlotDoppler") == 0)
// 		{
// 			
// 		}
// 	}
// }

void PlotManager::onRadioPixelClicked()
{
	ui.label_xPos->setText("pixel");
	ui.label_yPos->setText("pixel");
	ui.label_widthPos->setText("pixel");
	ui.label_heightPos->setText("pixel");

	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		m_radioPixelChecked = true;
		return;
	}
	if (!m_radioPixelChecked)
	{
		float percent = ui.lineEdit_plotPositionX->text().toFloat();
		int pixel = (int)(percent * m_tabWidgetRect.width());
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotPositionY->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.height());
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotWidth->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.width());
		ui.lineEdit_plotWidth->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotHeight->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.height());
		ui.lineEdit_plotHeight->setText(QString("%1").arg(pixel));
	}
	m_radioPixelChecked = true;
}

void PlotManager::onRadioPercentClicked()
{
	ui.label_xPos->setText("%");
	ui.label_yPos->setText("%");
	ui.label_widthPos->setText("%");
	ui.label_heightPos->setText("%");

	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		m_radioPixelChecked = false;
		return;
	}
	if (m_radioPixelChecked)
	{
		int pixel = ui.lineEdit_plotPositionX->text().toInt();
		float percent = (float)pixel / m_tabWidgetRect.width();
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotPositionY->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.height();
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotWidth->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.width();
		ui.lineEdit_plotWidth->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotHeight->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.height();
		ui.lineEdit_plotHeight->setText(QString("%1").arg(percent));
	}
	m_radioPixelChecked = false;
}

void PlotManager::onGetTabWidgetRect(QRect rect)
{
	m_tabWidgetRect = rect;
}

void PlotManager::onBtnCloseClicked()
{
	close();
}

void PlotManager::onPlotRectEditFinishing()
{
	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		return;
	}

	bool bX, bY, bW, bH;
	int x, y, w, h;
	if (m_radioPixelChecked)
	{
		x = ui.lineEdit_plotPositionX->text().toInt(&bX);
		y = ui.lineEdit_plotPositionY->text().toInt(&bY);
		w = ui.lineEdit_plotWidth->text().toInt(&bW);
		h = ui.lineEdit_plotHeight->text().toInt(&bH);
	}
	else
	{
		x = (int)(ui.lineEdit_plotPositionX->text().toFloat(&bX) * m_tabWidgetRect.width());
		y = (int)(ui.lineEdit_plotPositionY->text().toFloat(&bY) * m_tabWidgetRect.height());
		w = (int)(ui.lineEdit_plotWidth->text().toFloat(&bW) * m_tabWidgetRect.width());
		h = (int)(ui.lineEdit_plotHeight->text().toFloat(&bH) * m_tabWidgetRect.height());
	}

	if (bX && bY && bW && bH)
		emit sigRectChanged(QRect(x, y, w, h));
}
