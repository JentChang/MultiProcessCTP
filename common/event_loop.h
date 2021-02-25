#ifndef PROJECT_EVENT_LOOP_H
#define PROJECT_EVENT_LOOP_H

#include <memory>
#include <csignal>
#include <spdlog/spdlog.h>
#include "rapidjson/document.h"

#include "msg.h"
#include "md_struct.h"
#include "oms_struct.h"
#include "JournalReader.h"

#include "task_scheduler.h"
#include "Timer.h"

using namespace rapidjson;

namespace BackTraderCN
{
    typedef std::function<void (const Quote& quote)> QuoteCallback;
    typedef std::function<void (const Entrust& entrust)> EntrustCallback;
    typedef std::function<void (const Transaction& transaction)> TransactionCallback;
    typedef std::function<void (const std::string& recipient, std::vector<Instrument>&, bool is_level2)> SubscribeCallback;
    typedef std::function<void (const std::string& recipient, const std::string& client_id)> ReqLoginCallback;
    typedef std::function<void (const OrderInput& input)> OrderInputCallback;
    typedef std::function<void (const OrderAction& action)> OrderActionCallback;
    typedef std::function<void (const Order& order)> OrderCallback;
    typedef std::function<void (const Trade& trade)> TradeCallback;
    typedef std::function<void (uint64_t order_id, const std::string& client_id, const std::string& algo_type, const std::string& algo_order_input)> AlgoOrderInputCallback;
    typedef std::function<void (uint64_t order_id, const std::string& algo_type, const std::string& status)> AlgoOrderStatusCallback;
    typedef std::function<void (uint64_t order_id, uint64_t order_action_id, const std::string& cmd)> AlgoOrderActionCallback;
    typedef std::function<void(int sig)> SignalCallback;

    class EventLoop
    {
    public:
        EventLoop(const std::string& name): quit_(false), name_(name), reader_(nullptr), scheduler_(new TaskScheduler()) {};

        void subscribe_yjj_journal(const std::string& journal_folder, const std::string& journal_name, int64_t offset_nano);

        void register_nanotime_callback(int64_t nano, TSCallback callback); // if nano == 0, trigger at next update
        void register_nanotime_callback_at_next(const char* time_str, TSCallback callback);

        void register_quote_callback(QuoteCallback callback);
        void register_entrust_callback(EntrustCallback callback);
        void register_transaction_callback(TransactionCallback callback);

        void register_order_input_callback(OrderInputCallback callback);
        void register_order_action_callback(OrderActionCallback callback);

        void register_order_callback(OrderCallback callback);
        void register_trade_callback(TradeCallback callback);

        void register_subscribe_callback(SubscribeCallback callback);
        void register_req_login_callback(ReqLoginCallback callback);

        void register_algo_order_input_callback(AlgoOrderInputCallback callback);
        void register_algo_order_status_callback(AlgoOrderStatusCallback callback);
        void register_algo_order_action_callback(AlgoOrderActionCallback callback);

        void register_signal_callback(SignalCallback handler);

        void run();
        void stop();

        void iteration();

        int64_t get_nano() const { return scheduler_->get_nano(); };

    private:
        bool quit_;
        std::string name_;

		std::shared_ptr<JournalReader> reader_;

        std::unique_ptr<TaskScheduler> scheduler_;

        static volatile std::sig_atomic_t signal_received_;
        vector<SignalCallback> signal_callbacks_;

        QuoteCallback quote_callback_;
        EntrustCallback entrust_callback_;
        TransactionCallback transaction_callback_;

        ReqLoginCallback login_callback_;
        SubscribeCallback sub_callback_;
        OrderInputCallback order_input_callback_;
        OrderActionCallback order_action_callback_;
        OrderCallback order_callback_;
        TradeCallback trade_callback_;

        AlgoOrderInputCallback algo_order_input_callback_;
        AlgoOrderStatusCallback algo_order_status_callback_;
        AlgoOrderActionCallback algo_order_action_callback_;

        static void signal_handler(int signal);
    };

}
#endif //PROJECT_EVENT_LOOP_H
