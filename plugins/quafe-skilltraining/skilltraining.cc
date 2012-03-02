/**
 *
 *  Quafe - Eve tools for linux.
 *  Copyright (C) 2012 Christoph Nikic
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "skilltraining.h"
#include <gtkmm/label.h>
namespace Quafe {
namespace Plugin {

ConstructParams params = {
		"skilltraining",
		"Skill Training",
		"Training planer and more",
		"skilltraining.png"
	};

extern "C" const ConstructParams & get_params() {
	return params;
}

extern "C" SkillTraining* create() {
    return new SkillTraining(params);
}

extern "C" void destroy(SkillTraining* p) {
    delete p;
}

SkillTraining::SkillTraining(const ConstructParams &params) :
		PluginBase(params) {
	m_plugin_widget = new Gtk::Label("skilltraining");

}

SkillTraining::~SkillTraining() {
}

}
}
