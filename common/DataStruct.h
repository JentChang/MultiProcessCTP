#pragma once
#ifndef _DataStruct_H_
#define _DataStruct_H_

///mmap 地址位置宏
#define MMAP_DIR "D:/mmap_data" 
#define MMAP_MD_DIR "D:/mmap_data/CtpMD" 
#define MMAP_TD_DIR "D:/mmap_data/CtpTD"
#define MMAP_STG_DIR "D:/mmap_data/STG"

///mmap 文件名宏
#define MMAP_TD_ORDER_INSERT_PATH "order_insert" 
#define MMAP_TD_ORDER_ACTION_PATH "order_action"
#define MMAP_TD_POSITION_PATH "position_event"
#define MMAP_TD_OTHER_EVENT_PATH "other2td_event"
#define MMAP_RTN_REQUESTID_PATH "requset_id" //tdevent
#define MMAP_RTN_ORDER_PATH "rtn_order" 
#define MMAP_RTN_TRADE_PATH "rtn_trade" 
#define MMAP_RTN_INVERSTOR_PATH "rtn_inv" 
#define MMAP_RTN_ACCOUNT_PATH "rtn_acc" 

///mmap map映射对象名宏
#define MMAP_INSERT_ORDER_MAP "IN_ORDER" 
#define MMAP_ACTION_ORDER_MAP "AN_ORDER"
#define MMAP_POSITION_MAP "POSITINO"
#define MMAP_OTHER_EVENT_MAP "OTHER"
#define MMAP_RTN_REQUESTID_MAP "RTN_ID" //tdevent
#define MMAP_RTN_ORDER_MAP "RTN_ORDER"
#define MMAP_RTN_TRADE_MAP "RTN_TRADE"
#define MMAP_RTN_INVERSTOR_MAP "RTN_INV"
#define MMAP_RTN_ACCOUNT_MAP "RTN_ACC"

struct LoginInfo
{
	///@param FRONT_ADDR_MD行情地址
	///@param FRONT_ADDR_TD交易地址
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@param authenticate客户端认证请求
	char BrokerID[11];
	char UserID[16];
	char Password[41];
	
	char FRONT_ADDR_MD[100];
	char FRONT_ADDR_TD[100];

	char *ppInstrumentID[100];
	int nCount;

	char AuthCode[17];
	char AppID[33];
};

struct TickInfomation
{
	bool EMPTY = true;
	char datetime[24];

	///交易日
	char	TradingDay[9];
	///合约代码
	char	InstrumentID[31];
	///交易所代码
	char	ExchangeID[9];
	///合约在交易所的代码
	char	ExchangeInstID[31];
	///最新价
	double	LastPrice;
	///上次结算价
	double	PreSettlementPrice;
	///昨收盘
	double	PreClosePrice;
	///昨持仓量
	double	PreOpenInterest;
	///今开盘
	double	OpenPrice;
	///最高价
	double	HighestPrice;
	///最低价
	double	LowestPrice;
	///数量
	int	Volume;
	///成交金额
	double	Turnover;
	///持仓量
	double	OpenInterest;
	///今收盘
	double	ClosePrice;
	///本次结算价
	double	SettlementPrice;
	///涨停板价
	double	UpperLimitPrice;
	///跌停板价
	double	LowerLimitPrice;
	///昨虚实度
	double	PreDelta;
	///今虚实度
	double	CurrDelta;
	///最后修改时间
	char	UpdateTime[9];
	///最后修改毫秒
	int	UpdateMillisec;
	///申买价一
	double	BidPrice1;
	///申买量一
	int	BidVolume1;
	///申卖价一
	double	AskPrice1;
	///申卖量一
	int	AskVolume1;
	///申买价二
	double	BidPrice2;
	///申买量二
	int	BidVolume2;
	///申卖价二
	double	AskPrice2;
	///申卖量二
	int	AskVolume2;
	///申买价三
	double	BidPrice3;
	///申买量三
	int	BidVolume3;
	///申卖价三
	double	AskPrice3;
	///申卖量三
	int	AskVolume3;
	///申买价四
	double	BidPrice4;
	///申买量四
	int	BidVolume4;
	///申卖价四
	double	AskPrice4;
	///申卖量四
	int	AskVolume4;
	///申买价五
	double	BidPrice5;
	///申买量五
	int	BidVolume5;
	///申卖价五
	double	AskPrice5;
	///申卖量五
	int	AskVolume5;
	///当日均价
	double	AveragePrice;
	///业务日期
	char	ActionDay[9];
};

struct BarInfomation
{
	bool EMPTY = true;
	TThostFtdcInstrumentIDType InstrumentID;
	TThostFtdcDateType	TradingDay;
	TThostFtdcTimeType	UpdateTime;

	TThostFtdcPriceType Close;
	TThostFtdcPriceType High;
	TThostFtdcPriceType Low;
	TThostFtdcPriceType Open;

	TThostFtdcVolumeType Volume;
	TThostFtdcMoneyType	Turnover;

	///昨收盘
	TThostFtdcPriceType	PreClosePrice;
	///昨持仓量
	TThostFtdcLargeVolumeType	PreOpenInterest;
};

struct OrderInformation
{
	char StrategyName[30];
	CThostFtdcInputOrderField Order;
};

struct ActionOrderInformation
{
	char StrategyName[30];
	CThostFtdcInputOrderActionField Order;
};

struct AccountInformation
{
	bool EMPTY = true;
	CThostFtdcTradingAccountField AccInfo;
};

struct PostionInformation
{
	bool EMPTY = true;
	CThostFtdcInvestorPositionField PosInfo;
};

struct RtnRequestID
{
	char StrategyName[30];
	int RequestID;
};

struct RtnOrderInfo
{
	char StrategyName[30];
	CThostFtdcOrderField RtnOrderInfo;
};

struct EventType
{
	char StrategyName[30];
///stg请求event类型
#define EVENT_ACCOUNT "EN_ACC"
	char Type[30];
};

class DataStruct
{
};

#endif
