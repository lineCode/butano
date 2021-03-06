/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITES_MANAGER_ITEM_H
#define BTN_SPRITES_MANAGER_ITEM_H

#include "btn_display.h"
#include "btn_sprites.h"
#include "btn_optional.h"
#include "btn_sort_key.h"
#include "btn_camera_ptr.h"
#include "btn_fixed_point.h"
#include "btn_intrusive_list.h"
#include "btn_display_manager.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"
#include "btn_sprite_affine_mats_manager.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{
    class sprite_builder;

    using sprite_affine_mat_attach_node_type = intrusive_list_node_type;
}

namespace btn::sorted_sprites
{
    class layer;
}

namespace btn
{

class sprites_manager_item : public intrusive_list_node_type
{

public:
    sprite_affine_mat_attach_node_type affine_mat_attach_node;
    hw::sprites::handle_type handle;
    fixed_point position;
    point hw_position;
    unsigned usages = 1;
    sort_key sprite_sort_key;
    btn::sorted_sprites::layer* sort_layer_ptr;
    optional<sprite_tiles_ptr> tiles;
    optional<sprite_palette_ptr> palette;
    optional<sprite_affine_mat_ptr> affine_mat;
    optional<camera_ptr> camera;
    int8_t handles_index = -1;
    int8_t half_width;
    int8_t half_height;
    unsigned double_size_mode: 2;
    bool blending_enabled: 1;
    bool visible: 1;
    bool remove_affine_mat_when_not_needed: 1;
    bool on_screen: 1;
    bool check_on_screen: 1;

    [[nodiscard]] static sprites_manager_item& affine_mat_attach_node_item(
            sprite_affine_mat_attach_node_type& attach_node)
    {
        auto item_address = reinterpret_cast<int>(&attach_node);
        item_address -= sizeof(intrusive_list_node_type);

        auto item = reinterpret_cast<sprites_manager_item*>(item_address);
        return *item;
    }

    sprites_manager_item(const fixed_point& _position, const sprite_shape_size& shape_size,
                         sprite_tiles_ptr&& _tiles, sprite_palette_ptr&& _palette) :
        position(_position),
        sprite_sort_key(3, 0),
        tiles(move(_tiles)),
        palette(move(_palette)),
        double_size_mode(unsigned(sprite_double_size_mode::AUTO)),
        blending_enabled(false),
        remove_affine_mat_when_not_needed(true)
    {
        const sprite_palette_ptr& palette_ref = *palette;
        hw::sprites::setup_regular(shape_size, tiles->id(), palette_ref.id(), palette_ref.bpp_mode(),
                                   display_manager::blending_fade_enabled(), handle);
        update_half_dimensions();
        on_screen = false;
        visible = true;
        check_on_screen = true;
    }

    sprites_manager_item(sprite_builder&& builder, sprite_tiles_ptr&& _tiles, sprite_palette_ptr&& _palette) :
        position(builder.position()),
        sprite_sort_key(builder.bg_priority(), builder.z_order()),
        tiles(move(_tiles)),
        palette(move(_palette)),
        affine_mat(builder.release_affine_mat()),
        camera(builder.release_camera()),
        double_size_mode(unsigned(builder.double_size_mode())),
        blending_enabled(builder.blending_enabled()),
        remove_affine_mat_when_not_needed(builder.remove_affine_mat_when_not_needed())
    {
        const sprite_palette_ptr& palette_ref = *palette;

        if(affine_mat)
        {
            const sprite_affine_mat_ptr& affine_mat_ref = *affine_mat;

            if(remove_affine_mat_when_not_needed && affine_mat_ref.flipped_identity())
            {
                hw::sprites::setup_regular(builder, tiles->id(), palette_ref.id(), palette_ref.bpp_mode(),
                                           affine_mat_ref.horizontal_flip(), affine_mat_ref.vertical_flip(),
                                           display_manager::blending_fade_enabled(), handle);
                affine_mat.reset();
            }
            else
            {
                int affine_mat_id = affine_mat_ref.id();
                hw::sprites::setup_affine(builder, tiles->id(), palette_ref.id(), palette_ref.bpp_mode(),
                                          display_manager::blending_fade_enabled(), handle);
                hw::sprites::set_affine_mat(affine_mat_id, double_size(), handle);
                sprite_affine_mats_manager::attach_sprite(affine_mat_id, affine_mat_attach_node);
            }
        }
        else
        {
            hw::sprites::setup_regular(builder, tiles->id(), palette_ref.id(), palette_ref.bpp_mode(),
                                       display_manager::blending_fade_enabled(), handle);
        }

        update_half_dimensions();
        on_screen = false;

        if(builder.visible())
        {
            visible = true;
            check_on_screen = true;
        }
        else
        {
            visible = false;
            check_on_screen = false;
        }
    }

    [[nodiscard]] bool double_size() const
    {
        switch(sprite_double_size_mode(double_size_mode))
        {

        case sprite_double_size_mode::AUTO:
            return sprite_affine_mats_manager::double_size(affine_mat->id());

        case sprite_double_size_mode::ENABLED:
            return true;

        case sprite_double_size_mode::DISABLED:
            return false;

        default:
            BTN_ERROR("Invalid double size mode: ", int(double_size_mode));
            return false;
        }
    }

    [[nodiscard]] int bg_priority() const
    {
        return sprite_sort_key.priority();
    }

    void set_bg_priority(int bg_priority)
    {
        sprite_sort_key.set_priority(bg_priority);
    }

    [[nodiscard]] int z_order() const
    {
        return sprite_sort_key.z_order();
    }

    void set_z_order(int z_order)
    {
        sprite_sort_key.set_z_order(z_order);
    }

    void update_half_dimensions()
    {
        pair<int, int> dimensions = hw::sprites::dimensions(handle);
        half_width = int8_t(dimensions.first / 2);
        half_height = int8_t(dimensions.second / 2);
        update_hw_position();
    }

    void update_hw_position()
    {
        int real_x = position.x().right_shift_integer();
        int real_y = position.y().right_shift_integer();

        if(camera)
        {
            const fixed_point& camera_position = camera->position();
            real_x -= camera_position.x().right_shift_integer();
            real_y -= camera_position.y().right_shift_integer();
        }

        update_hw_x(real_x);
        update_hw_y(real_y);
    }

    void update_hw_x(int real_x)
    {
        int hw_x = real_x + (display::width() / 2) - int(half_width);
        hw_position.set_x(hw_x);
        hw::sprites::set_x(hw_x, handle);
    }

    void update_hw_y(int real_y)
    {
        int hw_y = real_y + (display::height() / 2) - int(half_height);
        hw_position.set_y(hw_y);
        hw::sprites::set_y(hw_y, handle);
    }
};

}

#endif
