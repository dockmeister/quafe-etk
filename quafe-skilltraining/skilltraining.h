/*
 * charinfo.h
 *
 *  Created on: 07.02.2012
 *      Author: cn
 */

#ifndef SKILLTRAINING_H_
#define SKILLTRAINING_H_

#include <quafe-etk.h>

#include <plugin/pluginbase.h>

#include <gtkmm/widget.h>
/*
 *
 */
namespace Quafe {
namespace Plugin {
class SkillTraining : public PluginBase {
public:
	SkillTraining();
	virtual ~SkillTraining();

	void show(Gtk::Widget &wg);

	gboolean close() const {
		return true;
	}

	ustring plugin_id() const {
		return "skilltraining";
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
};


}
}

#endif
