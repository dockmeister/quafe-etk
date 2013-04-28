/*
 * pluginbase.cc
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#include "pluginbase.h"

#include <settings.h>
#include <utils/exception.h>

#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>

namespace Quafe {

PluginBase::PluginBase() {
}

PluginBase::PluginBase(Glib::RefPtr<Gtk::Builder> &refGlade)
	: m_refGlade(refGlade) {
}

PluginBase::~PluginBase() {
}

void PluginBase::init_glade(const Glib::ustring glade_filename) {
	Glib::ustring glade_file = Settings().get_glade_directory(glade_filename);

	if (!Glib::file_test(glade_file, Glib::FILE_TEST_EXISTS)) {
		throw Exception("Could not locate Gladefile: " + glade_file);
	}

	try {
		m_refGlade = Gtk::Builder::create_from_file(glade_file);
	} catch(Glib::Exception &e) {
		throw Exception("Could not load Gladefile: " + glade_file);
	}
}

// ******************************************************************
// PluginBase::ConnectionHandler
PluginBase::ConnectionHandler::~ConnectionHandler() {
	disconnect_all();
}

void PluginBase::ConnectionHandler::operator +=(boost::signals2::connection conn) {
	m_connections.push_back(conn);
}

void PluginBase::ConnectionHandler::block_all() {
	std::for_each(m_connections.begin(), m_connections.end(), boost::bind(&PluginBase::ConnectionHandler::block, this, _1));
}

void PluginBase::ConnectionHandler::block(boost::signals2::connection &conn) {
	boost::signals2::shared_connection_block block(conn);
	m_blocks.push_back(block);
}

void PluginBase::ConnectionHandler::unblock_all() {
	std::for_each(m_blocks.begin(), m_blocks.end(), boost::bind(&boost::signals2::shared_connection_block::unblock, _1));
	m_blocks.clear();
}

void PluginBase::ConnectionHandler::disconnect_all() {
	std::for_each(m_connections.begin(), m_connections.end(), boost::bind(&PluginBase::ConnectionHandler::disconnect, this, _1));
	m_connections.clear();
}

void PluginBase::ConnectionHandler::disconnect(boost::signals2::connection &conn) {
	conn.disconnect();
	m_connections.remove(conn);
}
} /* namespace Quafe */
