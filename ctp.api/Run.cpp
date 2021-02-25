#include "CtpTD.h"
#include "CtpMD.h"
#include "TDEvent.h"
#include "stdafx.h"
#include "ClearFile.h"




int main(int argc, char* argv[]) {
	///清空mmap
	//ClearFile* clear_file = new ClearFile(MMAP_DIR);

	///登录信息
	ifstream ifs("../data.json/ctp_config/ctp_config.json");
	IStreamWrapper isw(ifs);

	Document docRead;
	docRead.ParseStream(isw);
	if (docRead.HasParseError()) {
		std::cout << "readFromJson error" << std::endl;
	}

	LoginInfo login_info;
	strcpy(login_info.BrokerID, docRead["brokerID"].GetString());
	strcpy(login_info.UserID, docRead["username"].GetString());
	strcpy(login_info.Password, docRead["password"].GetString());
	strcpy(login_info.FRONT_ADDR_MD, docRead["mdUrl"].GetString());
	strcpy(login_info.FRONT_ADDR_TD, docRead["tdUrl"].GetString());

	CThostFtdcTraderApi* tdapi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	CThostFtdcReqUserLoginField* loginField = new CThostFtdcReqUserLoginField();

	strncpy(loginField->UserID, login_info.UserID, sizeof(loginField->UserID));
	strncpy(loginField->Password, login_info.Password, sizeof(loginField->Password));
	strncpy(loginField->BrokerID, login_info.BrokerID, sizeof(loginField->BrokerID));

	std::string filePath = "../data.json/instruments/instruments.json";

	///注册td
	CtpTD* td = new CtpTD(tdapi, loginField);
	///验证
	td->InLoginInfo(&login_info);
	tdapi->RegisterSpi(td);

	///公有私有
	tdapi->SubscribePublicTopic(THOST_TERT_RESTART);
	tdapi->SubscribePrivateTopic(THOST_TERT_RESTART);

	tdapi->RegisterFront(login_info.FRONT_ADDR_TD);


	tdapi->Init();
	std::this_thread::sleep_for(std::chrono::seconds(10));

	///行情子线程
	TDEvent* tdevent = new TDEvent(td);
	tdevent->start();


	///mdevent 行情推送
	///start md
	CThostFtdcMdApi* mdapi = CThostFtdcMdApi::CreateFtdcMdApi();
	CtpMD* md = new CtpMD(mdapi, loginField);

	mdapi->RegisterSpi(md);

	mdapi->RegisterFront(login_info.FRONT_ADDR_MD);
	mdapi->Init();
	std::this_thread::sleep_for(std::chrono::seconds(2));


	char** insts = new char*[3];
	insts[0] = "rb2105";
	insts[1] = "hc2105";
	insts[2] = "m2105";

	int ret = md->OnSubscribeMarketData(insts, 3);


	//tdapi->Join();
	mdapi->Join();
	return 0;
}