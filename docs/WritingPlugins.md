Writing Plugins for Quafe 					{#writingplugins}
==============================

# Overview

- @ref gettingstarted
- @ref firstcompile
- @ref useeapi
- @ref quafebuild

# Getting started							{#gettingstarted}

#### Code: MySamplePlugin.h
~~~~~~~~~~~~~~~{.cpp}
#include <plugins/plugininterface.h>

class MySamplePlugin : public Quafe::PluginInterface {
public: 
	MySamplePlugin();
	virtual ~MySamplePlugin();

	# implement pure virtual methods
	Gtk::Widget& show();
	bool close();
}
~~~~~~~~~~~~~~~

#### Code: MySamplePlugin.cpp
~~~~~~~~~~~~~~~{.cpp}
#include "MySamplePlugin.h"

// Define plugin paramaters so quafe knows how to handle it
PluginParams params = {
		"mysampleplugin", // a unique plugin id 
		"Sample Plugin", // a short description
		"Does nothing so far!", // 
		"mysampleplugin.png", // the icon of this plugin
		EAPI::CAK_CHARACTERSHEET | 	// tell quafe which apis this plugin
		EAPI::CAK_SKILLQUEUE		// Its not guaranteed, that these APIs are available since it depends on the API access mask.
	};

extern "C" const PluginParams & get_params() {
	return params;
}

extern "C" MySamplePlugin* create() {
    return new MySamplePlugin(params);
}

extern "C" void destroy(MySamplePlugin* p) {
    delete p;
}

MySamplePlugin::MySamplePlugin(const PluginParams &params)
	: PluginInterface(params) {

	m_widget = Gtk::manage(new Gtk::Label("Sample plugin content"));
}

MySamplePlugin::~MySamplePlugin() {
	// so far nothing to clean up.
	// gtk manages widgets
}


~~~~~~~~~~~~~~~

# Compiling your first plugin				{#firstcompile}

# Using EAPI to get API data				{#useeapi}

# Usage of the Quafe build tree				{#quafebuild}

