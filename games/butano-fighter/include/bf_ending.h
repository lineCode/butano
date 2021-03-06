/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_ENDING_H
#define BF_ENDING_H

#include "btn_fixed.h"
#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_regular_bg_ptr.h"
#include "btn_blending_actions.h"
#include "bf_scene.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf
{

class butano_background;

class ending : public scene
{

public:
    ending(btn::sprite_text_generator& text_generator, butano_background& butano_background);

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    btn::regular_bg_ptr _bg;
    btn::vector<btn::sprite_ptr, 32> _text_sprites;
    btn::optional<btn::blending_transparency_alpha_to_action> _blending_action;
    btn::fixed _bg_y_inc;
    bool wait_for_key = false;
};

}

#endif
