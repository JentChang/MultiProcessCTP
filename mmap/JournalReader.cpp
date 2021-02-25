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
 * JournalReader
 * @Author cjiang (changhao.jiang@taurus.ai)
 * @since   March, 2017
 * provide read access to journals.
 * one journal reader may access multiple journals
 * and combine all these streams into one stream
 * with nano time order.
 */

#include "JournalReader.h"
#include "PageProvider.h"
#include "../common/Timer.h"
#include "../common/SystemDir.h"
#include <sstream>
#include <assert.h>


const string JournalReader::FILE_PREFIX = "reader";

JournalReader::JournalReader(std::shared_ptr<PageProvider> ptr): JournalHandler(ptr)
{
    journalMap.clear();
}

size_t JournalReader::addJournal(const string& dir, const string& jname)
{
    if (journalMap.find(jname) != journalMap.end())
    {
        return journalMap[jname];
    }
    else
    {
        size_t idx = JournalHandler::addJournal(dir, jname);
        journalMap[jname] = idx;
        return idx;
    }
}

std::shared_ptr<JournalReader> JournalReader::create(const vector<string>& dirs, const vector<string>& jnames, int64_t startTime, const string& readerName)
{
    std::stringstream ss;
    ss << readerName << "_R";
    string clientName = ss.str();
#ifdef USE_PAGED_SERVICE
    std::shared_ptr<PageProvider> provider = std::shared_ptr<PageProvider>(new ClientPageProvider(clientName, false));
#else
    std::shared_ptr<PageProvider> provider = std::shared_ptr<PageProvider>(new LocalPageProvider(false));
#endif
    std::shared_ptr<JournalReader> jrp = std::shared_ptr<JournalReader>(new JournalReader(provider));

    assert(dirs.size() == jnames.size());
    for (size_t i = 0; i < dirs.size(); i++)
        jrp->addJournal(dirs[i], jnames[i]);
    jrp->jumpStart(startTime);
    return jrp;
}

std::shared_ptr<JournalReader> JournalReader::create(const string& dir, const string& jname, int64_t startTime, const string& readerName)
{
    vector<string> dirs = {dir};
    vector<string> jnames = {jname};
    return create(dirs, jnames, startTime, readerName);
}

std::shared_ptr<JournalReader> JournalReader::create(const vector<string>& dirs, const vector<string>& jnames, int64_t startTime)
{
    return JournalReader::create(dirs, jnames, startTime, getDefaultName(JournalReader::FILE_PREFIX));
}

std::shared_ptr<JournalReader> JournalReader::create(const string& dir, const string& jname, int64_t startTime)
{
    return JournalReader::create(dir, jname, startTime, getDefaultName(JournalReader::FILE_PREFIX));
}

std::shared_ptr<JournalReader> JournalReader::create(const vector<string>& dirs,
                                       const vector<string>& jnames,
                                       const vector<IJournalVisitor*>& visitors,
                                       int64_t startTime,
                                       const string& readerName)
{
    std::shared_ptr<JournalReader> jrp = JournalReader::create(dirs, jnames, startTime, readerName);
    jrp->visitors = visitors;
    return jrp;
}

std::shared_ptr<JournalReader> JournalReader::createReaderWithSys(const vector<string>& dirs,
                                                    const vector<string>& jnames,
                                                    int64_t startTime,
                                                    const string& readerName)
{
    vector<string> ndirs(dirs.begin(), dirs.end());
    vector<string> njnames(jnames.begin(), jnames.end());
	JournalPair sysPair = SystemDir::getSysJournalPair();
    ndirs.push_back(sysPair.first);
    njnames.push_back(sysPair.second);
    return create(ndirs, njnames, startTime, readerName);
}

std::shared_ptr<JournalReader> JournalReader::createSysReader(const string& clientName)
{
	JournalPair sysPair = SystemDir::getSysJournalPair();
    return JournalReader::create(sysPair.first, sysPair.second, NanoTimer::getInstance()->getNanoTime(), clientName);
}

std::shared_ptr<JournalReader> JournalReader::createRevisableReader(const string& readerName)
{
    std::stringstream ss;
    ss << readerName << "_SR";
    string clientName = ss.str();
#ifdef USE_PAGED_SERVICE
    std::shared_ptr<PageProvider> provider = std::shared_ptr<PageProvider>(new ClientPageProvider(clientName, false, true));
#else
    std::shared_ptr<PageProvider> provider = std::shared_ptr<PageProvider>(new LocalPageProvider(false, true));
#endif
    std::shared_ptr<JournalReader> jrp = std::shared_ptr<JournalReader>(new JournalReader(provider));
	JournalPair sysPair = SystemDir::getSysJournalPair();
    jrp->addJournal(sysPair.first, sysPair.second);
    jrp->jumpStart(NanoTimer::getInstance()->getNanoTime());
    return jrp;
}

void JournalReader::jumpStart(int64_t startTime)
{
    for (std::shared_ptr<Journal>& journal: journals)
        journal->seekTime(startTime);
}

string JournalReader::getFrameName() const
{
    if (curJournal.get() == nullptr)
        return string("");
    else
        return curJournal->getShortName();
}

void JournalReader::startVisiting()
{
    std::shared_ptr<Frame> frame;
    while (true)
    {
        frame = getNextFrame();
        if (frame.get() != nullptr)
        {
            string name = getFrameName();
            for (auto visitor: visitors)
                visitor->visit(name, *frame);
        }
    }
}

bool JournalReader::addVisitor(IJournalVisitor* visitor)
{
    visitors.push_back(visitor);
    return true;
}

bool JournalReader::expireJournal(size_t idx)
{
    if (idx < journals.size())
    {
        journals[idx]->expire();
        return true;
    }
    return false;
}

bool JournalReader::seekTimeJournal(size_t idx, int64_t nano)
{
    if (idx < journals.size())
    {
        journals[idx]->seekTime(nano);
        return true;
    }
    return false;
}

bool JournalReader::expireJournalByName(const string& jname)
{
    auto iter = journalMap.find(jname);
    if (iter == journalMap.end())
        return false;
    return expireJournal(iter->second);
}

bool JournalReader::seekTimeJournalByName(const string& jname, int64_t nano)
{
    auto iter = journalMap.find(jname);
    if (iter == journalMap.end())
        return false;
    return seekTimeJournal(iter->second, nano);
}
