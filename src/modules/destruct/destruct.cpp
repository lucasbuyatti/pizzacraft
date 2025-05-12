#include "../modules.h"



void modules::destruct::destructUI() {

	if (ImGui::Button("Destruct")) {
		modules::config::cfg.des_cfg.destruct.store(true);
	}
}