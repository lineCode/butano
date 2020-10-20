/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HBLANK_EFFECTS_H
#define BTN_HBLANK_EFFECTS_H

#include "btn_common.h"

namespace btn::hblank_effects
{
    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();
}

#endif
