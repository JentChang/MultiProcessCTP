#include "event_loop.h"


namespace BackTraderCN
{
    volatile sig_atomic_t EventLoop::signal_received_ = -1;

    void EventLoop::subscribe_yjj_journal(const std::string& journal_folder, const std::string& journal_name, int64_t offset_nano)
    {
        if (reader_.get() == nullptr)
        {
            reader_ = JournalReader::create(journal_folder, journal_name, offset_nano, name_);
        }
        else
        {
            int journal_idx = reader_->addJournal(journal_folder, journal_name);
            reader_->seekTimeJournal(journal_idx, offset_nano);
        }
    }

    void EventLoop::register_nanotime_callback(int64_t nano, TSCallback callback)
    {
        scheduler_->insert_callback_at(nano, callback);
    }

    void EventLoop::register_nanotime_callback_at_next(const char *time_str, TSCallback callback)
    {
        scheduler_->insert_callback_at_next(time_str, callback);
    }

    void EventLoop::register_quote_callback(QuoteCallback callback)
    {
        quote_callback_ = callback;
    }

    void EventLoop::register_entrust_callback(EntrustCallback callback)
    {
        entrust_callback_ = callback;
    }

    void EventLoop::register_transaction_callback(TransactionCallback callback)
    {
        transaction_callback_ = callback;
    }

    void EventLoop::register_req_login_callback(ReqLoginCallback callback)
    {
        login_callback_ = callback;
    }

    void EventLoop::register_subscribe_callback(SubscribeCallback callback)
    {
        sub_callback_ = callback;
    }

    void EventLoop::register_order_input_callback(OrderInputCallback callback)
    {
        order_input_callback_ = callback;
    }

    void EventLoop::register_order_action_callback(OrderActionCallback callback)
    {
        order_action_callback_ = callback;
    }

    void EventLoop::register_order_callback(OrderCallback callback)
    {
        order_callback_ = callback;
    }

    void EventLoop::register_trade_callback(TradeCallback callback)
    {
        trade_callback_ = callback;
    }

    void EventLoop::register_algo_order_input_callback(AlgoOrderInputCallback callback)
    {
        algo_order_input_callback_ = callback;
    }

    void EventLoop::register_algo_order_status_callback(AlgoOrderStatusCallback callback)
    {
        algo_order_status_callback_ = callback;
    }

    void EventLoop::register_algo_order_action_callback(AlgoOrderActionCallback callback)
    {
        algo_order_action_callback_ = callback;
    }

    void EventLoop::register_signal_callback(SignalCallback signal_callback)
    {
        signal_callbacks_.push_back(signal_callback);
    }

    void EventLoop::signal_handler(int signal)
    {
        signal_received_ = signal;
    }

    void EventLoop::run()
    {
        for (int s = 1; s < 32; s++)
        {
            signal(s, EventLoop::signal_handler);
        }
        
        quit_ = false;

        while (! quit_ && signal_received_ < 0)
        {
            iteration();
        }
        if (signal_received_ >= 0)
        {
            SPDLOG_TRACE("signal received {}", signal_received_);
            for (const auto& cb: signal_callbacks_)
            {
                cb(signal_received_);
            }
        }
    }

    void EventLoop::stop()
    {
        quit_= true;
    }

    void EventLoop::iteration()
    {
        int64_t nano = -1;
        if (reader_ != nullptr)
        {
            std::shared_ptr<Frame> frame = reader_->getNextFrame();
            if (frame != nullptr)
            {
                nano = frame->getNano();
                int msg_type = frame->getMsgType();
                switch (msg_type)
                {
                    case (int)MsgType::Quote:
                    {   
                        if (quote_callback_)
                        {
							Quote* quote_ptr = (Quote*)frame->getData();
                            quote_callback_(*quote_ptr);
                        }
                        break;
                    }
                    case (int)MsgType::Entrust:
                    {   
                        if (entrust_callback_)
                        {
							Entrust* entrust_ptr = (Entrust*)frame->getData();
                            entrust_callback_(*entrust_ptr);
                        }
                        break;
                    }
                    case (int)MsgType::Transaction:
                    {
                        if (transaction_callback_)
                        {
							Transaction* transaction_ptr = (Transaction*)frame->getData();
							transaction_callback_(*transaction_ptr);
                        }
                        break;
                    }
                    case (int) MsgType::OrderInput:
                    {
                        if (order_input_callback_)
                        {
							OrderInput* input = (OrderInput*)frame->getData();
							order_input_callback_(*input);
                        }
                        break;
                    }
                    case (int)MsgType::OrderAction:
                    {
                        if (order_action_callback_)
                        {
							OrderAction* action = (OrderAction*)frame->getData();
							order_action_callback_(*action);
                        }
                        break;
                    }
                    case (int)MsgType::Order:
                    {
                        if (order_callback_)
                        {
							Order* order = (Order*)frame->getData();
							order_callback_(*order);
                        }
                        break;
                    }
                    case (int)MsgType::Trade:
                    {
                        if (trade_callback_)
                        {
							Trade* trade = (Trade*)frame->getData();
							trade_callback_(*trade);
                        }
                        break;
                    }
                    case (int)MsgType::AlgoOrderInput:
                    {
						if (algo_order_input_callback_)
						{
							try
							{
								Document doc;
								doc.Parse((char*)frame->getData());
								AlgoOrderInput msg;
								Value& v = doc["order_id"];
								msg.order_id = v.GetUint64();
								v = doc["client_id"];
								msg.client_id = v.GetString();
								v = doc["algo_type"];
								msg.algo_type = v.GetString();
								v = doc["input"];
								msg.input = v.GetString();
                            
									algo_order_input_callback_(msg.order_id, msg.client_id, msg.algo_type, msg.input);
                            
							}
							catch (std::exception& e)
							{
								SPDLOG_ERROR("failed to parse algo order input msg, data[{}], exception: {}", (char*)frame->getData(), e.what());
							}
						}
                        break;
                    }
                    case (int)MsgType::AlgoOrderStatus:
                    {
						if (algo_order_status_callback_)
						{
							try
							{
								Document doc;
								doc.Parse((char*)frame->getData());
								AlgoOrderStatus msg;
								Value& v = doc["order_id"];
								msg.order_id = v.GetUint64();
								v = doc["algo_type"];
								msg.algo_type = v.GetString();
								v = doc["status"];
								msg.status = v.GetString();
                            
									algo_order_status_callback_(msg.order_id, msg.algo_type, msg.status);
                            
							}
							catch (std::exception& e)
							{
								SPDLOG_ERROR("failed to parse algo order action msg, data[{}], exception: {}", (char*)frame->getData(), e.what());
							}
						}
                        break;
                    }
                    case (int)MsgType::AlgoOrderAction:
                    {
						if (algo_order_action_callback_)
						{
							try
							{
								Document doc;
								doc.Parse((char*)frame->getData());
								AlgoOrderAction msg;
								Value& v = doc["order_id"];
								msg.order_id = v.GetUint64();
								v = doc["order_id"];
								msg.order_action_id = v.GetUint64();
								v = doc["order_id"];
								msg.action = v.GetString();
                            
									algo_order_action_callback_(msg.order_id, msg.order_action_id, msg.action);
                            
							}
							catch (std::exception& e)
							{
								SPDLOG_ERROR("failed to parse algo order action msg, data[{}], exception: {}", (char*)frame->getData(), e.what());
							}
						}
                        break;
                    }
                    default:
                    {
                        ;
                    }
                }
            }
        }

        if (nano == -1)
        {
            nano = NanoTimer::getInstance()->getNanoTime();
        }
        scheduler_->update_nano(nano);

    }
}
