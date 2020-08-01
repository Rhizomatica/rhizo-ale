AM_CPPFLAGS = $(all_includes) -I$(top_srcdir)/src
AM_CFLAGS=-Wall -g $(LIBOSMOCORE_CFLAGS) $(LIBOSMOVTY_CFLAGS) \
		   $(LIBOSMONETIF_CFLAGS) \
		   $(COVERAGE_CFLAGS)
AM_LDFLAGS=$(COVERAGE_LDFLAGS)

EXTRA_DIST = internal.h

bin_PROGRAMS = rhizo-ale

rhizo_ale_SOURCES = ale_main.c ale_shm.c ale_vty.c ale_fsm.c ale_buf.c
rhizo_ale_LDADD = $(LIBOSMOCORE_LIBS) $(LIBOSMOVTY_LIBS) \
		 $(LIBOSMONETIF_LIBS)
