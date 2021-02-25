#pragma once

#ifndef CTP_MD_H_
#define CTP_MD_H_

#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiStruct.h"
#include "stdafx.h"



class CtpMD : public CThostFtdcMdSpi
{
public:
	CtpMD(CThostFtdcMdApi* api, CThostFtdcReqUserLoginField* pUserLogin);
	~CtpMD();

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse) {};

	///��������
	int OnSubscribeMarketData(char *ppInstrumentID[], int nCount);
	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData);

private:
	CThostFtdcMdApi* api = nullptr;
	
	CThostFtdcReqUserLoginField* userLogin = nullptr;
	int requestID;

	string mmap_dir;
	string mmap_path;
	int64_t startNano;
	map <string, shared_ptr<JournalWriter>> mmap_writer;
	map <string, int> mmap_count;

	char** insts = nullptr;
	int insts_count;
};

#endif 