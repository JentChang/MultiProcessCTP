#pragma once
#ifndef _TDEVENT_H_
#define _TDEVENT_H_
#include "CtpTD.h"
#include "stdafx.h"


class TDEvent
{
public:
	TDEvent(CtpTD* td);
	~TDEvent();

	void start();


private:
	///����¼��
	static void MMAPOrderReader(mutex* mutex);
	///����¼��
	static void MMAPOrderActionReader(mutex* mutex);
	///�ֲֲ�ѯ
	static void MMAPPostionReader(mutex* mutex);
	///event
	static void MMAPOtherEventReader(mutex* mutex);

	static void WriteRtnReq(int order_id, char* stgname);


private:
	static string mmap_td_dir;
	static string mmap_stg_dir;

	static int64_t startNano;
	static CtpTD* td;

	static RtnRequestID send_data;

	static map <string, shared_ptr<JournalWriter>> mmap_writer;
	static map <string, int> mmap_count;
	static map <string, shared_ptr<JournalReader>> mmap_reader;

	vector<thread> thread_vtr;
	mutex* td_mutex;
};

#endif