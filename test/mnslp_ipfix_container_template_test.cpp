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


class Mnslp_Container_Template_Test;


/*
 * We use a subclass for testing and make the test case a friend. This
 * way the test cases have access to protected methods and they don't have
 * to be public in mnslp_ipfix_fields.
 */
class mnslp_container_template_test : public mnslp_template_container
{
  public:

	mnslp_container_template_test()
		: mnslp_template_container() { }

	friend class Mnslp_Container_Template_Test;
};


class Mnslp_Container_Template_Test : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( Mnslp_Container_Template_Test );

	CPPUNIT_TEST( testAssign );

	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp();
	void tearDown();

	void testAssign();

  private:
    
      
    msnlp_ipfix_field_container field_container;   
    mnslp_container_template_test *container1;
    mnslp_container_template_test *container2;
    
    
};

CPPUNIT_TEST_SUITE_REGISTRATION( Mnslp_Container_Template_Test );


void Mnslp_Container_Template_Test::setUp() 
{
		
	container1 = new mnslp_container_template_test();
	container2 = new mnslp_container_template_test();
    msnlp_ipfix_field_container field_container = msnlp_ipfix_field_container();
	
}

void Mnslp_Container_Template_Test::tearDown() 
{
	delete container1;
	delete container2;
}

void Mnslp_Container_Template_Test::testAssign()
{

	mnslp_ipfix_template *template1;
	mnslp_ipfix_template *template2;
	mnslp_ipfix_template *template3;
	
	template1 = new mnslp_ipfix_template();
	template2 = new mnslp_ipfix_template();
	template3 = new mnslp_ipfix_template();
	
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

	container1->add_template(template1);
	container1->add_template(template2);
	container1->add_template(template3);
	
	CPPUNIT_ASSERT(container1->get_num_templates() == 3 );
	
	container1->delete_template(template3->get_template_id());
	
	CPPUNIT_ASSERT(container1->get_num_templates() == 2 );
	
	container1->delete_template(template2->get_template_id());
	
	CPPUNIT_ASSERT(container1->get_num_templates() == 1 );
	
	mnslp_template_container container3 = *container1;
	 
	CPPUNIT_ASSERT(*container1 == container3 );
	
	CPPUNIT_ASSERT(container3.get_num_templates() == 1 );

}
// EOF
