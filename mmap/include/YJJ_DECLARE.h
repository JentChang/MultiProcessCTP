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
 * Define basic macros.
 * @Author cjiang (changhao.jiang@taurus.ai)
 * @since   March, 2017
 * This file is supposed to be included by all files in this project
 */

#ifndef YIJINJING_DECLARE_H
#define YIJINJING_DECLARE_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>


using std::string; /** default using string */
using std::vector; /** default using vector */
using std::map;    /** default using map */

template<class Key, class T>
using umap = std::unordered_map<Key, T>; /** unordered_map usage, will merge more */

typedef unsigned char byte;


#define JOURNAL_PREFIX string("bt")        /**< journal file prefix */
#define JOURNAL_SUFFIX string("dat")    /**< journal file suffix */

/** fast type convert for moving address forward */
#define ADDRESS_ADD(x, delta) (void*)((uintptr_t)x + delta)

#endif //YIJINJING_DECLARE_H
