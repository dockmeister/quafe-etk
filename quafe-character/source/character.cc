/*
 * character.cc
 *
 *  Created on: 07.02.2012
 *      Author: cn
 */

#include "character.h"

namespace Quafe {
namespace Plugin {

extern "C" PluginCharacter* create() {
    return new PluginCharacter;
}

extern "C" void destroy(PluginCharacter* p) {
    delete p;
}

PluginCharacter::PluginCharacter() {
	// TODO Auto-generated constructor stub

}

PluginCharacter::~PluginCharacter() {
	// TODO Auto-generated destructor stub
}

void PluginCharacter::show(Gtk::Widget &parent) {

}
}
}
