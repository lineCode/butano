#include "bf_how_to_play.h"

#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_music_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_items_gem.h"
#include "btn_sprite_items_brightness.h"
#include "btn_sprite_items_hero_death.h"
#include "btn_sprite_items_hero_shield.h"
#include "btn_sprite_items_hero_weapons.h"
#include "btn_sprite_items_hero_bullets.h"
#include "btn_sprite_items_flash_palette.h"
#include "btn_sprite_items_hero_bomb_icon.h"
#include "btn_sprite_items_hero_bomb_mini.h"
#include "btn_sprite_items_experience_bar.h"
#include "btn_sprite_items_hero_body_walking.h"
#include "btn_sprite_items_hero_weapon_big_2.h"
#include "btn_sprite_items_experience_frame_back.h"
#include "btn_sprite_items_experience_frame_front.h"
#include "bf_status.h"
#include "bf_constants.h"
#include "bf_scene_type.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    constexpr const btn::fixed left_text_sprites_x = -70;
    constexpr const btn::fixed right_text_sprites_x = 60;
    constexpr const int blending_frames = 30;
    constexpr const int left_move_x = blending_frames;
    constexpr const int right_move_x = -blending_frames;
    constexpr const int pages = 5;

    void _enable_blending(btn::ivector<btn::sprite_ptr>& sprites)
    {
        for(btn::sprite_ptr& sprite : sprites)
        {
            sprite.set_blending_enabled(true);
        }
    }

    void _set_position(const btn::fixed_point& new_position, btn::ivector<btn::sprite_ptr>& sprites)
    {
        for(btn::sprite_ptr& sprite : sprites)
        {
            sprite.set_position(sprite.position() + new_position);
        }
    }
}

how_to_play::how_to_play(scene_type next_scene, status& status, btn::sprite_text_generator& text_generator,
                         butano_background& butano_background) :
    _next_scene(next_scene),
    _status(status),
    _text_generator(text_generator)
{
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);
    text_generator.generate(0, 12 - (btn::display::height() / 2), "HOW TO PLAY", _title_sprites);
    _enable_blending(_title_sprites);
    _entries.push_back(_create_hero_entry());
    _entries.push_back(_create_move_entry());

    btn::blending::set_transparency_alpha(0);
    _blending_action.emplace(blending_frames, 1);
    butano_background.put_under_all();

    if(! btn::music::playing())
    {
        btn::music_items::battle_clean.play(0.6);
    }
}

btn::optional<scene_type> how_to_play::update()
{
    btn::optional<scene_type> result;

    for(entry_type& entry : _entries)
    {
        entry.animate();
    }

    if(_music_volume_action)
    {
        _music_volume_action->update();

        if(_music_volume_action->done())
        {
            _music_volume_action.reset();
        }
    }

    if(_blending_action)
    {
        _blending_action->update();

        if(_blending_action->done())
        {
            _blending_action.reset();

            if(_page_index == 0)
            {
                for(btn::sprite_ptr& sprite : _title_sprites)
                {
                    sprite.set_blending_enabled(false);
                }
            }
        }
    }
    else
    {
        if(btn::blending::transparency_alpha() == 1)
        {
            if(btn::keypad::a_pressed())
            {
                _blending_action.emplace(blending_frames, 0);

                for(entry_type& entry : _entries)
                {
                    entry.move_counter = blending_frames;
                }

                if(_page_index == pages - 1)
                {
                    _enable_blending(_title_sprites);

                    if(_next_scene == scene_type::GAME)
                    {
                        _music_volume_action.emplace(blending_frames, 0);
                    }
                }
            }
        }
        else if(! _entries.empty())
        {
            _entries.clear();
        }
        else
        {
            ++_page_index;
            _blending_action.emplace(blending_frames, 1);

            switch(_page_index)
            {

            case 1:
                _entries.push_back(_create_shoot_entry());
                _entries.push_back(_create_bomb_entry());
                break;

            case 2:
                _entries.push_back(_create_shield_entry());
                _entries.push_back(_create_brightness_entry());
                break;

            case 3:
                _entries.push_back(_create_experience_entry());
                _entries.push_back(_create_weapon_entry());
                break;

            case 4:
                _entries.push_back(_create_gem_entry());
                _entries.push_back(_create_bomb_item_entry());
                break;

            default:
                _title_sprites.clear();
                btn::blending::set_transparency_alpha(1);
                _status.mark_how_to_play_as_viewed();
                result = _next_scene;

                if(_next_scene == scene_type::GAME && btn::music::playing())
                {
                    btn::music::stop();
                }
                break;
            }
        }
    }

    return result;
}

void how_to_play::entry_type::setup(int x, int y)
{
    _enable_blending(sprites);
    set_position(x, y);
    move_x = x > 0 ? -1 : 1;
    move_counter = blending_frames;
}

void how_to_play::entry_type::set_position(int x, int y)
{
    btn::fixed_point new_position(x, y);
    _set_position(new_position, sprites);
}

void how_to_play::entry_type::animate()
{
    if(move_counter)
    {
        --move_counter;
        set_position(move_x, 0);
    }

    if(toggle_action)
    {
        toggle_action->update();
    }

    if(rotate_action)
    {
        rotate_action->update();
    }

    if(palette_fade_action)
    {
        palette_fade_action->update();
    }

    if(palette_rotate_action)
    {
        palette_rotate_action->update();
    }

    for(auto& animate_action : animate_actions)
    {
        animate_action.update();
    }

    if(flash_palette)
    {
        --flash_palette_counter;

        if(! flash_palette_counter)
        {
            btn::sprite_ptr& last_sprite = sprites.back();
            btn::sprite_palette_ptr last_sprite_palette = last_sprite.palette();
            last_sprite.set_palette(*flash_palette);
            flash_palette = btn::move(last_sprite_palette);
            flash_palette_counter = constants::object_flash_frames / 2;
        }
    }
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_hero_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::LEFT);
    _text_generator.generate(left_text_sprites_x, 0,  "This is Butano Fighter!", result.sprites);
    _text_generator.generate(left_text_sprites_x, 12, "Your goal is to stay alive", result.sprites);
    _text_generator.generate(left_text_sprites_x, 24, "while you shoot to", result.sprites);
    _text_generator.generate(left_text_sprites_x, 36, "everything that moves!", result.sprites);

    btn::fixed_point body_position(left_text_sprites_x - 24, 18);
    btn::sprite_ptr body_sprite = btn::sprite_items::hero_body_walking.create_sprite(body_position);
    body_sprite.set_horizontal_flip(true);
    body_sprite.set_vertical_flip(true);
    result.sprites.push_back(body_sprite);
    result.animate_actions.push_back(btn::create_sprite_cached_animate_action_forever(
                btn::move(body_sprite), 16, btn::sprite_items::hero_body_walking.tiles_item(), 0, 1, 0, 1));

    btn::fixed_point weapon_position = body_position + btn::fixed_point(-2, 13);
    btn::sprite_ptr weapon_sprite = btn::sprite_items::hero_weapons.create_sprite(weapon_position, 1);
    weapon_sprite.set_horizontal_flip(true);
    weapon_sprite.set_vertical_flip(true);
    result.sprites.push_back(btn::move(weapon_sprite));

    result.setup(-left_move_x, -40);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_move_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::RIGHT);
    _text_generator.generate(right_text_sprites_x, 0,  "Move the hero with the", result.sprites);
    _text_generator.generate(right_text_sprites_x, 12, "control pad. If you don't", result.sprites);
    _text_generator.generate(right_text_sprites_x, 24, "shoot, you'll move faster!", result.sprites);

    btn::fixed_point body_position(right_text_sprites_x + 24, 12);
    btn::fixed_point shadow_position = body_position + btn::fixed_point(-12, -4);
    btn::sprite_ptr shadow_sprite = btn::sprite_items::hero_body_walking.create_sprite(shadow_position, 6);
    shadow_sprite.set_horizontal_flip(true);
    shadow_sprite.set_vertical_flip(true);
    result.sprites.push_back(shadow_sprite);
    result.animate_actions.push_back(btn::create_sprite_cached_animate_action_forever(
                btn::move(shadow_sprite), 16, btn::sprite_items::hero_body_walking.tiles_item(), 6, 7, 6, 7));

    shadow_position += btn::fixed_point(4, 1);
    shadow_sprite = btn::sprite_items::hero_body_walking.create_sprite(shadow_position, 4);
    shadow_sprite.set_horizontal_flip(true);
    shadow_sprite.set_vertical_flip(true);
    result.sprites.push_back(shadow_sprite);
    result.animate_actions.push_back(btn::create_sprite_cached_animate_action_forever(
                btn::move(shadow_sprite), 16, btn::sprite_items::hero_body_walking.tiles_item(), 4, 5, 4, 5));

    shadow_position += btn::fixed_point(4, 1);
    shadow_sprite = btn::sprite_items::hero_body_walking.create_sprite(shadow_position, 2);
    shadow_sprite.set_horizontal_flip(true);
    shadow_sprite.set_vertical_flip(true);
    result.sprites.push_back(shadow_sprite);
    result.animate_actions.push_back(btn::create_sprite_cached_animate_action_forever(
                btn::move(shadow_sprite), 16, btn::sprite_items::hero_body_walking.tiles_item(), 2, 3, 2, 3));

    btn::sprite_ptr body_sprite = btn::sprite_items::hero_body_walking.create_sprite(body_position);
    body_sprite.set_horizontal_flip(true);
    body_sprite.set_vertical_flip(true);
    result.sprites.push_back(body_sprite);
    result.animate_actions.push_back(btn::create_sprite_cached_animate_action_forever(
                btn::move(body_sprite), 16, btn::sprite_items::hero_body_walking.tiles_item(), 0, 1, 0, 1));

    btn::fixed_point weapon_position = body_position + btn::fixed_point(-2, 13);
    btn::sprite_ptr weapon_sprite = btn::sprite_items::hero_weapons.create_sprite(weapon_position, 1);
    weapon_sprite.set_horizontal_flip(true);
    weapon_sprite.set_vertical_flip(true);
    result.sprites.push_back(btn::move(weapon_sprite));

    result.setup(-right_move_x, 24);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_shoot_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::LEFT);
    _text_generator.generate(left_text_sprites_x, 0,  "Shoot with the B button!", result.sprites);
    _text_generator.generate(left_text_sprites_x, 12, "You can leave the button", result.sprites);
    _text_generator.generate(left_text_sprites_x, 24, "pressed, there's no need", result.sprites);
    _text_generator.generate(left_text_sprites_x, 36, "to mash it.", result.sprites);

    btn::fixed_point weapon_position(left_text_sprites_x - 24, 28);
    result.sprites.push_back(btn::sprite_items::hero_weapons.create_sprite(weapon_position, 1));

    btn::fixed_point bullet_position = weapon_position + btn::fixed_point(0, -18);
    result.sprites.push_back(btn::sprite_items::hero_bullets.create_sprite(bullet_position, 1));

    result.setup(-left_move_x, -40);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_bomb_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::RIGHT);
    _text_generator.generate(right_text_sprites_x, 0,  "Throw a bomb with the A", result.sprites);
    _text_generator.generate(right_text_sprites_x, 12, "button! Use it just before", result.sprites);
    _text_generator.generate(right_text_sprites_x, 24, "being hit to avoid all", result.sprites);
    _text_generator.generate(right_text_sprites_x, 36, "baddies and their bullets.", result.sprites);

    btn::fixed_point bomb_position(right_text_sprites_x + 24, 18);
    btn::sprite_ptr bomb_halo_sprite = btn::sprite_items::hero_bomb_mini.create_sprite(bomb_position);
    btn::sprite_palette_ptr bomb_halo_palette = bomb_halo_sprite.palette();
    bomb_halo_palette.set_fade_color(btn::color(31, 31, 31));
    result.sprites.push_back(btn::move(bomb_halo_sprite));
    result.sprites.push_back(btn::sprite_items::hero_bomb_icon.create_sprite(bomb_position));
    result.palette_fade_action.emplace(btn::move(bomb_halo_palette), 15, 0.125);

    result.setup(-right_move_x, 24);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_shield_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::LEFT);
    _text_generator.generate(left_text_sprites_x, 0,  "If you have two or more", result.sprites);
    _text_generator.generate(left_text_sprites_x, 12, "bombs when you are hit,", result.sprites);
    _text_generator.generate(left_text_sprites_x, 24, "a shield will be activated,", result.sprites);
    _text_generator.generate(left_text_sprites_x, 36, "but you'll lose all bombs!", result.sprites);

    btn::fixed_point body_position(left_text_sprites_x - 24, 18);
    btn::sprite_ptr body_sprite = btn::sprite_items::hero_body_walking.create_sprite(body_position);
    body_sprite.set_horizontal_flip(true);
    body_sprite.set_vertical_flip(true);
    result.sprites.push_back(body_sprite);
    result.animate_actions.push_back(btn::create_sprite_cached_animate_action_forever(
                btn::move(body_sprite), 16, btn::sprite_items::hero_body_walking.tiles_item(), 0, 1, 0, 1));

    btn::fixed_point weapon_position = body_position + btn::fixed_point(-2, 13);
    btn::sprite_ptr weapon_sprite = btn::sprite_items::hero_weapons.create_sprite(weapon_position, 1);
    weapon_sprite.set_horizontal_flip(true);
    weapon_sprite.set_vertical_flip(true);
    result.sprites.push_back(btn::move(weapon_sprite));

    btn::sprite_ptr shield_sprite = btn::sprite_items::hero_shield.create_sprite(body_position);
    shield_sprite.set_scale(0.65);
    result.sprites.push_back(shield_sprite);
    result.toggle_action.emplace(shield_sprite, 1);
    result.rotate_action.emplace(btn::move(shield_sprite), 5);

    result.setup(-left_move_x, -40);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_brightness_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::RIGHT);
    _text_generator.generate(right_text_sprites_x, 0,  "Change screen's", result.sprites);
    _text_generator.generate(right_text_sprites_x, 12, "brightness anytime with", result.sprites);
    _text_generator.generate(right_text_sprites_x, 24, "L and R buttons.", result.sprites);

    result.sprites.push_back(btn::sprite_items::brightness.create_sprite(right_text_sprites_x + 24, 12, 4));

    result.setup(-right_move_x, 24);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_experience_entry()
{
    entry_type result;
    btn::fixed x = left_text_sprites_x + 16;
    _text_generator.set_alignment(btn::horizontal_alignment_type::LEFT);
    _text_generator.generate(x, 0,  "Shoot a lot of baddies ", result.sprites);
    _text_generator.generate(x, 12, "to earn experience and ", result.sprites);
    _text_generator.generate(x, 24, "level up.", result.sprites);

    btn::fixed y = 12;
    x -= 24;
    result.sprites.push_back(btn::sprite_items::experience_frame_back.create_sprite(x - 8, y));

    {
        btn::sprite_builder builder(btn::sprite_items::experience_bar);
        builder.set_position(result.sprites.back().position());
        builder.set_scale_x(0.5);
        result.sprites.push_back(builder.release_build());
        result.palette_rotate_action.emplace(result.sprites.back().palette(), 2, 1);
    }

    {
        btn::sprite_builder builder(btn::sprite_items::experience_frame_front);
        builder.set_position(btn::fixed_point(x - 2, y));
        builder.set_horizontal_flip(true);
        result.sprites.push_back(builder.build());

        builder.set_position(btn::fixed_point(x - 13, y));
        builder.set_horizontal_flip(false);
        result.sprites.push_back(builder.release_build());
    }

    result.setup(-left_move_x, -40);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_weapon_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::RIGHT);
    _text_generator.generate(right_text_sprites_x, 0,  "If you get enough", result.sprites);
    _text_generator.generate(right_text_sprites_x, 12, "experience, a new weapon", result.sprites);
    _text_generator.generate(right_text_sprites_x, 24, "will appear. Get it!", result.sprites);

    result.sprites.push_back(btn::sprite_items::hero_weapon_big_2.create_sprite(right_text_sprites_x + 24, 12));
    result.flash_palette = btn::sprite_items::flash_palette.palette_item().create_palette();

    result.setup(-right_move_x, 24);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_gem_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::LEFT);
    _text_generator.generate(left_text_sprites_x, 0,  "You can get more experience", result.sprites);
    _text_generator.generate(left_text_sprites_x, 12, "with gems too. The quicker", result.sprites);
    _text_generator.generate(left_text_sprites_x, 24, "you pick up the item,", result.sprites);
    _text_generator.generate(left_text_sprites_x, 36, "the higher the experience!", result.sprites);

    btn::sprite_ptr gem_sprite = btn::sprite_items::gem.create_sprite(left_text_sprites_x - 24, 18);
    result.sprites.push_back(gem_sprite);
    result.animate_actions.push_back(btn::create_sprite_cached_animate_action_forever(
                btn::move(gem_sprite), 4, btn::sprite_items::gem.tiles_item(), 0, 1, 2, 3));

    result.setup(-left_move_x, -40);
    return result;
}

[[nodiscard]] how_to_play::entry_type how_to_play::_create_bomb_item_entry()
{
    entry_type result;
    _text_generator.set_alignment(btn::horizontal_alignment_type::RIGHT);
    _text_generator.generate(right_text_sprites_x, 0,  "At last, you can get", result.sprites);
    _text_generator.generate(right_text_sprites_x, 12, "more bombs too.", result.sprites);
    _text_generator.generate(right_text_sprites_x, 24, "Don't waste them!", result.sprites);

    result.sprites.push_back(btn::sprite_items::hero_bomb_icon.create_sprite(right_text_sprites_x + 24, 12));
    result.flash_palette = btn::sprite_items::flash_palette.palette_item().create_palette();

    result.setup(-right_move_x, 24);
    return result;
}

}
