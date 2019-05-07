#pragma once

#include "metro/MetroTypes.h"
#include "metro/scripts/Script.h"
#include "mymath.h"

class Config;
class MetroReflectionReader;

struct InitData {
    uint32_t   clsid;
    uint32_t   static_data_key;
    CharString att_bone_id;
    uint16_t   id;
    uint16_t   parent_id;
    pose       att_offset;
    bool       att_root;

    void Serialize(MetroReflectionReader& r);
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

    virtual void Serialize(MetroReflectionReader& r);
};

struct uobject {
    virtual ~uobject() {
    }

    virtual void Serialize(MetroReflectionReader& r);

    // it actually returns common_vss**
    virtual bool common_vss() {
        return false;
    }

    InitData   initData;
    CharString cls;
    CharString static_data;

    CharString         name;
    Flags8             oflags;
    Flags8             sflags;
    float              cull_distance;
    pose               pose;
    CharString         visual;
    uint16_t           dao_val;
    color4f            render_aux_val;
    MyArray<Script>    vss_ver_6;
    bool               vs_active;
    uint16_t           spatial_sector;
    uint8_t            qsave_chunk;
    MyArray<ScriptRef> commons_vs;
    MyArray<ScriptRef> removed_vs;

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

    void Serialize(MetroReflectionReader& r);
};

struct uobject_static : public uobject {
    DECLARE_INHERITED_CLASS(uobject_static, uobject);

    Flags8        flags;
    uint8_t       collision_group;
    interest_info interest;

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct uobject_effect : public uobject {
    DECLARE_INHERITED_CLASS(uobject_effect, uobject);

    AnimationString     startup_animation;
    CharString          bone_part;
    uint16_t            start_frame;
    float               speed;
    Flags8              startup_animation_flags;
    uint8_t             force_looped;
    CharString          sound;
    FloatQ8             sound_volume;
    uint8_t             sound_filter;
    CharString          particles;
    Flags8              particle_flags;
    interest_info       interest;
    MyArray<CharString> labels;

    virtual void Serialize(MetroReflectionReader& r) override;

    bool common_vss() override {
        return true;
    }
};

struct centity_static_params : public uobject_static_params {
    DECLARE_INHERITED_CLASS(centity_static_params, uobject_static_params);

    CharString collision_sound;
    CharString collision_track;
    uint32_t   collision_interval;
    float      collision_move;
    float      attach_threshold;
    float      attach_armor;

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct PhysicsShape {
    uint16_t bid;

    void Serialize(MetroReflectionReader& r);
};

struct PhysicsElement {
    uint16_t              root_bid;
    float                 accumulated_impulse;
    pose                  xform;
    vec3                  velocity;
    bool                  nx_awake;
    MyArray<PhysicsShape> shapes;

    void Serialize(MetroReflectionReader& r);
};

struct PhysicsShell {
    MyArray<PhysicsElement> elements;

    void Serialize(MetroReflectionReader& r);
};

struct PhysicsJointParam {
    MyArray<uint8_t> unknown;

    void Serialize(MetroReflectionReader& r);
};

struct PhysicsJoint {
    bool              enabled;
    EntityLink        entity_src;
    CharString        bone_src;
    EntityLink        entity_dst;
    CharString        bone_dst;
    vec3              pos;
    ang3              rot;
    uint16_t          joint_type;
    PhysicsJointParam params;

    void Serialize(MetroReflectionReader& r);
};

struct centity : public uobject_effect {
    DECLARE_INHERITED_CLASS(centity, uobject_effect);

    float    health;
    uint32_t dying_mask;
    Flags8   physics_flags;
    Flags8   physics_flags1;
    Flags8   physics_flags2;

    uint8_t      friend_type;
    uint8_t      reaction_type;
    CharString   fixed_bones;
    float        break_impulse_threshold;
    uint8_t      collisions_group;
    uint8_t      scene_type;
    CharString   break_particles_break;
    CharString   break_particles_death;
    CharString   break_sound_death;
    uint8_t      break_sound_death_ai_type;
    Flags64      type_mask;
    uint32_t     ph_shell_model_src;
    uint32_t     ph_shell_skltn_src;
    uint32_t     ph_shell_skltn_bcount;
    bool         ph_shell_writed;
    PhysicsShell physics_shell;
    bool         attach_with_joint;
    PhysicsJoint joint_section;
    float        footprint_size;
    float        footprint_power;

    void Serialize(MetroReflectionReader& r) override;
};

struct lamp : public centity {
    DECLARE_INHERITED_CLASS(lamp, centity);

    void Serialize(MetroReflectionReader& r) override;
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

    void Serialize(MetroReflectionReader& r);
};

struct inventory_item_object_static_params : public centity_static_params {
    DECLARE_INHERITED_CLASS(inventory_item_object_static_params, centity_static_params);

    inventory_item_static_params inventory_item;
    CharString                   hr_class;
    float                        take_impulse;
    CharString                   take_sound;
    bool                         can_be_taken_as_child;

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct inventory_item_object : public centity {
    DECLARE_INHERITED_CLASS(inventory_item_object, centity);

    // inventory_item
    Flags8   flags0;
    uint16_t trade_weight;
    uint8_t  ui_force_slot_id;
    bool     anim_simplification; // ? not sure which class should contain this

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct chud_item_container_static_params {
    void Serialize(MetroReflectionReader& r);
};

struct upgrade_item_static_params : public inventory_item_object_static_params {
    DECLARE_INHERITED_CLASS(upgrade_item_static_params, inventory_item_object_static_params);

    chud_item_container_static_params container;

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct upgrade_item : public inventory_item_object {
    DECLARE_INHERITED_CLASS(upgrade_item, inventory_item_object);

    virtual void Serialize(MetroReflectionReader& r) override;

    CharString upgrade_id;
};

struct device_upgrade_static_params : public upgrade_item_static_params {
    DECLARE_INHERITED_CLASS(device_upgrade_static_params, upgrade_item_static_params);

    uint8_t menu_event;

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct device_upgrade : public upgrade_item {
    DECLARE_INHERITED_CLASS(device_upgrade, upgrade_item);
};

struct player_timer_base : public device_upgrade {
    DECLARE_INHERITED_CLASS(player_timer_base, device_upgrade);
};

struct player_timer_static_params : public device_upgrade_static_params {
    DECLARE_INHERITED_CLASS(player_timer_static_params, device_upgrade_static_params);
};

struct player_timer : public player_timer_base {
    DECLARE_INHERITED_CLASS(player_timer, player_timer_base);
};

struct player_timer_hud_item_static_params {
    float       font_size;
    CharString  font_name;
    vec4i       color;
    vec4i       color_active;
    vec4i       color_time;
    vec4i       color_vs;
    CharString  light_bone;

    virtual void Serialize(MetroReflectionReader& r);
};

struct player_timer_hud_item_object_static_params : public player_timer_static_params {
    DECLARE_INHERITED_CLASS(player_timer_hud_item_object_static_params, player_timer_static_params);

    player_timer_hud_item_static_params hud_item;

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct player_timer_hud_item_object : public player_timer {
    DECLARE_INHERITED_CLASS(player_timer_hud_item_object, player_timer);
};

struct shooting_particles_data {
    void Serialize(MetroReflectionReader& r) {}
};

struct shooting_light_data {
    void Serialize(MetroReflectionReader& r) {}
};

struct shooting_weapon_data {
    void Serialize(MetroReflectionReader& r) {}
};

struct weapon_item_static_params : public upgrade_item_static_params {
    DECLARE_INHERITED_CLASS(weapon_item_static_params, upgrade_item_static_params);

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct chuditem_static_params {
    void Read(Config& cfg, uint16_t version);
};

struct weapon_item : public upgrade_item {
    DECLARE_INHERITED_CLASS(weapon_item, upgrade_item);

    bool vr_attach;
    bool free_on_level;

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct uobject_vs : public uobject {
    DECLARE_INHERITED_CLASS(uobject_vs, uobject);

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct unknown_static_params : public uobject_static_params {
    DECLARE_INHERITED_CLASS(unknown_static_params, uobject_static_params);

    BytesArray unknown;

    virtual void Serialize(MetroReflectionReader& r) override;
};

struct UnknownObject : public uobject {
    DECLARE_INHERITED_CLASS(UnknownObject, uobject);

    BytesArray unknown;

    virtual void Serialize(MetroReflectionReader& r) override;
};
