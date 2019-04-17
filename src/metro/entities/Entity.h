#pragma once

#include "../MetroTypes.h"
#include "../scripts/Script.h"
#include <array>
#include <mymath.h>
#include <variant>

class Config;

struct InitData {
    uint32_t   cls;
    uint32_t   static_data_key;
    CharString att_bone_id;
    uint16_t   id;
    uint16_t   parent_id;
    mat43T     att_offset;
    bool       att_root;

    void Read(Config& cfg);
};

struct uobject_static_params {
    uint16_t   version;
    CharString caption;
    bool       editable;
    bool       visible_for_ai;
    bool       block_ai_los;
    bool       accept_fast_explosion;
    bool       collideable;
    float      usage_distance;

    virtual void Read(Config& cfg, uint16_t version);
};

struct uobject {
    virtual ~uobject() {
    }

    virtual void Read(Config& cfg);
    // на самом деле возвращает common_vss**
    virtual bool common_vss() {
        return false;
    }

    InitData   initData;
    CharString cls;
    CharString static_data;

    CharString         name;
    uint8_t            oflags;
    uint8_t            sflags;
    float              cull_distance;
    mat43T             pose;
    CharString         visual;
    uint16_t           dao_val;
    vec4               render_aux_val;
    MyArray<Script>    vss_ver_6;
    bool               vs_active;
    uint16_t           spatial_sector;
    uint8_t            qsave_chunk;
    MyArray<ScriptRef> commons_vs;

    uobject_static_params* param;
};

struct interest_info {
    uint16_t min_importance;
    uint16_t max_importance;
    uint8_t  interest_type;
    uint16_t duration;
    float    speed;
    float    distance;
    float    max_angle_x;
    float    max_angle_y;
    float    angle_coef;

    void Read(Config& cfg);
};

struct uobject_static : public uobject {
    uint8_t       flags;
    uint8_t       collision_group;
    interest_info interest;

    void Read(Config& cfg) override;
};

struct uobject_effect : public uobject {
    void Read(Config& cfg) override;
    bool common_vss() override {
        return true;
    }

    CharString          startup_animation;
    CharString          bone_part;
    uint16_t            start_frame;
    float               speed;
    uint8_t             startup_animation_flags;
    uint8_t             force_looped;
    CharString          sound;
    float               sound_volume;
    uint8_t             sound_filter;
    CharString          particles;
    uint8_t             particle_flags;
    interest_info       interest;
    MyArray<CharString> labels;
};

struct centity_static_params : public uobject_static_params {
    CharString collision_sound;
    CharString collision_track;
    uint32_t   collision_interval;
    float      collision_move;
    float      attach_threshold;
    float      attach_armor;

    void Read(Config& cfg, uint16_t version) override;
};

struct centity : public uobject_effect {
    void Read(Config& cfg) override;

    float    health;
    uint32_t dying_mask;
    uint8_t  physics_flags;
    uint8_t  physics_flags1;
    uint8_t  physics_flags2;

    uint8_t    friend_type;
    uint8_t    reaction_type;
    CharString fixed_bones;
    float      break_impulse_threshold;
    uint8_t    collisions_group;
    uint8_t    scene_type;
    CharString break_particles_break;
    CharString break_particles_death;
    CharString break_sound_death;
    uint8_t    break_sound_death_ai_type;
    uint64_t   type_mask;
    uint32_t   ph_shell_model_src;
    uint32_t   ph_shell_skltn_src;
    uint32_t   ph_shell_skltn_bcount;
    bool       ph_shell_writed;
    bool       attach_with_joint;
    float      footprint_size;
    float      footprint_power;
};

struct lamp : public centity {
    void Read(Config& cfg) override;
};

struct inventory_item_static_params {
    uint32_t   slot;
    uint8_t    flags;
    float      control_inertion_factor;
    float      speed_coef;
    float      sens_coef;
    uint32_t   sprint2run_time;
    uint32_t   run2sprint_time;
    uint32_t   slot_max_num;
    uint32_t   keepsakes_count;
    CharString active_holder_attp;
    CharString holder_attp;
    CharString holder_attp1;
    CharString holder_attp2;
    CharString active_item_attp;
    CharString item_attp;
    CharString active_holder_attp_npc;
    CharString holder_attp_npc;
    CharString holder_attp1_npc;
    CharString holder_attp2_npc;
    CharString active_item_attp_npc;
    CharString item_attp_npc;
    uint16_t   ui_tag;

    void Read(Config& cfg, uint16_t version);
};

struct inventory_item_object_static_params : public centity_static_params {
    inventory_item_static_params inventory_item;
    CharString                   hr_class;
    float                        take_impulse;
    CharString                   take_sound;
    bool                         can_be_taken_as_child;

    void Read(Config& cfg, uint16_t version) override;
};

struct inventory_item_object : public centity {
    void Read(Config& cfg) override;

    // inventory_item
    uint8_t  flags0;
    uint16_t trade_weight;
    uint8_t  ui_force_slot_id;
    bool     anim_simplification; // ? неизвестно в каком классе должно быть
};

struct chud_item_container_static_params {
    void Read(Config& cfg, uint16_t version);
};

struct upgrade_item_static_params : public inventory_item_object_static_params {
    chud_item_container_static_params container;

    void Read(Config& cfg, uint16_t version) override;
};

struct upgrade_item : public inventory_item_object {
    void Read(Config& cfg) override;

    CharString upgrade_id;
};

struct device_upgrade_static_params : public upgrade_item_static_params {
    uint8_t menu_event;

    void Read(Config& cfg, uint16_t version) override;
};

struct device_upgrade : public upgrade_item {};

struct player_timer_base : public device_upgrade {};

struct player_timer_static_params : public device_upgrade_static_params {};

struct player_timer : public player_timer_base {};

struct player_timer_hud_item_static_params {
    float                  font_size;
    CharString             font_name;
    std::array<int32_t, 4> color;
    std::array<int32_t, 4> color_active;
    std::array<int32_t, 4> color_time;
    std::array<int32_t, 4> color_vs;
    CharString             light_bone;

    void Read(Config& cfg, uint16_t version);
};

struct player_timer_hud_item_object_static_params : public player_timer_static_params {
    player_timer_hud_item_static_params hud_item;

    void Read(Config& cfg, uint16_t version) override;
};

struct player_timer_hud_item_object : public player_timer {};

struct shooting_particles_data {
    void Read(Config& cfg, uint16_t version);
};

struct shooting_light_data {
    void Read(Config& cfg, uint16_t version);
};

struct shooting_weapon_data {
    void Read(Config& cfg, uint16_t version);
};

struct weapon_item_static_params : public upgrade_item_static_params {
    void Read(Config& cfg, uint16_t version) override;
};

struct chuditem_static_params {
    void Read(Config& cfg, uint16_t version);
};

struct weapon_item : public upgrade_item {
    void Read(Config& cfg) override;

    bool vr_attach;
    bool free_on_level;
};

struct uobject_vs : public uobject {
    void Read(Config& cfg) override;
};

struct unknown_static_params : public uobject_static_params {
    BytesArray unknown;

    void Read(Config& cfg, uint16_t version) override;
};

struct UnknownObject : public uobject {
    BytesArray unknown;

    void Read(Config& cfg) override;
};
