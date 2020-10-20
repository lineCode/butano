/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_MUSIC_ITEM_H
#define BTN_MUSIC_ITEM_H

#include "btn_fixed_fwd.h"
#include "btn_functional.h"

namespace btn
{

class music_item
{

public:
    constexpr explicit music_item(int id) :
        _id(id)
    {
    }

    [[nodiscard]] constexpr int id() const
    {
        return _id;
    }

    void play() const;

    void play(fixed volume) const;

    void play(fixed volume, bool loop) const;

    [[nodiscard]] constexpr friend bool operator==(music_item a, music_item b) = default;

private:
    int _id;
};


template<>
struct hash<music_item>
{
    [[nodiscard]] constexpr unsigned operator()(music_item value) const
    {
        return make_hash(value.id());
    }
};

}

#endif

