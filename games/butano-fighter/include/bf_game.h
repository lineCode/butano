/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_H
#define BF_GAME_H

#include "btn_camera_ptr.h"
#include "bf_scene.h"
#include "bf_game_background.h"
#include "bf_game_hero.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_hero_bullets.h"
#include "bf_game_intro.h"
#include "bf_game_enemies.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_objects.h"
#include "bf_game_scoreboard.h"
#include "bf_game_boss_intro.h"
#include "bf_game_pause.h"

namespace bf::game
{

class game : public scene
{

public:
    game(status& status, btn::sprite_text_generator& text_generator, butano_background& butano_background);

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    btn::camera_ptr _camera;
    background _background;
    hero _hero;
    hero_bullets _hero_bullets;
    hero_bomb _hero_bomb;
    intro _intro;
    enemies _enemies;
    enemy_bullets _enemy_bullets;
    objects _objects;
    scoreboard _scoreboard;
    boss_intro _boss_intro;
    pause _pause;
    butano_background& _butano_background;
};

}

#endif
