/*
 * pageskilltraining.cc
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#include "characterpage.h"

namespace Quafe {

CharacterPage::CharacterPage(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
	: Gtk::Paned(cobject) {
	//[ getting all labels
	add_label_from_glade("label_charname");
	add_label_from_glade("label_charname");
	add_label_from_glade("label_charinfo");
	add_label_from_glade("label_corp");
	add_label_from_glade("label_balance");
	add_label_from_glade("label_skillpoints");
	add_label_from_glade("label_skills");
	add_label_from_glade("label_attr_cha");
	add_label_from_glade("label_attr_int");
	add_label_from_glade("label_attr_per");
	add_label_from_glade("label_attr_mem");
	add_label_from_glade("label_attr_wil");
	//add_label_from_glade();
	//]
}

CharacterPage::~CharacterPage() {
}
}

