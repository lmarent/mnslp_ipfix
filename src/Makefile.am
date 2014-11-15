AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = msnlp_ipfix
msnlp_ipfix_SOURCES = main.c mnslp_libipfix.cpp

msnlp_ipfix_LDADD = @IPFIX_LIBS@

# what flags you want to pass to the C compiler & linker
AM_CPPFLAGS = -g -I@top_srcdir@/include @IPFIX_CPPFLAGS@
AM_LDFLAGS = @IPFIX_LDFLAGS@
LDADD = 
