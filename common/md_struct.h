#ifndef PROJECT_MD_STRUCT_H
#define PROJECT_MD_STRUCT_H

#include "constant.h"

namespace BackTraderCN
{
    //合约信息
    struct Instrument
    {
        char instrument_id[INSTRUMENT_ID_LEN];     //合约ID
        char exchange_id[EXCHANGE_ID_LEN];         //交易所ID
        InstrumentType instrument_type;            //合约类型
    };

    typedef Instrument StockInstrument;

    //期货合约信息
    struct FutureInstrument: Instrument
    {
        char product_id[PRODUCT_ID_LEN];           //产品ID

        int contract_multiplier;                   //合约乘数
        double price_tick;                         //最小变动价位

        char open_date[DATE_LEN];                  //上市日
        char create_date[DATE_LEN];                //创建日
        char expire_date[DATE_LEN];                //到期日

        int delivery_year;                         //交割年份
        int delivery_month;                        //交割月

        bool is_trading;                           //当前是否交易

        double long_margin_ratio;                  //多头保证金率
        double short_margin_ratio;                 //空头保证金率
    };

    //行情
    struct Quote
    {
        char source_id[SOURCE_ID_LEN];              //柜台ID
        char trading_day[DATE_LEN];                 //交易日

        int64_t rcv_time;                           //数据接收时间
        int64_t data_time;                          //数据生成时间

        char instrument_id[INSTRUMENT_ID_LEN];      //合约ID
        char exchange_id[EXCHANGE_ID_LEN];          //交易所ID

        InstrumentType instrument_type;             //合约类型

        double pre_close_price;                     //昨收价
        double pre_settlement_price;                //昨结价

        double last_price;                          //最新价
        int64_t volume;                             //数量
        double turnover;                            //成交金额

        double pre_open_interest;                   //昨持仓量
        double open_interest;                       //持仓量

        double open_price;                          //今开盘
        double high_price;                          //最高价
        double low_price;                           //最低价

        double upper_limit_price;                   //涨停板价
        double lower_limit_price;                   //跌停板价

        double close_price;                         //收盘价
        double settlement_price;                    //结算价

        double bid_price[10];                       //申买价
        double ask_price[10];                       //申卖价
        int64_t	bid_volume[10];                     //申买量
        int64_t	ask_volume[10];                     //申卖量

    };

    //逐笔委托
    struct Entrust
    {
        char source_id[SOURCE_ID_LEN];              //柜台ID
        char trading_day[DATE_LEN];                 //交易日

        int64_t rcv_time;                           //数据接收时间
        int64_t data_time;                          //数据生成时间

        char instrument_id[INSTRUMENT_ID_LEN];      //合约ID
        char exchange_id[EXCHANGE_ID_LEN];          //交易所代码

        InstrumentType instrument_type;             //合约类型

        double price;                               //委托价格
        int64_t volume;                             //委托量
        Side side;                                  //委托方向
        PriceType price_type;                       //订单价格类型（市价、限价、本方最优）

        int64_t main_seq;                           //主序号
        int64_t seq;                                //子序号

    };

    //逐笔成交
    struct Transaction
    {
        char source_id[SOURCE_ID_LEN];              //柜台ID
        char trading_day[DATE_LEN];                 //交易日

        int64_t rcv_time;                           //数据接收时间
        int64_t data_time;                          //数据生成时间

        char instrument_id[INSTRUMENT_ID_LEN];      //合约ID
        char exchange_id[EXCHANGE_ID_LEN];          //交易所代码

        InstrumentType instrument_type;             //合约类型

        double price;                               //成交价
        int64_t volume;                             //成交量

        int64_t bid_no;                             //买方订单号
        int64_t ask_no;                             //卖方订单号

        ExecType exec_type;                         //SZ: 成交标识
        BsFlag  bs_flag;                            //SH: 内外盘标识

        int64_t main_seq;                               //主序号
        int64_t seq;                                    //子序号

    };

    //CryptoCurrency
    typedef Instrument CryptoCurrencyInstrument;
	/*
    //行情
    struct LFMarketDataField //CryptoCurrencyQuote
    {
        char_13  	TradingDay;            //交易日
        char_31  	InstrumentID;          //合约代码
        char_9   	ExchangeID;            //交易所代码
        char_64  	ExchangeInstID;        //合约在交易所的代码
        double   	LastPrice;             //最新价
        double   	PreSettlementPrice;    //上次结算价
        double   	PreClosePrice;         //昨收盘
        double   	PreOpenInterest;       //昨持仓量
        double   	OpenPrice;             //今开盘
        double   	HighestPrice;          //最高价
        double   	LowestPrice;           //最低价
        int      	Volume;                //数量
        double   	Turnover;              //成交金额
        double   	OpenInterest;          //持仓量
        double   	ClosePrice;            //今收盘
        double   	SettlementPrice;       //本次结算价
        double   	UpperLimitPrice;       //涨停板价
        double   	LowerLimitPrice;       //跌停板价
        double   	PreDelta;              //昨虚实度
        double   	CurrDelta;             //今虚实度
        char_13  	UpdateTime;            //最后修改时间
        uint64_t    UpdateMillisec;        //最后修改毫秒
        int64_t   	BidPrice1;             //申买价一
        uint64_t    BidVolume1;            //申买量一
        int64_t   	AskPrice1;             //申卖价一
        uint64_t    AskVolume1;            //申卖量一
        int64_t   	BidPrice2;             //申买价二
        uint64_t    BidVolume2;            //申买量二
        int64_t   	AskPrice2;             //申卖价二
        uint64_t    AskVolume2;            //申卖量二
        int64_t   	BidPrice3;             //申买价三
        uint64_t    BidVolume3;            //申买量三
        int64_t   	AskPrice3;             //申卖价三
        uint64_t    AskVolume3;            //申卖量三
        int64_t   	BidPrice4;             //申买价四
        uint64_t    BidVolume4;            //申买量四
        int64_t   	AskPrice4;             //申卖价四
        uint64_t    AskVolume4;            //申卖量四
        int64_t   	BidPrice5;             //申买价五
        uint64_t    BidVolume5;            //申买量五
        int64_t   	AskPrice5;             //申卖价五
        uint64_t    AskVolume5;            //申卖量五

        int64_t   	BidPrice6;             //申买价
        uint64_t    BidVolume6;            //申买量
        int64_t   	AskPrice6;             //申卖价
        uint64_t    AskVolume6;            //申卖量
        int64_t   	BidPrice7;             //申买价
        uint64_t    BidVolume7;            //申买量
        int64_t   	AskPrice7;             //申卖价
        uint64_t    AskVolume7;            //申卖量
        int64_t   	BidPrice8;             //申买价
        uint64_t    BidVolume8;            //申买量
        int64_t   	AskPrice8;             //申卖价
        uint64_t    AskVolume8;            //申卖量
        int64_t   	BidPrice9;             //申买价
        uint64_t    BidVolume9;            //申买量
        int64_t   	AskPrice9;             //申卖价
        uint64_t    AskVolume9;            //申卖量
        int64_t   	BidPrice10;             //申买价
        uint64_t    BidVolume10;            //申买量
        int64_t   	AskPrice10;             //申卖价
        uint64_t    AskVolume10;            //申卖量
        int64_t   	BidPrice11;             //申买价
        uint64_t    BidVolume11;            //申买量
        int64_t   	AskPrice11;             //申卖价
        uint64_t    AskVolume11;            //申卖量
        int64_t   	BidPrice12;             //申买价
        uint64_t    BidVolume12;            //申买量
        int64_t   	AskPrice12;             //申卖价
        uint64_t    AskVolume12;            //申卖量
        int64_t   	BidPrice13;             //申买价
        uint64_t    BidVolume13;            //申买量
        int64_t   	AskPrice13;             //申卖价
        uint64_t    AskVolume13;            //申卖量
        int64_t   	BidPrice14;             //申买价
        uint64_t    BidVolume14;            //申买量
        int64_t   	AskPrice14;             //申卖价
        uint64_t    AskVolume14;            //申卖量
        int64_t   	BidPrice15;             //申买价
        uint64_t    BidVolume15;            //申买量
        int64_t   	AskPrice15;             //申卖价
        uint64_t    AskVolume15;            //申卖量
        int64_t   	BidPrice16;             //申买价
        uint64_t    BidVolume16;            //申买量
        int64_t   	AskPrice16;             //申卖价
        uint64_t    AskVolume16;            //申卖量
        int64_t   	BidPrice17;             //申买价
        uint64_t    BidVolume17;            //申买量
        int64_t   	AskPrice17;             //申卖价
        uint64_t    AskVolume17;            //申卖量
        int64_t   	BidPrice18;             //申买价
        uint64_t    BidVolume18;            //申买量
        int64_t   	AskPrice18;             //申卖价
        uint64_t    AskVolume18;            //申卖量
        int64_t   	BidPrice19;             //申买价
        uint64_t    BidVolume19;            //申买量
        int64_t   	AskPrice19;             //申卖价
        uint64_t    AskVolume19;            //申卖量
        int64_t   	BidPrice20;             //申买价
        uint64_t    BidVolume20;            //申买量
        int64_t   	AskPrice20;             //申卖价
        uint64_t    AskVolume20;            //申卖量
    };


    struct LFBarMarketDataField
    {
        char_9   	TradingDay;            //交易日
        char_31  	InstrumentID;          //合约代码
        char_9   	ExchangeID;			   //交易所代码
        int64_t   	UpperLimitPrice;       //涨停板价
        int64_t   	LowerLimitPrice;       //跌停板价
        char_13  	StartUpdateTime;       //首tick修改时间
        int      	StartUpdateMillisec;   //首tick最后修改毫秒
        char_13  	EndUpdateTime;         //尾tick最后修改时间
        int      	EndUpdateMillisec;     //尾tick最后修改毫秒
        int			PeriodMillisec;        //周期（毫秒）
        int64_t   	Open;                  //开
        int64_t   	Close;                 //收
        int64_t   	Low;                   //低
        int64_t   	High;                  //高
        uint64_t   	Volume;                //区间交易量
        uint64_t   	StartVolume;           //初始总交易量
        double   	Turnover;              //区间成交金额
        double   	StartTurnover;         //初始成交金额
        int64_t     BestBidPrice;
        int64_t     BestAskPrice;
    };

    struct LFPriceBook20Field
    {
        char_31  	InstrumentID;
        char_9   	ExchangeID;
        uint64_t    UpdateMicroSecond;
        int		BidLevelCount;              //有效的数目
        int		AskLevelCount;              //有效的数目
        int64_t price;
        uint64_t volume;
        int64_t price;
        uint64_t volume;
    };


    struct LFL2TradeField
    {
        char_9   	TradeTime;             //成交时间（秒）
        char_9   	ExchangeID;            //交易所代码
        char_31  	InstrumentID;          //合约代码
        int64_t   	Price;                 //成交价格
        uint64_t   	Volume;                //成交数量
        char_2   	OrderKind;             //报单类型
        char_2   	OrderBSFlag;           //内外盘标志
    };
	*/
}
#endif //PROJECT_MD_STRUCT_H
