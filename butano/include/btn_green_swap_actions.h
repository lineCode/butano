#ifndef BTN_GREEN_SWAP_ACTIONS_H
#define BTN_GREEN_SWAP_ACTIONS_H

#include "btn_green_swap.h"
#include "btn_template_actions.h"

namespace btn
{

class green_swap_manager
{

public:
    [[nodiscard]] static bool get()
    {
        return green_swap::enabled();
    }

    static void set(bool enabled)
    {
        green_swap::set_enabled(enabled);
    }
};


class green_swap_toggle_action : public bool_toggle_template_action<green_swap_manager>
{

public:
    green_swap_toggle_action(int duration_frames) :
        bool_toggle_template_action(duration_frames)
    {
    }
};

}

#endif
