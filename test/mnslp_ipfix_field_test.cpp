/*
 * Test the mnslp_fields_test class.
 *
 * $Id: mnslp_fields.cpp 2014-11-28 10:16:00 amarentes $
 * $HeadURL: https://./test/mnslp_fields_test.cpp $
 */
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ipfix_def.h"
#include "mnslp_ipfix_fields.h"


using namespace mnslp_ipfix;


class Mnslp_Ipfix_Field_Test;


/*
 * We use a subclass for testing and make the test case a friend. This
 * way the test cases have access to protected methods and they don't have
 * to be public in mnslp_ipfix_fields.
 */
class mnslp_ipfix_field_test : public mnslp_ipfix_field {
  public:
	mnslp_ipfix_field_test(ipfix_field_type_t param)
		: mnslp_ipfix_field(param) { }

	friend class Mnslp_Ipfix_Field_Test;
};


class Mnslp_Ipfix_Field_Test : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( Mnslp_Ipfix_Field_Test );

	CPPUNIT_TEST( testGetters );
	CPPUNIT_TEST( testAssign );
	CPPUNIT_TEST( testAddFields );
	CPPUNIT_TEST_EXCEPTION( testException, mnslp_ipfix_bad_argument );
    CPPUNIT_TEST( testFieldValues );
	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp();
	void tearDown();

	void testGetters();
	void testAssign();
	void testException();
	void testAddFields();
	void testFieldValues();

  private:
    ipfix_field_type_t field1;
    ipfix_field_type_t field2;
    ipfix_field_type_t field3;
    
    mnslp_ipfix_field *ptrField1;
    mnslp_ipfix_field *ptrField2;
    
    msnlp_ipfix_field_container field_container;

};

CPPUNIT_TEST_SUITE_REGISTRATION( Mnslp_Ipfix_Field_Test );




void Mnslp_Ipfix_Field_Test::setUp() {
	
	field1.eno = 0;  
	field1.ftype = IPFIX_FT_PACKETDELTACOUNT;
	field1.length = 8; 
	field1.coding =  IPFIX_CODING_UINT;
	field1.name = "packetDeltaCount";
	field1.documentation = "";

	field2.eno = 0;  
	field2.ftype = IPFIX_FT_IPCLASSOFSERVICE;
	field2.length = 1; 
	field2.coding =  IPFIX_CODING_UINT;
	field2.name = "ipClassOfService";
	field2.documentation = "";

	field3.eno = 0;  
	field3.ftype = IPFIX_FT_PACKETDELTACOUNT;
	field3.length = 8; 
	field3.coding =  IPFIX_CODING_UINT;
	field3.name = "packetDeltaCount";
	field3.documentation = "";

	ptrField1 = new mnslp_ipfix_field(field1);
	ptrField2 = new mnslp_ipfix_field(field2);

    msnlp_ipfix_field_container field_container = msnlp_ipfix_field_container();
}

void Mnslp_Ipfix_Field_Test::tearDown() 
{
	delete(ptrField1);
	delete(ptrField2);
}


void Mnslp_Ipfix_Field_Test::testGetters() 
{
	ipfix_field_type_t tmp = ptrField1->get_field_type();
	CPPUNIT_ASSERT( tmp.ftype == field1.ftype );
	CPPUNIT_ASSERT( tmp.length == field1.length );
	CPPUNIT_ASSERT( tmp.coding == field1.coding );
	CPPUNIT_ASSERT( tmp.name == field1.name );
	CPPUNIT_ASSERT( tmp.documentation == field1.documentation );
}

void Mnslp_Ipfix_Field_Test::testAssign() 
{
    mnslp_ipfix_field tmpField = *ptrField2;
    ipfix_field_type_t tmp = tmpField.get_field_type();
    
	CPPUNIT_ASSERT( tmp.ftype == field2.ftype );
	CPPUNIT_ASSERT( tmp.length == field2.length );
	CPPUNIT_ASSERT( tmp.coding == field2.coding );
	CPPUNIT_ASSERT( tmp.name == field2.name );
	CPPUNIT_ASSERT( tmp.documentation == field2.documentation );
	
	
}

void Mnslp_Ipfix_Field_Test::testException() 
{

    field_container.clear();     

	field_container.AddFieldType( 0, IPFIX_FT_DROPPEDPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "droppedPacketTotalCount", "" );

	field_container.AddFieldType( 0, IPFIX_FT_DROPPEDPACKETTOTALCOUNT, 8, IPFIX_CODING_UINT, 
     "droppedPacketTotalCount", "" );
     
    CPPUNIT_ASSERT( field_container.get_num_fields() == 1 );

}

void Mnslp_Ipfix_Field_Test::testAddFields()
{
	field_container.initialize_forward();
	
	CPPUNIT_ASSERT( field_container.get_num_fields() == 213 );
	
	field_container.initialize_reverse();
	
	CPPUNIT_ASSERT( field_container.get_num_fields() == 426 );
	
	// Gets one of the fields and verifies its data.
	mnslp_ipfix_field * tmpField = field_container.get_field( 0, IPFIX_FT_IPCLASSOFSERVICE );
    ipfix_field_type_t tmp = tmpField->get_field_type();
    
	CPPUNIT_ASSERT( tmp.ftype == field2.ftype );
	CPPUNIT_ASSERT( tmp.length == field2.length );
	CPPUNIT_ASSERT( tmp.coding == field2.coding );
	CPPUNIT_ASSERT( tmp.name == field2.name );
	CPPUNIT_ASSERT( tmp.documentation == field2.documentation );
	
}

void Mnslp_Ipfix_Field_Test::testFieldValues()
{
	uint8_t value8 = 1;
	uint16_t value16 = 2;
	uint32_t value32 = 3;
	uint64_t value64 = 4;
	char valuechar[5] = "1234";
	char valuechar2[17] = "1234567891011122";
	uint8_t valuebyte0[5] = "1234";
	uint8_t valuebyte1[7] = "123456";
	uint8_t valuebyte2[15] = "12345678912313";
	
    field_container.clear();   
	field_container.initialize_forward();

	mnslp_ipfix_field * tmpField;

	// Int 1
	tmpField = field_container.get_field( 0, IPFIX_FT_IGMPTYPE );
	msnl_ipfix_value_field fvalue1 = msnl_ipfix_value_field(*tmpField, value8);
	CPPUNIT_ASSERT( (uint8_t) 1 == fvalue1.get_value_int8() );

	// Int 2
	tmpField = field_container.get_field( 0, IPFIX_FT_ICMPTYPECODEIPV4 );
	msnl_ipfix_value_field fvalue2 = msnl_ipfix_value_field(*tmpField, value16);
	CPPUNIT_ASSERT( (uint16_t) 2 == fvalue2.get_value_int16() );

	// Int 4
	tmpField = field_container.get_field( 0, IPFIX_FT_INGRESSINTERFACE );
	msnl_ipfix_value_field fvalue3 = msnl_ipfix_value_field(*tmpField, value32);
	CPPUNIT_ASSERT( (uint32_t) 3 == fvalue3.get_value_int32() );
	
    //Address 4
	tmpField = field_container.get_field( 0, IPFIX_FT_SOURCEIPV4ADDRESS );
	msnl_ipfix_value_field fvalue4 = msnl_ipfix_value_field(*tmpField, (uint8_t *) valuebyte0, 4);
	uint8_t * valuebyte3 = fvalue4.get_value_address();
	std::cout << "val:" << std::endl;
	for (int i=0; i<4; i++)
		std::cout << valuebyte3[i];
	//CPPUNIT_ASSERT( valuebyte0 == fvalue4.get_value_address() );
	  
	// Int 8 
	//tmpField = field_container.get_field( 0, IPFIX_FT_POSTMCASTPACKETDELTACOUNT );
	//msnl_ipfix_value_field fvalue5 = msnl_ipfix_value_field(*tmpField, value64);
	//uint64_t printv5 = fvalue5.get_value_int64();
	//std::cout << "val:" << printv5 << std::endl;

	// addres 16
	//tmpField = field_container.get_field( 0, IPFIX_FT_SOURCEIPV6ADDRESS );
	//msnl_ipfix_value_field fvalue6 = msnl_ipfix_value_field(*tmpField, (char *) valuechar2, 16);
	//std::cout << "val:" << fvalue6.get_value_string() << std::endl;

	// Bytes
	//tmpField = field_container.get_field( 0, IPFIX_FT_POSTDESTINATIONMACADDRESS );
	//msnl_ipfix_value_field fvalue7 = msnl_ipfix_value_field(*tmpField, (uint8_t *) valuebyte, 6);
	//std::cout << "val:" << fvalue7.get_value_byte() << std::endl;

	// Bytes - variable length
	//tmpField = field_container.get_field( 0, IPFIX_FT_MPLSLABELSTACKSECTION9 );
	//msnl_ipfix_value_field fvalue8 = msnl_ipfix_value_field(*tmpField, (uint8_t *) valuebyte2, 14);
	//std::cout << "val:" << fvalue8.get_value_byte() << std::endl;
	
	// Float 8
	//tmpField = field_container.get_field( 0, IPFIX_FT_PACKETSOBSERVED );
	//msnl_ipfix_value_field fvalue9 = msnl_ipfix_value_field(*tmpField, value64);
	//std::cout << "val:" << fvalue9.get_value_float64() << std::endl;

	
}
// EOF
