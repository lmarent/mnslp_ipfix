/*
 * Test the mnslp_fields_test class.
 *
 * $Id: mnslp_fields.cpp 2014-11-28 10:16:00 amarentes $
 * $HeadURL: https://./test/mnslp_fields_test.cpp $
 */
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ipfix_def.h"
#include "mnslp_ipfix_message.h"

using namespace mnslp_ipfix;


class Mnslp_Ipfix_Message_Test : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( Mnslp_Ipfix_Message_Test );
	// CPPUNIT_TEST( testAddTemplate );
	CPPUNIT_TEST( testDeleteTemplate );
	//CPPUNIT_TEST_EXCEPTION( testException, mnslp_ipfix_bad_argument );

	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp();
	void tearDown();
	void testAddTemplate();
	void testDeleteTemplate();

  private:
    
    mnslp_ipfix_message * mes;


};

void Mnslp_Ipfix_Message_Test::setUp() {
	
	int sourceid = 12345;
	mes = new mnslp_ipfix_message(sourceid, IPFIX_VERSION);
}

void Mnslp_Ipfix_Message_Test::testAddTemplate()
{
	mnslp_ipfix_template *templdata=NULL;
	mnslp_ipfix_template *templscope=NULL;
	mnslp_ipfix_template *templscope2=NULL;
	int nfields = 3;
	int result;
	data_record data;
	char   buf[31]  = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                           11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                           21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };

	// Verifies that a data field cannot be added to a empty template
	result = mes->add_field(templdata, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	CPPUNIT_ASSERT( result == -1 );
	
	// Verifies the field add method.
	mes->new_data_template( &templdata, nfields );
	result = mes->add_field(templdata, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	CPPUNIT_ASSERT( result == 0 );

	// Verifies that a scope field cannot be added to a empty template
	result = mes->add_scope_field(templscope, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	CPPUNIT_ASSERT( result == -1 );

	// Verifies that a scope field cannot be added to a data template
	result = mes->add_scope_field(templdata, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	CPPUNIT_ASSERT( result == -1 );

	// Verifies the scope field add method.
	mes->new_option_template( &templscope, nfields );
	result = mes->add_scope_field(templscope, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	CPPUNIT_ASSERT( result == 0 );
	
	// Verifies the method delete template, first verifies with null data
	mes->delete_template(templscope2);

	// Verifies the method delete template, second it verifies with a valid template
	mes->delete_template(templscope);
	
	data.insert_field(0, IPFIX_FT_SOURCEIPV4ADDRESS, (void *) buf);
	data.insert_field_lenght(0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	
	mes->output(templdata, &data);
	std::cout << "output" << result << std::endl;
	
}

void Mnslp_Ipfix_Message_Test::testDeleteTemplate()
{

}

void Mnslp_Ipfix_Message_Test::tearDown() 
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( Mnslp_Ipfix_Message_Test );
