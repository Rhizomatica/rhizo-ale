AUTOMAKE_OPTIONS = foreign dist-bzip2 1.6

AM_CPPFLAGS = $(all_includes) -I$(top_srcdir)/include
SUBDIRS = src # tests contrib

EXTRA_DIST = doc/examples/rhizo-ale.cfg

AM_DISTCHECK_CONFIGURE_FLAGS = \
	--with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir)

