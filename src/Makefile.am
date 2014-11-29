AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = mnslp_ipfix

mnslp_ipfix_SOURCES = mnslp_ipfix_field.cpp \
					  mnslp_ipfix_templates.cpp \
					  mnslp_ipfix_message.cpp \
					  main.cpp
					  
#mlog.cpp
		   

mnslp_ipfix_LDADD = @IPFIX_LIBS@

# what flags you want to pass to the C compiler & linker
AM_CPPFLAGS = -g -I@top_srcdir@/include @IPFIX_CPPFLAGS@
AM_LDFLAGS = @IPFIX_LDFLAGS@
LDADD = 
