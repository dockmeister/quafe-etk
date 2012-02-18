AC_PREREQ([2.59])
AC_INIT([quafe-etk], [0.1+devel], [https://github.com/dockmeister/quafe-etk])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_SRCDIR(quafe-bin/main.cc)
AC_CONFIG_HEADERS(config.h)

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET

LT_INIT([disable-static])

AC_CONFIG_MACRO_DIR([m4])

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test x$PKG_CONFIG = xno ; then
  AC_MSG_ERROR([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/])
fi

AC_HEADER_STDC
AC_CHECK_LIB(stdc++, main, , AC_MSG_ERROR(quafe-etk requires libstdc++))

PKG_CHECK_MODULES([gtkmm], [gtkmm-3.0 >= 3.2.0])
AC_SUBST(gtkmm_CFLAGS)
AC_SUBST(gtkmm_LIBS)

PKG_CHECK_MODULES([glibmm], [glibmm-2.4 >= 2.30.0])
AC_SUBST(glibmm_CFLAGS)
AC_SUBST(glibmm_LIBS)

#AC_SUBST([plugindir], ['${sharedir}/quafe-etk/plugins'])

#BOOST_REQUIRE(1.42.0)
AC_CONFIG_FILES([
	Makefile 
	quafe-bin/Makefile
	quafe-skilltraining/Makefile
	data/Makefile
])
AC_OUTPUT
