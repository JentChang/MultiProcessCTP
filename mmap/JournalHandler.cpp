/*****************************************************************************
 * Copyright [2017] [taurus.ai]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

/**
 * Journal Handler
 * @Author cjiang (changhao.jiang@taurus.ai)
 * @since   March, 2017
 * base class of Journal reader / writer
 */

#include "JournalHandler.h"
#include "PageProvider.h"
#include "Journal.h"
#include "../common/Timer.h"
#include "../common/SystemDir.h"
#include <sstream>
#include <filesystem>

string JournalHandler::getDefaultName(const string& prefix)
{
    std::stringstream ss;
    ss << prefix << "_" << NanoTimer::getInstance()->getNanoTime();
    return ss.str();
}

size_t JournalHandler::addJournal(const string& _dir, const string& jname)
{
    string dir = SystemDir::getDirNotEndWithPathSep(_dir);
    std::experimental::filesystem::path page_folder_path = _dir;
    if(!std::experimental::filesystem::exists(page_folder_path)) {
        std::experimental::filesystem::create_directories(page_folder_path);
    }

    // register this journal
    int service_idx = page_provider->register_journal(dir, jname);
    journals.push_back(Journal::create(dir, jname, service_idx, page_provider));
    return journals.size() - 1;
}

JournalHandler::~JournalHandler()
{
    page_provider->exit_client();
}