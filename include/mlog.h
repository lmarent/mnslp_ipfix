/*
** mnslp.h - export declarations for log funcs
**
** Copyright System and Computing Engineering, Universidad de los Andes
**
** $Id: mlog.h 22 2014-11-26 14:31:00 $
**
** Remarks: This is experimental code!
**
*/
#ifndef _MLOG_H
#define _MLOG_H

#include <stdarg.h>

#ifdef   __cplusplus
extern "C" {
#endif

extern int mlog_vlevel;

void errorf ( char fmt[], ... ) __attribute__ ((format (printf, 1, 2)));
void debugf ( char fmt[], ... ) __attribute__ ((format (printf, 1, 2)));
void mlogf  ( int verbosity,
              char fmt[], ... ) __attribute__ ((format (printf, 2, 3)));
int  mlog_open  ( char *logfile, char *prefix );
void mlog_close ( void );
void mlog_set_vlevel( int vlevel );

#ifdef   __cplusplus
}
#endif
#endif 

