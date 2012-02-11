/*
 * charinfo.h
 *
 *  Created on: 07.02.2012
 *      Author: cn
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <quafe-etk.h>

#include <plugin/pluginbase.h>

#include <gtkmm/widget.h>
/*
 *
 */
namespace Quafe {
namespace Plugin {
class PluginCharacterInfo : public PluginBase {
public:
	PluginCharacterInfo();
	virtual ~PluginCharacterInfo();

	void show(Gtk::Widget &wg);

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
		return "Character Info";
	}

	ustring plugin_description() const {
		return "Assets, Wallet Journal and more";
	}
};


}
}

#endif
