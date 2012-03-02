/*
 * preferences_translator.h
 *
 *  Created on: 29.02.2012
 *      Author: cn
 */

#ifndef PREFERENCES_TRANSLATOR_H_
#define PREFERENCES_TRANSLATOR_H_

#include "preferences.h"
#include "utility.h"
#include "pluginmanager.h"

#include <boost/bind.hpp>
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
// translate ustring
template <>
void Preferences::translate<ustring>::to(pugi::xml_node &node,  const po::variable_value &value) {
	node.append_child(pugi::node_pcdata).set_value(value.as<ustring>().c_str());
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
// translate AccountInfoList
template <>
void Preferences::translate<AccountInfoList>::to(pugi::xml_node &node,  const AccountInfoList &value) {
	AccountInfoList::const_iterator ac_it = value.begin();
	for (; ac_it != value.end(); ++ac_it) {
		pugi::xml_node acc = node.append_child("account");

		AccountInfo ac_info = (*ac_it);
		acc.append_attribute("active") = ac_info.active;
		acc.append_child("authid").append_child(pugi::node_pcdata).set_value(ac_info.authid.c_str());
		acc.append_child("authkey").append_child(pugi::node_pcdata).set_value(ac_info.authkey.c_str());

		for (gint i = 0; i < 3; i++) {
			pugi::xml_node chara = acc.append_child("character");
			chara.append_child(pugi::node_pcdata).set_value((ac_info.characters[i].second).c_str());
			chara.append_attribute("active") = ac_info.characters[i].first;
		}
	}
}

template <>
void Preferences::translate<AccountInfoList>::from(const pugi::xml_node &node, AccountInfoList &acc_list) {

}

}
#endif /* PREFERENCES_TRANSLATOR_H_ */
