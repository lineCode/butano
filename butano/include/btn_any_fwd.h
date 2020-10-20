/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_ANY_FWD_H
#define BTN_ANY_FWD_H

#include "btn_common.h"

namespace btn
{
    class iany;

    template<int MaxSize, int Alignment = alignof(int)>
    class any;
}

#endif
