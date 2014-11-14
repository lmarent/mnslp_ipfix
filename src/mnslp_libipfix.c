/*
**     exporter.c - example exporter
**
**     Copyright Fraunhofer FOKUS
**
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <ipfix.h>
#include <mlog.h>



typedef struct mnslp_ipfix_message
{
    char        *buffer;   						/* message buffer */
    int         nrecords;                       /* no. of records in buffer */
    size_t      offset;                         /* output buffer fill level */
} mnslp_ipfix_message_t;


int mnslp_ipfix_export( ipfix_t *ifh, 
					    mnslp_ipfix_message_t *mes, 
					    ipfix_template_t *templ, ... )
{
    int       i;
    va_list   args;

    if ( !templ ) {
        errno = EINVAL;
        return -1;
    }

    if ( templ->nfields > g_data.maxfields ) {
        if ( g_data.addrs ) free( g_data.addrs );
        if ( g_data.lens ) free( g_data.lens );
        if ( (g_data.lens=calloc( templ->nfields, sizeof(uint16_t))) ==NULL) {
            g_data.maxfields = 0;
            return -1;
        }
        if ( (g_data.addrs=calloc( templ->nfields, sizeof(void*))) ==NULL) {
            free( g_data.lens );
            g_data.lens = NULL;
            g_data.maxfields = 0;
            return -1;
        }
        g_data.maxfields = templ->nfields;
    }

    /** collect pointers
     */
    va_start(args, templ);
    for ( i=0; i<templ->nfields; i++ )
    {
        g_data.addrs[i] = va_arg(args, char*);          /* todo: change this! */
        if ( templ->fields[i].flength == IPFIX_FT_VARLEN )
            g_data.lens[i] = va_arg(args, int);
        else
            g_data.lens[i] = templ->fields[i].flength;
    }
    va_end(args);

    return _mnslp_ipfix_export_array( ifh, templ, mes, templ->nfields,
									   g_data.addrs, g_data.lens );
}


int _mnslp_ipfix_export_array( ipfix_t          *ifh,
							  ipfix_template_t *templ,
							  mnslp_ipfix_message_t *mes, 
							  int              nfields,
							  void             **fields,
							  uint16_t         *lengths )
{
    int ret;

    mod_lock();
    ret = _mnslp_ipfix_export_array( ifh, templ, mes, nfields, fields, lengths );
    mod_unlock();

    return ret;
}

void allocate_additional_memory(mnslp_ipfix_message_t *mes, size_t additional)
{
	if ((mes->offset + additional) > sizeof(mes->buffer))
		mes->buffer=(char *)realloc(mes->buffer, mes->offset + additional + 1);
}

/*
 * name:        _ipfix_write_template()
 * parameters:
 * return:      0/-1
 */
int _mnslp_ipfix_write_template( ipfix_t           *ifh,
								 mnslp_ipfix_message_t *mes,
								 ipfix_template_t  *templ )
{
    size_t            buflen, tsize=0, ssize=0, osize=0;
    char              *buf;
    uint16_t          tmp16;
    int               i, n;

    /** calc template size
     */
    if ( templ->type == OPTION_TEMPLATE ) {
        for ( i=0, ssize=0; i<templ->nscopefields; i++ ) {
            ssize += 4;
            if (templ->fields[i].elem->ft->eno != IPFIX_FT_NOENO)
                ssize += 4;
        }
        for ( osize=0; i<templ->nfields; i++ ) {
            osize += 4;
            if (templ->fields[i].elem->ft->eno != IPFIX_FT_NOENO)
                osize += 4;
        }
        tsize = 10 + osize + ssize;
    } else {
        for ( tsize=8,i=0; i<templ->nfields; i++ ) {
            tsize += 4;
            if (templ->fields[i].elem->ft->eno != IPFIX_FT_NOENO)
                tsize += 4;
        }
    }

    /** insert template set into buffer
     */
    if ( ifh->version == IPFIX_VERSION_NF9 ) {
        if ( templ->type == OPTION_TEMPLATE ) {
			size_t additional = 10;
			allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, IPFIX_SETID_OPTTEMPLATE_NF9);
            INSERTU16( mes->buffer+mes->offset, mes->offset, tsize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->tid );
            INSERTU16( mes->buffer+mes->offset, mes->offset, ssize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, osize );
        } else {
            size_t additional = 8;
            allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, IPFIX_SETID_TEMPLATE_NF9);
            INSERTU16( mes->buffer+mes->offset, mes->offset, tsize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->tid );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->nfields );
        }
    } else {
        if ( templ->type == OPTION_TEMPLATE ) {
            size_t additional = 10;
            allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, IPFIX_SETID_OPTTEMPLATE );
            INSERTU16( mes->buffer+mes->offset, mes->offset, tsize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->tid );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->nfields );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->nscopefields );
        } else {
            size_t additional = 8;
            allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, IPFIX_SETID_TEMPLATE);
            INSERTU16( mes->buffer+mes->offset, mes->offset, tsize );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->tid );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->nfields );
        }
    }

    if ( templ->type == OPTION_TEMPLATE ) {
        n = templ->nfields;
        for ( i=0; i<templ->nscopefields; i++ ) {
            if ( templ->fields[i].elem->ft->eno == IPFIX_FT_NOENO ) {
				size_t additional = 4;
				allocate_additional_memory(mes, additional);
                INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].elem->ft->ftype );
                INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].flength );
            } else {
				size_t additional = 6;
				allocate_additional_memory(mes, additional);
                tmp16 = templ->fields[i].elem->ft->ftype|IPFIX_EFT_VENDOR_BIT;
                INSERTU16( mes->buffer+mes->offset, mes->offset, tmp16 );
                INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].flength );
                INSERTU32( mes->buffer+mes->offset, mes->offset, templ->fields[i].elem->ft->eno );
            }
        }
    } else {
        i = 0;
        n = templ->nfields;
    }

    for ( ; i<templ->nfields; i++ )
    {
        if ( templ->fields[i].elem->ft->eno == IPFIX_FT_NOENO ) {
			size_t additional = 4;
			allocate_additional_memory(mes, additional);
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].elem->ft->ftype );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].flength );
        } else {
			size_t additional = 8;
			allocate_additional_memory(mes, additional);
            tmp16 = templ->fields[i].elem->ft->ftype|IPFIX_EFT_VENDOR_BIT;
            INSERTU16( mes->buffer+mes->offset, mes->offset, tmp16 );
            INSERTU16( mes->buffer+mes->offset, mes->offset, templ->fields[i].flength );
            INSERTU32( mes->buffer+mes->offset, mes->offset, templ->fields[i].elem->ft->eno );
        }
    }
    templ->tsend = time(NULL);

    return 0;
}

/* Name      : do_writen
 * Parameter : > mes   		output message
 *             > *ptr       buffer
 *             > nbytes     number of bytes to write
 * Purpose   : write 'n' bytes to message
 * Returns   : number of written bytes.
 */
static int _mnslp_do_writen( mnslp_ipfix_message_t *mes, char *ptr, int nbytes )
{
    int     nleft, nwritten;
#ifdef DEBUG
    int     i;

    for ( i=0; i<nbytes; i++ )
        fprintf( stderr, "[%02x]", (ptr[i]&0xFF) );
    fprintf( stderr, "\n" );
#endif

    allocate_additional_memory(mes, nbytes);
    memcpy( mes->buffer+mes->offset, ptr, nbytes );
    return nbytes;
}


int _mnslp_ipfix_send_msg( ipfix_t *ifh, mnslp_ipfix_message_t *mes, iobuf_t *buf )
{
    int i, retval =-1;


    /* send ipfix message */
    if ( _mnslp_do_writen( mes, buf->buffer, buf->buflen )
         != (int)buf->buflen ) {
         return -1;
    }
    retval =0;
    return retval;
}

/*
 * name:        _msnlp_ipfix_write_hdr()
 * parameters:
 * return:      0/-1
 */
int _mnslp_ipfix_write_hdr( ipfix_t *ifh, iobuf_t *buf )
{
    time_t      now = time(NULL);

    /** fill ipfix header
     */
    if ( ifh->version == IPFIX_VERSION_NF9 ) {
        buf->buflen = 0;
        ifh->seqno++;
        INSERTU16( buf->buffer+buf->buflen, buf->buflen, ifh->version );
        INSERTU16( buf->buffer+buf->buflen, buf->buflen, ifh->nrecords );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ((now-g_tstart)*1000));
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, now );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ifh->seqno );
        MSNLP_INSERTU32( buf->buffer+buf->buflen, buf->buflen, ifh->sourceid );
    }
    else {
        buf->buflen = 0;
        INSERTU16( buf->buffer+buf->buflen, buf->buflen, ifh->version );
        INSERTU16( buf->buffer+buf->buflen, buf->buflen,
                   ifh->offset + IPFIX_HDR_BYTES );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, now );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ifh->seqno - ifh->nrecords );
        INSERTU32( buf->buffer+buf->buflen, buf->buflen, ifh->sourceid );
    }

    return 0;
}

/* name:        _mnslp_ipfix_export_flush()
 * parameters:
 */
int _mnslp_ipfix_export_flush( ipfix_t *ifh, mnslp_ipfix_message_t *mes )
{
    iobuf_t           *buf;
    int               ret;

    if ( (ifh==NULL) || (ifh->offset==0) )
        return 0;

    if ( ifh->cs_tid > 0 ) {
        /* finish current dataset */
        _finish_cs( ifh );
    }
    
    if ( (buf=_ipfix_getbuf()) ==NULL )
        return -1;

#ifdef DEBUG
    mlogf( 0, "[ipfix_export_flush] msg has %d records, %d bytes\n",
           ifh->nrecords, ifh->offset );
#endif
    _mnslp_ipfix_write_hdr( ifh, buf );
    memcpy( buf->buffer+buf->buflen, ifh->buffer, ifh->offset );
    buf->buflen += ifh->offset;

    ret = 0;
    if ( _mnslp_ipfix_send_msg( ifh, mes, buf ) <0 ) {
        ret = -1;
    }
    else {
        ifh->offset = 0;    /* ugh! */
        ifh->nrecords = 0;
    }

    _ipfix_freebuf( buf );
    return ret;
}



int _mnlsp_ipfix_export_array( ipfix_t          *ifh,
							   ipfix_template_t *templ,
							   mnslp_ipfix_message_t *mes,
							   int              nfields,
							   void             **fields,
							   uint16_t         *lengths )
{
    int               i, newset_f=0;
    size_t            buflen, datasetlen;
    uint8_t           *p, *buf;

    /** parameter check
     */
    if ( (templ==NULL) || (nfields!=templ->nfields) ) {
        errno = EINVAL;
        return -1;
    }

    if ( templ->tsend == 0 ) {
		if ( _mnslp_ipfix_write_template( ifh, mes, templ ) <0 )
			return -1;
    }

    /** get size of data set, check space
     */
    if ( templ->tid == ifh->cs_tid ) {
        newset_f = 0;
        datasetlen = 0;
    }
    else {
        if ( ifh->cs_tid > 0 ) {
            _mnslp_finish_cs( ifh );
        }
        newset_f = 1;
        datasetlen = 4;
    }
        
    for ( i=0; i<nfields; i++ ) {
        if ( templ->fields[i].flength == IPFIX_FT_VARLEN ) {
            if ( lengths[i]>254 )
                datasetlen += 3;
            else
                datasetlen += 1;
        } else {
            if ( lengths[i] > templ->fields[i].flength ) {
                errno = EINVAL;
                return -1;
            }
        }
        datasetlen += lengths[i];
    }

    if ( (ifh->offset + datasetlen) > IPFIX_DEFAULT_BUFLEN ) {
        if ( ifh->cs_tid )
            _finish_cs( ifh );
        newset_f = 1;

        if ( _mnslp_ipfix_export_flush( ifh, mes ) <0 )
            return -1;
    }


    /* fill buffer */
    buf    = (uint8_t*)(ifh->buffer) + ifh->offset;
    buflen = 0;

    if ( newset_f ) {
        /* insert data set
         */
        ifh->cs_bytes = 0;
        ifh->cs_header = buf;
        ifh->cs_tid = templ->tid;
        INSERTU16( buf+buflen, buflen, templ->tid );
        INSERTU16( buf+buflen, buflen, datasetlen );
    }
    /* csc: to be checked with Lutz whether the usage of "datasetlen" 
     * in the last 30 lines of code is correct */

    /* insert data record
     */
    for ( i=0; i<nfields; i++ ) {
        if ( templ->fields[i].flength == IPFIX_FT_VARLEN ) {
            if ( lengths[i]>254 ) {
                *(buf+buflen) = 0xFF;
                buflen++;
                INSERTU16( buf+buflen, buflen, lengths[i] );
            }
            else {
                *(buf+buflen) = lengths[i];
                buflen++;
            }
        }
        p = fields[i];
        if ( templ->fields[i].relay_f ) {
            ipfix_encode_bytes( p, buf+buflen, lengths[i] ); /* no encoding */
        }
        else {
            templ->fields[i].elem->encode( p, buf+buflen, lengths[i] );
        }
        buflen += lengths[i];
    }

    ifh->nrecords ++;
    ifh->offset += buflen;
    ifh->cs_bytes += buflen;
    if ( ifh->version == IPFIX_VERSION ) {
        ifh->seqno ++;
    }
    return 0;
}



int main ( int argc, char **argv )
{
    char      *optstr="hc:p:vstu";
    int       opt;
    char      chost[256];
    int       protocol = IPFIX_PROTO_TCP;
    int       j;
    uint32_t  bytes    = 1234;
    char      buf[31]  = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                           11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                           21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };

    ipfix_t           *ipfixh  = NULL;
    ipfix_template_t  *ipfixt  = NULL;
    int               sourceid = 12345;
    int               port     = IPFIX_PORTNO;
    int               verbose_level = 0;

    /* set default host */
    strcpy(chost, "localhost");

    /** process command line args
     */
    while( ( opt = getopt( argc, argv, optstr ) ) != EOF )
    {
	switch( opt )
	{
	  case 'p':
	    if ((port=atoi(optarg)) <0) {
		fprintf( stderr, "Invalid -p argument!\n" );
		exit(1);
	    }
            break;

	  case 'c':
            strcpy(chost, optarg);
	    break;

          case 's':
              protocol = IPFIX_PROTO_SCTP;
              break;

          case 't':
              protocol = IPFIX_PROTO_TCP;
              break;

          case 'u':
              protocol = IPFIX_PROTO_UDP;
              break;

          case 'v':
              verbose_level ++;
              break;

	  case 'h':
	  default:
              fprintf( stderr, "usage: %s [-hstuv] [-c collector] [-p portno]\n" 
                       "  -h               this help\n"
                       "  -c <collector>   collector address\n"
                       "  -p <portno>      collector port number (default=%d)\n"
                       "  -s               send data via SCTP\n"
                       "  -t               send data via TCP (default)\n"
                       "  -u               send data via UDP\n"
                       "  -v               increase verbose level\n\n",
                       argv[0], IPFIX_PORTNO  );
              exit(1);
	}
    }

    /** init loggin
     */
    mlog_set_vlevel( verbose_level );

    /** init lib 
     */
    if ( ipfix_init() <0) {
        fprintf( stderr, "cannot init ipfix module: %s\n", strerror(errno) );
        exit(1);
    }

    /** open ipfix exporter
     */
    if ( ipfix_open( &ipfixh, sourceid, IPFIX_VERSION ) <0 ) {
        fprintf( stderr, "ipfix_open() failed: %s\n", strerror(errno) );
        exit(1);
    }

    /** set collector to use
     */
    if ( ipfix_add_collector( ipfixh, chost, port, protocol ) <0 ) {
        fprintf( stderr, "ipfix_add_collector(%s,%d) failed: %s\n", 
                 chost, port, strerror(errno));
        exit(1);
    }

    /** get template
     */
    if ( ipfix_new_data_template( ipfixh, &ipfixt, 2 ) <0 ) {
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

    if ( ipfix_export( ipfixh, ipfixt, buf, &bytes ) <0 ) {
          fprintf( stderr, "ipfix_export() failed: %s\n", 
                     strerror(errno) );
          exit(1);
    }

    iobuf_t           *buf;
    _ipfix_write_hdr( ifh, buf );
    memcpy( buf->buffer+buf->buflen, ifh->buffer, ifh->offset );
    buf->buflen += ifh->offset;

    printf( "[%d] export some data ... ", j );
    fflush( stdout) ;

    if ( ipfix_export( ipfixh, ipfixt, buf, &bytes ) <0 ) {
         fprintf( stderr, "ipfix_export() failed: %s\n", 
                     strerror(errno) );
         exit(1);
    }



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
     */

    printf( "data exported.\n" );

    /** clean up
     */
    ipfix_delete_template( ipfixh, ipfixt );
    ipfix_close( ipfixh );
    ipfix_cleanup();
    exit(0);
}
