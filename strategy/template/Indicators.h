#pragma once

#ifndef _INDICATOR_H_
#define _INDICATOR_H_

#include "DataStruct.h"

class Indicators
{
public:
	Indicators();
	~Indicators();

public:
	void make_bar(TickInfomation);
	BarInfomation* bar();
	bool barclose();
	void fre(int);

private:
	TickInfomation __tick_pre__;
	BarInfomation __bar__;
	int __updateTime__ = 1;
	int __updateH__ = 9;

	int __fre__ = 15;

	bool __barclose__=false;
	BarInfomation __newBar__;

	double __i__ = 0;
	double __j__ = 0;
	bool __BS__ = true;
	vector<string> time_strs;
};

#endif