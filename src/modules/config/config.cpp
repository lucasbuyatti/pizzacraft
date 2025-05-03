#include "../modules.h"



void modules::config::load_config() {
    std::ifstream in("config.json");

    if (!in) {
        create_config();
        return;
    }

    set_config();

}

void modules::config::set_config() {
    std::ifstream in("config.json");
    if (!in.is_open())
        return;

    json j;
    in >> j;

    if (j.contains("aimbot")) {
        auto& aim_config_json = j["aimbot"];
        cfg.aim_cfg.enable.store(aim_config_json.value("enable", false));
        cfg.aim_cfg.key.store(aim_config_json.value("key", 1)); 
        cfg.aim_cfg.fov.store(aim_config_json.value("fov", 1.0f));
        cfg.aim_cfg.speed.store(aim_config_json.value("speed", 1.0f));
        cfg.aim_cfg.smooth.store(aim_config_json.value("smooth", 1.0f));
        cfg.aim_cfg.distance.store(aim_config_json.value("distance", 1.0f));
    }

    if (j.contains("autoclick")) {
        auto& click_config_json = j["autoclick"];
        cfg.click_cfg.enable.store(click_config_json.value("enable", false));
        cfg.click_cfg.key.store(click_config_json.value("key", 1));
        cfg.click_cfg.cps.store(click_config_json.value("cps", 10));
    }

    if (j.contains("fastplace")) {
        auto& fplace_config_json = j["fastplace"];
        cfg.fplace_cfg.enable.store(fplace_config_json.value("enable", false));
        cfg.fplace_cfg.delay.store(fplace_config_json.value("delay", 1));
        cfg.fplace_cfg.blocks_projectiles.store(fplace_config_json.value("blocks_projectiles", false));
    }

    if (j.contains("velocity")) {
        auto& vel_config_json = j["velocity"];
        cfg.vel_cfg.enable.store(vel_config_json.value("enable", false));
        cfg.vel_cfg.chance.store(vel_config_json.value("chance", 1));
    }
}

void modules::config::create_config() {
    std::ofstream out("config.json");
    
    cfg.aim_cfg.enable.store(false);
    cfg.aim_cfg.key.store(1);
    cfg.aim_cfg.fov.store(1.f);
    cfg.aim_cfg.speed.store(1.f);
    cfg.aim_cfg.smooth.store(1.f);
    cfg.aim_cfg.distance.store(1.f);

    cfg.click_cfg.enable.store(false);
    cfg.click_cfg.key.store(1);
    cfg.click_cfg.cps.store(10);

    cfg.fplace_cfg.enable.store(false);
    cfg.fplace_cfg.delay.store(1);
    cfg.fplace_cfg.blocks_projectiles.store(false);

    cfg.vel_cfg.enable.store(false);
    cfg.vel_cfg.chance.store(1);

    json j = cfg;

    if (out.is_open()) {
        out << std::setw(4) << j;
        out.close();
    }
}

void  modules::config::configUI() {
    if (ImGui::Button("Save config")) {
        save_config();
    }
}

void modules::config::save_config() {
    std::ofstream out("config.json");
    if (out.is_open()) {
        json j = cfg;
        out << j.dump(4); 
    }
}