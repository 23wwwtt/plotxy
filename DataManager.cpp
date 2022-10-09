/* * @filename: DataManager.cpp
* @brief: DataManagerʵ�����ݼ��ع���
* @author: zhameng
* @create time: 2022-09-27
*  */

#include "DataManager.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>

std::shared_ptr<DataManager> DataManager::m_pInstance = nullptr;
QMutex DataManager::m_mutex;

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

void DataManager::loadCSV(const QString& filePath)
{
	if (filePath.isEmpty())
	{
		qDebug() << "DataManager::loadCSV filePath is empty." << endl;
		return;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "DataManager::loadCSV read file failure." << endl;
		return;
	}
	
	QTextStream source(&file);
	QStringList allLines = source.readAll().split("\n");
	
	QString currEntityType;
	QStringList currEntityAttrs;
	QVector<QList<double>> currEntityAttrValuesVec;

	for (int i = 0; i < allLines.size(); i++)
	{
		QList<double> currEntityAttrValues;

		QStringList currLineItems = allLines.at(i).split(",");

		if (currLineItems.size() <= 0)
			continue;
		if (currLineItems.size() == 1)
			continue;
		QString firstItem = currLineItems.at(0);

		if (firstItem.at(0) == '#')	//�ж����ַ��Ƿ�Ϊ#	entityType entityAttr1 entityAttr2 entityAttr3 entityAttr4 
		{
			if (!currEntityType.isEmpty())	//��־����һ�����ݴ�����ϣ���Ҫ����һ��������ӽ�map
			{
				QMap<QString, QList<double>> lastGroupDataMap;

				//���������У���������װ���ݣ�Rows->Columns
				for (int k = 0; k < currEntityAttrValuesVec.size();k++)
				{
					for (int t = 0;t < currEntityAttrValuesVec[k].size();t++)
					{
						double currEntityAttrValue = currEntityAttrValuesVec.at(k).at(t);

						QString currEntityAttr;
						if(t < currEntityAttrs.size())
							currEntityAttr = currEntityAttrs.at(t);

						if (!currEntityAttr.isEmpty())
						{
							lastGroupDataMap[currEntityAttr].push_back(currEntityAttrValue);
						}
					}
				}
				currEntityAttrValuesVec.clear();
				m_entityDataMap.insert(currEntityType, lastGroupDataMap);
			}

			//��ʼ������һ������
			currEntityType = firstItem.remove('#');

			currEntityAttrs.clear();
			for (int j = 1;j < currLineItems.size();j++)
			{
				QString currEntityAttr = currLineItems.at(j);
				currEntityAttrs.push_back(currEntityAttr);
			}
		}
		else
		{
			for (int j = 1; j < currLineItems.size(); j++)
			{
				double currEntityAttrValue = currLineItems.at(j).toDouble();
				currEntityAttrValues.push_back(currEntityAttrValue);
			}
			currEntityAttrValuesVec.push_back(currEntityAttrValues);
		}

	}
	
	file.close();
	QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ��Ϣ"), QString::fromLocal8Bit("�ѳɹ���������"));
}


QMap<QString, QMap<QString, QList<double>>>& DataManager::getDataMap()
{
	return m_entityDataMap;
}
