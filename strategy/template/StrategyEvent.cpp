#include "StrategyEvent.h"


string StrategyEvent::mmap_md_dir;
string StrategyEvent::mmap_td_dir;
string StrategyEvent::mmap_stg_dir;

int64_t StrategyEvent::startNano;
string StrategyEvent::insID;

CThostFtdcDepthMarketDataField* StrategyEvent::th_tick;
CThostFtdcOrderField* StrategyEvent::th_rtn_order;

vector<int> StrategyEvent::order_request;

StrategyEvent::StrategyEvent(string insID, char* stgname)
{
	StrategyEvent::insID = insID;
	StrategyEvent::STGName = stgname;

	StrategyEvent::mmap_md_dir = MMAP_MD_DIR;
	StrategyEvent::mmap_td_dir = MMAP_TD_DIR;
	StrategyEvent::mmap_stg_dir = MMAP_STG_DIR;

	StrategyEvent::startNano = NanoTimer::getInstance()->getNano();
	StrategyEvent::tick_receive_mutex = new mutex();
	StrategyEvent::rtn_order_mutex = new mutex();
	StrategyEvent::th_tick = new CThostFtdcDepthMarketDataField();
	StrategyEvent::th_rtn_order = new CThostFtdcOrderField();

	///mmap ¶ÔÏó
	///write
	this->mmap_writer[MMAP_INSERT_ORDER_MAP] = JournalWriter::create(this->mmap_stg_dir+"/order", MMAP_TD_ORDER_INSERT_PATH);
	this->mmap_count[MMAP_INSERT_ORDER_MAP] = 0;
	this->mmap_writer[MMAP_ACTION_ORDER_MAP] = JournalWriter::create(this->mmap_stg_dir + "/order", MMAP_TD_ORDER_ACTION_PATH);
	this->mmap_count[MMAP_ACTION_ORDER_MAP] = 0;

	this->mmap_writer[MMAP_POSITION_MAP] = JournalWriter::create(this->mmap_stg_dir + "/serch", MMAP_TD_POSITION_PATH);
	this->mmap_count[MMAP_POSITION_MAP] = 0;
	this->mmap_writer[MMAP_OTHER_EVENT_MAP] = JournalWriter::create(this->mmap_stg_dir + "/serch", MMAP_TD_OTHER_EVENT_PATH);
	this->mmap_count[MMAP_OTHER_EVENT_MAP] = 0;
	
	///read
	this->mmap_reader[MMAP_RTN_REQUESTID_MAP] = JournalReader::create(this->mmap_td_dir + "/rtnID", MMAP_RTN_REQUESTID_PATH, StrategyEvent::startNano);
	this->mmap_reader[MMAP_RTN_INVERSTOR_MAP] = JournalReader::create(this->mmap_td_dir + "/rtn_pos", MMAP_RTN_INVERSTOR_PATH, StrategyEvent::startNano);
	this->mmap_reader[MMAP_RTN_ACCOUNT_MAP] = JournalReader::create(this->mmap_td_dir + "/rtn_acc", MMAP_RTN_ACCOUNT_PATH, StrategyEvent::startNano);
	this->mmap_reader[MMAP_RTN_ORDER_MAP] = JournalReader::create(this->mmap_td_dir + "/rtn_order", MMAP_RTN_ORDER_PATH, StrategyEvent::startNano);


}

StrategyEvent::~StrategyEvent()
{
	delete StrategyEvent::tick_receive_mutex;
	delete StrategyEvent::rtn_order_mutex;
	delete StrategyEvent::th_tick;
	delete StrategyEvent::th_rtn_order;
}

void StrategyEvent::start()
{
	std::cout << "strategy event start" << endl;
	this->thread_vtr.push_back(thread(StrategyEvent::TickReceiveFun, StrategyEvent::tick_receive_mutex));
	//this->thread_vtr.push_back(thread(StrategyEvent::OnRtnOrderReceiveFun, StrategyEvent::rtn_order_mutex));
}



int StrategyEvent::THTickInfoMillisec()
{
	//StrategyEvent::tick_receive_mutex->lock();
	int millisec = StrategyEvent::th_tick->UpdateMillisec;
	//StrategyEvent::tick_receive_mutex->unlock();
	return millisec;
}

CThostFtdcDepthMarketDataField* StrategyEvent::UpdateTickInfo(CThostFtdcDepthMarketDataField* tick)
{
	StrategyEvent::tick_receive_mutex->lock();
	memcpy(tick, StrategyEvent::th_tick, sizeof(TickInfomation));
	StrategyEvent::tick_receive_mutex->unlock();

	return tick;
}

void StrategyEvent::TickReceiveFun(mutex* mutex)
{
	string path = "tick_info_" + StrategyEvent::insID;

	std::shared_ptr<JournalReader> reader = JournalReader::create(StrategyEvent::mmap_md_dir, path, StrategyEvent::startNano);
	std::shared_ptr<Frame> frame;
	int total_number = 0;

	while (true)
	{
		frame = reader->getNextFrame();
		if (frame.get() != nullptr)
		{
			total_number++;
			int64_t cur_time = frame->getNano();
			short msg_type = frame->getMsgType();
			short msg_source = frame->getSource();
			int request_id = frame->getRequestId();

			void *data = frame->getData();

			mutex->lock();
			StrategyEvent::th_tick = (CThostFtdcDepthMarketDataField*)data;
			mutex->unlock();
		}
	}
}

//void StrategyEvent::OnRtnOrderReceiveFun(mutex * mutex)
//{
//	std::shared_ptr<JournalReader> reader = JournalReader::create(StrategyEvent::mmap_td_dir + "/rtn_order", MMAP_RTN_ORDER_PATH, StrategyEvent::startNano);
//	std::shared_ptr<Frame> frame;
//	int total_number = 0;
//
//	while (true)
//	{
//		frame = reader->getNextFrame();
//		if (frame.get() != nullptr)
//		{
//			total_number++;
//			int64_t cur_time = frame->getNano();
//			short msg_type = frame->getMsgType();
//			short msg_source = frame->getSource();
//			int request_id = frame->getRequestId();
//
//			void *data = frame->getData();
//			mutex->lock();
//			StrategyEvent::th_rtn_order = (CThostFtdcOrderField*)data;
//			if (StrategyEvent::order_request.size() > 0)
//			{
//				if (StrategyEvent::th_rtn_order->RequestID == StrategyEvent::order_request[0] || StrategyEvent::th_rtn_order->RequestID==-1)
//				{
//					StrategyEvent::order_request.erase(StrategyEvent::order_request.begin());
//				}
//			}
//			mutex->unlock();
//		}
//	}
//}

CThostFtdcOrderField* StrategyEvent::LONG(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(CThostFtdcInputOrderField));

	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Buy;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	return this->SendOrder(order);
}

CThostFtdcOrderField* StrategyEvent::SHORT(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID)
{

	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(CThostFtdcInputOrderField));
	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Sell;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	return this->SendOrder(order);
}

CThostFtdcOrderField* StrategyEvent::SELL(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today)
{
	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(CThostFtdcInputOrderField));
	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Sell;
	if (today)
	{
		order.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	}
	else {
		order.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	}

	return this->SendOrder(order);
}

CThostFtdcOrderField* StrategyEvent::COVER(TThostFtdcPriceType price, TThostFtdcVolumeType volume, TThostFtdcInstrumentIDType InstrumentID, bool today)
{
	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(CThostFtdcInputOrderField));
	order.LimitPrice = price;
	order.VolumeTotalOriginal = volume;
	strcpy(order.InstrumentID, InstrumentID);

	order.Direction = THOST_FTDC_D_Buy;
	if (today)
	{
		order.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	}
	else {
		order.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	}

	return this->SendOrder(order);
}

int StrategyEvent::ACTION(TThostFtdcOrderSysIDType sys_id, TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcInputOrderActionField pInputOrderAction;
	memset(&pInputOrderAction, 0, sizeof(CThostFtdcInputOrderActionField));
	///OrderSysID FrontID+SessionID+OrderRef
	strcpy(pInputOrderAction.ExchangeID, ExchangeID);
	strcpy(pInputOrderAction.InstrumentID, InstrumentID);
	strcpy_s(pInputOrderAction.OrderSysID, sys_id);
	return StrategyEvent::ActionOrder(pInputOrderAction);
}

PostionInformation StrategyEvent::Postion(TThostFtdcExchangeIDType ExchangeID, TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcQryInvestorPositionField pQryInvestorPosition;
	memset(&pQryInvestorPosition, 0, sizeof(CThostFtdcQryInvestorPositionField));

	strcpy(pQryInvestorPosition.ExchangeID, ExchangeID);
	strcpy(pQryInvestorPosition.ExchangeID, InstrumentID);
	this->mmap_writer[MMAP_POSITION_MAP]->write_frame(
		&pQryInvestorPosition,
		sizeof(CThostFtdcQryInvestorPositionField), 1, 12345, true,
		++this->mmap_count[MMAP_POSITION_MAP]
	);

	///reader
	std::shared_ptr<Frame> frame;
	int total_number = 0;
	while (true)
	{

		frame = this->mmap_reader[MMAP_RTN_INVERSTOR_MAP]->getNextFrame();
		if (frame.get() != nullptr)
		{
			total_number++;
			int64_t cur_time = frame->getNano();
			short msg_type = frame->getMsgType();
			short msg_source = frame->getSource();
			int request_id = frame->getRequestId();

			void *data = frame->getData();
			PostionInformation* pof_info = (PostionInformation*)data;
			if (pof_info->EMPTY)
			{
				return *pof_info;
			}
			if (strcmp(pof_info->PosInfo.InstrumentID, InstrumentID) == 0)
			{
				return *pof_info;
			}
		}
	}
}

AccountInformation StrategyEvent::Account()
{
	EventType account_event;
	memset(&account_event, 0, sizeof(EventType));

	strcpy(account_event.StrategyName, this->STGName);
	strcpy(account_event.Type, EVENT_ACCOUNT);

	this->mmap_writer[MMAP_OTHER_EVENT_MAP]->write_frame(
		&account_event,
		sizeof(EventType), 1, 12345, true,
		++this->mmap_count[MMAP_OTHER_EVENT_MAP]
	);
	///reader
	std::shared_ptr<Frame> frame;
	int total_number = 0;
	while (true)
	{
		frame = this->mmap_reader[MMAP_RTN_ACCOUNT_MAP]->getNextFrame();
		if (frame.get() != nullptr)
		{
			total_number++;
			int64_t cur_time = frame->getNano();
			short msg_type = frame->getMsgType();
			short msg_source = frame->getSource();
			int request_id = frame->getRequestId();

			void *data = frame->getData();
			AccountInformation* acc_info = (AccountInformation*)data;

			return *acc_info;
		}
	}
}

//CThostFtdcOrderField* StrategyEvent::OnRtnOrder(CThostFtdcOrderField *pOrder)
//{
//
//	while (true)
//	{
//		this->rtn_order_mutex->lock();
//		if (pOrder->RequestID == StrategyEvent::th_rtn_order->RequestID)
//		{
//			memcpy(pOrder, StrategyEvent::th_rtn_order, sizeof(CThostFtdcOrderField));
//			return pOrder;
//		}
//		if (StrategyEvent::order_request.size() == 0 || StrategyEvent::th_rtn_order->RequestID == -1)
//		{
//			return nullptr;
//		}
//		this->rtn_order_mutex->unlock();
//	}
//}

vector<int> StrategyEvent::order_ID_vtr()
{
	return this->order_request;
}

CThostFtdcOrderField* StrategyEvent::SendOrder(CThostFtdcInputOrderField order)
{
	OrderInformation send_order;

	send_order.Order = order;
	strcpy(send_order.StrategyName, this->STGName);

	this->mmap_writer[MMAP_INSERT_ORDER_MAP]->write_frame(&send_order,
															sizeof(OrderInformation), 1, 12345, true,
															++this->mmap_count[MMAP_INSERT_ORDER_MAP]);
	std::cout << "has writed Order" << endl;
	///reader rtn id
	return this->RtnOrder();
}

int StrategyEvent::ActionOrder(CThostFtdcInputOrderActionField order)
{
	ActionOrderInformation send_order;
	send_order.Order = order;
	strcpy(send_order.StrategyName, this->STGName);
	this->mmap_writer[MMAP_ACTION_ORDER_MAP]->write_frame(&send_order,
															sizeof(ActionOrderInformation), 1, 12345, true,
															++this->mmap_count[MMAP_ACTION_ORDER_MAP]);
	///reader rtn id
	//return this->RequestID();
	return 0;
}

CThostFtdcOrderField* StrategyEvent::RtnOrder()
{
	///reader rtn id
	
	std::shared_ptr<Frame> frame;
	int total_number = 0;
	while (true)
	{
		frame = this->mmap_reader[MMAP_RTN_ORDER_MAP]->getNextFrame();
		if (frame.get() != nullptr)
		{
			total_number++;
			int64_t cur_time = frame->getNano();
			short msg_type = frame->getMsgType();
			short msg_source = frame->getSource();
			int request_id = frame->getRequestId();

			void *data = frame->getData();
			RtnOrderInfo* rtn_order = (RtnOrderInfo*)data;
			if (strcmp(rtn_order->StrategyName, this->STGName) == 0)
			{
				return &rtn_order->RtnOrderInfo;
			}
		}
	}
}

