#include "CtpMD.h"



CtpMD::CtpMD(CThostFtdcMdApi* api, CThostFtdcReqUserLoginField* pUserLogin) :
	api(api), userLogin(pUserLogin)
{
	requestID = 1;

	this->mmap_dir = MMAP_MD_DIR;
	this->mmap_path = "tick_info_";
	this->startNano = NanoTimer::getInstance()->getNano();
}


CtpMD::~CtpMD()
{
	if (api)
	{
		api->RegisterSpi(nullptr);
		api->Release();
		api = nullptr;
	}
}


void CtpMD::OnFrontConnected()
{
	std::cout << "OnFrontConnected..." << std::endl;
	if (userLogin)
	{
		api->ReqUserLogin(userLogin, requestID++);
	}
}


void CtpMD::OnFrontDisconnected(int nReason)
{
	std::cout << "OnFrontDisconnected, nReason:"<< nReason << std::endl;
}


int CtpMD::OnSubscribeMarketData(char * ppInstrumentID[], int nCount)
{
	if (this->insts == nullptr)
	{
		this->insts = ppInstrumentID;
		this->insts_count = nCount;
	}
	for (size_t i = 0; i < nCount; i++)
	{
		this->mmap_writer[ppInstrumentID[i]] = JournalWriter::create(this->mmap_dir, this->mmap_path + ppInstrumentID[i]);
		this->mmap_count[ppInstrumentID[i]] = 0;
	}
	return this->api->SubscribeMarketData(ppInstrumentID, nCount);
}

void CtpMD::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "OnRspUserLogin ok"<< std::endl;
		if (this->insts != nullptr)
		{
			this->OnSubscribeMarketData(this->insts, this->insts_count);
		}
	}
	else {
		std::cout << "OnRspUserLogin error:" 
			<< pRspInfo->ErrorID 
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}


///错误应答
void CtpMD::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspError:"
		<< pRspInfo->ErrorID
		<< pRspInfo->ErrorMsg
		<< std::endl;
}

///订阅行情应答
void CtpMD::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		//std::cout << "OnRspSubMarketData ok" << std::endl;
	}
	else {
		std::cout << "OnRspSubMarketData error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
	}
}


///深度行情通知
void CtpMD::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	//std::cout << "OnRtnDepthMarketData,"
	//	<< pDepthMarketData->InstrumentID << ","
	//	<< pDepthMarketData->ExchangeID << ","
	//	<< pDepthMarketData->LastPrice << ","
	//	<< pDepthMarketData->OpenPrice << ","
	//	<< pDepthMarketData->HighestPrice << ","
	//	<< pDepthMarketData->LowestPrice << ","
	//	<< pDepthMarketData->Volume << ","
	//	<< pDepthMarketData->Turnover << ","
	//	<< pDepthMarketData->OpenInterest << ","
	//	<< pDepthMarketData->ClosePrice << ","
	//	<< pDepthMarketData->ActionDay << ","
	//	<< pDepthMarketData->UpdateTime << ","
	//	<< pDepthMarketData->UpdateMillisec << ","
	//	<< pDepthMarketData->AskPrice1 << ","
	//	<< pDepthMarketData->AskVolume1 << ","
	//	<< pDepthMarketData->BidPrice1 << ","
	//	<< pDepthMarketData->BidVolume1 << ","
	//	<< std::endl;
	this->mmap_writer[pDepthMarketData->InstrumentID]->write_frame(pDepthMarketData,
																	sizeof(CThostFtdcDepthMarketDataField), 1, 12345, true, 
																	this->mmap_count[pDepthMarketData->InstrumentID]);
	this->mmap_count[pDepthMarketData->InstrumentID]++;
	char* ins = "rb2105";
	if (strcmp(pDepthMarketData->InstrumentID, ins) == 0)
	{
		std::cout
			<< pDepthMarketData->InstrumentID << " "
			<< pDepthMarketData->TradingDay << " "
			<< pDepthMarketData->UpdateTime << " "
			<< pDepthMarketData->UpdateMillisec << " "
			<< endl;
	}
}