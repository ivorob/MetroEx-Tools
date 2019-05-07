#include "Entity.h"
#include "metro/Config.h"
#include "metro/MetroReflection.h"
#include "metro/scripts/Block.h"
#include "metro/scripts/BlockFactory.h"
#include <array>

void InitData::Serialize(MetroReflectionReader& r) {
    METRO_READ_CLASS_MEMBER(r, clsid)
    METRO_READ_MEMBER(r, static_data_key);
    METRO_READ_MEMBER_CHOOSE(r, att_bone_id);
    METRO_READ_MEMBER(r, id);
    METRO_READ_MEMBER(r, parent_id);
    METRO_READ_MEMBER(r, att_offset);
    METRO_READ_MEMBER(r, att_root);
}

void uobject_static_params::Serialize(MetroReflectionReader& r) {
    this->version = scast<uint16_t>(r.GetUserData());

    MetroReflectionReader editSection = r.OpenSection("__edit");
    if (editSection.Good()) {
        METRO_READ_MEMBER(editSection, caption);
    }
    r.CloseSection(editSection);

    METRO_READ_MEMBER(r, editable);
    METRO_READ_MEMBER(r, visible_for_ai);
    METRO_READ_MEMBER(r, block_ai_los);
    METRO_READ_MEMBER(r, accept_fast_explosion);
    METRO_READ_MEMBER(r, collideable);
    METRO_READ_MEMBER(r, usage_distance);
}

void uobject::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER_CHOOSE(r, name);
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

void uobject_static::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);

    METRO_READ_MEMBER(r, flags);
    METRO_READ_MEMBER(r, collision_group);
    METRO_READ_CHILD_STRUCT(r, interest);
}

void uobject_effect::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);

    METRO_READ_MEMBER(r, startup_animation);
    METRO_READ_MEMBER_CHOOSE(r, bone_part);
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

void centity_static_params::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);

    METRO_READ_MEMBER_CHOOSE(r, collision_sound);
    METRO_READ_MEMBER_CHOOSE(r, collision_track);
    METRO_READ_MEMBER(r, collision_interval);
    METRO_READ_MEMBER(r, collision_move);
    METRO_READ_MEMBER(r, attach_threshold);
    METRO_READ_MEMBER(r, attach_armor);
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
    METRO_READ_MEMBER_CHOOSE(r, bone_src);
    METRO_READ_MEMBER(r, entity_dst);
    METRO_READ_MEMBER_CHOOSE(r, bone_dst);
    METRO_READ_MEMBER(r, pos);
    METRO_READ_MEMBER(r, rot);
    // g_physics_world->vfptr->load_joint_desc
    METRO_READ_MEMBER(r, joint_type);
    METRO_READ_CHILD_STRUCT(r, params);
}

void centity::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, health);
    METRO_READ_MEMBER(r, dying_mask);
    METRO_READ_MEMBER(r, physics_flags);
    METRO_READ_MEMBER(r, physics_flags1);
    METRO_READ_MEMBER(r, physics_flags2);

    METRO_SERIALIZE_PARENT(r);

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

void lamp::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);
}

void inventory_item_static_params::Serialize(MetroReflectionReader& r) {
    MetroReflectionReader slotSection = r.OpenSection("slot");
    if (slotSection.Good()) {
        METRO_READ_MEMBER(slotSection, slot);
    }
    r.CloseSection(slotSection);

    METRO_READ_MEMBER(r, flags);
    METRO_READ_MEMBER(r, control_inertion_factor);
    METRO_READ_MEMBER(r, speed_coef);
    METRO_READ_MEMBER(r, sens_coef);
    METRO_READ_MEMBER(r, sprint2run_time);
    METRO_READ_MEMBER(r, run2sprint_time);
    METRO_READ_MEMBER(r, slot_max_num);
    METRO_READ_MEMBER(r, keepsakes_count);
    METRO_READ_MEMBER_CHOOSE(r, active_holder_attp);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp1);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp2);
    METRO_READ_MEMBER_CHOOSE(r, active_item_attp);
    METRO_READ_MEMBER_CHOOSE(r, item_attp);
    METRO_READ_MEMBER_CHOOSE(r, active_holder_attp_npc);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp_npc);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp1_npc);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp2_npc);
    METRO_READ_MEMBER_CHOOSE(r, active_item_attp_npc);
    METRO_READ_MEMBER_CHOOSE(r, item_attp_npc);
    METRO_READ_MEMBER(r, ui_tag);
}

void inventory_item_object_static_params::Serialize(MetroReflectionReader& r) {
    r >> inventory_item;

    METRO_SERIALIZE_PARENT(r);

    METRO_READ_MEMBER_CHOOSE(r, hr_class);
    METRO_READ_MEMBER(r, take_impulse);
    METRO_READ_MEMBER_CHOOSE(r, take_sound);
    METRO_READ_MEMBER(r, can_be_taken_as_child);
}

void inventory_item_object::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, flags0);
    METRO_READ_MEMBER(r, trade_weight);
    METRO_READ_MEMBER(r, ui_force_slot_id);

    METRO_SERIALIZE_PARENT(r);

    METRO_READ_MEMBER(r, anim_simplification);
}

void chud_item_container_static_params::Serialize(MetroReflectionReader& r) {
}

void upgrade_item_static_params::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);

    r >> container;
}

void upgrade_item::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);

    METRO_READ_MEMBER(r, upgrade_id);
}

void device_upgrade_static_params::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);

    METRO_READ_MEMBER(r, menu_event);
}

void player_timer_hud_item_static_params::Serialize(MetroReflectionReader& r) {
    // chud_item::static_params::load
    // player_timer_digital::static_params::load
    METRO_READ_MEMBER(r, font_size);
    METRO_READ_MEMBER_CHOOSE(r, font_name);
    METRO_READ_MEMBER(r, color);
    METRO_READ_MEMBER(r, color_active);
    METRO_READ_MEMBER(r, color_time);
    METRO_READ_MEMBER(r, color_vs);
    METRO_READ_MEMBER_CHOOSE(r, light_bone);
}

void player_timer_hud_item_object_static_params::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);

    r >> hud_item;
}

void weapon_item::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);

    METRO_READ_MEMBER(r, vr_attach);
    METRO_READ_MEMBER(r, free_on_level);
}

void uobject_vs::Serialize(MetroReflectionReader& r) {
    METRO_SERIALIZE_PARENT(r);
}

void unknown_static_params::Serialize(MetroReflectionReader& r) {
    this->version = scast<uint16_t>(r.GetUserData());

    //unknown = cfg.r_bytes(cfg.Remains());
}

void UnknownObject::Serialize(MetroReflectionReader& r) {
    //unknown.resize(r.GetStream().Remains());
    //r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
}
