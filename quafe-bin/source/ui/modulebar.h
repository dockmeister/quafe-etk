/*
 * gnome-baseball - a digital scoresheet
 *
 * Copyright (C) 2010 Christoph Nikic <c.nikic@gmx.de>
 * All rights reserved.
 *
 * Use and distribution licensed under the simplified BSD license.
 * See the COPYING file in this directory for full text.
 *
 */

#ifndef MODULEBAR_H
#define	MODULEBAR_H

#include <quafe-etk.h>

#include <gdkmm/color.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>

namespace Quafe {

typedef std::list<Gtk::ToggleButton *> ButtonList;

/** Widget ModuleBar
 *
 * @param mod
 */
class ModuleBar : public Gtk::Frame {
public:
    /**
     *
     * @param cobject
     * @param ref_builder
     */
    ModuleBar();
    
    /** Adds a module to the list, creates a button and a label.
     *
     * @param mod Glib::RefPtr to a module
     */
    virtual void add_module(Gtk::ToggleButton* b, Gtk::Label* l);

    /**
     *
     */
    virtual void change_style();

    virtual ~ModuleBar()
    {
        
    }

protected:
    // default con- / destructors
   
    
private:
   RefPtr<Gtk::CssProvider> m_refCssProvider;

    /** methods to darken background color
     *
     * @see planner v.14
     */
    static void hls_to_rgb(gdouble &h, gdouble &l, gdouble &s);
    static void rgb_to_hls(gdouble &r, gdouble &g, gdouble &b);
    static void style_shade(Gdk::RGBA &a, Gdk::RGBA &b, gdouble k);
    static void darken_color(Gdk::RGBA &color, gint darken_count);

    ButtonList button_list;

    Gdk::RGBA c_normal, c_prelight;
    Gtk::EventBox *m_eventbox;
    Gtk::VBox *m_vbox;
};

}
#endif	/* MODULEBAR_H */

