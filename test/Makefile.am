AUTOMAKE_OPTIONS = foreign

# Rules for the test code (use `make check` to execute)
TESTS = test_runner
check_PROGRAMS = $(TESTS)


test_runner_SOURCES = @top_srcdir@/src/mnslp_ipfix_field.cpp \
					  @top_srcdir@/src/mnslp_ipfix_template.cpp \
					  @top_srcdir@/src/mnslp_ipfix_data_record.cpp \
					  @top_srcdir@/src/mnslp_ipfix_message.cpp \
					  @top_srcdir@/src/ipfix_t.cpp \
					  @top_srcdir@/test/mnslp_ipfix_field_test.cpp \
					  @top_srcdir@/test/mnslp_ipfix_data_record_test.cpp \
					  @top_srcdir@/test/mnslp_ipfix_template_test.cpp \
					  @top_srcdir@/test/mnslp_ipfix_container_template_test.cpp \
					  @top_srcdir@/test/ipfix_t_test.cpp \
					  @top_srcdir@/test/mnslp_ipfix_message_test.cpp \
					  @top_srcdir@/test/test_runner.cpp

# what flags you want to pass to the C compiler & linker
AM_CPPFLAGS = -g -I@top_srcdir@/include @IPFIX_CPPFLAGS@ $(CPPUNIT_CFLAGS)
AM_LDFLAGS = @IPFIX_LDFLAGS@ $(CPPUNIT_LIBS) -ldl
LDADD = 
