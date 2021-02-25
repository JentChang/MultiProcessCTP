#include "StrategyTemplate.h"


class StrategyTest :
	public StrategyTemplate
{
public:
	StrategyTest();
	~StrategyTest();
	void InIt();
	void OnTick(CThostFtdcDepthMarketDataField* tick);
	void OnBar(BarInfomation* bar);
public:
	char* ClassName;
private:

};

StrategyTest::StrategyTest():
	StrategyTemplate()
{
}

StrategyTest::~StrategyTest()
{
}

void StrategyTest::InIt()
{
	this->__InstrumentID = "rb2105";
	this->ClassName = "StrategyTest";
	this->stgevent = new StrategyEvent(this->__InstrumentID, this->ClassName);
	this->stgevent->start();
}

void StrategyTest::OnTick(CThostFtdcDepthMarketDataField *tick)
{

	std::cout 
		<< tick->InstrumentID << " "
		<< tick->TradingDay << " "
		<< tick->UpdateTime << " "
		<< tick->UpdateMillisec << " "
		<< endl;
	CThostFtdcOrderField *pOrder;
	pOrder = this->LONG(4520, 1, "rb2105");
	std::cout << "order id : " << pOrder->OrderSysID << endl;
	std::cout << " 成交数量 " << pOrder->VolumeTraded << " 剩余数量 " << pOrder->VolumeTotal << endl;


	///资金
	AccountInformation acc = this->stgevent->Account();
	///持仓
	PostionInformation pos = this->stgevent->Postion("SHFE", this->__InstrumentID);

	if (acc.EMPTY)
	{
		std::cout << "资金查询返回 nullpter" << endl;
	}
	else {
		std::cout << " 可用资金: " << acc.AccInfo.Available << " 当前保证金总额: " << acc.AccInfo.CurrMargin << endl;
	}

	if (pos.EMPTY)
	{
		std::cout << "持仓查询返回 nullpter" << endl;
	}
	else {
		std::cout << " 今日持仓: " << pos.PosInfo.Position << " 持仓方向: " << pos.PosInfo.PosiDirection << endl;
	}

	int action_rtn = this->stgevent->ACTION(pOrder->OrderSysID, "SHFE", this->__InstrumentID);
	std::cout << "action: " << action_rtn << endl;

}

void StrategyTest::OnBar(BarInfomation * bar)
{
}


int main()
{

	StrategyTemplate* stg = new StrategyTest();
	stg->InIt();

	CThostFtdcDepthMarketDataField *tick = new CThostFtdcDepthMarketDataField();
	int millisec = 0;

	while (true)
	{
		millisec = stg->stgevent->THTickInfoMillisec();
		if (tick->UpdateMillisec != millisec)
		{
			stg->stgevent->UpdateTickInfo(tick);
			stg->OnTick(tick);
			break;
		}
	}

	return 0;
}