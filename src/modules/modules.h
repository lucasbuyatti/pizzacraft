#pragma once  
#include "../../includes.h"  

using json = nlohmann::json;

namespace modules {
	
	namespace aimbot {

		struct aim_config {
			std::atomic<bool> enable;
			std::atomic<int> key;
			std::atomic<float> fov;
			std::atomic<float> speed;
			std::atomic<float> smooth;
			std::atomic<double> distance;
		};

		inline void to_json(json& j, const aim_config& c) {
			j = json{
				{"enable", c.enable.load()},
				{"key", c.key.load()},
				{"speed", c.speed.load()},
				{"fov", c.fov.load()},
				{"smooth", c.smooth.load()},
				{"distance", c.distance.load()}
			};
		}

		inline void from_json(const json& j, aim_config& c) {
			if (j.contains("enable")) c.enable.store(j["enable"]);
			if (j.contains("key"))    c.key.store(j["key"]);
			if (j.contains("fov"))    c.fov.store(j["fov"]);
			if (j.contains("speed"))  c.speed.store(j["speed"]);
			if (j.contains("smooth")) c.smooth.store(j["smooth"]);
			if (j.contains("distance")) c.distance.store(j["distance"]);
		}


		void aim();

		void aimUI();
	}

	namespace autoclick {

		struct click_config {
			std::atomic<bool> enable;
			std::atomic<int> key;
			std::atomic<int> cps;
		};

		inline void to_json(json& j, const click_config& c) {
			j = json{
				{"enable", c.enable.load()},
				{"key", c.key.load()},
				{"cps", c.cps.load()}
			};
		}

		// Función para deserializar 'click_config' desde JSON
		inline void from_json(const json& j, click_config& c) {
			if (j.contains("enable")) c.enable.store(j["enable"]);
			if (j.contains("key")) c.key.store(j["key"]);
			if (j.contains("cps")) c.cps.store(j["cps"]);
		}

		void click();

		void clickUI();
	}

	namespace fastplace {
		
		struct fplace_config {
			std::atomic<bool> enable;
			std::atomic<bool> blocks_projectiles;
			std::atomic<int> delay;
		};

		inline void to_json(json& j, const fplace_config& c) {
			j = json{
				{"enable", c.enable.load()},
				{"blocks_projectiles", c.blocks_projectiles.load()},
				{"delay", c.delay.load()}
			};
		}

		inline void from_json(const json& j, fplace_config& c) {
			if (j.contains("enable")) c.enable.store(j["enable"]);
			if (j.contains("blocks_projectiles")) c.blocks_projectiles.store(j["blocks_projectiles"]);
			if (j.contains("delay")) c.delay.store(j["delay"]);
		}

		void fplace();

		void fplaceUI();
	}

	namespace velocity { // agregar
		struct vel_config {
			std::atomic<bool> enable;
			std::atomic<int> chance;
		};

		inline void to_json(json& j, const vel_config& c) {
			j = json{
				{"enable", c.enable.load()},
				{"chance", c.chance.load()}
			};
		}

		inline void from_json(const json& j, vel_config& c) {
			if (j.contains("enable")) c.enable.store(j["enable"]);
			if (j.contains("chance")) c.chance.store(j["chance"]);
		}


		void vel();

		void velUI();

	}

	namespace config {

		struct global_config {
			modules::aimbot::aim_config aim_cfg;
			modules::autoclick::click_config click_cfg;
			modules::fastplace::fplace_config fplace_cfg;
			modules::velocity::vel_config vel_cfg;
		};

		inline void to_json(json& j, const global_config& c) {
			j = json{
				{"aimbot", c.aim_cfg},
				{"autoclick", c.click_cfg} ,
				{"fastplace", c.fplace_cfg},
				{"velocity", c.vel_cfg}
			};
		}

		inline void from_json(const json& j, global_config& c) {
			if (j.contains("aimbot")) j.at("aimbot").get_to(c.aim_cfg);
			if (j.contains("autoclick")) j.at("autoclick").get_to(c.click_cfg); 
			if (j.contains("fastplace")) j.at("fastplace").get_to(c.fplace_cfg);
			if (j.contains("velocity")) j.at("velocity").get_to(c.vel_cfg);
		}

		inline global_config cfg;

		void load_config();
		void set_config();
		void create_config();
		void save_config();

		void configUI();
	}

}