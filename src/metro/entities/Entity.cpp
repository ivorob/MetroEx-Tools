#include "Entity.h"
#include "../Config.h"
#include "../scripts/Block.h"
#include "../scripts/BlockFactory.h"
#include <array>

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

void uobject::Read(Config& cfg) {
    name = cfg.r_name("name");
    oflags = cfg.r_bool8("oflags");
    sflags = cfg.r_bool8("sflags");
    cull_distance = cfg.r_fp32("cull_distance");
    pose = cfg.r_pose("");
    visual = cfg.r_choose("visual");
    dao_val = cfg.r_u16("dao_val");
    render_aux_val = cfg.r_color("render_aux_val");
    cfg.ReadArray("vss_ver_6", [](Config& cfg, uint32_t idx) {
        cfg.ReadArray("groups", [](Config& cfg, uint32_t idx) {
            assert(false);
        });
        cfg.ReadSection("blocks", [](Config& cfg) {
            uint16_t version = cfg.r_u16("version");
            uint32_t blockCount = cfg.r_u32("block_count");
            cfg.ReadArray(nullptr, blockCount, [](Config& cfg, uint32_t idx) {
                uint32_t clsid = cfg.r_u32("clsid");
                Block*   block = BlockFactory::Create(clsid);
                block->Read(cfg);
            });
        });
        uint32_t linkCount = cfg.r_u32("link_count");
        for (uint32_t j = 0; j != linkCount; j++) {
            char                    buf[10];
            std::array<uint16_t, 4> link = cfg.r_vec4s16(itoa(j, buf, 10));
        }
    });
    vs_active = cfg.r_bool("vs_active");
    spatial_sector = cfg.r_u16("spatial_sector");
    qsave_chunk = cfg.r_u8("qsave_chunk");
    if (common_vss()) {
        cfg.ReadArray("commons_vs", [this](Config& cfg, uint32_t idx) {
            CharString vs_name = cfg.r_sz("vs_name");
            bool       vs_debug = cfg.r_bool("vs_debug");
            bool       vs_active = cfg.r_bool("vs_active");
            bool       disable_qsave = cfg.r_bool("disable_qsave");
            bool       save_on_nextlevel = cfg.r_bool("save_on_nextlevel");
            CharString vs_ref = cfg.r_sz("vs_ref");
            bool       dyn_state_exists = cfg.r_bool("vs_ref_dyn_state_exist");
            assert(dyn_state_exists == false);
            if (!vs_ref.empty()) {
                cfg.ReadArray("exposed_blocks", [this](Config& cfg, uint32_t idx) {
                    uint16_t blkid = cfg.r_u16("blkid");
                    uint32_t clsid = cfg.r_u32("clsid");
                    Block*   block = BlockFactory::Create(clsid);
                    block->Read(cfg);
                });
            }
        });
        cfg.ReadArray("removed_vs", [](Config& cfg, uint32_t idx) {
            assert(false);
        });
    }
}

void interest_info::Read(Config& cfg) {
    cfg.ReadSection("interest", [this](Config& cfg) {
        min_importance = cfg.r_u16("min_importance");
        max_importance = cfg.r_u16("max_importance");
        interest_type = cfg.r_u8("interest_type");
        duration = cfg.r_u16("duration");
        speed = cfg.r_fp32("speed");
        distance = cfg.r_fp32("distance");
        max_angle_x = cfg.r_angle("max_angle_x");
        max_angle_y = cfg.r_angle("max_angle_y");
        angle_coef = cfg.r_fp32("angle_coef");
    });
}

void uobject_static::Read(Config& cfg) {
    uobject::Read(cfg);
    flags = cfg.r_bool8("flags");
    collision_group = cfg.r_u8("collision_group");
    interest.Read(cfg);
}

void uobject_effect::Read(Config& cfg) {
    uobject::Read(cfg);
    startup_animation = cfg.r_animation_str("startup_animation");
    bone_part = cfg.r_part_str("bone_part");
    start_frame = cfg.r_u16("start_frame");
    speed = cfg.r_fp32("speed");
    startup_animation_flags = cfg.r_bool8("startup_animation_flags");
    force_looped = cfg.r_u8("force_looped");
    sound = cfg.r_sound("sound");
    sound_volume = cfg.r_fp32_q8("sound_volume");
    sound_filter = cfg.r_u8("sound_filter");
    particle_flags = cfg.r_bool8("particle_flags");
    particles = cfg.r_choose("particles");
    interest.Read(cfg);
    // do_r_str_array("labels", ait_string);
    uint32_t count = cfg.r_u32("labels");
    labels.resize(count);
    for (uint32_t i = 0; i != count; i++)
        labels[i] = cfg.r_sz("labels");
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

void centity::Read(Config& cfg) {
    health = cfg.r_fp32("health");
    dying_mask = cfg.r_u32("dying_mask");
    physics_flags = cfg.r_bool8("physics_flags");
    physics_flags1 = cfg.r_bool8("physics_flags1");
    physics_flags2 = cfg.r_bool8("physics_flags2");
    uobject_effect::Read(cfg);
    friend_type = cfg.r_u8("friend_type");
    reaction_type = cfg.r_u8("reaction_type");
    fixed_bones = cfg.r_choose_array("fixed_bones");
    break_impulse_threshold = cfg.r_fp32("break_impulse_threshold");
    collisions_group = cfg.r_u8("collisions_group");
    scene_type = cfg.r_u8("scene_type");
    break_particles_break = cfg.r_choose("break_particles_break");
    break_particles_death = cfg.r_choose("break_particles_death");
    break_sound_death = cfg.r_sound("break_sound_death");
    break_sound_death_ai_type = cfg.r_u8("break_sound_death_ai_type");
    type_mask = cfg.r_flags64("type_mask");
    ph_shell_model_src = cfg.r_u32("ph_shell_model_src");
    ph_shell_skltn_src = cfg.r_u32("ph_shell_skltn_src");
    ph_shell_skltn_bcount = cfg.r_u32("ph_shell_skltn_bcount");
    ph_shell_writed = cfg.r_bool("ph_shell_writed");
    if (ph_shell_writed) {
        cfg.ReadSection("physics_shell", [this](Config& cfg) {
            cfg.ReadArray("elements", [this](Config& cfg, uint32_t idx) {
                cfg.r_u16("root_bid");
                cfg.r_fp32("accumulated_impulse");
                cfg.r_pose("xform");
                cfg.r_vec3f("velocity");
                cfg.r_bool("nx_awake");
                cfg.ReadArray("shapes", [this](Config& cfg, uint32_t idx) {
                    cfg.r_u16("bid");
                });
            });
        });
    }
    attach_with_joint = cfg.r_bool("attach_with_joint");
    if (attach_with_joint) {
        cfg.ReadSection("joint_section", [this](Config& cfg) {
            bool enabled = cfg.r_bool("enabled");
            auto entity_src = cfg.r_entity_link("entity_src");
            auto bone_src = cfg.r_attp_str("bone_src");
            auto entity_dst = cfg.r_entity_link("entity_dst");
            auto bone_dst = cfg.r_attp_str("bone_dst");
            auto pos = cfg.r_vec3f("pos");
            auto rot = cfg.r_ang3f("rot");
            // g_physics_world->vfptr->load_joint_desc
            auto joint_type = cfg.r_u16("joint_type");
            cfg.ReadSection("params", [this](Config& cfg) {
                cfg.r_bytes(cfg.SectionRemains());
            });
        });
    }
    footprint_size = cfg.r_fp32("footprint_size");
    footprint_power = cfg.r_fp32("footprint_power");
}

void lamp::Read(Config& cfg) {
    centity::Read(cfg);
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

void inventory_item_object::Read(Config& cfg) {
    flags0 = cfg.r_bool8("flags0");
    trade_weight = cfg.r_u16("trade_weight");
    ui_force_slot_id = cfg.r_u8("ui_force_slot_id");
    centity::Read(cfg);
    anim_simplification = cfg.r_bool("anim_simplification");
}

void chud_item_container_static_params::Read(Config& cfg, uint16_t version) {
}

void upgrade_item_static_params::Read(Config& cfg, uint16_t version) {
    inventory_item_object_static_params::Read(cfg, version);
    container.Read(cfg, version);
}

void upgrade_item::Read(Config& cfg) {
    inventory_item_object::Read(cfg);
    upgrade_id = cfg.r_sz("upgrade_id");
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

void weapon_item::Read(Config& cfg) {
    upgrade_item::Read(cfg);
    vr_attach = cfg.r_bool("vr_attach");
    free_on_level = cfg.r_bool("free_on_level");
}

void unknown_static_params::Read(Config& cfg, uint16_t version) {
    this->version = version;

    unknown = cfg.r_bytes(cfg.Remains());
}

void UnknownObject::Read(Config& cfg) {
    unknown = cfg.r_bytes(cfg.SectionRemains());
}
