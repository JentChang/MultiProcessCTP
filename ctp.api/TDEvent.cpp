#include "TDEvent.h"

string TDEvent::mmap_td_dir;
string TDEvent::mmap_stg_dir;
int64_t TDEvent::startNano;
CtpTD* TDEvent::td;
RtnRequestID TDEvent::send_data;

map <string, shared_ptr<JournalWriter>> TDEvent::mmap_writer;
map <string, int> TDEvent::mmap_count;
map <string, shared_ptr<JournalReader>> TDEvent::mmap_reader;

TDEvent::TDEvent(CtpTD* td)
{
	TDEvent::mmap_td_dir = MMAP_TD_DIR;
	TDEvent::mmap_stg_dir = MMAP_STG_DIR;

	TDEvent::startNano = NanoTimer::getInstance()->getNano();
	TDEvent::td = td;

	TDEvent::td_mutex = new mutex();

	///mmap 对象
	///write
	TDEvent::mmap_writer[MMAP_RTN_REQUESTID_MAP] = JournalWriter::create(this->mmap_td_dir + "/rtnID", MMAP_RTN_REQUESTID_PATH);
	TDEvent::mmap_count[MMAP_RTN_REQUESTID_MAP] = 0;
	TDEvent::mmap_writer[MMAP_RTN_ORDER_MAP] = JournalWriter::create(this->mmap_td_dir + "/rtn_order", MMAP_RTN_ORDER_PATH);
	TDEvent::mmap_count[MMAP_RTN_ORDER_MAP] = 0;


	///read
	TDEvent::mmap_reader[MMAP_INSERT_ORDER_MAP] = JournalReader::create(TDEvent::mmap_stg_dir + "/order", MMAP_TD_ORDER_INSERT_PATH, TDEvent::startNano);
	TDEvent::mmap_reader[MMAP_ACTION_ORDER_MAP] = JournalReader::create(TDEvent::mmap_stg_dir + "/order", MMAP_TD_ORDER_ACTION_PATH, TDEvent::startNano);
	TDEvent::mmap_reader[MMAP_POSITION_MAP] = JournalReader::create(TDEvent::mmap_stg_dir + "/serch", MMAP_TD_POSITION_PATH, TDEvent::startNano);
	TDEvent::mmap_reader[MMAP_OTHER_EVENT_MAP] = JournalReader::create(TDEvent::mmap_stg_dir + "/serch", MMAP_TD_OTHER_EVENT_PATH, TDEvent::startNano);

}

TDEvent::~TDEvent()
{
	delete TDEvent::td_mutex;
}



void TDEvent::start()
{
	this->thread_vtr.push_back(thread(TDEvent::MMAPOrderReader, TDEvent::td_mutex));
	this->thread_vtr.push_back(thread(TDEvent::MMAPOrderActionReader, TDEvent::td_mutex));
	this->thread_vtr.push_back(thread(TDEvent::MMAPPostionReader, TDEvent::td_mutex));
	this->thread_vtr.push_back(thread(TDEvent::MMAPOtherEventReader, TDEvent::td_mutex));
}

void TDEvent::MMAPOrderReader(mutex* mutex)
{
	///录单
	OrderInformation *order_info;
	RtnOrderInfo rtn_order_info;
	std::shared_ptr<Frame> frame;
	int total_number = 0;

	while (true)
	{
		frame = TDEvent::mmap_reader[MMAP_INSERT_ORDER_MAP]->getNextFrame();
		if (frame.get() != nullptr)
		{
			total_number++;
			int64_t cur_time = frame->getNano();
			short msg_type = frame->getMsgType();
			short msg_source = frame->getSource();
			int request_id = frame->getRequestId();
			void *data = frame->getData();

			mutex->lock();
			order_info = (OrderInformation*)data;
			rtn_order_info.RtnOrderInfo = td->ReqOrderInsert(order_info->Order);
			strcpy(rtn_order_info.StrategyName, order_info->StrategyName);
			
			///write Rtn
			TDEvent::mmap_writer[MMAP_RTN_ORDER_MAP]->write_frame(&rtn_order_info,
																	sizeof(RtnOrderInfo), 1, 12345, true,
																	++TDEvent::mmap_count[MMAP_RTN_ORDER_MAP]);
			mutex->unlock();
		}
	}
}

void TDEvent::MMAPOrderActionReader(mutex* mutex)
{
	///撤单
	ActionOrderInformation *order_info;
	std::shared_ptr<Frame> frame;
	int total_number = 0;

	while (true)
	{
		frame = TDEvent::mmap_reader[MMAP_ACTION_ORDER_MAP]->getNextFrame();
		if (frame.get() != nullptr)
		{
			total_number++;
			int64_t cur_time = frame->getNano();
			short msg_type = frame->getMsgType();
			short msg_source = frame->getSource();
			int request_id = frame->getRequestId();
			void *data = frame->getData();

			mutex->lock();
			order_info = (ActionOrderInformation*)data;
			///td lock
			int order_id = td->ReqOrderAction(order_info->Order);
			///write Rtn
			TDEvent::WriteRtnReq(order_id, order_info->StrategyName);
			mutex->unlock();
		}
	}
}

void TDEvent::MMAPPostionReader(mutex * mutex)
{
	CThostFtdcQryInvestorPositionField* pQryInvestorPosition;
	std::shared_ptr<Frame> frame;
	int total_number = 0;

	while (true)
	{
		frame = TDEvent::mmap_reader[MMAP_POSITION_MAP]->getNextFrame();
		if (frame.get() != nullptr)
		{
			total_number++;
			int64_t cur_time = frame->getNano();
			short msg_type = frame->getMsgType();
			short msg_source = frame->getSource();
			int request_id = frame->getRequestId();
			void *data = frame->getData();

			pQryInvestorPosition = (CThostFtdcQryInvestorPositionField*)data;
			///td lock
			mutex->lock();
			td->ReqQryInvestorPosition(*pQryInvestorPosition);
			mutex->unlock();
		}
	}
}

void TDEvent::MMAPOtherEventReader(mutex * mutex)
{
	EventType* other_event;
	std::shared_ptr<Frame> frame;
	int total_number = 0;

	while (true)
	{
		frame = TDEvent::mmap_reader[MMAP_OTHER_EVENT_MAP]->getNextFrame();
		if (frame.get() != nullptr)
		{
			total_number++;
			int64_t cur_time = frame->getNano();
			short msg_type = frame->getMsgType();
			short msg_source = frame->getSource();
			int request_id = frame->getRequestId();
			void *data = frame->getData();

			other_event = (EventType*)data;
			///td lock
			if (strcmp(other_event->Type, EVENT_ACCOUNT) == 0)
			{
				///查询资金
				mutex->lock();
				td->ReqQryTradingAccount();
				mutex->unlock();
			}
			else if (true)
			{

			}
		}
	}
}

void TDEvent::WriteRtnReq(int order_id, char * stgname)
{
	///写request
	TDEvent::send_data.RequestID = order_id;
	strcpy(TDEvent::send_data.StrategyName, stgname);

	TDEvent::mmap_writer[MMAP_RTN_REQUESTID_MAP]->write_frame(&TDEvent::send_data,
		sizeof(RtnRequestID), 1, 12345, true,
		TDEvent::mmap_count[MMAP_RTN_REQUESTID_MAP]);
	TDEvent::mmap_count[MMAP_RTN_REQUESTID_MAP]++;
}
