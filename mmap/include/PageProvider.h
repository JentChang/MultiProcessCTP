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
 * Page Provider.
 * @Author cjiang (changhao.jiang@taurus.ai)
 * @since   March, 2017
 * implements IPageProvider, diverge to different usage
 */

#ifndef YIJINJING_PAGEPROVIDER_H
#define YIJINJING_PAGEPROVIDER_H

#include "IPageProvider.h"



/**
 * PageProvider,
 * abstract class with virtual interfaces,
 * utilized by JournalHandler
 */
class PageProvider: public IPageProvider
{
protected:
    /** true if provider is used by a JournalWriter */
    bool    is_writer;
    /** true if it is allowed to revise */
    bool    revise_allowed;
public:
    /** register journal when added into JournalHandler */
    virtual int  register_journal(const string& dir, const string& jname) { 
		return -1; 
	};
    /** exit client after JournalHandler is released */
    virtual void exit_client() {};
    /** override IPageProvider */
    virtual bool isWriter() const {return is_writer; };
};


/**
 * LocalPageProvider,
 * provide local page, no need to connect with service.
 */
class LocalPageProvider: public PageProvider
{
public:
    /** constructor */
    LocalPageProvider(bool isWriting, bool reviseAllowed=false);
    /** override IPageProvider */
    virtual std::shared_ptr<Page> getPage(const string &dir, const string &jname, int serviceIdx, short pageNum);
    /** override IPageProvider */
    virtual void releasePage(void* buffer, int size, int serviceIdx);
};


#endif //YIJINJING_PAGEPROVIDER_H
