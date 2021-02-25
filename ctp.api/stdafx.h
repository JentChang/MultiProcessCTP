#pragma once
#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <Windows.h>
#include "DataStruct.h"

#include "YJJ_DECLARE.h"
#include "FrameHeader.h"
#include "Frame.hpp"
#include "Page.h"
#include "Journal.h"
#include "PageProvider.h"
#include "JournalWriter.h"
#include "JournalReader.h"
#include "../common/Timer.h"

#include <cstring>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>


using namespace std;

using namespace rapidjson;


#endif