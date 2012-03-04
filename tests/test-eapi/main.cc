#include <iostream>
#include <glibmm.h>
#include <eapi/eapi.h>
#include <eapi/keyinfo.h>

#include "sens_data.h"

int main(int, char**) {
	EAPI::Main::init("data/");

	EAPI::KeyInfo *m_key = EAPI::KeyInfo::create(keyID, vCode);
}

