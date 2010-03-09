/***************************************************************************
*                        Bit File Class Usage Sample
*
*   File    : sample.c
*   Purpose : Demonstrates usage of bit file class.
*   Author  : Michael Dipperstein
*   Date    : July 20, 2004
*
****************************************************************************
*   HISTORY
*
*   $Id: sample.cpp,v 1.3 2006/02/10 04:30:47 michael Exp $
*   $Log: sample.cpp,v $
*   Revision 1.3  2006/02/10 04:30:47  michael
*   Applied fix for error discovered by Peter Husemann
*   <peter.husemann (at) cebitec (dot) uni-bielefeld (dot) de>.
*   When GetBit() reads a 0xFF byte, it would mistake it for EOF.
*
*   Revision 1.2  2005/12/10 05:20:01  michael
*   Added methods to get/put bits from/to integer types.
*
*   Revision 1.1.1.1  2004/08/04 13:45:38  michael
*   bitfile class
*
****************************************************************************
*
* Sample: A bit file class sample usage program
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
#include <iostream>
#include "bitfile.h"

using namespace std;

/***************************************************************************
*                                 MACROS
***************************************************************************/
#define NUM_CALLS       5

/***************************************************************************
*                                FUNCTIONS
***************************************************************************/

/***************************************************************************
*   Function   : main
*   Description: This function demonstrates the usage of each of the bit
*                bit file functions.
*   Parameters : argc - the number command line arguements (not used)
*   Parameters : argv - array of command line arguements (not used)
*   Effects    : Writes bit file, reads back results, printing them to
*                stdout.
*   Returned   : EXIT_SUCCESS
***************************************************************************/
int main(int argc, char *argv[])
{
    bit_file_c bf;
    int i, value;

    /* open bit file for writing */
    try
    {
        bf.Open("testfile", BF_WRITE);
    }
    catch (const char *errorMsg)
    {
        cout << errorMsg << endl;
        return (EXIT_FAILURE);
    }
    catch (...)
    {
        cout << "Unknown error opening file" << endl;
        return (EXIT_FAILURE);
    }


    /* set format for cout */
    cout << uppercase << hex;

    /* write chars */
    value = (int)'A';
    for (i = 0; i < NUM_CALLS; i++)
    {
        cout << "writing char " << (char)value << endl;

        if (bf.PutChar(value) == EOF)
        {
            cerr << "Error: writing char" << endl;
            bf.Close();
            return (EXIT_FAILURE);
        }

        value++;
    }

    /* write single bits */
    value = 1;
    for (i = 0; i < NUM_CALLS; i++)
    {
        cout << "writing bit " << value << endl;
        if(bf.PutBit(value) == EOF)
        {
            cerr << "Error: writing bit" << endl;
            bf.Close();
            return (EXIT_FAILURE);
        }

        value = !value;
    }

    /* write ints as bits */
    value = 0x11111111;
    for (i = 0; i < NUM_CALLS; i++)
    {
        cout << "writing bits " << value << endl;

        if(bf.PutBits(&value, (8 * sizeof(int))) == EOF)
        {
            cerr << "Error: writing bits" << endl;
            bf.Close();
            return (EXIT_FAILURE);
        }

        value += 0x11111111;
    }

    /* write some bits from an integer */
    value = 0x111;
    for (i = 0; i < NUM_CALLS; i++)
    {
        cout << "writing 12 bits from an integer " << value << endl;
        if(bf.PutBitsInt(&value, 12, sizeof(value)) == EOF)
        {
            cerr << "Error: writing bits from an integer" << endl;
            bf.Close();
            return (EXIT_FAILURE);
        }

        value += 0x111;
    }

    bf.Close();

    /* now read back writes */

    /* open bit file for reading */
    try
    {
        bf.Open("testfile", BF_READ);
    }
    catch (const char *errorMsg)
    {
        cout << errorMsg << endl;
        return (EXIT_FAILURE);
    }
    catch (...)
    {
        cout << "Unknown error opening file" << endl;
        return (EXIT_FAILURE);
    }

    /* read chars */
    for (i = 0; i < NUM_CALLS; i++)
    {
        value = bf.GetChar();
        if(value == EOF)
        {
            cerr << "Error: reading char" << endl;
            bf.Close();
            return (EXIT_FAILURE);
        }
        else
        {
            cout << "read char " << (char)value << endl;
        }
    }

    /* read single bits */
    for (i = 0; i < NUM_CALLS; i++)
    {
        value = bf.GetBit();
        if(value == EOF)
        {
            cerr << "Error: reading bit" << endl;
            bf.Close();
            return (EXIT_FAILURE);
        }
        else
        {
            cout << "read bit " << value << endl;
        }
    }

    /* read ints as bits */
    for (i = 0; i < NUM_CALLS; i++)
    {
        if(bf.GetBits(&value, (8 * sizeof(int))) == EOF)
        {
            cerr << "Error: reading bits" << endl;
            bf.Close();
            return (EXIT_FAILURE);
        }
        else
        {
            cout << "read bits " << value << endl;
        }
    }

    /* read some bits into an integer */
    for (i = 0; i < NUM_CALLS; i++)
    {
        value = 0;
        if(bf.GetBitsInt(&value, 12, sizeof(value)) == EOF)
        {
            cerr << "Error: reading bits from an integer" << endl;
            bf.Close();
            return (EXIT_FAILURE);
        }
        else
        {
            cout << "read 12 bits into an integer " << value << endl;
        }
    }

    return(EXIT_SUCCESS);
}
