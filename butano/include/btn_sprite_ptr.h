/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_PTR_H
#define BTN_SPRITE_PTR_H

#include "btn_utility.h"
#include "btn_fixed_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"

namespace btn
{

class size;
class camera_ptr;
class fixed_point;
class sprite_item;
class sprite_builder;
class regular_bg_ptr;
class sprite_tiles_ptr;
class sprite_shape_size;
class sprite_tiles_item;
class sprite_palette_ptr;
class sprite_palette_item;
class sprite_affine_mat_ptr;
class sprite_first_attributes;
class sprite_third_attributes;
class sprite_affine_second_attributes;
class sprite_regular_second_attributes;
enum class sprite_double_size_mode;

class sprite_ptr
{

public:
    [[nodiscard]] static sprite_ptr create(fixed x, fixed y, const sprite_item& item);

    [[nodiscard]] static sprite_ptr create(fixed x, fixed y, const sprite_item& item, int graphics_index);

    [[nodiscard]] static sprite_ptr create(const fixed_point& position, const sprite_item& item);

    [[nodiscard]] static sprite_ptr create(const fixed_point& position, const sprite_item& item, int graphics_index);

    [[nodiscard]] static sprite_ptr create(fixed x, fixed y, const sprite_shape_size& shape_size,
                                           sprite_tiles_ptr tiles, sprite_palette_ptr palette);

    [[nodiscard]] static sprite_ptr create(const fixed_point& position, const sprite_shape_size& shape_size,
                                           sprite_tiles_ptr tiles, sprite_palette_ptr palette);

    [[nodiscard]] static sprite_ptr create(const sprite_builder& builder);

    [[nodiscard]] static sprite_ptr create(sprite_builder&& builder);

    [[nodiscard]] static optional<sprite_ptr> create_optional(fixed x, fixed y, const sprite_item& item);

    [[nodiscard]] static optional<sprite_ptr> create_optional(fixed x, fixed y, const sprite_item& item,
                                                              int graphics_index);

    [[nodiscard]] static optional<sprite_ptr> create_optional(const fixed_point& position, const sprite_item& item);

    [[nodiscard]] static optional<sprite_ptr> create_optional(const fixed_point& position, const sprite_item& item,
                                                              int graphics_index);

    [[nodiscard]] static optional<sprite_ptr> create_optional(
            fixed x, fixed y, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette);

    [[nodiscard]] static optional<sprite_ptr> create_optional(
            const fixed_point& position, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles,
            sprite_palette_ptr palette);

    [[nodiscard]] static optional<sprite_ptr> create_optional(const sprite_builder& builder);

    [[nodiscard]] static optional<sprite_ptr> create_optional(sprite_builder&& builder);

    sprite_ptr(const sprite_ptr& other);

    sprite_ptr& operator=(const sprite_ptr& other);

    sprite_ptr(sprite_ptr&& other) noexcept :
        sprite_ptr(other._handle)
    {
        other._handle = nullptr;
    }

    sprite_ptr& operator=(sprite_ptr&& other) noexcept
    {
        btn::swap(_handle, other._handle);
        return *this;
    }

    ~sprite_ptr()
    {
        if(_handle)
        {
            _destroy();
        }
    }

    [[nodiscard]] sprite_shape_size shape_size() const;

    [[nodiscard]] size dimensions() const;

    [[nodiscard]] const sprite_tiles_ptr& tiles() const;

    void set_tiles(const sprite_tiles_ptr& tiles);

    void set_tiles(sprite_tiles_ptr&& tiles);

    void set_tiles(const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles);

    void set_tiles(const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles);

    void set_tiles(const sprite_tiles_item& tiles_item);

    void set_tiles(const sprite_tiles_item& tiles_item, int graphics_index);

    void set_tiles(const sprite_tiles_item& tiles_item, const sprite_shape_size& shape_size);

    void set_tiles(const sprite_tiles_item& tiles_item, const sprite_shape_size& shape_size, int graphics_index);

    [[nodiscard]] const sprite_palette_ptr& palette() const;

    void set_palette(const sprite_palette_ptr& palette);

    void set_palette(sprite_palette_ptr&& palette);

    void set_palette(const sprite_palette_item& palette_item);

    void set_tiles_and_palette(sprite_tiles_ptr tiles, sprite_palette_ptr palette);

    void set_tiles_and_palette(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette);

    void set_item(const sprite_item& item);

    void set_item(const sprite_item& item, int graphics_index);

    [[nodiscard]] fixed x() const;

    [[nodiscard]] fixed y() const;

    [[nodiscard]] const fixed_point& position() const;

    void set_x(fixed x);

    void set_y(fixed y);

    void set_position(fixed x, fixed y);

    void set_position(const fixed_point& position);

    [[nodiscard]] fixed rotation_angle() const;

    void set_rotation_angle(fixed rotation_angle);

    [[nodiscard]] fixed horizontal_scale() const;

    void set_horizontal_scale(fixed horizontal_scale);

    [[nodiscard]] fixed vertical_scale() const;

    void set_vertical_scale(fixed vertical_scale);

    void set_scale(fixed scale);

    void set_scale(fixed horizontal_scale, fixed vertical_scale);

    [[nodiscard]] int bg_priority() const;

    void set_bg_priority(int bg_priority);

    [[nodiscard]] int z_order() const;

    void set_z_order(int z_order);

    [[nodiscard]] optional<bool> above(const sprite_ptr& other) const;

    [[nodiscard]] bool above(const regular_bg_ptr& bg_ptr) const;

    void put_above(const sprite_ptr& other);

    void put_above(const regular_bg_ptr& bg_ptr);

    [[nodiscard]] bool horizontal_flip() const;

    void set_horizontal_flip(bool horizontal_flip);

    [[nodiscard]] bool vertical_flip() const;

    void set_vertical_flip(bool vertical_flip);

    [[nodiscard]] bool mosaic_enabled() const;

    void set_mosaic_enabled(bool mosaic_enabled);

    [[nodiscard]] bool blending_enabled() const;

    void set_blending_enabled(bool blending_enabled);

    [[nodiscard]] bool window_enabled() const;

    void set_window_enabled(bool window_enabled);

    [[nodiscard]] bool double_size() const;

    [[nodiscard]] sprite_double_size_mode double_size_mode() const;

    void set_double_size_mode(sprite_double_size_mode double_size_mode);

    [[nodiscard]] bool visible() const;

    void set_visible(bool visible);

    [[nodiscard]] const optional<camera_ptr>& camera() const;

    void set_camera(optional<camera_ptr> camera);

    void remove_camera();

    [[nodiscard]] const optional<sprite_affine_mat_ptr>& affine_mat() const;

    void set_affine_mat(const sprite_affine_mat_ptr& affine_mat);

    void set_affine_mat(sprite_affine_mat_ptr&& affine_mat);

    void remove_affine_mat();

    [[nodiscard]] bool remove_affine_mat_when_not_needed() const;

    void set_remove_affine_mat_when_not_needed(bool remove_when_not_needed);

    [[nodiscard]] sprite_first_attributes first_attributes() const;

    void set_first_attributes(const sprite_first_attributes& first_attributes);

    [[nodiscard]] sprite_regular_second_attributes regular_second_attributes() const;

    void set_regular_second_attributes(const sprite_regular_second_attributes& second_attributes);

    [[nodiscard]] sprite_affine_second_attributes affine_second_attributes() const;

    void set_affine_second_attributes(const sprite_affine_second_attributes& second_attributes);

    [[nodiscard]] sprite_third_attributes third_attributes() const;

    void set_third_attributes(const sprite_third_attributes& third_attributes);

    [[nodiscard]] const void* handle() const
    {
        return _handle;
    }

    [[nodiscard]] void* handle()
    {
        return _handle;
    }

    void swap(sprite_ptr& other)
    {
        btn::swap(_handle, other._handle);
    }

    friend void swap(sprite_ptr& a, sprite_ptr& b)
    {
        btn::swap(a._handle, b._handle);
    }

    [[nodiscard]] friend bool operator==(const sprite_ptr& a, const sprite_ptr& b) = default;

private:
    using handle_type = void*;

    handle_type _handle;

    explicit sprite_ptr(handle_type handle) :
        _handle(handle)
    {
    }

    void _destroy();
};


template<>
struct hash<sprite_ptr>
{
    [[nodiscard]] unsigned operator()(const sprite_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
