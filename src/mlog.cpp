/*

mnslp_libipfix - a library which provides an implementation of the IPFIX Messages
       		 for flexible flow data support. IPFIX is the successor of NetFlow v9
	         (see http://www.ietf.org/dyn/wg/charter/ipfix-charter.html and
        	 RFC5101 and RFC5102 for details) 

Copyright (C) 2012-2014, all rights reserved by
- System and Computing Engineering, Universidad de los Andes

More information and contact:
https://www.uniandes.edu.co/

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
 */
/*
** mlog.c - loggin funcs
**
** Copyright System and Computing Engineering, Universidad de los Andes
**
** $Date: 2014-11-26 14:31:00 $
**
** $Revision: 0.1 $
**
** Remarks: 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>

// #include "misc.h"

/*------ defines ---------------------------------------------------------*/

/*------ stuctures -------------------------------------------------------*/

/*------ globals ---------------------------------------------------------*/

int         mlog_vlevel = 0;
FILE        *mlog_fp    = NULL;

static char tmpbuf[4100];      /* !! */

/*------ revision id -----------------------------------------------------*/

static char const cvsid[]="$Id: mlog.c 956 2008-10-14 10:40:12Z hir $";

/*------ export funcs ----------------------------------------------------*/

/*
 * Name         : debugf
 * Parameter    : > fmt, ...   varargs
 * Purpose      : print debug message
 * Return values: none
 */
void debugf ( char fmt[], ... )
{
#ifdef NODEBUG
    return;
#else
    va_list args;

    va_start(args, fmt);
    (void) vsnprintf( tmpbuf, sizeof(tmpbuf), fmt, args );
    va_end(args);

    fprintf ( stderr, "DEBUG <" );
    fprintf ( stderr, "%s", tmpbuf );
    fprintf ( stderr, ">\n" );
    fflush  ( stderr );
#endif
}

/* Name         : errorf
 * Parameter    : > fmt, ...   varargs
 * Purpose      : print error message
 * Return values: none
 */
void errorf ( char fmt[], ... )
{
    va_list args;

    va_start(args, fmt);
    (void) vsnprintf( tmpbuf, sizeof(tmpbuf), fmt, args );
    va_end(args);

    fprintf ( stderr, "%s", tmpbuf );
}

/* Name         : mlogf
 * Parameter    : > int        verbosity
 *                > fmt, ...   varargs
 * Purpose      : write log message
 * Return values: none
 */
void mlogf ( int vlevel, char fmt[], ... )
{
    va_list args;

    if ( ! mlog_fp )
        mlog_fp = stderr;
    if ( vlevel > mlog_vlevel )
        return;

    va_start(args, fmt);
    (void) vsnprintf( tmpbuf, sizeof(tmpbuf), fmt, args );
    va_end(args);

    fprintf( mlog_fp, "%s", tmpbuf );
    if ( mlog_vlevel > 1 )
        fflush( mlog_fp );
}

int mlog_open( char *logfile, char *prefix )
{
    if ( logfile && ((mlog_fp=fopen( logfile, "w+" )) ==NULL))
    {
        errorf( "[mlog_open] cannot open log file <%s>, <%s>!\n",
                logfile, strerror(errno) );
        mlog_fp = stderr;
        return -1;
    }

    return 0;
}

void mlog_close( void )
{
    if ( mlog_fp && (mlog_fp != stderr) )
        fclose( mlog_fp );
}

void mlog_set_vlevel( int vlevel )
{
    mlog_vlevel = vlevel;
}

