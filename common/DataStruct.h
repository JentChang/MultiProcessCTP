#pragma once
#ifndef _DataStruct_H_
#define _DataStruct_H_

///mmap ��ַλ�ú�
#define MMAP_DIR "D:/mmap_data" 
#define MMAP_MD_DIR "D:/mmap_data/CtpMD" 
#define MMAP_TD_DIR "D:/mmap_data/CtpTD"
#define MMAP_STG_DIR "D:/mmap_data/STG"

///mmap �ļ�����
#define MMAP_TD_ORDER_INSERT_PATH "order_insert" 
#define MMAP_TD_ORDER_ACTION_PATH "order_action"
#define MMAP_TD_POSITION_PATH "position_event"
#define MMAP_TD_OTHER_EVENT_PATH "other2td_event"
#define MMAP_RTN_REQUESTID_PATH "requset_id" //tdevent
#define MMAP_RTN_ORDER_PATH "rtn_order" 
#define MMAP_RTN_TRADE_PATH "rtn_trade" 
#define MMAP_RTN_INVERSTOR_PATH "rtn_inv" 
#define MMAP_RTN_ACCOUNT_PATH "rtn_acc" 

///mmap mapӳ���������
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
	///@param FRONT_ADDR_MD�����ַ
	///@param FRONT_ADDR_TD���׵�ַ
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@param authenticate�ͻ�����֤����
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

	///������
	char	TradingDay[9];
	///��Լ����
	char	InstrumentID[31];
	///����������
	char	ExchangeID[9];
	///��Լ�ڽ������Ĵ���
	char	ExchangeInstID[31];
	///���¼�
	double	LastPrice;
	///�ϴν����
	double	PreSettlementPrice;
	///������
	double	PreClosePrice;
	///��ֲ���
	double	PreOpenInterest;
	///����
	double	OpenPrice;
	///��߼�
	double	HighestPrice;
	///��ͼ�
	double	LowestPrice;
	///����
	int	Volume;
	///�ɽ����
	double	Turnover;
	///�ֲ���
	double	OpenInterest;
	///������
	double	ClosePrice;
	///���ν����
	double	SettlementPrice;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
	///����ʵ��
	double	PreDelta;
	///����ʵ��
	double	CurrDelta;
	///����޸�ʱ��
	char	UpdateTime[9];
	///����޸ĺ���
	int	UpdateMillisec;
	///�����һ
	double	BidPrice1;
	///������һ
	int	BidVolume1;
	///������һ
	double	AskPrice1;
	///������һ
	int	AskVolume1;
	///����۶�
	double	BidPrice2;
	///��������
	int	BidVolume2;
	///�����۶�
	double	AskPrice2;
	///��������
	int	AskVolume2;
	///�������
	double	BidPrice3;
	///��������
	int	BidVolume3;
	///��������
	double	AskPrice3;
	///��������
	int	AskVolume3;
	///�������
	double	BidPrice4;
	///��������
	int	BidVolume4;
	///��������
	double	AskPrice4;
	///��������
	int	AskVolume4;
	///�������
	double	BidPrice5;
	///��������
	int	BidVolume5;
	///��������
	double	AskPrice5;
	///��������
	int	AskVolume5;
	///���վ���
	double	AveragePrice;
	///ҵ������
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

	///������
	TThostFtdcPriceType	PreClosePrice;
	///��ֲ���
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
///stg����event����
#define EVENT_ACCOUNT "EN_ACC"
	char Type[30];
};

class DataStruct
{
};

#endif
