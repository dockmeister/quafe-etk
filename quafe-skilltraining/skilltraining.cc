/*
 * character.cc
 *
 *  Created on: 07.02.2012
 *      Author: cn
 */

#include "skilltraining.h"

namespace Quafe {
namespace Plugin {

extern "C" SkillTraining* create() {
    return new SkillTraining;
}

extern "C" void destroy(SkillTraining* p) {
    delete p;
}

SkillTraining::SkillTraining() {
	// TODO Auto-generated constructor stub

}

SkillTraining::~SkillTraining() {
	// TODO Auto-generated destructor stub
}

void SkillTraining::show(Gtk::Widget &parent) {

}
}
}
