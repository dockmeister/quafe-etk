/*
 * pluginbase.h
 *
 *  Created on: 17.02.2013
 *      Author: cn
 */

#ifndef PLUGINBASE_H_
#define PLUGINBASE_H_

#include <boost/signals2.hpp>
#include <gtkmm/builder.h>

namespace Quafe {

class PluginBase {
public:
	PluginBase();
	PluginBase(Glib::RefPtr<Gtk::Builder> &refGlade);
	virtual ~PluginBase();


protected:
	//!< Utility struct to handle multiple signals2::connection
	struct ConnectionHandler {
	public:
		~ConnectionHandler();
		//!< adds a connection
		void operator +=(boost::signals2::connection &connection);

		//!< blocks all connections. use this if the plugin is not active
		void block_all();

		//!< blocks a single connection
		void block(boost::signals2::connection &connection);

		//!< unblocks all connections. use this if the plugin becomes active
		void unblock_all();

		//!< disconnects all connections. disconnected connections are not recoverable
		void disconnect_all();

		//!< disconnects a single connection
		void disconnect(boost::signals2::connection &connection);

	private:
		std::list<boost::signals2::connection> m_connections;
		std::list<boost::signals2::shared_connection_block> m_blocks;
	} m_sig_connections;

	void init_glade(const Glib::ustring glade_filename);

	Glib::RefPtr<Gtk::Builder> m_refGlade;

private:

};

} /* namespace Quafe */
#endif /* PLUGINBASE_H_ */
