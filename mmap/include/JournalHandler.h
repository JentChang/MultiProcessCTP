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

#ifndef YIJINJING_JOURNALHANDLER_H
#define YIJINJING_JOURNALHANDLER_H

/*                  |- JournalReader (read)
 * JournalHandler  -|
 *       |          |- JournalWriter (write)
 *       |
 *       |- add journal
 *       |- connect to PageEngine-service via page_provider
 */

//#include "YJJ_DECLARE.h"
#include "PageProvider.h"
#include "Journal.h"


/*
 * JournalHandler
 */
class JournalHandler
{
protected:
    /** all page-engine-interact stuff is handled by pageProvider */
    std::shared_ptr<PageProvider> page_provider;
    /** journals */
    vector<std::shared_ptr<Journal>> journals;
    /** current journal */
    std::shared_ptr<Journal>  curJournal;
public:
    /** default constructor with outside page provider */
    JournalHandler(std::shared_ptr<PageProvider> ptr): page_provider(ptr) {};
    /** default destructor */
    virtual ~JournalHandler();
    /** return the journal's index in the vector */
    virtual size_t addJournal(const string& dir, const string& jname);
    /** default name */
    static string getDefaultName(const string& prefix);
};


#endif //YIJINJING_JOURNALHANDLER_H
