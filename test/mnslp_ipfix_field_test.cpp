/*
 * Test the mnslp_fields_test class.
 *
 * $Id: mnslp_fields.cpp 2014-11-28 10:16:00 amarentes $
 * $HeadURL: https://./test/mnslp_fields_test.cpp $
 */
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ipfix_def.h"
#include "mnslp_ipfix_field.h"


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
	CPPUNIT_TEST_EXCEPTION( testExceptionInt1, mnslp_ipfix_bad_argument );
	CPPUNIT_TEST_EXCEPTION( testExceptionInt2, mnslp_ipfix_bad_argument );
	CPPUNIT_TEST_EXCEPTION( testExceptionInt4, mnslp_ipfix_bad_argument );
	CPPUNIT_TEST_EXCEPTION( testExceptionInt8, mnslp_ipfix_bad_argument );
	CPPUNIT_TEST_EXCEPTION( testExceptionAddress4, mnslp_ipfix_bad_argument );
	CPPUNIT_TEST_EXCEPTION( testExceptionAddress16, mnslp_ipfix_bad_argument );
	CPPUNIT_TEST_EXCEPTION( testExceptionString, mnslp_ipfix_bad_argument );
	CPPUNIT_TEST_EXCEPTION( testExceptionByteFixed, mnslp_ipfix_bad_argument );
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
	void testExceptionInt1();
	void testExceptionInt2();
	void testExceptionInt4();
	void testExceptionInt8();
	void testExceptionAddress4();
	void testExceptionAddress16();
	void testExceptionString();
	void testExceptionByteFixed();

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
	mnslp_ipfix_field tmpField = field_container.get_field( 0, IPFIX_FT_IPCLASSOFSERVICE );
    ipfix_field_type_t tmp = tmpField.get_field_type();
    
	CPPUNIT_ASSERT( tmp.ftype == field2.ftype );
	CPPUNIT_ASSERT( tmp.length == field2.length );
	CPPUNIT_ASSERT( tmp.coding == field2.coding );
	CPPUNIT_ASSERT( tmp.name == field2.name );
	CPPUNIT_ASSERT( tmp.documentation == field2.documentation );	
	
}

void Mnslp_Ipfix_Field_Test::testFieldValues()
{
	uint8_t value8 = 8;
	uint16_t value16 = 21;
	uint32_t value32 = 3210;
	uint64_t value64 = 87654321;
	float valfloat = 12;
	uint64_t valfloatu = (uint64_t) valfloat;
	char valuechar[5] = "1234";
	uint8_t valuebyte0[5] = { 1, 2, 3, 4 };
	uint8_t valuebyte1[7] = { 1, 2, 3, 4, 5, 6 };
	uint8_t valuebyte2[15] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	uint8_t valuebyte3[17] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	char * charprint;
	char * charprint2;
	
	uint8_t *out;
	bool equal = true;
	
    field_container.clear();   
	field_container.initialize_forward();
	out = ( uint8_t* ) calloc (100, sizeof(uint8_t) );
	charprint = (char *) calloc (100, sizeof(char) );
	charprint2 = (char *) calloc (100, sizeof(char) );
	int relay_f;
	mnslp_ipfix_field tmpField;
	

	// Int 1
	tmpField = field_container.get_field( 0, IPFIX_FT_IGMPTYPE );
	mnslp_ipfix_value_field fvalue1 = tmpField.get_ipfix_value_field(value8);
	
		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_int( fvalue1, out, relay_f );
	mnslp_ipfix_value_field fvalue1aResult = tmpField.ipfix_decode_int( out, fvalue1.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint8_t) 8 == fvalue1aResult.get_value_int8() );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_int( fvalue1, out, relay_f );
	mnslp_ipfix_value_field fvalue1bResult = tmpField.ipfix_decode_int( out, fvalue1.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint8_t) 8 == fvalue1bResult.get_value_int8() );
	tmpField.ipfix_snprint_int( charprint, 100, fvalue1bResult );
	std::string str(charprint, charprint + fvalue1bResult.get_length());
	CPPUNIT_ASSERT( str.compare("8")  == 0 );

	// Int 2
	tmpField = field_container.get_field( 0, IPFIX_FT_ICMPTYPECODEIPV4 );
	mnslp_ipfix_value_field fvalue2 = tmpField.get_ipfix_value_field(value16);
	CPPUNIT_ASSERT( (uint16_t) 21 == fvalue2.get_value_int16() );
	tmpField.ipfix_snprint_int( charprint, 100, fvalue2 );
	std::string str2(charprint, charprint + fvalue2.get_length());
	CPPUNIT_ASSERT( str2.compare("21")  == 0 );

		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_int( fvalue2, out, relay_f );
	mnslp_ipfix_value_field fvalue2aResult = tmpField.ipfix_decode_int( out, fvalue2.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint16_t) 21 == fvalue2aResult.get_value_int16() );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_int( fvalue2, out, relay_f );
	mnslp_ipfix_value_field fvalue2bResult = tmpField.ipfix_decode_int( out, fvalue2.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint16_t) 21 == fvalue2bResult.get_value_int16() );

	// Int 4
	tmpField = field_container.get_field( 0, IPFIX_FT_INGRESSINTERFACE );
	mnslp_ipfix_value_field fvalue3 = tmpField.get_ipfix_value_field(value32);
	CPPUNIT_ASSERT( (uint32_t) 3210 == fvalue3.get_value_int32() );
	tmpField.ipfix_snprint_int( charprint, 100, fvalue3 );
	std::string str3(charprint, charprint + fvalue3.get_length());
	CPPUNIT_ASSERT( str3.compare("3210")  == 0 );


		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_int( fvalue3, out, relay_f );
	mnslp_ipfix_value_field fvalue3aResult = tmpField.ipfix_decode_int( out, fvalue3.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint32_t) 3210 == fvalue3aResult.get_value_int32() );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_int( fvalue3, out, relay_f );
	mnslp_ipfix_value_field fvalue3bResult = tmpField.ipfix_decode_int( out, fvalue3.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint32_t) 3210 == fvalue3bResult.get_value_int32() );
	
    //Address 4
	tmpField = field_container.get_field( 0, IPFIX_FT_SOURCEIPV4ADDRESS );
	mnslp_ipfix_value_field fvalue4 = tmpField.get_ipfix_value_field((uint8_t *) valuebyte0, 4);
	uint8_t * valuebyte4 = fvalue4.get_value_address();
	for (int i=0; i < 4; i++)
		if (valuebyte0[i] != valuebyte4[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );
	
		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_int( fvalue4, out, relay_f );
	mnslp_ipfix_value_field fvalue4aResult = tmpField.ipfix_decode_int( out, fvalue4.get_length(), relay_f );	
	uint8_t * valuebyte4a = fvalue4.get_value_address();
	for (int i=0; i < 4; i++)
		if (valuebyte0[i] != valuebyte4a[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_int( fvalue4, out, relay_f );
	mnslp_ipfix_value_field fvalue4bResult = tmpField.ipfix_decode_int( out, fvalue4.get_length(), relay_f );	
	uint8_t * valuebyte4b = fvalue4.get_value_address();
	for (int i=0; i < 4; i++)
		if (valuebyte0[i] != valuebyte4b[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );

	  
	// Int 8 
	tmpField = field_container.get_field( 0, IPFIX_FT_POSTMCASTPACKETDELTACOUNT );
	mnslp_ipfix_value_field fvalue5 = tmpField.get_ipfix_value_field(value64);
	uint64_t printv5 = fvalue5.get_value_int64();
	CPPUNIT_ASSERT( printv5 == value64 );
	
	tmpField.ipfix_snprint_int( charprint, 100, fvalue5 );
	std::string str5(charprint, charprint + fvalue5.get_length());
	std::ostringstream o1;
	o1 << value64;
	std::string str6 = o1.str();
	CPPUNIT_ASSERT( str5.compare(str6)  == 0 );
	

		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_int( fvalue5, out, relay_f );
	mnslp_ipfix_value_field fvalue5aResult = tmpField.ipfix_decode_int( out, fvalue5.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint64_t) 87654321 == fvalue5aResult.get_value_int64() );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_int( fvalue5, out, relay_f );
	mnslp_ipfix_value_field fvalue5bResult = tmpField.ipfix_decode_int( out, fvalue5.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint64_t) 87654321 == fvalue5bResult.get_value_int64() );
	

	// Address 16
	tmpField = field_container.get_field( 0, IPFIX_FT_SOURCEIPV6ADDRESS );
	mnslp_ipfix_value_field fvalue6 = tmpField.get_ipfix_value_field((uint8_t *) valuebyte3, 16);
	uint8_t * valuebyte6 = fvalue6.get_value_address();
	for (int i=0; i < 16; i++)
		if (valuebyte6[i] != valuebyte3[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );	

		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_bytes( fvalue6, out, relay_f );
	mnslp_ipfix_value_field fvalue6aResult = tmpField.ipfix_decode_bytes( out, fvalue6.get_length(), relay_f );	
	uint8_t * valuebyte6a = fvalue6aResult.get_value_address();
	for (int i=0; i < 16; i++)
		if (valuebyte6a[i] != valuebyte3[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_bytes( fvalue6, out, relay_f );
	mnslp_ipfix_value_field fvalue6bResult = tmpField.ipfix_decode_bytes( out, fvalue6.get_length(), relay_f );	
	uint8_t * valuebyte6b = fvalue6bResult.get_value_address();
	for (int i=0; i < 16; i++)
		if (valuebyte6b[i] != valuebyte3[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );


	// Bytes
	tmpField = field_container.get_field( 0, IPFIX_FT_POSTDESTINATIONMACADDRESS );
	mnslp_ipfix_value_field fvalue7 = tmpField.get_ipfix_value_field((uint8_t *) valuebyte1, 6);
	uint8_t * valuebyte7 = fvalue7.get_value_byte();
	for (int i=0; i < 6; i++)
		if (valuebyte7[i] != valuebyte1[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );
    

		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_bytes( fvalue7, out, relay_f );
	mnslp_ipfix_value_field fvalue7aResult = tmpField.ipfix_decode_bytes( out, fvalue7.get_length(), relay_f );	
	uint8_t * valuebyte7a = fvalue7aResult.get_value_byte();
	for (int i=0; i < 6; i++)
		if (valuebyte7a[i] != valuebyte3[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_bytes( fvalue7, out, relay_f );
	mnslp_ipfix_value_field fvalue7bResult = tmpField.ipfix_decode_bytes( out, fvalue7.get_length(), relay_f );	
	uint8_t * valuebyte7b = fvalue7bResult.get_value_byte();
	for (int i=0; i < 6; i++)
		if (valuebyte7b[i] != valuebyte3[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );


	// Bytes - variable length
	tmpField = field_container.get_field( 0, IPFIX_FT_MPLSLABELSTACKSECTION9 );
	mnslp_ipfix_value_field fvalue8 = tmpField.get_ipfix_value_field((uint8_t *) valuebyte2, 14);
	uint8_t * valuebyte8 = fvalue8.get_value_byte();
	for (int i=0; i < 14; i++)
		if (valuebyte8[i] != valuebyte2[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );


		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_bytes( fvalue8, out, relay_f );
	mnslp_ipfix_value_field fvalue8aResult = tmpField.ipfix_decode_bytes( out, fvalue8.get_length(), relay_f );	
	uint8_t * valuebyte8a = fvalue8aResult.get_value_byte();
	for (int i=0; i < 14; i++)
		if (valuebyte8a[i] != valuebyte2[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_bytes( fvalue8, out, relay_f );
	mnslp_ipfix_value_field fvalue8bResult = tmpField.ipfix_decode_bytes( out, fvalue8.get_length(), relay_f );	
	uint8_t * valuebyte8b = fvalue8bResult.get_value_byte();
	for (int i=0; i < 14; i++)
		if (valuebyte8b[i] != valuebyte2[i]) equal = false;
	CPPUNIT_ASSERT( true == equal );

	
	// Float 8
	tmpField = field_container.get_field( 0, IPFIX_FT_PACKETSOBSERVED );
	mnslp_ipfix_value_field fvalue9 = tmpField.get_ipfix_value_field(value64);
	uint64_t float1 = fvalue9.get_value_float64();
	CPPUNIT_ASSERT( float1 == value64 );


		// Test without encoding
	relay_f = 1;
	tmpField.ipfix_encode_float( fvalue9, out, relay_f );
	mnslp_ipfix_value_field fvalue9aResult = tmpField.ipfix_decode_float( out, fvalue9.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint64_t) 87654321 == fvalue9aResult.get_value_int64() );
		
		// Test with encoding
	relay_f = 0;
	tmpField.ipfix_encode_float( fvalue9, out, relay_f );
	mnslp_ipfix_value_field fvalue9bResult = tmpField.ipfix_decode_int( out, fvalue9.get_length(), relay_f );	
	CPPUNIT_ASSERT( (uint64_t) 87654321 == fvalue9bResult.get_value_int64() );
		
	
}

void Mnslp_Ipfix_Field_Test::testExceptionInt1()
{
	uint8_t value8 = 1;
	uint16_t value16 = 2;

    field_container.clear();   
	field_container.initialize_forward();

	mnslp_ipfix_field tmpField;

	// Int 1
	tmpField = field_container.get_field( 0, IPFIX_FT_IGMPTYPE );
	mnslp_ipfix_value_field fvalue1 = tmpField.get_ipfix_value_field(value16);
	
}

void Mnslp_Ipfix_Field_Test::testExceptionInt2()
{
	uint8_t value8 = 1;
	uint16_t value16 = 2;

    field_container.clear();   
	field_container.initialize_forward();

	mnslp_ipfix_field tmpField;

	// Int 2
	tmpField = field_container.get_field( 0, IPFIX_FT_ICMPTYPECODEIPV4 );
	mnslp_ipfix_value_field fvalue2 = tmpField.get_ipfix_value_field(value8);	
	
}

void Mnslp_Ipfix_Field_Test::testExceptionInt4()
{
	uint16_t value16 = 2;
	uint32_t value32 = 3;
	

    field_container.clear();   
	field_container.initialize_forward();

	mnslp_ipfix_field tmpField;

	// Int 4
	tmpField = field_container.get_field( 0, IPFIX_FT_INGRESSINTERFACE );
	mnslp_ipfix_value_field fvalue3 = tmpField.get_ipfix_value_field(value16);
	
}

void Mnslp_Ipfix_Field_Test::testExceptionInt8()
{
	uint32_t value32 = 3;
	uint64_t value64 = 4;
	
    field_container.clear();   
	field_container.initialize_forward();

	mnslp_ipfix_field tmpField;

	// Int 8 
	tmpField = field_container.get_field( 0, IPFIX_FT_POSTMCASTPACKETDELTACOUNT );
	mnslp_ipfix_value_field fvalue5 = tmpField.get_ipfix_value_field(value32);
	
}

void Mnslp_Ipfix_Field_Test::testExceptionAddress4()
{
	uint8_t valuebyte0[5] = "1234";

    field_container.clear();   
	field_container.initialize_forward();

	mnslp_ipfix_field tmpField;

    //Address 4
	tmpField = field_container.get_field( 0, IPFIX_FT_SOURCEIPV4ADDRESS );
	mnslp_ipfix_value_field fvalue4 = tmpField.get_ipfix_value_field((uint8_t *) valuebyte0, 10);


}

void Mnslp_Ipfix_Field_Test::testExceptionAddress16()
{
	uint8_t valuebyte3[17] = "1234567891011122";

    field_container.clear();   
	field_container.initialize_forward();

	mnslp_ipfix_field tmpField;

	// Address 16
	tmpField = field_container.get_field( 0, IPFIX_FT_SOURCEIPV6ADDRESS );
	mnslp_ipfix_value_field fvalue6 = tmpField.get_ipfix_value_field( (uint8_t *) valuebyte3, 10);

}

void Mnslp_Ipfix_Field_Test::testExceptionString()
{
	
	char valuechar[7] = "123467";
    field_container.clear();   
	field_container.initialize_forward();
	mnslp_ipfix_field tmpField;
	
	tmpField = field_container.get_field( 0, IPFIX_FT_SOURCEIPV4ADDRESS );
	mnslp_ipfix_value_field fvalue6 = tmpField.get_ipfix_value_field( (char *) valuechar, 4);
	
	
}

void Mnslp_Ipfix_Field_Test::testExceptionByteFixed()
{
	uint8_t valuebyte1[7] = "123456";

    field_container.clear();   
	field_container.initialize_forward();

	mnslp_ipfix_field tmpField;

	tmpField = field_container.get_field( 0, IPFIX_FT_POSTDESTINATIONMACADDRESS );
	mnslp_ipfix_value_field fvalue7 = tmpField.get_ipfix_value_field((uint8_t *) valuebyte1, 5);
}
// EOF
