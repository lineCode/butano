/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_bg_tiles.h"

#include "btn_bg_blocks_manager.h"

namespace btn::bg_tiles
{

int used_tiles_count()
{
    return bg_blocks_manager::used_tiles_count();
}

int available_tiles_count()
{
    return bg_blocks_manager::available_tiles_count();
}

int used_blocks_count()
{
    return bg_blocks_manager::used_tile_blocks_count();
}

int available_blocks_count()
{
    return bg_blocks_manager::available_tile_blocks_count();
}

}
