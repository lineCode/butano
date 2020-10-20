/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_PROFILER_H
#define BTN_CONFIG_PROFILER_H

#include "btn_common.h"

#ifndef BTN_CFG_PROFILER_ENABLED
    #define BTN_CFG_PROFILER_ENABLED false
#endif

#ifndef BTN_CFG_PROFILER_LOG_ENGINE
    #define BTN_CFG_PROFILER_LOG_ENGINE false
#endif

#ifndef BTN_CFG_PROFILER_TICKS_DIVISOR
    #define BTN_CFG_PROFILER_TICKS_DIVISOR 1
#endif

#ifndef BTN_CFG_PROFILER_MAX_ENTRIES
    #define BTN_CFG_PROFILER_MAX_ENTRIES 64
#endif

#endif
