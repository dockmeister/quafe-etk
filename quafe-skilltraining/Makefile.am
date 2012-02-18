noinst_LTLIBRARIES = libquafe-skilltraining.la
libquafe_skilltraining_la_CPPFLAGS = -I ../quafe-bin $(gtkmm_CFLAGS)
libquafe_skilltraining_la_LDFLAGS = -rpath $(pkgdatadir)/plugins -module -avoid-version 
libquafe_skilltraining_la_SOURCES = skilltraining.cc

noinst_HEADERS = skilltraining.h

install-exec-local:
	install -d $(pkgdatadir)/plugins
	$(LIBTOOL) --mode=install install libquafe-skilltraining.la $(pkgdatadir)/plugins

install-exec-hook: 
	find $(pkgdatadir)/plugins -type f -name \*.la -delete
