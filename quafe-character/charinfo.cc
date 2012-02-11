/*
 * character.cc
 *
 *  Created on: 07.02.2012
 *      Author: cn
 */

#include "charinfo.h"

namespace Quafe {
namespace Plugin {

extern "C" PluginCharacterInfo* create() {
    return new PluginCharacterInfo;
}

extern "C" void destroy(PluginCharacterInfo* p) {
    delete p;
}

PluginCharacterInfo::PluginCharacterInfo() {
	// TODO Auto-generated constructor stub

}

PluginCharacterInfo::~PluginCharacterInfo() {
	// TODO Auto-generated destructor stub
}

void PluginCharacterInfo::show(Gtk::Widget &parent) {

}
}
}
