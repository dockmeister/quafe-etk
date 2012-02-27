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

#ifndef SKILLTRAINING_H_
#define SKILLTRAINING_H_

#include <quafe-etk.h>

#include <include/pluginbase.h>

#include <gtkmm/widget.h>
namespace Quafe {
namespace Plugin {
/*!\brief
 *
 */
class SkillTraining : public PluginBase {
public:
	SkillTraining();
	virtual ~SkillTraining();

	gboolean close() const {
		return true;
	}

	ustring plugin_id() const {
		return "charinfo";
	}

	ustring plugin_icon_path() const {
		return "data/module-icon.png";
	}

	ustring plugin_title() const {
		return "Skill Training";
	}

	ustring plugin_description() const {
		return "Skill Training";
	}
	Gtk::Widget *m_plugin_widget;
};


}
}

#endif
