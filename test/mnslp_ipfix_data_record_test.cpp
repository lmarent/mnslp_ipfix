/*
 * Test the mnslp_ipfix_data_record_test class.
 *
 * $Id: mnslp_ipfix_data_record_test.cpp 2014-11-29 10:36:00 amarentes $
 * $HeadURL: https://./test/mnslp_ipfix_data_record_test.cpp $
 */
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ipfix_def.h"
#include "mnslp_ipfix_fields.h"
#include "mnslp_ipfix_data_record.h"
#include <iostream>

using namespace mnslp_ipfix;


class Mnslp_Ipfix_Data_Record_Test;


/*
 * We use a subclass for testing and make the test case a friend. This
 * way the test cases have access to protected methods and they don't have
 * to be public in mnslp_ipfix_fields.
 */
class mnslp_ipfix_data_record_test : public mnslp_ipfix_data_record 
{
  public:

	mnslp_ipfix_data_record_test()
		: mnslp_ipfix_data_record() { }

	friend class Mnslp_Ipfix_Data_Record_Test;
};


class Mnslp_Ipfix_Data_Record_Test : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( Mnslp_Ipfix_Data_Record_Test );

	CPPUNIT_TEST( testAssign );

	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp();
	void tearDown();

	void testAssign();

  private:
    
    mnslp_ipfix_field *ptrField1;
    mnslp_ipfix_field *ptrField2;
    mnslp_ipfix_field *ptrField3;
    
    msnlp_ipfix_field_container field_container;
    mnslp_ipfix_data_record data;

};

CPPUNIT_TEST_SUITE_REGISTRATION( Mnslp_Ipfix_Data_Record_Test );


void Mnslp_Ipfix_Data_Record_Test::setUp() 
{
		
	field_container.initialize_forward();
	
}

void Mnslp_Ipfix_Data_Record_Test::tearDown() 
{
	
}

void Mnslp_Ipfix_Data_Record_Test::testAssign()
{

	uint8_t value8 = 1;
	uint32_t value32 = 3;	
	uint8_t valuebyte0[5] = "1234";
	int num_fields = 0;
	int num_field_length = 0;

	// Int 1
	ptrField1 = field_container.get_field( 0, IPFIX_FT_IGMPTYPE );
	mnslp_ipfix_value_field fvalue1 = ptrField1->get_ipfix_value_field(value8);
	data.insert_field(0, IPFIX_FT_IGMPTYPE, fvalue1);
	data.insert_field_length(0, IPFIX_FT_IGMPTYPE, 1);
	
	// Int 4
	ptrField2 = field_container.get_field( 0, IPFIX_FT_INGRESSINTERFACE );
	mnslp_ipfix_value_field fvalue2 = ptrField2->get_ipfix_value_field(value32);
	data.insert_field(0, IPFIX_FT_INGRESSINTERFACE, fvalue2);
	data.insert_field_length(0, IPFIX_FT_INGRESSINTERFACE, 4);

    //Address 4
	ptrField3 = field_container.get_field( 0, IPFIX_FT_SOURCEIPV4ADDRESS );
	mnslp_ipfix_value_field fvalue3 = ptrField3->get_ipfix_value_field((uint8_t *) valuebyte0, 4);
	data.insert_field(0, IPFIX_FT_SOURCEIPV4ADDRESS, fvalue3);
	data.insert_field_length(0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);

	num_fields = data.get_num_fields();
	CPPUNIT_ASSERT( num_fields == 3 );
	
	mnslp_ipfix_value_field fieltmp = data.get_field(0, IPFIX_FT_INGRESSINTERFACE);
	CPPUNIT_ASSERT( fieltmp.get_value_int32() == value32 );
	
	// Verifies the length insert.
	num_field_length = data.get_num_field_length();
	CPPUNIT_ASSERT( num_fields == 3 );
}
// EOF
