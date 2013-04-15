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

#include <plugins/plugininterface.h>
#include <plugins/pluginbase.h>

#include <gtkmm/notebook.h>

namespace EAPI {
class KeyInfo;
class CharacterSheet;
class SkillQueue;
}
namespace Quafe {

namespace Plugin {

/*!\brief
 *
 */
class SkillTraining : public PluginInterface, public PluginBase {
public:
	SkillTraining(const PluginParams &params);
	virtual ~SkillTraining();

	bool close() const;
	Gtk::Widget& show() const;
	void hide();

	void on_keyinfo_added(EAPI::KeyInfo *key);
	void on_keyinfo_removed(EAPI::KeyInfo *key);
	void on_charactersheet_added(EAPI::CharacterSheet *sheet);
	void on_charactersheet_removed(EAPI::CharacterSheet *sheet);
	void on_charactersheet_updated(EAPI::CharacterSheet *sheet);
	void on_skillqueue_updated(EAPI::SkillQueue *queue);

protected:
	void update_character_page(EAPI::CharacterSheet *sheet, CharacterPage *page);
private:
	Gtk::Notebook *m_notebook;
};

}
}

#endif
