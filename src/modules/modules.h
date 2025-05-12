#pragma once  
#include "../../includes.h"  

using json = nlohmann::json;

namespace modules {
	
	namespace reach {
		struct reach_config {
			std::atomic<bool> enable;
			std::atomic<double> min_distance;
			std::atomic<double> max_distance;
			std::atomic<double> skip_chance; // Nuevo campo
		};

		inline void to_json(json& j, const reach_config& c) {
			j = json{
				{"enable", c.enable.load()},
				{"min_distance", c.min_distance.load()},
				{"max_distance", c.max_distance.load()},
				{"skip_chance", c.skip_chance.load()} // Guardado en JSON
			};
		}

		inline void from_json(const json& j, reach_config& c) {
			if (j.contains("enable"))        c.enable.store(j["enable"]);
			if (j.contains("min_distance"))  c.min_distance.store(j["min_distance"]);
			if (j.contains("max_distance"))  c.max_distance.store(j["max_distance"]);
			if (j.contains("skip_chance"))   c.skip_chance.store(j["skip_chance"]); // Cargado desde JSON
		}

		void reach();

		void reachUI();

	}

	namespace aimbot {

		struct aim_config {
			std::atomic<bool> enable;
			std::atomic<int> key;
			std::atomic<float> fov;
			std::atomic<float> speed;
			std::atomic<float> smooth;
			std::atomic<double> distance;
			std::atomic<bool> ignore_targeted;
		};

		inline void to_json(json& j, const aim_config& c) {
			j = json{
				{"enable", c.enable.load()},
				{"key", c.key.load()},
				{"speed", c.speed.load()},
				{"fov", c.fov.load()},
				{"smooth", c.smooth.load()},
				{"distance", c.distance.load()},
				{"ignore_targeted", c.ignore_targeted.load()}
			};
		}

		inline void from_json(const json& j, aim_config& c) {
			if (j.contains("enable"))          c.enable.store(j["enable"]);
			if (j.contains("key"))             c.key.store(j["key"]);
			if (j.contains("fov"))             c.fov.store(j["fov"]);
			if (j.contains("speed"))           c.speed.store(j["speed"]);
			if (j.contains("smooth"))          c.smooth.store(j["smooth"]);
			if (j.contains("distance"))        c.distance.store(j["distance"]);
			if (j.contains("ignore_targeted")) c.ignore_targeted.store(j["ignore_targeted"]);
		}


		void aim();

		void aimUI();
	}

	namespace autoclick {

		struct click_config {
			std::atomic<bool> enable;
			std::atomic<int> key;
			std::atomic<int> cps;
			std::atomic<bool> ignore_blocks;
		};

		inline void to_json(json& j, const click_config& c) {
			j = json{
				{"enable", c.enable.load()},
				{"key", c.key.load()},
				{"cps", c.cps.load()},
				{"ignore_blocks", c.ignore_blocks.load()}
			};
		}

		inline void from_json(const json& j, click_config& c) {
			if (j.contains("enable")) c.enable.store(j["enable"]);
			if (j.contains("key")) c.key.store(j["key"]);
			if (j.contains("cps")) c.cps.store(j["cps"]);
			if (j.contains("ignore_blocks")) c.ignore_blocks.store(j["ignore_blocks"]);
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

	namespace velocity {
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

	namespace noclickdelay {
		struct noclickdelay_cfg {
			std::atomic<bool> enable;
		};

		inline void to_json(json& j, const noclickdelay_cfg& c) {
			j = json{
				{"enable", c.enable.load()}
			};
		}

		inline void from_json(const json& j, noclickdelay_cfg& c) {
			if (j.contains("enable"))          c.enable.store(j["enable"]);
		}

		void ncd();

		void ncdUI();

	}

	namespace destruct {
		struct destruct_cfg {
			std::atomic<bool> destruct = false;
		};

		void destructUI();
	}

	namespace config {

		struct global_config {
			modules::reach::reach_config reach_cfg;
			modules::aimbot::aim_config aim_cfg;
			modules::autoclick::click_config click_cfg;
			modules::fastplace::fplace_config fplace_cfg;
			modules::velocity::vel_config vel_cfg;
			modules::noclickdelay::noclickdelay_cfg ncd_cfg;
			modules::destruct::destruct_cfg des_cfg;
		};

		inline void to_json(json& j, const global_config& c) {
			j = json{
				{"reach", c.reach_cfg},
				{"aimbot", c.aim_cfg},
				{"autoclick", c.click_cfg} ,
				{"fastplace", c.fplace_cfg},
				{"velocity", c.vel_cfg},
				{"noclickdelay", c.ncd_cfg}
			};
		}

		inline void from_json(const json& j, global_config& c) {
			if (j.contains("reach")) j.at("reach").get_to(c.reach_cfg);
			if (j.contains("aimbot")) j.at("aimbot").get_to(c.aim_cfg);
			if (j.contains("autoclick")) j.at("autoclick").get_to(c.click_cfg); 
			if (j.contains("fastplace")) j.at("fastplace").get_to(c.fplace_cfg);
			if (j.contains("velocity")) j.at("velocity").get_to(c.vel_cfg);
			if (j.contains("noclickdelay")) j.at("noclickdelay").get_to(c.ncd_cfg);
		}

		inline global_config cfg;

		void load_config();
		void set_config();
		void create_config();
		void save_config();

		void configUI();
	}

}