#include "StrategyTemplate.h"

StrategyTemplate::StrategyTemplate()
{
	this->__tick = TickInfomation();
}

StrategyTemplate::~StrategyTemplate()
{
	this->__InstrumentID = nullptr;
}

void StrategyTemplate::InIt()
{
	this->stgevent = new StrategyEvent(this->__InstrumentID, this->ClassName);
	this->stgevent->start();
}

void StrategyTemplate::OnTick(CThostFtdcDepthMarketDataField * tick)
{
}

void StrategyTemplate::OnBar(BarInfomation * bar)
{
}

int StrategyTemplate::SendOrder(CThostFtdcInputOrderField * order_info)
{

	return 0;
}



char* StrategyTemplate::InstrumentID()
{
	return this->__InstrumentID;
}

CThostFtdcOrderField* StrategyTemplate::LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	std::cout << "LONG ORDER" << endl;
	return this->stgevent->LONG(price, volume, InstrumentID);
}

CThostFtdcOrderField* StrategyTemplate::SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	return this->stgevent->SHORT(price, volume, InstrumentID);
}

CThostFtdcOrderField* StrategyTemplate::SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	return this->stgevent->SELL(price, volume, InstrumentID);
}

CThostFtdcOrderField* StrategyTemplate::COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	return this->stgevent->COVER(price, volume, InstrumentID);
}

