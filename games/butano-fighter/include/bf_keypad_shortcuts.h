/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_KEYPAD_SHORTCUTS_H
#define BF_KEYPAD_SHORTCUTS_H

#include "btn_sprite_ptr.h"

namespace bf
{

class keypad_shortcuts
{

public:
    keypad_shortcuts();

    void update();

private:
    btn::sprite_ptr _brightness_sprite;
    int _brightness_counter;
    bool _sleep_ready = false;
    bool _reset_ready = false;
};

}

#endif
