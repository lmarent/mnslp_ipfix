/*
 * Test the mnslp_ipfix_template_test class.
 *
 * $Id: mnslp_ipfix_template_test.cpp 2015-01-02 09:31:09 amarentes $
 * $HeadURL: https://./test/mnslp_ipfix_template_test.cpp $
 */
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ipfix_def.h"
#include "mnslp_ipfix_field.h"
#include "mnslp_ipfix_template.h"
#include <iostream>

using namespace mnslp_ipfix;


class Mnslp_Ipfix_Templates_Test;


/*
 * We use a subclass for testing and make the test case a friend. This
 * way the test cases have access to protected methods and they don't have
 * to be public in mnslp_ipfix_fields.
 */
class msnlp_ipfix_templates_test : public mnslp_ipfix_template
{
  public:

	msnlp_ipfix_templates_test()
		: mnslp_ipfix_template() { }

	friend class Mnslp_Ipfix_Templates_Test;
};


class Mnslp_Ipfix_Templates_Test : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( Mnslp_Ipfix_Templates_Test );

	CPPUNIT_TEST( testAssign );

	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp();
	void tearDown();

	void testAssign();

  private:
    
    msnlp_ipfix_templates_test *template1;
    msnlp_ipfix_templates_test *template2;
    msnlp_ipfix_templates_test *template3;
    
    ipfix_field_type_t field1;
    ipfix_field_type_t field2;
    ipfix_field_type_t field3;  
      
    msnlp_ipfix_field_container field_container;   
    
};

CPPUNIT_TEST_SUITE_REGISTRATION( Mnslp_Ipfix_Templates_Test );


void Mnslp_Ipfix_Templates_Test::setUp() 
{
		
	template1 = new msnlp_ipfix_templates_test();
	template2 = new msnlp_ipfix_templates_test();
	template3 = new msnlp_ipfix_templates_test();
		
    msnlp_ipfix_field_container field_container = msnlp_ipfix_field_container();
	
}

void Mnslp_Ipfix_Templates_Test::tearDown() 
{
	delete template1;
	delete template2;
	delete template3;
}

void Mnslp_Ipfix_Templates_Test::testAssign()
{

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

	CPPUNIT_ASSERT( template1->get_template_id() == 1 );
	CPPUNIT_ASSERT( template1->get_type() == DATA_TEMPLATE );
	CPPUNIT_ASSERT( template1->get_number_datafields() == 3 );
	CPPUNIT_ASSERT( template1->get_number_scopefields() == 0 );
	CPPUNIT_ASSERT( template1->get_numfields() == 3 );
	CPPUNIT_ASSERT( template1->get_maxfields() == 3 );


	template2->set_id(1);
	template2->set_type(DATA_TEMPLATE);
	template2->set_maxfields(3);
	template2->add_field((field1.get_field_type()).length, KNOWN, 1, field1);
	template2->add_field((field2.get_field_type()).length, KNOWN, 1, field2);
	template2->add_field((field3.get_field_type()).length, KNOWN, 1, field3);
	
	CPPUNIT_ASSERT(*template1 == *template2);

	template3->set_id(1);
	template3->set_type(DATA_TEMPLATE);
	template3->set_maxfields(3);
	template3->add_field((field1.get_field_type()).length, KNOWN, 1, field1);
	template3->add_field((field2.get_field_type()).length, KNOWN, 1, field2);
	template3->add_field((field4.get_field_type()).length, KNOWN, 1, field4);

	CPPUNIT_ASSERT(*template1 != *template3);
	
	
	mnslp_ipfix_template template4 = *template3;
	
	CPPUNIT_ASSERT(template4 == *template3);
	
	mnslp_ipfix_template *template5 = template3->copy();
	
	CPPUNIT_ASSERT(template4 == *template5);
	
}
// EOF
