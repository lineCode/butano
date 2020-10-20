/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HBLANK_EFFECTS_MANAGER_H
#define BTN_HBLANK_EFFECTS_MANAGER_H

#include "btn_common.h"

namespace btn::hblank_effects_manager
{
    enum class handler_type : uint8_t
    {
        BG_PALETTE_COLOR,
        BG_PALETTES_TRANSPARENT_COLOR,
        BLENDING_FADE_ALPHA,
        BLENDING_TRANSPARENCY_ATTRIBUTES,
        GREEN_SWAP,
        MOSAIC_ATTRIBUTES,
        RECT_WINDOW_HORIZONTAL_BOUNDARIES,
        RECT_WINDOW_VERTICAL_BOUNDARIES,
        REGULAR_BG_ATTRIBUTES,
        REGULAR_BG_HORIZONTAL_POSITION,
        REGULAR_BG_VERTICAL_POSITION,
        SPRITE_AFFINE_MAT_PA_REGISTER_ATTRIBUTES,
        SPRITE_AFFINE_MAT_PA_REGISTER_VALUES,
        SPRITE_AFFINE_MAT_PB_REGISTER_ATTRIBUTES,
        SPRITE_AFFINE_MAT_PB_REGISTER_VALUES,
        SPRITE_AFFINE_MAT_PC_REGISTER_ATTRIBUTES,
        SPRITE_AFFINE_MAT_PC_REGISTER_VALUES,
        SPRITE_AFFINE_MAT_PD_REGISTER_ATTRIBUTES,
        SPRITE_AFFINE_MAT_PD_REGISTER_VALUES,
        SPRITE_AFFINE_SECOND_ATTRIBUTES,
        SPRITE_FIRST_ATTRIBUTES,
        SPRITE_HORIZONTAL_POSITION,
        SPRITE_VERTICAL_POSITION,
        SPRITE_PALETTE_COLOR,
        SPRITE_REGULAR_SECOND_ATTRIBUTES,
        SPRITE_THIRD_ATTRIBUTES,
    };

    void init();

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    void enable();

    void disable();

    [[nodiscard]] int create(const void* values_ptr, int values_count, int target_id, handler_type handler);

    [[nodiscard]] int create_optional(const void* values_ptr, int values_count, int target_id, handler_type handler);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] const void* values_ref(int id);

    void set_values_ref(int id, const void* values_ptr, int values_count);

    void reload_values_ref(int id);

    [[nodiscard]] bool visible(int id);

    void set_visible(int id, bool visible);

    void update();

    void commit();
}

#endif
