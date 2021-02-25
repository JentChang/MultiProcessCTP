#include "CtpTD.h"


CtpTD::CtpTD(CThostFtdcTraderApi* api, CThostFtdcReqUserLoginField* pUserLogin) :
	api(api), userLogin(pUserLogin)
{
	this->requestID = 1;
	this->rtn_order = new CThostFtdcOrderField();

	CtpTD::mmap_td_dir = MMAP_TD_DIR;
	CtpTD::startNano = NanoTimer::getInstance()->getNano();
	///write
	CtpTD::mmap_writer[MMAP_RTN_TRADE_MAP] = JournalWriter::create(this->mmap_td_dir + "/rtn_trade", MMAP_RTN_TRADE_PATH);
	CtpTD::mmap_count[MMAP_RTN_TRADE_MAP] = 0;
	CtpTD::mmap_writer[MMAP_RTN_INVERSTOR_MAP] = JournalWriter::create(this->mmap_td_dir + "/rtn_pos", MMAP_RTN_INVERSTOR_PATH);
	CtpTD::mmap_count[MMAP_RTN_INVERSTOR_MAP] = 0;
	CtpTD::mmap_writer[MMAP_RTN_ACCOUNT_MAP] = JournalWriter::create(this->mmap_td_dir + "/rtn_acc", MMAP_RTN_ACCOUNT_PATH);
	CtpTD::mmap_count[MMAP_RTN_ACCOUNT_MAP] = 0;
}


CtpTD::~CtpTD()
{
	this->api = nullptr;
	this->userLogin = nullptr;
	this->userLogin_RSP = nullptr;
	delete this->rtn_order;
}


void CtpTD::OnFrontConnected()
{
	std::cout << "OnFrontConnected..." << std::endl;
	if (userLogin)
	{
		//api->ReqUserLogin(userLogin, requestID++);
		CThostFtdcReqAuthenticateField auth_info = { 0 };
		strncpy(auth_info.BrokerID, userLogin->BrokerID, sizeof(TThostFtdcBrokerIDType));
		strncpy(auth_info.UserID, userLogin->UserID, sizeof(TThostFtdcUserIDType));

		strcpy(auth_info.AuthCode, this->login_info->AuthCode);
		strcpy(auth_info.AppID, this->login_info->AppID);

		int AuthRet = api->ReqAuthenticate(&auth_info, requestID++);
		std::cout << "ReqAuthenticate response:" << AuthRet << std::endl;

	}
}

void CtpTD::OnFrontDisconnected(int nReason)
{
	std::cout << "OnFrontDisconnected, nReason:" << nReason << std::endl;
}

void CtpTD::OnRspAuthenticate(CThostFtdcRspAuthenticateField * pRspAuthenticateField, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "OnRspAuthenticate ok" << std::endl;
		api->ReqUserLogin(userLogin, requestID++);
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspAuthenticate error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///��¼������Ӧ
void CtpTD::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		std::cout << "OnRspUserLogin ok" << std::endl;
		this->tradingDate = api->GetTradingDay();

		this->userLogin_RSP = pRspUserLogin;

		std::cout << "TradingDay:" << tradingDate.c_str() << std::endl;
		cout << "��¼�ɹ�. " << endl
			<< "ǰ�ñ��:" << pRspUserLogin->FrontID << endl
			<< "�Ự���" << pRspUserLogin->SessionID << endl
			<< "��󱨵�����:" << pRspUserLogin->MaxOrderRef << endl
			<< "������ʱ�䣺" << pRspUserLogin->SHFETime << endl
			<< "������ʱ�䣺" << pRspUserLogin->DCETime << endl
			<< "֣����ʱ�䣺" << pRspUserLogin->CZCETime << endl
			<< "�н���ʱ�䣺" << pRspUserLogin->FFEXTime << endl
			<< "��Դ����ʱ�䣺" << pRspUserLogin->INETime << endl;
		
		//���ڻ��̵�һ�η��ͽ���ָ��ǰ����Ҫ�Ȳ�ѯͶ���߽������� Ͷ����ȷ���Ժ���ܽ��ס�
		//��ѯ�Ƿ��Ѿ�����ȷ��
		CThostFtdcSettlementInfoConfirmField pSettlementInfoConfirm;
		memset(&pSettlementInfoConfirm, 0, sizeof(CThostFtdcSettlementInfoConfirmField));
		strncpy(pSettlementInfoConfirm.BrokerID, pRspUserLogin->BrokerID, sizeof(TThostFtdcBrokerIDType));
		strncpy(pSettlementInfoConfirm.InvestorID, pRspUserLogin->UserID, sizeof(TThostFtdcInvestorIDType));
		api->ReqSettlementInfoConfirm(&pSettlementInfoConfirm, requestID++);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspUserLogin error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}


///�����ѯ������Ϣȷ����Ӧ
void CtpTD::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (nullptr == pSettlementInfoConfirm)
		{
			std::cout << "OnRspQrySettlementInfoConfirm:"
				<< pSettlementInfoConfirm->ConfirmDate << ","
				<< pSettlementInfoConfirm->ConfirmTime << ","
				<< bIsLast
				<< std::endl;


			string lastConfirmDate = pSettlementInfoConfirm->ConfirmDate;
			cout << lastConfirmDate.c_str() << endl;
			cout << pSettlementInfoConfirm->ConfirmTime << endl;

			if (lastConfirmDate != tradingDate) {
				//���컹ûȷ�Ϲ�, ��һ�η��ͽ���ָ��ǰ����ѯͶ���߽�����
				std::this_thread::sleep_for(std::chrono::seconds(1));

				CThostFtdcQrySettlementInfoField field;
				memset(&field, 0, sizeof(CThostFtdcQrySettlementInfoField));
				strncpy(field.BrokerID, pSettlementInfoConfirm->BrokerID, sizeof(TThostFtdcBrokerIDType));
				strncpy(field.InvestorID, pSettlementInfoConfirm->InvestorID, sizeof(TThostFtdcInvestorIDType));
				strncpy(field.TradingDay, pSettlementInfoConfirm->ConfirmDate, sizeof(TThostFtdcDateType));

				api->ReqQrySettlementInfo(&field, requestID++);
			}
			else {
				//�����Ѿ�ȷ��
				CThostFtdcQryTradingAccountField account;
				memset(&account, 0, sizeof(CThostFtdcQryTradingAccountField));
				strncpy(account.BrokerID, pSettlementInfoConfirm->BrokerID, sizeof(TThostFtdcBrokerIDType));
				strncpy(account.InvestorID, pSettlementInfoConfirm->InvestorID, sizeof(TThostFtdcInvestorIDType));
				api->ReqQryTradingAccount(&account, requestID++);
			}

		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspQrySettlementInfoConfirm error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///�����ѯͶ���߽�������Ӧ
void CtpTD::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	cout << "�����ѯͶ���߽�������Ӧ, ,  ID: " << nRequestID << endl;
	cout << pSettlementInfo->Content << endl;
	if (pRspInfo == nullptr || pRspInfo->ErrorID == 0) {
		if (bIsLast == true) {
			//ȷ��Ͷ���߽�����
			CThostFtdcSettlementInfoConfirmField field;
			memset(&field, 0, sizeof(CThostFtdcQryTradingAccountField));
			strncpy(field.BrokerID, pSettlementInfo->BrokerID, sizeof(TThostFtdcBrokerIDType));
			strncpy(field.InvestorID, pSettlementInfo->InvestorID, sizeof(TThostFtdcInvestorIDType));
			int result = api->ReqSettlementInfoConfirm(&field, requestID++);
			cout << "result:" << result << endl;
		}
	}
	else {
		cout << "����Ӧ��:" << pRspInfo->ErrorID << pRspInfo->ErrorMsg << " ����IDΪ" << requestID << endl;
	}
}


void CtpTD::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (pSettlementInfoConfirm != nullptr) {
			cout << "���͹�˾����:" << pSettlementInfoConfirm->BrokerID << endl
				<< "�û��˺�:" << pSettlementInfoConfirm->InvestorID << endl
				<< "ȷ�����ڣ�" << pSettlementInfoConfirm->ConfirmDate << endl
				<< "ȷ��ʱ�䣺" << pSettlementInfoConfirm->ConfirmTime << endl;

			//��ѯ���п��ú�Լ��Ϣ
			CThostFtdcQryInstrumentField myreq = CThostFtdcQryInstrumentField();
			memset(&myreq, 0, sizeof(CThostFtdcQryInstrumentField));
			api->ReqQryInstrument(&myreq, requestID++);
		}
		else {
			cout << "Ͷ���߽�����ȷ����ӦΪ��, pSettlementInfoConfirm:" << pSettlementInfoConfirm << endl;
		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspUserLogin error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}



///�����ѯ��Լ��Ӧ
void CtpTD::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast)
		{
			//do nothing
		}
		else {
			CThostFtdcInstrumentField instrument;
			memcpy(&instrument, pInstrument, sizeof(CThostFtdcInstrumentField));
			instruments.push_back(instrument);
		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspQryInstrument error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

void CtpTD::InLoginInfo(LoginInfo* login_info)
{
	this->login_info = login_info;
}

CThostFtdcOrderField CtpTD::ReqOrderInsert(CThostFtdcInputOrderField order)
{
	CThostFtdcInputOrderField* order_insert = new CThostFtdcInputOrderField();
	memset(order_insert, 0, sizeof(CThostFtdcInputOrderField));

	//strcpy(order.InstrumentID, InstrumentID);

	//order.Direction = THOST_FTDC_D_Buy;
	//order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	order_insert->LimitPrice = order.LimitPrice;
	order_insert->VolumeTotalOriginal = order.VolumeTotalOriginal;
	strcpy(order_insert->InstrumentID, order.InstrumentID);
	order_insert->Direction = order.Direction;
	order_insert->CombOffsetFlag[0] = order.CombOffsetFlag[0];

	///���͹�˾����
	strcpy(order_insert->BrokerID, this->login_info->BrokerID);
	///Ͷ���ߴ���
	strcpy(order_insert->InvestorID, this->login_info->UserID);
	strcpy(order_insert->UserID, this->login_info->UserID);
	///��������
	sprintf(order_insert->OrderRef, "%d", this->requestID++);
	///�û�����
//	TThostFtdcUserIDType	UserID;
	///�����۸�����: �޼�
	order_insert->OrderPriceType = THOST_FTDC_OPT_LimitPrice;

	///���Ͷ���ױ���־
	order_insert->CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;

	///GTD����
	//TThostFtdcDateType	GTDDate;

	///��Ч������: ������Ч
	order_insert->TimeCondition = THOST_FTDC_TC_GFD;///IOC
	///�ɽ�������: �κ�����
	order_insert->VolumeCondition = THOST_FTDC_VC_AV;//CV
	///��С�ɽ���: 1
	order_insert->MinVolume = 1;
	///��������: ����
	order_insert->ContingentCondition = THOST_FTDC_CC_Immediately;

	///ֹ���
	//	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��: ��ǿƽ
	order_insert->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	order_insert->IsAutoSuspend = 0;
	///ҵ��Ԫ
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
	order_insert->UserForceClose = 0;

	memset(this->rtn_order, 0, sizeof(CThostFtdcOrderField));
	this->rtn_order->RequestID = -1;
	int iResult = this->api->ReqOrderInsert(order_insert, ++requestID);
	delete order_insert;
	std::cout << "order requestID: " << requestID-1;
	cerr << "--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
	while (this->rtn_order->RequestID == -1)
	{

	}
	std::cout << " order id : " << this->rtn_order->OrderSysID << endl;
	if (iResult == 0)
	{
		return 	*this->rtn_order;
	}
	else
	{
		this->rtn_order->RequestID = -1;
		return *this->rtn_order;
	}
}

void CtpTD::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderInsert" << endl;
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast)
		{
			//do nothing
		}
		else {

		}
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspOrderInsert error:"
			<< pRspInfo->ErrorID << ", "
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}

///����֪ͨ
void CtpTD::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	memcpy(this->rtn_order, pOrder, sizeof(CThostFtdcOrderField));

}

void CtpTD::OnRtnTrade(CThostFtdcTradeField * pTrade)
{
	this->mmap_writer[MMAP_RTN_TRADE_MAP]->write_frame(
		pTrade,
		sizeof(CThostFtdcTradeField), 1, 12345, true,
		this->mmap_count[MMAP_RTN_TRADE_MAP]);
	this->mmap_count[MMAP_RTN_TRADE_MAP] ++;
	
}

void CtpTD::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField * pInvestorPosition, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	PostionInformation* pos_info = new PostionInformation();
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast || pInvestorPosition == nullptr)
		{
			pos_info->EMPTY = true;
		}
		else {
			pos_info->EMPTY = false;
			pos_info->PosInfo = *pInvestorPosition;
		}
	}
	else {
		pos_info->EMPTY = true;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspQryTradingAccount error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
	this->CtpTD::mmap_writer[MMAP_RTN_INVERSTOR_MAP]->write_frame(
		pos_info,
		sizeof(PostionInformation), 1, 12345, true,
		this->mmap_count[MMAP_RTN_INVERSTOR_MAP]);
	this->mmap_count[MMAP_RTN_INVERSTOR_MAP] ++;
	delete pos_info;
}

void CtpTD::OnRspQryTradingAccount(CThostFtdcTradingAccountField * pTradingAccount, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	AccountInformation* acc_info = new AccountInformation();
	if (nullptr == pRspInfo || 0 == pRspInfo->ErrorID)
	{
		if (bIsLast || pTradingAccount==nullptr)
		{
			acc_info->EMPTY = true;
		}
		else {
			acc_info->EMPTY = false;
			acc_info->AccInfo = *pTradingAccount;
		}
	}
	else {
		acc_info->EMPTY = true;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "OnRspQryTradingAccount error:"
			<< pRspInfo->ErrorID
			<< pRspInfo->ErrorMsg
			<< std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
	this->CtpTD::mmap_writer[MMAP_RTN_ACCOUNT_MAP]->write_frame(
		acc_info,
		sizeof(AccountInformation), 1, 12345, true,
		++this->mmap_count[MMAP_RTN_ACCOUNT_MAP]);
	delete acc_info;
}

int CtpTD::ReqOrderAction(CThostFtdcInputOrderActionField pInputOrderAction)
{
	///���͹�˾����
	strcpy(pInputOrderAction.BrokerID, this->login_info->BrokerID);
	///Ͷ���ߴ���
	strcpy(pInputOrderAction.InvestorID, this->login_info->UserID);
	strcpy(pInputOrderAction.UserID, this->login_info->UserID);
	///ǰ�ñ��
	pInputOrderAction.FrontID = this->userLogin_RSP->FrontID;
	///�Ự���
	pInputOrderAction.SessionID = this->userLogin_RSP->SessionID;

	pInputOrderAction.ActionFlag = THOST_FTDC_AF_Delete;
	int iResult = this->api->ReqOrderAction(&pInputOrderAction, requestID++);
	std::cout << "action order requestID: " << requestID;
	cerr << "--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
	if (iResult == 0)
	{
		return requestID;
	}
	else
	{
		return -1;
	}

}

void CtpTD::ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField pQryInvestorPosition)
{
	///����������
	///��Լ����
	///���͹�˾����
	strcpy(pQryInvestorPosition.BrokerID, this->login_info->BrokerID);
	///Ͷ���ߴ���
	strcpy(pQryInvestorPosition.InvestorID, this->login_info->UserID);

	int iResult = this->api->ReqQryInvestorPosition(&pQryInvestorPosition, requestID++);
	cerr << "--->>> ��ѯ�ֲ�����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CtpTD::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField pQryTradingAccount;
	memset(&pQryTradingAccount, 0, sizeof(CThostFtdcQryTradingAccountField));

	///���͹�˾����
	strcpy(pQryTradingAccount.BrokerID, this->login_info->BrokerID);
	///Ͷ���ߴ���
	strcpy(pQryTradingAccount.InvestorID, this->login_info->UserID);
	///Ͷ�����ʺ�
	strcpy(pQryTradingAccount.AccountID, this->login_info->UserID);

	int iResult = this->api->ReqQryTradingAccount(&pQryTradingAccount, requestID++);
	cerr << "--->>> ��ѯ�ʽ�����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}


