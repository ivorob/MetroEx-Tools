#include "MetroWeaponsDatabase.h"
#include "MetroReflection.h"
#include "MetroTypes.h"
#include "MetroBinArrayArchive.h"
#include "MetroBinArchive.h"
#include "MetroFileSystem.h"
#include "MetroTypedStrings.h"


#include <fstream>


void MetroWeaponHandle::Serialize(MetroReflectionStream& s) {
    //#TODO_SK: fix this shit! Should detect based on the Editor flag!
    METRO_SERIALIZE_MEMBER_CHOOSE(s, upgrades);

    upgradesArray = StrSplit(upgrades, ',');
}

void MetroShootingParticlesData::Serialize(MetroReflectionStream& s) {
    METRO_SERIALIZE_MEMBER_CHOOSE(s, flame_particles_hud);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, flame_particles);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, smoke_particles_hud);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, smoke_particles);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, smoke_particles_long_hud);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, smoke_particles_long);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, smoke_particles_end_hud);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, smoke_particles_end);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, shot_particles);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, shell_particles);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, overheat_particles);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, overheat_smoke_particles);
}

void MetroShootingLightData::Serialize(MetroReflectionStream& s) {
    MetroReflectionStream* data = s.OpenSection("shooting_light");
    if (data) {
        METRO_SERIALIZE_MEMBER(*data, type);
        METRO_SERIALIZE_MEMBER(*data, color);
        METRO_SERIALIZE_MEMBER(*data, brightness);
        METRO_SERIALIZE_MEMBER(*data, range_far);
        METRO_SERIALIZE_MEMBER(*data, lod_scale);
        METRO_SERIALIZE_MEMBER(*data, data1);
        METRO_SERIALIZE_MEMBER(*data, data2);
        METRO_SERIALIZE_MEMBER(*data, ibl_gen_radius);
        METRO_SERIALIZE_MEMBER(*data, range_near);
        METRO_SERIALIZE_MEMBER(*data, source_size);
        METRO_SERIALIZE_MEMBER(*data, cone);
        METRO_SERIALIZE_MEMBER(*data, quality);
        METRO_SERIALIZE_MEMBER(*data, position);
        METRO_SERIALIZE_MEMBER(*data, direction);
        METRO_SERIALIZE_MEMBER(*data, right);
        METRO_SERIALIZE_MEMBER_CHOOSE(*data, color_ca);
        METRO_SERIALIZE_MEMBER_CHOOSE(*data, texture);
        METRO_SERIALIZE_MEMBER_CHOOSE(*data, faces);
        METRO_SERIALIZE_MEMBER(*data, light_flags1);
        METRO_SERIALIZE_MEMBER(*data, light_flags2);

        s.CloseSection(data);
    }

    METRO_SERIALIZE_MEMBER(s, light_var_color);
    METRO_SERIALIZE_MEMBER(s, light_var_range);
    METRO_SERIALIZE_MEMBER(s, light_time);
    METRO_SERIALIZE_MEMBER(s, light_luminosity);
}

void MetroWeaponModifier::Serialize(MetroReflectionStream& s) {
    METRO_SERIALIZE_MEMBER(s, weapon_sdata_key);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_shot);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_shot_golden);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_draw);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_holster);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_reload);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_jam);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_jammed_shot);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_deafening);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_echo);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_dbl_echo);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_doubletap);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_no_pressure);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_pump_low);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_pump_normal);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_pump_high);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, snd_charging);
    METRO_SERIALIZE_MEMBER(s, snd_shoot_ai_type);
    METRO_SERIALIZE_MEMBER(s, aim_track_accrue);
    METRO_SERIALIZE_MEMBER(s, aim_track_falloff);
    METRO_SERIALIZE_MEMBER(s, pressure_shot_drop);
    METRO_SERIALIZE_MEMBER(s, pressure_overpump_drop);
    METRO_SERIALIZE_MEMBER(s, pressure_rotation_drop);
    METRO_SERIALIZE_MEMBER(s, pressure_after_shot_increase);
    METRO_SERIALIZE_MEMBER(s, track_barrel_spin_modifier);
    METRO_SERIALIZE_MEMBER(s, track_barrel_spin_max);
    METRO_SERIALIZE_MEMBER(s, spin_sens);
    METRO_SERIALIZE_MEMBER(s, heating_speed);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, steam_particles);

    // doubletap_particles section
    MetroReflectionStream* doubletap_particles_section = s.OpenSection("doubletap_particles");
    if (doubletap_particles_section) {
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, flame_particles_hud);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, flame_particles);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, smoke_particles_hud);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, smoke_particles);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, smoke_particles_long_hud);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, smoke_particles_long);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, smoke_particles_end_hud);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, smoke_particles_end);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, shot_particles);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, shell_particles);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, overheat_particles);
        METRO_SERIALIZE_MEMBER_CHOOSE(*doubletap_particles_section, overheat_smoke_particles);

        s.CloseSection(doubletap_particles_section);
    }
    //

    METRO_SERIALIZE_MEMBER(s, need_doubletap_particles);
    METRO_SERIALIZE_MEMBER(s, hud);
    METRO_SERIALIZE_MEMBER(s, need_offset);
    METRO_SERIALIZE_MEMBER(s, need_echo);
    METRO_SERIALIZE_MEMBER(s, can_long_idle);
    METRO_SERIALIZE_MEMBER_ANIMSTR(s, jump_idle);
    METRO_SERIALIZE_MEMBER_ANIMSTR(s, long_idle);
    METRO_SERIALIZE_MEMBER_ANIMSTR(s, friend_idle);
    METRO_SERIALIZE_MEMBER_ANIMSTR(s, friend_idle_run);
    METRO_SERIALIZE_MEMBER_ANIMSTR(s, lock_shoot);
    METRO_SERIALIZE_MEMBER_ANIMSTR(s, lock_aim_shoot);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, lock_shoot_track);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, lock_aim_shoot_track);
}

void MetroShootingWeaponData::Serialize(MetroReflectionStream& s) {
    METRO_SERIALIZE_MEMBER(s, dispersion_base);
    METRO_SERIALIZE_MEMBER(s, dispersion_base_surv);
    METRO_SERIALIZE_MEMBER(s, dispersion_aim);
    METRO_SERIALIZE_MEMBER(s, dispersion_aim_surv);
    METRO_SERIALIZE_MEMBER(s, dispersion_move_factor);
    METRO_SERIALIZE_MEMBER(s, disp_rot_factor);
    METRO_SERIALIZE_MEMBER(s, dispersion_lh);
    METRO_SERIALIZE_MEMBER(s, dispersion_inc);
    METRO_SERIALIZE_MEMBER(s, dispersion_min);
    METRO_SERIALIZE_MEMBER(s, dispersion_max);
    METRO_SERIALIZE_MEMBER(s, dispersion_max_npc);
    METRO_SERIALIZE_MEMBER(s, dispersion_dec);
    METRO_SERIALIZE_MEMBER(s, min_ammo_show);
    METRO_SERIALIZE_MEMBER(s, num_shot_dispersed);
    METRO_SERIALIZE_MEMBER(s, rpm);
    METRO_SERIALIZE_MEMBER(s, ui_damage);
    METRO_SERIALIZE_MEMBER(s, ui_accuracy);
    METRO_SERIALIZE_MEMBER(s, ui_rpm);
    // bullet params
    METRO_SERIALIZE_MEMBER(s, blt_power);
    METRO_SERIALIZE_MEMBER(s, blt_impulse);
    METRO_SERIALIZE_MEMBER(s, blt_power_falloff);
    METRO_SERIALIZE_MEMBER(s, blt_impulse_falloff);
    METRO_SERIALIZE_MEMBER(s, blt_speed);
    METRO_SERIALIZE_MEMBER(s, blt_head_coef);
    METRO_SERIALIZE_MEMBER(s, blt_monster_coef);
    METRO_SERIALIZE_MEMBER(s, blt_fire_distance_min);
    METRO_SERIALIZE_MEMBER(s, blt_fire_distance_max);
    METRO_SERIALIZE_MEMBER(s, blt_fire_distance);
    METRO_SERIALIZE_MEMBER(s, blt_pierce);
    METRO_SERIALIZE_MEMBER(s, blt_gravity_mod);
    METRO_SERIALIZE_MEMBER(s, blt_tracer_scale_xy);
    METRO_SERIALIZE_MEMBER(s, blt_tracer_scale_z);
    METRO_SERIALIZE_MEMBER(s, blt_tracer_min_dist);
    METRO_SERIALIZE_MEMBER(s, blt_tracer_probability);
    METRO_SERIALIZE_MEMBER(s, blt_trail_probability);
    METRO_SERIALIZE_MEMBER(s, blt_material);
    //
    METRO_SERIALIZE_STRUCT_ARRAY_MEMBER(s, camera_track_attack_arr);
    // recoil
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_vert);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_horiz);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_shake);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_shot_first_vert);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_shot_burst_vert);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_shot_first_horiz);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_shot_burst_horiz);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_shot_first_shake);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_shot_burst_shake);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_return_vert);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, recoil_curve_return_horiz);
    METRO_SERIALIZE_MEMBER(s, recoil_increase);
    METRO_SERIALIZE_MEMBER(s, recoil_increase_pad);
    METRO_SERIALIZE_MEMBER(s, recoil_increase_time);
    METRO_SERIALIZE_MEMBER(s, recoil_vert_coef);
    METRO_SERIALIZE_MEMBER(s, recoil_vert_coef_pad);
    METRO_SERIALIZE_MEMBER(s, recoil_horiz_coef);
    METRO_SERIALIZE_MEMBER(s, recoil_horiz_coef_pad);
    METRO_SERIALIZE_MEMBER(s, recoil_horiz_random);
    METRO_SERIALIZE_MEMBER(s, recoil_decrease_time);
    METRO_SERIALIZE_MEMBER(s, recoil_shake_coef);
    METRO_SERIALIZE_MEMBER(s, recoil_shake_speed_coef);
    METRO_SERIALIZE_MEMBER(s, recoil_shake_falloff);
    METRO_SERIALIZE_MEMBER(s, recoil_crouch_coef);
    METRO_SERIALIZE_MEMBER(s, recoil_aim_coef);
    METRO_SERIALIZE_MEMBER(s, recoil_one_hand_coef);
    METRO_SERIALIZE_MEMBER(s, recoil_weapon_coef);
    METRO_SERIALIZE_MEMBER(s, recoil_double_shot_coef);
    METRO_SERIALIZE_MEMBER(s, ui_recoil);
    //
    METRO_SERIALIZE_MEMBER(s, shot_heat_increase);
    METRO_SERIALIZE_MEMBER(s, heat_decrease_speed);
    METRO_SERIALIZE_MEMBER(s, critical_heat);
    METRO_SERIALIZE_MEMBER(s, critical_heat_smoke);
    METRO_SERIALIZE_MEMBER(s, sway_crouch_coef);
    METRO_SERIALIZE_MEMBER(s, sway_aim_coef);
    METRO_SERIALIZE_MEMBER(s, sway_one_hand_coef);
    METRO_SERIALIZE_MEMBER(s, sway_increase_coef);
    METRO_SERIALIZE_MEMBER(s, sway_stabilize_speed_coef);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, sway_point_loc_name);

    // dirt modifiers
    MetroReflectionStream* dirt_modifiers_section = s.OpenSection("dirt_modifiers");
    if (dirt_modifiers_section) {
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_dispersion_base);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_dispersion_dec);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_blt_power);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_blt_impulse);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_blt_fire_distance_min);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_blt_fire_distance_max);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_blt_speed);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_rpm);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_decrease_time);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_shake_speed_coef);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_shake_coef);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_horiz_random);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_horiz_coef);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_horiz_coef_pad);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_increase);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_increase_pad);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_vert_coef);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_recoil_vert_coef_pad);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_wpn_aim_shot_weight);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_wpn_shot_weight);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_shot_heat_increase);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_heat_decrease_speed);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_ui_recoil);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_ui_damage);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_ui_accuracy);
        METRO_SERIALIZE_MEMBER(*dirt_modifiers_section, mod_ui_rpm);

        s.CloseSection(dirt_modifiers_section);
    }
    //

    METRO_SERIALIZE_MEMBER(s, shot_weight);
    METRO_SERIALIZE_MEMBER(s, shot_weight_surv);
    METRO_SERIALIZE_MEMBER(s, aim_shot_weight);
    METRO_SERIALIZE_MEMBER(s, aim_shot_weight_surv);

    // axis dispplacement
    MetroReflectionStream* axis_disp_section = s.OpenSection("axis_disp");
    if (axis_disp_section) {
        METRO_SERIALIZE_MEMBER(*axis_disp_section, scale_horizontal);
        METRO_SERIALIZE_MEMBER(*axis_disp_section, scale_vertical);
        METRO_SERIALIZE_MEMBER(*axis_disp_section, deviation_horizontal);
        METRO_SERIALIZE_MEMBER(*axis_disp_section, deviation_vertical);

        s.CloseSection(axis_disp_section);
    }
    //

    METRO_SERIALIZE_MEMBER(s, track_shot_weight);
    METRO_SERIALIZE_MEMBER(s, aim_track_shot_weight);
    METRO_SERIALIZE_MEMBER(s, magazine_size);
    METRO_SERIALIZE_MEMBER(s, max_bullets_in_barrel);
    METRO_SERIALIZE_MEMBER(s, ammo_to_fire);
    METRO_SERIALIZE_MEMBER(s, barrel_state);
    METRO_SERIALIZE_MEMBER(s, wi_flags);
    METRO_SERIALIZE_MEMBER(s, wi_flags2);
    METRO_SERIALIZE_MEMBER(s, has_auto_fire);
    METRO_SERIALIZE_MEMBER(s, has_regular_barrel_loading);
    METRO_SERIALIZE_MEMBER(s, blend_interrupt_by_fire_allowed);
    METRO_SERIALIZE_MEMBER(s, can_jam);
    METRO_SERIALIZE_MEMBER(s, need_update_ssss);
    METRO_SERIALIZE_MEMBER(s, type_priority);
    METRO_SERIALIZE_MEMBER(s, busy_on_shot_time);
    // hud
    METRO_SERIALIZE_MEMBER(s, hud_deltas_weight);
    METRO_SERIALIZE_MEMBER(s, hud_deltas_weight_aim);
    METRO_SERIALIZE_MEMBER(s, hud_deltas_weight_vert);
    METRO_SERIALIZE_MEMBER(s, hud_deltas_weight_coll_max);
    METRO_SERIALIZE_MEMBER(s, hud_deltas_weight_vert_coll_max);
    METRO_SERIALIZE_MEMBER(s, hud_scale_x_aim);
    METRO_SERIALIZE_MEMBER(s, hud_scale_y_aim);
    // aim
    METRO_SERIALIZE_MEMBER_CHOOSE(s, aim_in_camera_track);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, aim_idle_camera_track);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, aim_out_camera_track);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, aim_idle_excl_camera_track);
    METRO_SERIALIZE_MEMBER(s, aim_track_accrue)
    METRO_SERIALIZE_MEMBER(s, aim_track_falloff);
    //
    METRO_SERIALIZE_MEMBER_CHOOSE(s, track_fire_preparation);
    METRO_SERIALIZE_MEMBER(s, fire_bone);
    METRO_SERIALIZE_MEMBER(s, shell_bone);
    METRO_SERIALIZE_MEMBER(s, armory_holder_front);
    METRO_SERIALIZE_MEMBER(s, armory_holder_back);
    METRO_SERIALIZE_MEMBER(s, hud_scale_base);
    METRO_SERIALIZE_MEMBER(s, static_offset_bone);
    // pumping tracks and pneumo stuff
    METRO_SERIALIZE_MEMBER_CHOOSE(s, track_pumping_low);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, track_pumping_med);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, track_pumping_high);
    METRO_SERIALIZE_MEMBER(s, pressure);
    METRO_SERIALIZE_MEMBER(s, pressure_low);
    METRO_SERIALIZE_MEMBER(s, pressure_normal);
    METRO_SERIALIZE_MEMBER(s, pressure_max);
    METRO_SERIALIZE_MEMBER(s, pressure_auto_pump_raise);
    METRO_SERIALIZE_MEMBER(s, pressure_shot_overdrop);
    METRO_SERIALIZE_MEMBER(s, pressure_shot_overdrop_coef_min);
    METRO_SERIALIZE_MEMBER(s, pressure_shot_overdrop_coef_speed_min);
    METRO_SERIALIZE_MEMBER(s, pressure_low_stat_coef_min);
    METRO_SERIALIZE_MEMBER(s, pressure_low_stat_coef_speed_min);
    METRO_SERIALIZE_MEMBER(s, pneumo_enabled);
    METRO_SERIALIZE_MEMBER(s, has_pressure_drop_on_empty_fire);
    //
    METRO_SERIALIZE_MEMBER(s, customize_id);
    METRO_SERIALIZE_MEMBER(s, breaking_speed);
    METRO_SERIALIZE_MEMBER(s, speed_coef_as);
    METRO_SERIALIZE_MEMBER(s, swap_speed_coef);
    METRO_SERIALIZE_MEMBER(s, speed_coef_rpm);
    METRO_SERIALIZE_MEMBER(s, disp_anim_idle_horiz);
    METRO_SERIALIZE_MEMBER(s, disp_anim_idle_horiz_speed);
    METRO_SERIALIZE_MEMBER(s, disp_anim_idle_vert);
    METRO_SERIALIZE_MEMBER(s, disp_anim_idle_vert_speed);
    METRO_SERIALIZE_MEMBER(s, disp_anim_aim);
    METRO_SERIALIZE_MEMBER(s, disp_anim_aim_speed);
    METRO_SERIALIZE_MEMBER(s, disp_anim_horiz_lh_coef);
    METRO_SERIALIZE_MEMBER(s, disp_anim_vert_lh_coef);
    METRO_SERIALIZE_MEMBER(s, crosshair_size_lh_coef);
    METRO_SERIALIZE_MEMBER(s, pressure_rpm_low);
    METRO_SERIALIZE_MEMBER(s, rpm_low_unmodified);
    METRO_SERIALIZE_MEMBER(s, acceleration);
    METRO_SERIALIZE_MEMBER(s, deceleration);
    METRO_SERIALIZE_MEMBER(s, aim_box_distance);
    METRO_SERIALIZE_MEMBER(s, aim_box_y_offset);
    METRO_SERIALIZE_MEMBER(s, aim_box_buffer_size);
    METRO_SERIALIZE_MEMBER_CHOOSE(s, item_attp_trade);
    METRO_SERIALIZE_MEMBER_STRARRAY_CHOOSE(s, attp_invisible_ui);
    //
    METRO_SERIALIZE_STRUCT_ARRAY_MEMBER(s, modifiers);
}


void MetroWeaponAttachmentDesc::Serialize(MetroReflectionStream& s) {
    METRO_SERIALIZE_MEMBER(s, type);
    METRO_SERIALIZE_MEMBER(s, upgrade_id);
    METRO_SERIALIZE_MEMBER(s, base_handle);
    // data section
    MetroReflectionStream* data = s.OpenSection("data");
    if (data) {
        METRO_SERIALIZE_MEMBER_CHOOSE(*data, shared_weapon_params);
        METRO_SERIALIZE_MEMBER(*data, priority);
        METRO_SERIALIZE_MEMBER(*data, ui_tag_lib);
        METRO_SERIALIZE_ARRAY_16_MEMBER(*data, tags);
        METRO_SERIALIZE_ARRAY_16_MEMBER(*data, excl_tags);
        METRO_SERIALIZE_ARRAY_16_MEMBER(*data, dep_tags);
        METRO_SERIALIZE_ARRAY_16_MEMBER(*data, autoinst_tags);
        METRO_SERIALIZE_MEMBER(*data, is_installed);
        METRO_SERIALIZE_MEMBER(*data, editor_name);
        METRO_SERIALIZE_MEMBER(*data, attach_hud_loc);
        METRO_SERIALIZE_MEMBER(*data, attach_offset);
        METRO_SERIALIZE_MEMBER(*data, attach_to_root);
        METRO_SERIALIZE_MEMBER_CHOOSE(*data, visual);
        METRO_SERIALIZE_MEMBER(*data, anim_track_prefix);
        METRO_SERIALIZE_MEMBER(*data, anim_track_suffix);

        if (data->IsIn()) {
            MetroReflectionBinaryReadStream* binaryData = scast<MetroReflectionBinaryReadStream*>(data);
            hasPayload = binaryData->GetRemains() > 0;
            if (hasPayload) {
                (*data) >> particles_data;
                (*data) >> light_data;
                (*data) >> shooting_weapon_data;
            }
        }

        s.CloseSection(data);
    }
}


MetroWeaponsDatabase::MetroWeaponsDatabase() {
}

MetroWeaponsDatabase::~MetroWeaponsDatabase() {
}

bool MetroWeaponsDatabase::Initialize() {
    bool result = false;

    const MetroFileSystem& mfs = MetroFileSystem::Get();

    MyHandle fileHandle = mfs.FindFile(R"(content\weaponry\weapons_handles_storage.bin)");
    if (fileHandle != kInvalidHandle) {
        MemStream stream = mfs.OpenFileStream(fileHandle);
        if (stream) {
            this->ReadWeaponHandles(stream);
        }
    }

    fileHandle = mfs.FindFile(R"(content\weaponry\attaches_handles_storage.bin)");
    if (fileHandle != kInvalidHandle) {
        MemStream stream = mfs.OpenFileStream(fileHandle);
        if (stream) {
            this->ReadWeaponAttaches(stream);
        }
    }

    if (!mWeaponHandles.empty() && !mWeaponAttachments.empty()) {
        result = true;
    }

    return result;
}

void MetroWeaponsDatabase::ReadWeaponHandles(MemStream& stream) {
    const size_t numEntries = stream.ReadTyped<uint32_t>();

    mWeaponHandles.resize(numEntries);
    for (size_t i = 0; i < numEntries; ++i) {
        const size_t idx = stream.ReadTyped<uint32_t>();
        const size_t size = stream.ReadTyped<uint32_t>();

        MemStream subStream = stream.Substream(size);

        MetroWeaponHandle& weaponHandle = mWeaponHandles[i];

        weaponHandle.name = subStream.ReadStringZ();
        const uint8_t flags = subStream.ReadTyped<uint8_t>();

        MetroReflectionBinaryReadStream reader(subStream, flags);
        reader >> weaponHandle;

        stream.SkipBytes(size);
    }
}

void MetroWeaponsDatabase::ReadWeaponAttaches(MemStream& stream) {
    const MetroTypedStrings& mts = MetroTypedStrings::Get();

    const size_t numEntries = stream.ReadTyped<uint32_t>();

//    std::ofstream testFile("123\\test.txt");

    mWeaponAttachments.resize(numEntries);
    for (size_t i = 0; i < numEntries; ++i) {
        const size_t idx = stream.ReadTyped<uint32_t>();
        const size_t size = stream.ReadTyped<uint32_t>();

        MemStream subStream = stream.Substream(size);

        MetroWeaponAttachmentDesc& attachment = mWeaponAttachments[i];

        attachment.name = subStream.ReadStringZ();
        const uint8_t flags = subStream.ReadTyped<uint8_t>();

        MetroReflectionBinaryReadStream reader(subStream, flags);
        reader.SetUserData(i);
        reader >> attachment;

        //testFile << std::setfill('0') << std::setw(8) << i << std::endl;
        //testFile << "name = " << attachment.name.str << std::endl;
        //testFile << "type = " << (int)attachment.type << std::endl;
        //testFile << "upgrade_id = " << attachment.upgrade_id << std::endl;
        //testFile << "upgrade name = " << mts.GetString(attachment.upgrade_id).str << std::endl;
        //testFile << "base_handle = " << attachment.base_handle << std::endl;
        //testFile << "shared_weapon_params = " << attachment.shared_weapon_params << std::endl;
        //testFile << "priority = " << attachment.priority << std::endl;
        //testFile << "ui_tag_lib = " << attachment.ui_tag_lib << std::endl;
        //testFile << "tags:" << std::endl;
        //for (auto t : attachment.tags) {
        //    testFile << t << std::endl;
        //}
        //testFile << "excl_tags:" << std::endl;
        //for (auto t : attachment.excl_tags) {
        //    testFile << t << std::endl;
        //}
        //testFile << "dep_tags:" << std::endl;
        //for (auto t : attachment.dep_tags) {
        //    testFile << t << std::endl;
        //}
        //testFile << "autoinst_tags:" << std::endl;
        //for (auto t : attachment.autoinst_tags) {
        //    testFile << t << std::endl;
        //}
        //testFile << "is_installed = " << attachment.is_installed << std::endl;
        //testFile << "editor_name = " << attachment.editor_name << std::endl;
        //testFile << "attach_hud_loc = " << attachment.attach_hud_loc << std::endl;
        //testFile << "attach_offset = " << attachment.attach_offset[0][0] << attachment.attach_offset[0][1] << attachment.attach_offset[0][2] << attachment.attach_offset[0][3]
        //                               << attachment.attach_offset[1][0] << attachment.attach_offset[1][1] << attachment.attach_offset[1][2] << attachment.attach_offset[1][3]
        //                               << attachment.attach_offset[2][0] << attachment.attach_offset[2][1] << attachment.attach_offset[2][2] << attachment.attach_offset[2][3] << std::endl;
        //testFile << "attach_to_root = " << attachment.attach_to_root << std::endl;
        //testFile << "visual = " << attachment.visual << std::endl;
        //testFile << "anim_track_prefix = " << attachment.anim_track_prefix << std::endl;
        //testFile << "anim_track_suffix = " << attachment.anim_track_suffix << std::endl;

        //testFile << "============================================================" << std::endl;

        stream.SkipBytes(size);
    }

    //testFile.flush();
    //testFile.close();
}
