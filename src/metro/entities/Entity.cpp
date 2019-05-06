#include "Entity.h"
#include "../Config.h"
#include "../MetroReflection.h"
#include "../scripts/Block.h"
#include "../scripts/BlockFactory.h"
#include <array>

void InitData::Serialize(MetroReflectionReader& r) {
    r.VerifyTypeInfo("class", MetroTypeGetAlias<decltype(clsid)>());
    r >> clsid;
    METRO_READ_MEMBER(r, static_data_key);
    METRO_READ_MEMBER_CHOOSE(r, att_bone_id);
    METRO_READ_MEMBER(r, id);
    METRO_READ_MEMBER(r, parent_id);
    METRO_READ_MEMBER(r, att_offset);
    METRO_READ_MEMBER(r, att_root);
}

void uobject_static_params::Read(Config& cfg, uint16_t version) {
    this->version = version;

    cfg.ReadSubconfig("__edit", [this](Config& cfg) {
        caption = cfg.r_sz("caption");
    });
    editable = cfg.r_bool("editable");
    visible_for_ai = cfg.r_bool("visible_for_ai");
    block_ai_los = cfg.r_bool("block_ai_los");
    accept_fast_explosion = cfg.r_bool("accept_fast_explosion");
    collideable = cfg.r_bool("collideable");
    usage_distance = cfg.r_fp32("usage_distance");
}

void uobject::Read(MetroReflectionReader& r) {
    METRO_READ_MEMBER_NAME(r, name);
    METRO_READ_MEMBER(r, oflags);
    METRO_READ_MEMBER(r, sflags);
    METRO_READ_MEMBER(r, cull_distance);
    r.VerifyTypeInfo("", MetroTypeGetAlias<decltype(pose)>());
    r >> pose;
    METRO_READ_MEMBER_CHOOSE(r, visual);
    METRO_READ_MEMBER(r, dao_val);
    METRO_READ_MEMBER(r, render_aux_val);
    METRO_READ_CHILD_STRUCT_ARRAY(r, vss_ver_6);
    METRO_READ_MEMBER(r, vs_active);
    METRO_READ_MEMBER(r, spatial_sector);
    METRO_READ_MEMBER(r, qsave_chunk);
    if (common_vss()) {
        METRO_READ_CHILD_STRUCT_ARRAY(r, commons_vs);
        METRO_READ_CHILD_STRUCT_ARRAY(r, removed_vs);
    }
}

void interest_info::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, min_importance);
    METRO_READ_MEMBER(r, max_importance);
    METRO_READ_MEMBER(r, interest_type);
    METRO_READ_MEMBER(r, duration);
    METRO_READ_MEMBER(r, speed);
    METRO_READ_MEMBER(r, distance);
    METRO_READ_MEMBER(r, max_angle_x);
    METRO_READ_MEMBER(r, max_angle_y);
    METRO_READ_MEMBER(r, angle_coef);
}

void uobject_static::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    METRO_READ_MEMBER(r, flags);
    METRO_READ_MEMBER(r, collision_group);
    METRO_READ_CHILD_STRUCT(r, interest);
}

void uobject_effect::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    METRO_READ_MEMBER(r, startup_animation);
    METRO_READ_MEMBER_PART_STR(r, bone_part);
    METRO_READ_MEMBER(r, start_frame);
    METRO_READ_MEMBER(r, speed);
    METRO_READ_MEMBER(r, startup_animation_flags);
    METRO_READ_MEMBER(r, force_looped);
    METRO_READ_MEMBER_CHOOSE(r, sound);
    METRO_READ_MEMBER(r, sound_volume);
    METRO_READ_MEMBER(r, sound_filter);
    METRO_READ_MEMBER(r, particle_flags);
    METRO_READ_MEMBER_CHOOSE(r, particles);
    METRO_READ_CHILD_STRUCT(r, interest);
    METRO_READ_ARRAY32_MEMBER(r, labels);
}

void centity_static_params::Read(Config& cfg, uint16_t version) {
    uobject_static_params::Read(cfg, version);
    collision_sound = cfg.r_choose("collision_sound");
    collision_track = cfg.r_choose("collision_track");
    collision_interval = cfg.r_u32("collision_interval");
    collision_move = cfg.r_fp32("collision_move");
    attach_threshold = cfg.r_fp32("attach_threshold");
    attach_armor = cfg.r_fp32("attach_armor");
}

void PhysicsShape::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, bid);
}

void PhysicsElement::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, root_bid);
    METRO_READ_MEMBER(r, accumulated_impulse);
    METRO_READ_MEMBER(r, xform);
    METRO_READ_MEMBER(r, velocity);
    METRO_READ_MEMBER(r, nx_awake);
    METRO_READ_CHILD_STRUCT_ARRAY(r, shapes);
}

void PhysicsShell::Serialize(MetroReflectionReader& r) {
    METRO_READ_CHILD_STRUCT_ARRAY(r, elements);
}

void PhysicsJointParam::Serialize(MetroReflectionReader& r) {
    unknown.resize(r.GetStream().Remains());
    r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
}

void PhysicsJoint::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, enabled);
    METRO_READ_MEMBER(r, entity_src);
    METRO_READ_MEMBER_ATTP_SRC(r, bone_src);
    METRO_READ_MEMBER(r, entity_dst);
    METRO_READ_MEMBER_ATTP_SRC(r, bone_dst);
    METRO_READ_MEMBER(r, pos);
    METRO_READ_MEMBER(r, rot);
    // g_physics_world->vfptr->load_joint_desc
    METRO_READ_MEMBER(r, joint_type);
    METRO_READ_CHILD_STRUCT(r, params);
}

void centity::Read(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, health);
    METRO_READ_MEMBER(r, dying_mask);
    METRO_READ_MEMBER(r, physics_flags);
    METRO_READ_MEMBER(r, physics_flags1);
    METRO_READ_MEMBER(r, physics_flags2);
    uobject_effect::Read(r);
    METRO_READ_MEMBER(r, friend_type);
    METRO_READ_MEMBER(r, reaction_type);
    METRO_READ_MEMBER_CHOOSE(r, fixed_bones); // choose_array, str_shared
    METRO_READ_MEMBER(r, break_impulse_threshold);
    METRO_READ_MEMBER(r, collisions_group);
    METRO_READ_MEMBER(r, scene_type);
    METRO_READ_MEMBER_CHOOSE(r, break_particles_break);
    METRO_READ_MEMBER_CHOOSE(r, break_particles_death);
    METRO_READ_MEMBER_CHOOSE(r, break_sound_death);
    METRO_READ_MEMBER(r, break_sound_death_ai_type);
    METRO_READ_MEMBER(r, type_mask);
    METRO_READ_MEMBER(r, ph_shell_model_src);
    METRO_READ_MEMBER(r, ph_shell_skltn_src);
    METRO_READ_MEMBER(r, ph_shell_skltn_bcount);
    METRO_READ_MEMBER(r, ph_shell_writed);
    if (ph_shell_writed) {
        METRO_READ_CHILD_STRUCT(r, physics_shell);
    }
    METRO_READ_MEMBER(r, attach_with_joint);
    if (attach_with_joint) {
        METRO_READ_CHILD_STRUCT(r, joint_section);
    }
    METRO_READ_MEMBER(r, footprint_size);
    METRO_READ_MEMBER(r, footprint_power);
}

void lamp::Read(MetroReflectionReader& r) {
    centity::Read(r);
}

void inventory_item_static_params::Read(Config& cfg, uint16_t version) {
    cfg.ReadSubconfig("slot", [this](Config& cfg) {
        slot = cfg.r_u32("slot");
    });
    flags = cfg.r_bool8("flags");
    control_inertion_factor = cfg.r_fp32("control_inertion_factor");
    speed_coef = cfg.r_fp32("speed_coef");
    sens_coef = cfg.r_fp32("sens_coef");
    sprint2run_time = cfg.r_u32("sprint2run_time");
    run2sprint_time = cfg.r_u32("run2sprint_time");
    slot_max_num = cfg.r_u32("slot_max_num");
    keepsakes_count = cfg.r_u32("keepsakes_count");
    active_holder_attp = cfg.r_choose("active_holder_attp");
    holder_attp = cfg.r_choose("holder_attp");
    holder_attp1 = cfg.r_choose("holder_attp1");
    holder_attp2 = cfg.r_choose("holder_attp2");
    active_item_attp = cfg.r_choose("active_item_attp");
    item_attp = cfg.r_choose("item_attp");
    active_holder_attp_npc = cfg.r_choose("active_holder_attp_npc");
    holder_attp_npc = cfg.r_choose("holder_attp_npc");
    holder_attp1_npc = cfg.r_choose("holder_attp1_npc");
    holder_attp2_npc = cfg.r_choose("holder_attp2_npc");
    active_item_attp_npc = cfg.r_choose("active_item_attp_npc");
    item_attp_npc = cfg.r_choose("item_attp_npc");
    ui_tag = cfg.r_u16("ui_tag");
}

void inventory_item_object_static_params::Read(Config& cfg, uint16_t version) {
    inventory_item.Read(cfg, version);
    centity_static_params::Read(cfg, version);
    hr_class = cfg.r_choose("hr_class");
    take_impulse = cfg.r_fp32("take_impulse");
    take_sound = cfg.r_choose("take_sound");
    can_be_taken_as_child = cfg.r_bool("can_be_taken_as_child");
}

void inventory_item_object::Read(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, flags0);
    METRO_READ_MEMBER(r, trade_weight);
    METRO_READ_MEMBER(r, ui_force_slot_id);
    centity::Read(r);
    METRO_READ_MEMBER(r, anim_simplification);
}

void chud_item_container_static_params::Read(Config& cfg, uint16_t version) {
}

void upgrade_item_static_params::Read(Config& cfg, uint16_t version) {
    inventory_item_object_static_params::Read(cfg, version);
    container.Read(cfg, version);
}

void upgrade_item::Read(MetroReflectionReader& r) {
    inventory_item_object::Read(r);
    METRO_READ_MEMBER(r, upgrade_id);
}

void device_upgrade_static_params::Read(Config& cfg, uint16_t version) {
    upgrade_item_static_params::Read(cfg, version);
    menu_event = cfg.r_u8("menu_event");
}

void player_timer_hud_item_static_params::Read(Config& cfg, uint16_t version) {
    // chud_item::static_params::load
    // player_timer_digital::static_params::load
    font_size = cfg.r_fp32("font_size");
    font_name = cfg.r_choose("font_name");
    color = cfg.r_vec4i("color");
    color_active = cfg.r_vec4i("color_active");
    color_time = cfg.r_vec4i("color_time");
    color_vs = cfg.r_vec4i("color_vs");
    light_bone = cfg.r_choose("light_bone");
}

void player_timer_hud_item_object_static_params::Read(Config& cfg, uint16_t version) {
    player_timer_static_params::Read(cfg, version);
    hud_item.Read(cfg, version);
}

void weapon_item::Read(MetroReflectionReader& r) {
    upgrade_item::Read(r);
    METRO_READ_MEMBER(r, vr_attach);
    METRO_READ_MEMBER(r, free_on_level);
}

void uobject_vs::Read(MetroReflectionReader& r) {
    uobject::Read(r);
}

void unknown_static_params::Read(Config& cfg, uint16_t version) {
    this->version = version;

    unknown = cfg.r_bytes(cfg.Remains());
}

void UnknownObject::Read(MetroReflectionReader& r) {
    unknown.resize(r.GetStream().Remains());
    r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
}
