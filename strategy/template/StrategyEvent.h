#pragma once

#ifndef _STRATEGYEVENT_H_
#define _STRATEGEVENT_H_

#include "stdafx.h"

class StrategyEvent
{
public:
	StrategyEvent(string insID, char* stgname);
	~StrategyEvent();

public:
	void start();

	int THTickInfoMillisec();
	CThostFtdcDepthMarketDataField* UpdateTickInfo(CThostFtdcDepthMarketDataField* tick);

	CThostFtdcOrderField* LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);
	CThostFtdcOrderField* SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID);
	CThostFtdcOrderField* SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today = false);
	CThostFtdcOrderField* COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today = false);
	int ACTION(TThostFtdcOrderSysIDType sys_id, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID);

	///查询持仓
	PostionInformation Postion(TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID);
	///查询账户资金
	AccountInformation Account();
	///保单返回
	//CThostFtdcOrderField* OnRtnOrder(CThostFtdcOrderField *pOrder);
	vector<int> order_ID_vtr();

private:
	CThostFtdcOrderField* SendOrder(CThostFtdcInputOrderField order);
	int ActionOrder(CThostFtdcInputOrderActionField order);
	CThostFtdcOrderField* RtnOrder();

	static void TickReceiveFun(mutex *mutex);
	//static void OnRtnOrderReceiveFun(mutex *mutex);

private:
	static string mmap_md_dir;
	static string mmap_td_dir;
	static string mmap_stg_dir;

	static int64_t startNano;
	static string insID;
	mutex* tick_receive_mutex;
	mutex* rtn_order_mutex;
	static CThostFtdcDepthMarketDataField *th_tick;
	static CThostFtdcOrderField *th_rtn_order;


	vector<std::thread> thread_vtr;

	map <string, shared_ptr<JournalWriter>> mmap_writer;
	map <string, int> mmap_count;

	map <string, shared_ptr<JournalReader>> mmap_reader;

	char* STGName;

	static vector<int> order_request;
};





#endif