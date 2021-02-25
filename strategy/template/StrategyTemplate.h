#pragma once

#ifndef _StrategyTemplate_H_
#define _StrategyTemplate_H_
#include "StrategyEvent.h"
#include "stdafx.h"


class StrategyTemplate
{
public:
	StrategyTemplate();
	~StrategyTemplate();

public:
	///初始化函数 不可传参
	///重写InIt
	virtual void InIt();
	virtual void OnTick(CThostFtdcDepthMarketDataField* tick);
	virtual void OnBar(BarInfomation* bar);

	int SendOrder(CThostFtdcInputOrderField *order_info);

	char* InstrumentID();


public:
	virtual CThostFtdcOrderField* LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	virtual CThostFtdcOrderField* SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	virtual CThostFtdcOrderField* SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

	virtual CThostFtdcOrderField* COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);

public:
	char* ClassName;

public:
	bool funend = true;
	TickInfomation __tick;
	char* __InstrumentID;
	StrategyEvent* stgevent=nullptr;
	
};

#endif