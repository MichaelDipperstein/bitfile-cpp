/***************************************************************************
*                       Bit Stream File Class Header
*
*   File    : bitfile.h
*   Purpose : Provides definitions and prototypes for a simple class of I/O
*             methods for files that contain data in sizes that aren't
*             integral bytes.  An attempt was made to make the methods in
*             this class analogous to the methods provided to manipulate
*             file streams.  The methods contained in this class were
*             created with compression algorithms in mind, but may be
*             suited to other applications.
*   Author  : Michael Dipperstein
*   Date    : July 20, 2004
*
****************************************************************************
*   UPDATES
*
*   $Id: bitfile.h,v 1.1.1.1 2004/08/04 13:45:38 michael Exp $
*   $Log: bitfile.h,v $
*   Revision 1.1.1.1  2004/08/04 13:45:38  michael
*   bitfile class
*
*
****************************************************************************
*
* Bitfile: Bit Stream File I/O Class
* Copyright (C) 2004 by Michael Dipperstein (mdipper@cs.ucsb.edu)
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
***************************************************************************/

#ifndef __BITFILE_H
#define __BITFILE_H

#include <iostream>
#include <fstream>

/***************************************************************************
*                            TYPE DEFINITIONS
***************************************************************************/
typedef enum
{
    BF_READ = 0,
    BF_WRITE = 1,
    BF_APPEND= 2,
    BF_NO_MODE
} BF_MODES;

class bit_file_c
{
    public:
        bit_file_c(void);
        bit_file_c(const char *fileName, const BF_MODES mode);
        virtual ~bit_file_c(void);

        /* open/close bit file */
        void Open(const char *fileName, const BF_MODES mode);
        void Close(void);

        /* get/put character */
        int GetChar(void);
        int PutChar(const int c);

        /* get/put single bit */
        int GetBit(void);
        int PutBit(const int c);

        /* get/put number of bits */
        int GetBits(void *bits, const unsigned int count);
        int PutBits(void *bits, const unsigned int count);

        /* status */
        bool eof(void);
        bool good(void);
        bool bad(void);

    private:
        std::ifstream *m_InStream;      /* input file stream pointer */
        std::ofstream *m_OutStream;     /* output file stream pointer */
        char m_BitBuffer;               /* bits waiting to be read/written */
        unsigned char m_BitCount;       /* number of bits in bitBuffer */
        BF_MODES m_Mode;                /* open for read, write, or append */
};

#endif  /* ndef __BITFILE_H */
