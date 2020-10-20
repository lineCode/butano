/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CAMERAS_MANAGER_H
#define BTN_CAMERAS_MANAGER_H

#include "btn_fixed_fwd.h"
#include "btn_optional_fwd.h"

namespace btn
{
    class fixed_point;
}

namespace btn::cameras_manager
{
    void init();

    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();

    [[nodiscard]] int create(const fixed_point& position);

    [[nodiscard]] int create_optional(const fixed_point& position);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] const fixed_point& position(int id);

    void set_x(int id, fixed x);

    void set_y(int id, fixed y);

    void set_position(int id, const fixed_point& position);

    void update();
}

#endif
