// This file is part of DASM (DTN Applications for Symbian Mobile-phones).
// Copyright (C) 2006 Omar Mukhtar (omar.mukhtar@gmail.com)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
/******************************************************************************

FILE NAME	:	FNV.cpp

DESCRIPTION	:	This file implements FNV class to compute FNV hash.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	June 02, 2006

MODIFICATION:	June 02, 2006

HISTORY		:
				02-06-2006 : File created
				

******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "FNV.h"


/*-----------------------------------------------------------------------------
			Globals
-----------------------------------------------------------------------------*/

	
/*-----------------------------------------------------------------------------
			Class Implementations
-----------------------------------------------------------------------------*/

TUint32 FNV::FNV1a_Hash32(const TDesC8& aDataIn)
{
	TInt lp_indx;
	TUint32 hval;
	TUint8 tmpByte;
	
	hval = FNV1_INIT_32;
	
	for(lp_indx = 0; lp_indx < aDataIn.Size(); lp_indx++)
	{
		tmpByte = aDataIn[lp_indx];
		
		// xor the bottom with the current octet
		hval ^= tmpByte;
		
		// multiply by the 32 bit FNV magic prime mod 2^32
		// computer unsigned multiplication is always a mod. operation
		hval *= FNV_32_PRIME;
	}
	
	return hval;
}

//-----------------------------------------------------------------------------

/*TInt64 FNV::FNV1a_Hash64(const TDesC8& aDataIn)
{
}*/

//-----------------------------------------------------------------------------
	
