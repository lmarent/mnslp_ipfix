#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include "mnslp_ipfix_message.h"

using namespace mnslp_ipfix;

int main ( int argc, char **argv )
{
	mnslp_ipfix_template *templdata;
	mnslp_ipfix_template *templscope;
	data_record data;
	
	int nfields = 3;
	int result;
	int sourceid = 12345;
	char   buf[31]  = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                           11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                           21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };

	mnslp_ipfix_message message = mnslp_ipfix_message(sourceid, IPFIX_VERSION);
	
	message.new_data_template( &templdata, nfields );
	result = message.add_field(templdata, 0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	
	data.insert_field(0, IPFIX_FT_SOURCEIPV4ADDRESS, (void *) buf);
	data.insert_field_lenght(0, IPFIX_FT_SOURCEIPV4ADDRESS, 4);
	
	message.output(templdata, &data);
	std::cout << "output" << result << std::endl;
	

    /** init lib 
     
    if ( ipfix_init() <0) {
        fprintf( stderr, "cannot init ipfix module: %s\n", strerror(errno) );
        exit(1);
    }

    /** open ipfix exporter
     
    if ( ipfix_open( &ipfixh, sourceid, IPFIX_VERSION ) <0 ) {
        fprintf( stderr, "ipfix_open() failed: %s\n", strerror(errno) );
        exit(1);
    }
	
    /** get template
     
    if ( ipfix_new_data_template( ipfixh, &ipfixt, 3 ) <0 ) {
        fprintf( stderr, "ipfix_new_template() failed: %s\n", 
                 strerror(errno) );
        exit(1);
    }
    if ( (ipfix_add_field( ipfixh, ipfixt, 
                           0, IPFIX_FT_SOURCEIPV4ADDRESS, 4 ) <0 ) 
         || (ipfix_add_field( ipfixh, ipfixt, 
                              0, IPFIX_FT_PACKETDELTACOUNT, 4 ) <0 ) ) {
        fprintf( stderr, "ipfix_new_template() failed: %s\n", 
                 strerror(errno) );
        exit(1);
    }

    if ( (ipfix_add_field( ipfixh, ipfixt, 0, IPFIX_FT_INGRESSINTERFACE, 4 ) ) <0 ) {
        
        fprintf( stderr, "ipfix_insert field() failed: %s\n", 
                 strerror(errno) );
        exit(1);
							   
	}


    printf( "[%d] export some data a ", j );
    fflush( stdout) ;
    
    if (mnslp_message_create( &mes  ) <0)
    {
        fprintf( stderr, "ipfix_create_message() failed: %s\n", 
                     strerror(errno) );		
		exit(1);
	}

    printf( "[%d] export some data b ", j );
    fflush( stdout) ;

    if ( mnslp_ipfix_export( ipfixh, mes, ipfixt, con, buf, &bytes ) <0 ) {
         fprintf( stderr, "ipfix_export() failed: %s\n", 
                     strerror(errno) );
         exit(1);
    }

	printf( "[%d] export some data b ", bytes );
    fflush( stdout) ;

    // With the following code lines, we test that we can decode the message.

	ipfix_datarecord_t  data = {NULL, NULL, 0};
	if ( mnslp_ipfix_import( ipfixh, mes,  data ) < 0 )
	{
		fprintf( stderr, "ipfix_import() failed: %s\n", 
                     strerror(errno) );
         exit(1);
	} 
	
	printf (" num fields [%d]", data.maxfields);
	fflush( stdout) ;

    /** export some data
    for( j=0; j<10; j++ ) {

        printf( "[%d] export some data ... ", j );
        fflush( stdout) ;

        if ( ipfix_export( ipfixh, ipfixt, buf, &bytes ) <0 ) {
            fprintf( stderr, "ipfix_export() failed: %s\n", 
                     strerror(errno) );
            exit(1);
        }

        if ( ipfix_export_flush( ipfixh ) <0 ) {
            fprintf( stderr, "ipfix_export_flush() failed: %s\n", 
                     strerror(errno) );
            exit(1);
        }

        printf( "done.\n" );
        bytes++;
        sleep(1);
    }
     

    printf( "data exported.\n" );

    /** clean up
     
    ipfix_delete_template( ipfixh, ipfixt );
    ipfix_message_release (mes);
    ipfix_close( ipfixh );
    ipfix_cleanup();
    */
    exit(0);
}
