/* Copyright (c) 2020, Dyssol Development Team. All rights reserved. This file is part of Dyssol. See LICENSE file for license information. */

#pragma once

#include "QtTable.h"
#include "DenseDistr2D.h"
#include <QHBoxLayout>

class CDDTable : public QWidget
{
	Q_OBJECT

private:
	CDenseDistr2D* m_pData;
	CQtTable* m_pTable;
	bool m_bAvoidSignal;
	bool m_bNormalize;
	QHBoxLayout *layout;

public:
	CDDTable( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
	~CDDTable();

	void SetDistribution( CDenseDistr2D* _pDistribution );
	void SetNormalizationCheck( bool _bAnalyse );
	void SetEditable(bool _bEditable);				// Turn on/off edit possibility.

private:
	void SetHeaders();
	void CheckNormalization();

public slots:
	void UpdateWholeView();
	//void setVisible( bool _bVisible );

private slots:
	void ItemWasChanged( QTableWidgetItem* _pItem );

signals:
	void DataChanged();
};
