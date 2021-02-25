#ifndef PROJECT_FEEDHANDLER_H
#define PROJECT_FEEDHANDLER_H

#include "md_struct.h"
#include "oms_struct.h"

namespace BackTraderCN
{
    class MarketDataStreamHandler
    {
    public:
        virtual void on_quote(const Quote* quote) {};
        virtual void on_entrust(const Entrust* entrust) {};
        virtual void on_transaction(const Transaction* transaction) {};

        //CryptoCurrency
		/*
        void on_market_data(const LFMarketDataField* data);
        void on_market_bar_data(const LFBarMarketDataField* data);
        void on_price_book_data(const LFPriceBook20Field* data);
        void on_trade_data(const LFL2TradeField* trade);
		*/
    };

    class TraderDataStreamHandler
    {
    public:
        virtual void on_order_input(const OrderInput* input) {};
        virtual void on_order_action(const OrderAction* action) {};
        virtual void on_order(const Order* order) {};
        virtual void on_trade(const Trade* trade) {};
        virtual void on_position(const Position* pos) {};
        virtual void on_position_detail(const Position* pos) {};
        virtual void on_account(const AccountInfo* account) {};
    };
}
#endif //PROJECT_FEEDHANDLER_H
