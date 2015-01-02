/*
 * Test the mnslp_ipfix_data_record_test class.
 *
 * $Id: mnslp_ipfix_data_record_test.cpp 2014-11-29 10:36:00 amarentes $
 * $HeadURL: https://./test/mnslp_ipfix_data_record_test.cpp $
 */
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ipfix_t.h"
#include <iostream>

using namespace mnslp_ipfix;


class Ipfix_T_Test;


/*
 * We use a subclass for testing and make the test case a friend. This
 * way the test cases have access to protected methods and they don't have
 * to be public in mnslp_ipfix_fields.
 */
class ipfix_t_test : public ipfix_t
{
  public:

	ipfix_t_test()
		: ipfix_t() { }

	friend class Ipfix_T_Test;
};


class Ipfix_T_Test : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( Ipfix_T_Test );

	CPPUNIT_TEST( testAssign );

	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp();
	void tearDown();

	void testAssign();

  private:
    
    msnlp_ipfix_field_container field_container; 
    ipfix_t *ptrIpfix_t1;
    ipfix_t *ptrIpfix_t2;
    ipfix_t *ptrIpfix_t3;
    
};

CPPUNIT_TEST_SUITE_REGISTRATION( Ipfix_T_Test );


void Ipfix_T_Test::setUp() 
{
		
	ptrIpfix_t1 = new ipfix_t();

	mnslp_ipfix_template *template1;
	mnslp_ipfix_template *template2;
	mnslp_ipfix_template *template3;
	
	template1 = new mnslp_ipfix_template();
	template2 = new mnslp_ipfix_template();
	template3 = new mnslp_ipfix_template();
	
	msnlp_ipfix_field_container field_container = msnlp_ipfix_field_container();
	field_container.initialize_forward();
	mnslp_ipfix_field field1 = field_container.get_field( 0, IPFIX_FT_IPCLASSOFSERVICE );
	mnslp_ipfix_field field2 = field_container.get_field( 0, IPFIX_FT_PACKETDELTACOUNT );
	mnslp_ipfix_field field3 = field_container.get_field( 0, IPFIX_FT_SAMPLING_INTERVAL );
	mnslp_ipfix_field field4 = field_container.get_field( 0, IPFIX_FT_MPLSLABELSTACKSECTION5 );

	template1->set_id(1);
	template1->set_type(DATA_TEMPLATE);
	template1->set_maxfields(3);
	template1->add_field((field1.get_field_type()).length, KNOWN, 1, field1);
	template1->add_field((field2.get_field_type()).length, KNOWN, 1, field2);
	template1->add_field((field3.get_field_type()).length, KNOWN, 1, field3);

	template2->set_id(2);
	template2->set_type(OPTION_TEMPLATE);
	template2->set_maxfields(3);
	template2->add_field((field1.get_field_type()).length, KNOWN, 1, field1);
	template2->add_field((field2.get_field_type()).length, KNOWN, 1, field2);
	template2->add_field((field3.get_field_type()).length, KNOWN, 1, field3);
		
	template3->set_id(3);
	template3->set_type(DATA_TEMPLATE);
	template3->set_maxfields(3);
	template3->add_field((field1.get_field_type()).length, KNOWN, 1, field1);
	template3->add_field((field2.get_field_type()).length, KNOWN, 1, field2);
	template3->add_field((field4.get_field_type()).length, KNOWN, 1, field4);

	(ptrIpfix_t1->templates).add_template(template1);
	(ptrIpfix_t1->templates).add_template(template2);
	(ptrIpfix_t1->templates).add_template(template3);
	
	
}

void Ipfix_T_Test::tearDown() 
{
	delete ptrIpfix_t1;
	delete ptrIpfix_t2;
}

void Ipfix_T_Test::testAssign()
{

	CPPUNIT_ASSERT( ptrIpfix_t1->templates.get_num_templates() == 3 );
	
	
	ptrIpfix_t1->buffer[0] =  'a';
	ptrIpfix_t1->buffer[1] =  'b';
	ptrIpfix_t1->buffer[2] =  'c';
	ptrIpfix_t1->buffer[3] =  'd';
	ptrIpfix_t1->buffer[4] =  'e';
	ptrIpfix_t1->buffer[5] =  'f';
	ptrIpfix_t1->buffer[6] =  '\0';
	ptrIpfix_t1->offset = 7;
	ptrIpfix_t1->nrecords = 0;
	ptrIpfix_t1->seqno = 0;
	ptrIpfix_t1->cs_tid = 1;
	ptrIpfix_t1->cs_bytes = 3;
	ptrIpfix_t1->cs_offset= 4;
	ptrIpfix_t1->cs_header = (uint8_t*) ptrIpfix_t1->buffer + ptrIpfix_t1->cs_offset;
	
	ptrIpfix_t2 = new ipfix_t();
	*ptrIpfix_t2 = *ptrIpfix_t1;
	
	CPPUNIT_ASSERT( *ptrIpfix_t2  == *ptrIpfix_t1 );
	CPPUNIT_ASSERT( *(ptrIpfix_t2->cs_header)  == 'e' );
	
	
}
// EOF
