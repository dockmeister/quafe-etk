/**
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

#ifndef WINDOW_PREFERENCES_H_
#define WINDOW_PREFERENCES_H_

#include <quafe-etk.h>
#include <utility.h>

#include <boost/function.hpp>
#include <gtkmm/window.h>

namespace Quafe {

/*!\brief
 *
 */
class WindowPreferences : public Gtk::Window {
	friend class Preferences;
public:
	WindowPreferences();
	virtual ~WindowPreferences();

private:
	Gtk::Widget *create_general_tab();
	Gtk::Widget *create_account_tab();
	boost::function<void ()> action_ok;
	boost::function<void ()> action_cancel;
	boost::function<void (ustring id, ustring key)> action_add_api_key;
};

} /* namespace Quafe */
#endif /* WINDOW_PREFERENCES_H_ */
