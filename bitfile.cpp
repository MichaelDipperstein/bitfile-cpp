/***************************************************************************
*                       Bit Stream File Class Implementation
*
*   File    : bitfile.cpp
*   Purpose : This file implements a simple class of I/O methods for files
*             that contain data in sizes that aren't integral bytes.  An
*             attempt was made to make the methods in this class analogous
*             to the methods provided to manipulate file streams.  The
*             methods contained in this class were created with compression
*             algorithms in mind, but may besuited to other applications.
*   Author  : Michael Dipperstein
*   Date    : July 20, 2004
*
****************************************************************************
*   UPDATES
*
*   $Id: bitfile.cpp,v 1.1.1.1 2004/08/04 13:45:38 michael Exp $
*   $Log: bitfile.cpp,v $
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

/***************************************************************************
*                             INCLUDED FILES
***************************************************************************/
#include "bitfile.h"

using namespace std;

/***************************************************************************
*                                 METHODS
***************************************************************************/

/***************************************************************************
*   Method     : bit_file_c - default constructor
*   Description: This is the default bit_file_c constructor.  It
*                initializes stream pointers to NULL and clears the bit
*                buffer.
*   Parameters : None
*   Effects    : Initializes private members.
*   Returned   : None
***************************************************************************/
bit_file_c::bit_file_c(void)
{
    m_InStream = NULL;
    m_OutStream = NULL;
    m_BitBuffer = 0;
    m_BitCount = 0;
    m_Mode = BF_NO_MODE;
}

/***************************************************************************
*   Method     : bit_file_c - constructor
*   Description: This is a bit_file_c constructor.  It opens an input or
*                output steram and clears the bit buffer.  An exception
*                will be thrown on error.
*   Parameters : fileName - NULL terminated string containing the name of
*                           the file to be opened.
*                mode - The mode of the file to be opened
*   Effects    : Initializes private members.  Creates and opens an input
*                or output stream.
*   Returned   : None
*   Exception  : "Error: Invalid File Type" - for unknown mode
*                "Error: Unable To Open File" - if stream cannot be opened
***************************************************************************/
bit_file_c::bit_file_c(const char *fileName, const BF_MODES mode)
{
    m_InStream = NULL;
    m_OutStream = NULL;
    m_BitBuffer = 0;
    m_BitCount = 0;

    switch (mode)
    {
        case BF_READ:
            m_InStream = new ifstream(fileName, ios::in | ios::binary);

            if (!m_InStream->good())
            {
                delete m_InStream;
                m_InStream = NULL;
            }
            else
            {
                m_Mode = mode;
            }
            break;

        case BF_WRITE:
            m_OutStream = new ofstream(fileName, ios::out | ios::binary);

            if (!m_OutStream->good())
            {
                delete m_OutStream;
                m_OutStream = NULL;
            }
            else
            {
                m_Mode = mode;
            }
            break;

        case BF_APPEND:
            m_OutStream =
                new ofstream(fileName, ios::out | ios::binary | ios::app);

            if (!m_OutStream->good())
            {
                delete m_OutStream;
                m_OutStream = NULL;
            }
            else
            {
                m_Mode = mode;
            }
            break;

        default:
            throw("Error: Invalid File Type");
            break;
    }

    /* make sure we opened a file */
    if ((m_InStream == NULL) && (m_OutStream == NULL))
    {
        throw("Error: Unable To Open File");
    }
}

/***************************************************************************
*   Method     : ~bit_file_c - destructor
*   Description: This is the bit_file_c destructor.  It closes and frees
*                any open file streams.  The bit buffer will be flushed
*                prior to closing an output stream.
*   Parameters : None
*   Effects    : Closes and frees open file streams.
*   Returned   : None
***************************************************************************/
bit_file_c::~bit_file_c(void)
{
    if (m_InStream != NULL)
    {
        m_InStream->close();
        delete m_InStream;
    }

    if (m_OutStream != NULL)
    {
        /* write out any unwritten bits */
        if (m_BitCount != 0)
        {
            m_BitBuffer <<= (8 - m_BitCount);
            m_OutStream->put(m_BitBuffer);
        }

        m_OutStream->close();
        delete m_InStream;
    }
}

/***************************************************************************
*   Method     : Open
*   Description: This method opens an input or output steram and initializes
*                the bit buffer.  An exception will be thrown on error.
*   Parameters : fileName - NULL terminated string containing the name of
*                           the file to be opened.
*                mode - The mode of the file to be opened
*   Effects    : Creates and opens an input or output stream.
*   Returned   : None
*   Exception  : "Error: File Already Open" - if object has an open file
*                "Error: Invalid File Type" - for unknown mode
*                "Error: Unable To Open File" - if stream cannot be opened
***************************************************************************/
void bit_file_c::Open(const char *fileName, const BF_MODES mode)
{
    /* make sure file isn't already open */
    if ((m_InStream != NULL) || (m_OutStream != NULL))
    {
        throw("Error: File Already Open");
    }

    switch (mode)
    {
        case BF_READ:
            m_InStream = new ifstream(fileName, ios::in | ios::binary);

            if (!m_InStream->good())
            {
                delete m_InStream;
                m_InStream = NULL;
            }
            else
            {
                m_Mode = mode;
            }

            m_BitBuffer = 0;
            m_BitCount = 0;
            break;

        case BF_WRITE:
            m_OutStream = new ofstream(fileName, ios::out | ios::binary);

            if (!m_OutStream->good())
            {
                delete m_OutStream;
                m_OutStream = NULL;
            }
            else
            {
                m_Mode = mode;
            }

            m_BitBuffer = 0;
            m_BitCount = 0;
            break;

        case BF_APPEND:
            m_OutStream =
                new ofstream(fileName, ios::out | ios::binary | ios::app);

            if (!m_OutStream->good())
            {
                delete m_OutStream;
                m_OutStream = NULL;
            }
            else
            {
                m_Mode = mode;
            }

            m_BitBuffer = 0;
            m_BitCount = 0;
            break;

        default:
            throw("Error: Invalid File Type");
            break;
    }

    /* make sure we opened a file */
    if ((m_InStream == NULL) && (m_OutStream == NULL))
    {
        throw("Error: Unable To Open File");
    }
}

/***************************************************************************
*   Method     : Close
*   Description: This method closes and frees any open file streams.  The
*                bit buffer will be flushed prior to closing an output
*                stream.  All member variables are re-initialized.
*   Parameters : None
*   Effects    : Closes and frees open file streams.  Resets member
*                variables.
*   Returned   : None
***************************************************************************/
void bit_file_c::Close(void)
{
    if (m_InStream != NULL)
    {
        m_InStream->close();
        delete m_InStream;

        m_InStream = NULL;
        m_BitBuffer = 0;
        m_BitCount = 0;
        m_Mode = BF_NO_MODE;
    }

    if (m_OutStream != NULL)
    {
        /* write out any unwritten bits */
        if (m_BitCount != 0)
        {
            m_BitBuffer <<= (8 - m_BitCount);
            m_OutStream->put(m_BitBuffer);
        }

        m_OutStream->close();
        delete m_OutStream;

        m_OutStream = NULL;
        m_BitBuffer = 0;
        m_BitCount = 0;
        m_Mode = BF_NO_MODE;
    }
}

/***************************************************************************
*   Method     : GetChar
*   Description: This method returns the next byte from the input stream.
*   Parameters : None
*   Effects    : Reads next byte from file and updates buffer accordingly.
*   Returned   : EOF if a whole byte cannot be obtained.  Otherwise,
*                the character read.
***************************************************************************/
int bit_file_c::GetChar(void)
{
    int returnValue, tmp;

    if (m_InStream == NULL)
    {
        return(EOF);
    }

    returnValue = m_InStream->get();

    if (m_BitCount == 0)
    {
        /* we can just get byte from file */
        return returnValue;
    }

    if (returnValue != EOF)
    {
        /* we have some buffered bits to return too */
        tmp = (returnValue >> m_BitCount);
        tmp |= m_BitBuffer << (8 - m_BitCount);

        /* put remaining in buffer. count shouldn't change. */
        m_BitBuffer = (char)returnValue;

        returnValue = tmp;
    }

    return returnValue;
}

/***************************************************************************
*   Method     : PutChar
*   Description: This method writes the byte passed as a parameter to the
*                output stream.
*   Parameters : c - the character to be written
*   Effects    : Writes a byte to the file and updates buffer accordingly.
*   Returned   : On success, the character written, otherwise EOF.
***************************************************************************/
int bit_file_c::PutChar(const int c)
{
    int tmp;

    if (m_OutStream == NULL)
    {
        return EOF;
    }

    if (m_BitCount == 0)
    {
        /* we can just put byte from file */
        m_OutStream->put(c);
        return c;
    }

    /* figure out what to write */
    tmp = (c & 0xFF) >> m_BitCount;
    tmp = tmp | ((m_BitBuffer) << (8 - m_BitCount));

    m_OutStream->put((char)tmp);    /* check for error */

    /* put remaining in buffer. count shouldn't change. */
    m_BitBuffer = (char)c;

    return tmp;
}

/***************************************************************************
*   Method     : GetBit
*   Description: This method returns the next bit from the input stream.
*                The bit value returned is the msb in the bit buffer.
*   Parameters : None
*   Effects    : Reads next bit from bit buffer.  If the buffer is empty,
*                a new byte will be read from the input stream.
*   Returned   : 0 if bit == 0, 1 if bit == 1, and EOF if operation fails.
***************************************************************************/
int bit_file_c::GetBit(void)
{
    char returnValue;

    if (m_InStream == NULL)
    {
        return EOF;
    }

    if (m_BitCount == 0)
    {
        /* buffer is empty, read another character */
        if ((returnValue = m_InStream->get()) == EOF)
        {
            return EOF;
        }
        else
        {
            m_BitCount = 8;
            m_BitBuffer = returnValue;
        }
    }

    /* bit to return is msb in buffer */
    m_BitCount--;
    returnValue = m_BitBuffer >> m_BitCount;

    return (returnValue & 0x01);
}

/***************************************************************************
*   Method     : PutBit
*   Description: This method writes the bit passed as a parameter to the
*                output stream.
*   Parameters : c - the bit value to be written
*   Effects    : Writes a bit to the bit buffer.  If the buffer has a byte,
*                the buffer is written to the output stream and cleared.
*   Returned   : On success, the bit value written, otherwise EOF.
***************************************************************************/
int bit_file_c::PutBit(const int c)
{
    int returnValue = c;

    if (m_OutStream == NULL)
    {
        return EOF;
    }

    m_BitCount++;
    m_BitBuffer <<= 1;

    if (c != 0)
    {
        m_BitBuffer |= 1;
    }

    /* write bit buffer if we have 8 bits */
    if (m_BitCount == 8)
    {
        m_OutStream->put(m_BitBuffer);    /* check for error */

        /* reset buffer */
        m_BitCount = 0;
        m_BitBuffer = 0;
    }

    return returnValue;
}

/***************************************************************************
*   Method     : GetBits
*   Description: This method reads the specified number of bits from the
*                input stream and writes them to the requested memory
*                location (msb to lsb).
*   Parameters : bits - address to store bits read
*                count - number of bits to read
*   Effects    : Reads bits from the bit buffer and file stream.  The bit
*                buffer will be modified as necessary.
*   Returned   : EOF for failure, otherwise the number of bits read.  If
*                an EOF is reached before all the bits are read, bits
*                will contain every bit through the last complete byte.
***************************************************************************/
int bit_file_c::GetBits(void *bits, const unsigned int count)
{
    char *bytes, shifts;
    int offset, remaining, returnValue;

    if ((m_InStream == NULL) || (bits == NULL))
    {
        return EOF;
    }

    bytes = (char *)bits;

    offset = 0;
    remaining = count;

    /* read whole bytes */
    while (remaining >= 8)
    {
        returnValue = this->GetChar();

        if (returnValue == EOF)
        {
            return EOF;
        }

        bytes[offset] = (char)returnValue;
        remaining -= 8;
        offset++;
    }

    /* read remaining bits */
    shifts = 8 - remaining;
    while (remaining > 0)
    {
        returnValue = this->GetBit();

        if (returnValue == EOF)
        {
            return EOF;
        }

        bytes[offset] <<= 1;
        bytes[offset] |= (returnValue & 0x01);
        remaining--;
    }

    /* shift last bits into position */
    bytes[offset] <<= shifts;

    return count;
}

/***************************************************************************
*   Method     : PutBits
*   Description: This method writes the specified number of bits from the
*                memory location passed as a parameter to the output
*                stream.   Bits are written msb to lsb.
*   Parameters : bits - pointer to bits to write
*                count - number of bits to write
*   Effects    : Writes bits to the bit buffer and file stream.  The bit
*                buffer will be modified as necessary.
*   Returned   : EOF for failure, otherwise the number of bits written.  If
*                an error occurs after a partial write, the partially
*                written bits will not be unwritten.
***************************************************************************/
int bit_file_c::PutBits(void *bits, const unsigned int count)
{
    char *bytes, tmp;
    int offset, remaining, returnValue;

    if ((m_OutStream == NULL) || (bits == NULL))
    {
        return EOF;
    }

    bytes = (char *)bits;

    offset = 0;
    remaining = count;

    /* write whole bytes */
    while (remaining >= 8)
    {
        returnValue = this->PutChar(bytes[offset]);

        if (returnValue == EOF)
        {
            return EOF;
        }

        remaining -= 8;
        offset++;
    }

    /* write remaining bits */
    tmp = bytes[offset];
    while (remaining > 0)
    {
        returnValue = this->PutBit(tmp & 0x80);

        if (returnValue == EOF)
        {
            return EOF;
        }

        tmp <<= 1;
        remaining--;
    }

    return count;
}

/***************************************************************************
*   Method     : eof
*   Description: This method indictaes whether or not the open file stream
*                is at the end of file.
*   Parameters : None
*   Effects    : None
*   Returned   : Returns true if the opened file stream is at an EOF.
*                Otherwise false is returned.
***************************************************************************/
bool bit_file_c::eof(void)
{
    if (m_InStream != NULL)
    {
        return (m_InStream->eof());
    }

    if (m_OutStream != NULL)
    {
        return (m_OutStream->eof());
    }

    /* return false for no file */
    return false;
}

/***************************************************************************
*   Method     : good
*   Description: This method is analogous to good for file streams.
*   Parameters : None
*   Effects    : None
*   Returned   : Returns good for the opened file stream.  False is
*                returned if there is no open file stream.
***************************************************************************/
bool bit_file_c::good(void)
{
    if (m_InStream != NULL)
    {
        return (m_InStream->good());
    }

    if (m_OutStream != NULL)
    {
        return (m_OutStream->good());
    }

    /* return false for no file */
    return false;
}

/***************************************************************************
*   Method     : bad
*   Description: This method is analogous to bad for file streams.
*   Parameters : None
*   Effects    : None
*   Returned   : Returns bad for the opened file stream.  False is
*                returned if there is no open file stream.
***************************************************************************/
bool bit_file_c::bad(void)
{
    if (m_InStream != NULL)
    {
        return (m_InStream->bad());
    }

    if (m_OutStream != NULL)
    {
        return (m_OutStream->bad());
    }

    /* return false for no file */
    return false;
}

