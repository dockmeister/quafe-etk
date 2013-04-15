/*
 * preferences_translator.h
 *
 *  Created on: 29.02.2012
 *      Author: cn
 */

#ifndef PREFERENCES_TRANSLATOR_H_
#define PREFERENCES_TRANSLATOR_H_

#include "quafe-config.h"
#include "preferences.h"
#include "pluginmanager.h"

#include <boost/bind.hpp>
#include <eapi/sheets/keyinfo.h>

namespace Quafe {
// ******************************************************************
// generic from
template <>
void Preferences::translate<po::option>::from(const pugi::xml_node &node, po::option &option) {
	option.string_key = node.name();
	if(!node.attribute("value").empty()) {
		option.value.push_back(node.attribute("value").value());
	} else {
		option.value.push_back(node.child_value());
	}
}

// ******************************************************************
// translate bool
template <>
void Preferences::translate<bool>::to(pugi::xml_node &node,  const po::variable_value &value) {
	node.append_attribute("value") = value.as<bool>();
}

// ******************************************************************
// translate bool
template <>
void Preferences::translate<int>::to(pugi::xml_node &node,  const po::variable_value &value) {
	node.append_attribute("value") = value.as<int>();
}

// ******************************************************************
// translate ustring
template <>
void Preferences::translate<Glib::ustring>::to(pugi::xml_node &node,  const po::variable_value &value) {
	node.append_child(pugi::node_pcdata).set_value(value.as<Glib::ustring>().c_str());
}

// ******************************************************************
// translate PluginInfoList
template <>
void Preferences::translate<PluginInfoList>::to(pugi::xml_node &node,  const PluginInfoList &value) {
	PluginInfoList::const_iterator it = value.begin();
	for (; it != value.end(); ++it) {
		PluginInfo info = *it;
		if(!info.validate())
			continue;

		pugi::xml_node pl = node.append_child("plugin");
		pl.append_child(pugi::node_pcdata).set_value(info.file.c_str());
		pl.append_attribute("active") = info.active;
		pl.append_attribute("id") = info.id.c_str();
		pl.append_attribute("title") = info.title.c_str();
	}
}

template <>
void Preferences::translate<PluginInfoList>::from(const pugi::xml_node &plugins_node, PluginInfoList &pl_list) {
	pugi::xml_node_iterator node = plugins_node.begin();
	for (; node != plugins_node.end(); ++node) {
		//[ fill the plugin list with the configuration
		PluginInfo info;
		info.file = node->child_value();
		info.active = node->attribute("active").as_bool();
		info.id = node->attribute("id").value();
		info.title = node->attribute("title").value();
		//]
	}
}

// ******************************************************************
// translate KeyInfo
template <>
void Preferences::translate<EAPI::KeyInfo>::to(pugi::xml_node &node,  const EAPI::KeyInfo *&key) {
	pugi::xml_node acc = node.append_child("account");

	Glib::ustring keyID = key->value<Glib::ustring>("keyID");
	acc.append_child("keyID").append_child(pugi::node_pcdata).set_value(keyID.c_str());

	Glib::ustring vCode = key->value<Glib::ustring>("vCode");
	acc.append_child("vCode").append_child(pugi::node_pcdata).set_value(vCode.c_str());

	pugi::xml_node acc_characters = acc.append_child("characters");

	EAPI::KeyInfo::CharacterList char_list = key->get_character_list();
	for (EAPI::KeyInfo::CharacterList::iterator it = char_list.begin(); it != char_list.end(); it++) {
		pugi::xml_node character_node = acc_characters.append_child("character");

		EAPI::KeyInfo::Character character = (*it);
		character_node.append_child(pugi::node_pcdata).set_value(character.name.c_str());
		character_node.append_attribute("active") = character.active;
		character_node.append_attribute("id") = character.id;
	}
}

template <>
void Preferences::translate<EAPI::KeyInfo>::from(const pugi::xml_node &node, EAPI::KeyInfo &not_needed) {
	EAPI::KeyInfo *key = 0;
	try {
		int keyID = boost::lexical_cast<int>(node.child("keyID").child_value());
		Glib::ustring vCode = node.child("vCode").child_value();
		key = EAPI::KeyInfo::create(keyID, vCode);
	} catch(...) {
		//LOG_WARNING("Malformed account keyID found in xml_data");
	}

	// now KeyInfo should have parsed the cache
	// activating wanted characters
	for(pugi::xml_node_iterator it = node.child("characters").begin(); it != node.child("characters").end(); ++it) {
		pugi::xml_node char_node = (*it);
		if(!key->set_character_active(char_node.attribute("id").as_int(), char_node.attribute("active").as_bool())) {
			//LOG_WARNING("Unknown character in xml list: " << char_node.child_value());
		}
	}
	EAPI::KeyInfo::CharacterList clist = key->get_character_list();
	for(EAPI::KeyInfo::CharacterList::const_iterator it = clist.begin(); it != clist.end(); ++it) {

	}

}

}
#endif /* PREFERENCES_TRANSLATOR_H_ */
