#ifndef BF_GAME_ENEMY_EVENT_H
#define BF_GAME_ENEMY_EVENT_H

#include "bf_game_enemy_data.h"

namespace bf::game
{

class enemy_event
{

public:
    const enemy_data& enemy;
    btn::fixed_point start_position;
    btn::span<const enemy_move_event> move_events;
    int wait_frames;

    constexpr enemy_event(const enemy_data& _enemy, const btn::fixed_point& _start_position,
                          const btn::span<const enemy_move_event>& _move_events, int _wait_frames) :
        enemy(_enemy),
        start_position(_start_position),
        move_events(_move_events),
        wait_frames(_wait_frames)
    {
        BTN_CONSTEXPR_ASSERT(! move_events.empty(), "Move events is empty");
        BTN_CONSTEXPR_ASSERT(_wait_frames >= 1, "Invalid wait frames");
    }
};

}

#endif