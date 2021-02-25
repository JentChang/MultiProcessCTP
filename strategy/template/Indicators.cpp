#include "Indicators.h"


Indicators::Indicators()
{
	Indicators::__tick_pre__.EMPTY = true;
	Indicators::__bar__.EMPTY = true;
}

Indicators::~Indicators()
{
}


void Indicators::make_bar(TickInfomation tick)
{
	string bar_time = tick.UpdateTime;
	bar_time = bar_time.substr(0, 5);
	if (bar_time < "09:00" ||
		(bar_time >= "15:00" && bar_time < "21:00") ||
		(bar_time >= "11:30" && bar_time < "13:30") ||
		bar_time >= "23:00" ||
		(bar_time >= "10:15" & bar_time < "10:30")
		)
	{
		return;
	}
	// 合成k线
	bool newMinitue = false;
	Indicators::__barclose__ = false;
	Indicators::time_strs = Split(bar_time, ":");
	if (time_strs.size() < 2)
	{
		return;
	}
	if (Indicators::__bar__.EMPTY)
	{
		newMinitue = true;
	}
	else
	{
		if (atoi(time_strs[1].c_str()) - Indicators::__updateTime__ >= Indicators::__fre__ || 
			atoi(time_strs[0].c_str()) != Indicators::__updateH__ 
			)
		{
			///起始时间不对
			if (atoi(time_strs[1].c_str()) % 15 != 0)
			{
				int utime = atoi(time_strs[1].c_str()) - (atoi(time_strs[1].c_str()) % 15);
				string sign = ":";
				string udatetime = time_strs[0].c_str() + sign + to_string(utime) + sign + time_strs[2].c_str();
				bar_time = udatetime;
				time_strs = Split(bar_time, ":");
			}
			//当前tick是否属于当前bar时间段
			newMinitue = true;
			Indicators::__newBar__ = Indicators::__bar__;
			Indicators::__barclose__ = true;
		}
	}

	if (newMinitue)
	{
		////新建bar
		Indicators::__bar__.EMPTY = false;

		strcpy(Indicators::__bar__.InstrumentID, tick.InstrumentID);
		strcpy(Indicators::__bar__.TradingDay, tick.TradingDay);
		Indicators::__bar__.Close = tick.LastPrice;
		Indicators::__bar__.High = tick.LastPrice;
		Indicators::__bar__.Low = tick.LastPrice;
		Indicators::__bar__.Volume = tick.Volume;
		Indicators::__bar__.Open = tick.LastPrice;
		strcpy(Indicators::__bar__.UpdateTime, tick.UpdateTime);
		Indicators::__updateTime__ = atoi(time_strs[1].c_str());
		Indicators::__updateH__ = atoi(time_strs[0].c_str());

		Indicators::__bar__.Turnover = tick.Turnover;
		Indicators::__bar__.PreClosePrice = tick.PreClosePrice;
		Indicators::__bar__.PreOpenInterest = tick.PreOpenInterest;
	}
	else
	{
		////更新bar
		Indicators::__bar__.Close = tick.LastPrice;
		Indicators::__bar__.High = max(Indicators::__bar__.High, tick.LastPrice);
		Indicators::__bar__.Low = min(Indicators::__bar__.Low, tick.LastPrice);

		Indicators::__bar__.Volume += tick.Volume;
		Indicators::__bar__.Turnover += tick.Turnover;
	}
}

BarInfomation* Indicators::bar()
{
	return &(Indicators::__newBar__);
}

bool Indicators::barclose()
{
	return Indicators::__barclose__;
}

void Indicators::fre(int f)
{
	Indicators::__fre__ = f;
}
