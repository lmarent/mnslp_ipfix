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
#include "mnslp_ipfix_data_record.h"


using namespace mnslp_ipfix;


class Mnslp_Ipfix_Message_Test;

/*
 * We use a subclass for testing and make the test case a friend. This
 * way the test cases have access to protected methods and they don't have
 * to be public in mnslp_ipfix_fields.
 */
class mnslp_ipfix_message_test : public mnslp_ipfix_message {
  public:
	mnslp_ipfix_message_test(int sourceid, int ipfix_version, bool _encode_network)
		: mnslp_ipfix_message(sourceid, ipfix_version, _encode_network) { }
   
    mnslp_ipfix_message_test( char * param, size_t message_length, bool _encode_network)
		: mnslp_ipfix_message(param, message_length, _encode_network) { }


	friend class Mnslp_Ipfix_Message_Test;
};

class Mnslp_Ipfix_Message_Test : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( Mnslp_Ipfix_Message_Test );
	CPPUNIT_TEST( testAddTemplate );
	CPPUNIT_TEST( testExceptionAddTemplate );
	CPPUNIT_TEST( testDataRecords );
	CPPUNIT_TEST( testExportImport );

	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp();
	void tearDown();
	void testAddTemplate();
	void testExceptionAddTemplate();
	void testDataRecords();
	void testExportImport();

  private:
    
    mnslp_ipfix_message_test * mes;
    mnslp_ipfix_message_test * mes2;


};

void Mnslp_Ipfix_Message_Test::setUp() {
	
	int sourceid = 12345;
	mes = new mnslp_ipfix_message_test(sourceid, IPFIX_VERSION, true);
}

void Mnslp_Ipfix_Message_Test::testAddTemplate()
{
	
	uint16_t templatedataid = 0;
	uint16_t templatescopeid = 0;
	int num_templates = 0;
	
	int nfields = 3;
	int result;
	uint8_t   buf[5]  = { 1, 2, 3, 4 };
	
	// Verifies the field add method.
	templatedataid = mes->new_data_template( nfields );
	mes->add_field(templatedataid, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	num_templates = mes->get_num_templates();
	CPPUNIT_ASSERT( num_templates == 1 );

	// Verifies the scope field add method.
	templatescopeid = mes->new_option_template( nfields );
	mes->add_scope_field(templatescopeid, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	num_templates = mes->get_num_templates();
	CPPUNIT_ASSERT( num_templates == 2 );
	
	// Verifies the method delete template, second it verifies with a valid template
	mes->delete_all_templates();
	num_templates = mes->get_num_templates();
	CPPUNIT_ASSERT( num_templates == 0 );
	
	templatedataid = mes->new_data_template( nfields );
	num_templates = mes->get_num_templates();
	CPPUNIT_ASSERT( num_templates == 1 );
	
	// Verifies the method delete template, second it verifies with a valid template
	mes->delete_template(templatedataid);
	num_templates = mes->get_num_templates();
	CPPUNIT_ASSERT( num_templates == 0 );
	
	mnslp_ipfix_field ptrField1 = mes->get_field_definition( 0, IPFIX_FT_SOURCEIPV4ADDRESS );
	mnslp_ipfix_value_field fvalue3 = ptrField1.get_ipfix_value_field( (uint8_t *) buf, 4);

	mes->delete_all_templates();
	
	export_fields_t a[3]; 
	a[0].eno = 0;
	a[0].ienum = IPFIX_FT_OCTETDELTACOUNT;
	a[0].length = 8;

	a[1].eno = 0;
	a[1].ienum = IPFIX_FT_PACKETDELTACOUNT;
	a[1].length = 8;

	a[2].eno = 0;
	a[2].ienum = IPFIX_FT_FLOWS;
	a[2].length = 8;
	
	mes->make_template(a, 3);
	num_templates = mes->get_num_templates();
	CPPUNIT_ASSERT( num_templates == 1 );
	
}

void Mnslp_Ipfix_Message_Test::testExceptionAddTemplate()
{

	int nfields = 0;
	CPPUNIT_ASSERT_THROW( mes->new_data_template( nfields ),mnslp_ipfix_bad_argument);
	CPPUNIT_ASSERT_THROW( mes->new_option_template( nfields ),mnslp_ipfix_bad_argument);

	uint16_t templatedataid = 0;
	uint16_t templatescopeid = 0;
	nfields = 3;

	// Verifies that a data field cannot be added to a empty template
	CPPUNIT_ASSERT_THROW( mes->add_field(templatedataid, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4), 
						  mnslp_ipfix_bad_argument);

	// Verifies that a scope field cannot be added to a empty template
	CPPUNIT_ASSERT_THROW( mes->add_scope_field(templatescopeid, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4),
						  mnslp_ipfix_bad_argument);
		
	// Verifies that a scope field cannot be added to a data template
	templatedataid = mes->new_data_template( nfields );
	CPPUNIT_ASSERT_THROW( mes->add_scope_field(templatedataid, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4),
						  mnslp_ipfix_bad_argument);
	
	// Verifies that a data field cannot be added to a scope template
	templatescopeid = mes->new_option_template( nfields );
	CPPUNIT_ASSERT_THROW( mes->add_field(templatescopeid, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4),
						  mnslp_ipfix_bad_argument);
		
	// Verifies that only add field in the collection
	CPPUNIT_ASSERT_THROW( mes->add_field(templatedataid, 0, 3000, 4),
						  mnslp_ipfix_bad_argument);
						  
	// Verifies that only the maximum number of data fields can be inserted.
	mes->add_field(templatedataid, 0, IPFIX_FT_OCTETDELTACOUNT, 8);
	mes->add_field(templatedataid, 0, IPFIX_FT_PACKETDELTACOUNT, 8);
	mes->add_field(templatedataid, 0, IPFIX_FT_FLOWS, 8);
	CPPUNIT_ASSERT_THROW( mes->add_field(templatedataid, 0, IPFIX_FT_PROTOCOLIDENTIFIER, 1),
						  mnslp_ipfix_bad_argument);
	

	// Verifies that only the maximum number of scope fields can be inserted.
	mes->add_scope_field(templatescopeid, 0, IPFIX_FT_OCTETDELTACOUNT, 8);
	mes->add_scope_field(templatescopeid, 0, IPFIX_FT_PACKETDELTACOUNT, 8);
	mes->add_scope_field(templatescopeid, 0, IPFIX_FT_FLOWS, 8);
	CPPUNIT_ASSERT_THROW( mes->add_scope_field(templatescopeid, 0, IPFIX_FT_PROTOCOLIDENTIFIER, 1),
						  mnslp_ipfix_bad_argument);

	mes->delete_all_templates();

	export_fields_t a[3]; 
	a[0].eno = 0;
	a[0].ienum = IPFIX_FT_OCTETDELTACOUNT;
	a[0].length = 8;

	a[1].eno = 0;
	a[1].ienum = IPFIX_FT_PACKETDELTACOUNT;
	a[1].length = 8;

	a[2].eno = 0;
	a[2].ienum = IPFIX_FT_FLOWS;
	a[2].length = 8;
	
	CPPUNIT_ASSERT_THROW( mes->make_template(a, 4), mnslp_ipfix_bad_argument);
	
}

void Mnslp_Ipfix_Message_Test::testDataRecords()
{

	uint16_t templatedataid = 0;
	uint16_t templatescopeid = 0;
	uint64_t octdel = 100;
	uint64_t packdel = 300;
	uint64_t packdel2 = 400;
	
	int num_templates = 0;
	int nfields = 3;
	uint8_t   buf[5]  = { 1, 2, 3, 4 };

	mes->delete_all_templates();

	templatedataid = mes->new_data_template( nfields );
	mes->add_field(templatedataid, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	mes->add_field(templatedataid, 0, IPFIX_FT_OCTETDELTACOUNT, 8);
	mes->add_field(templatedataid, 0, IPFIX_FT_PACKETDELTACOUNT, 8);

	mnslp_ipfix_field field1 = mes->get_field_definition( 0, IPFIX_FT_SOURCEIPV4ADDRESS );
	mnslp_ipfix_value_field fvalue1 = field1.get_ipfix_value_field( (uint8_t *) buf, 4);

	mnslp_ipfix_field field2 = mes->get_field_definition( 0, IPFIX_FT_OCTETDELTACOUNT );
	mnslp_ipfix_value_field fvalue2 = field2.get_ipfix_value_field( octdel);

	mnslp_ipfix_field field3 = mes->get_field_definition( 0, IPFIX_FT_PACKETDELTACOUNT );
	mnslp_ipfix_value_field fvalue3 = field3.get_ipfix_value_field( packdel );
	mnslp_ipfix_value_field fvalue3b = field3.get_ipfix_value_field( packdel2 );


	mnslp_ipfix_data_record data(templatedataid);
	data.insert_field(0, IPFIX_FT_SOURCEIPV4ADDRESS, fvalue1);
	data.insert_field(0, IPFIX_FT_OCTETDELTACOUNT, fvalue2);
	data.insert_field(0, IPFIX_FT_PACKETDELTACOUNT, fvalue3);
	mes->include_data(templatedataid, data);
	
	mnslp_ipfix_data_record data2(templatedataid);
	data2.insert_field(0, IPFIX_FT_SOURCEIPV4ADDRESS, fvalue1);
	data2.insert_field(0, IPFIX_FT_OCTETDELTACOUNT, fvalue2);
	data2.insert_field(0, IPFIX_FT_PACKETDELTACOUNT, fvalue3);
	mes->include_data(templatedataid, data2);

	mnslp_ipfix_data_record data3(templatedataid);
	data3.insert_field(0, IPFIX_FT_SOURCEIPV4ADDRESS, fvalue1);
	data3.insert_field(0, IPFIX_FT_OCTETDELTACOUNT, fvalue2);
	data3.insert_field(0, IPFIX_FT_PACKETDELTACOUNT, fvalue3b);
	mes->include_data(templatedataid, data3);

	CPPUNIT_ASSERT( data2 == data );
	CPPUNIT_ASSERT( data2 != data3);

    mnslp_ipfix_data_record data4 = data3;
    CPPUNIT_ASSERT( data3 == data4);
    
    int num_fields_record = data3.get_num_fields();
    CPPUNIT_ASSERT( num_fields_record == 3);

    mnslp_ipfix_value_field fvalue1b = data3.get_field(0, IPFIX_FT_SOURCEIPV4ADDRESS);
    CPPUNIT_ASSERT( fvalue1 == fvalue1b );
    
    mes->delete_all_templates();

}

void Mnslp_Ipfix_Message_Test::testExportImport()
{
	uint16_t templatedataid = 0;
	uint16_t templatescopeid = 0;
	uint64_t octdel = 100;
	uint64_t octdel2 = 200;
	uint64_t packdel = 300;
	uint64_t packdel2 = 400;
	
	int num_templates = 0;
	int nfields = 3;
	uint8_t   buf[5]  = { 1, 2, 3, 4 };
	uint8_t   buf2[5]  = { 5, 6, 7, 8 };
	char * message;
	int offset;

	mes->delete_all_templates();

	templatedataid = mes->new_data_template( nfields );
	mes->add_field(templatedataid, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	mes->add_field(templatedataid, 0, IPFIX_FT_OCTETDELTACOUNT, 8);
	mes->add_field(templatedataid, 0, IPFIX_FT_PACKETDELTACOUNT, 8);

	mnslp_ipfix_field field1 = mes->get_field_definition( 0, IPFIX_FT_SOURCEIPV4ADDRESS );
	mnslp_ipfix_value_field fvalue1 = field1.get_ipfix_value_field( (uint8_t *) buf, 4);
	mnslp_ipfix_value_field fvalue1a = field1.get_ipfix_value_field( (uint8_t *) buf2, 4);

	mnslp_ipfix_field field2 = mes->get_field_definition( 0, IPFIX_FT_OCTETDELTACOUNT );
	mnslp_ipfix_value_field fvalue2 = field2.get_ipfix_value_field( octdel);
	mnslp_ipfix_value_field fvalue2a = field2.get_ipfix_value_field( octdel2);

	mnslp_ipfix_field field3 = mes->get_field_definition( 0, IPFIX_FT_PACKETDELTACOUNT );
	mnslp_ipfix_value_field fvalue3 = field3.get_ipfix_value_field( packdel );
	mnslp_ipfix_value_field fvalue3a = field3.get_ipfix_value_field( packdel2 );

	mnslp_ipfix_data_record data(templatedataid);
	data.insert_field(0, IPFIX_FT_SOURCEIPV4ADDRESS, fvalue1);
	data.insert_field(0, IPFIX_FT_OCTETDELTACOUNT, fvalue2);
	data.insert_field(0, IPFIX_FT_PACKETDELTACOUNT, fvalue3);
	mes->include_data(templatedataid, data);

	mnslp_ipfix_data_record data2(templatedataid);
	data2.insert_field(0, IPFIX_FT_SOURCEIPV4ADDRESS, fvalue1a);
	data2.insert_field(0, IPFIX_FT_OCTETDELTACOUNT, fvalue2a);
	data2.insert_field(0, IPFIX_FT_PACKETDELTACOUNT, fvalue3a);
	mes->include_data(templatedataid, data2);
	
	mes->output(templatedataid);
	message = mes->get_message();
	offset = mes->get_offset();
	
	std::cout << "offset:" << offset << std::endl;
	
	mnslp_ipfix_message_test msgb (message, offset, true);
	num_templates = msgb.get_num_templates();
	CPPUNIT_ASSERT( num_templates == 1 );
	
	if (msgb.operator==( *mes))
		std::cout << "equal messages" << std::endl;
	else
	    std::cout << "not equal messages" << std::endl;

	
}


void Mnslp_Ipfix_Message_Test::tearDown() 
{
	delete(mes);
}

CPPUNIT_TEST_SUITE_REGISTRATION( Mnslp_Ipfix_Message_Test );
