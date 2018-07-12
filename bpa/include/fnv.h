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

FILE NAME	:	FNV.h

DESCRIPTION	:	This file defines FNV class to compute FNV hash.

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
			Defines
-----------------------------------------------------------------------------*/

#ifndef __FNV_H__
#define __FNV_H__


/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32base.h>

/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class FNV

  @discussion
  
*/

class FNV
{

private:

/***** member data *****/

	const static TUint32 FNV1_INIT_32 = 0x811c9dc5;
	//const static TInt64  FNV1_INIT_64(0xcbf29ce4, 0x84222325);
	
	const static TUint32 FNV_32_PRIME = 0x01000193;
	//const static TUint32 FNV_64_PRIME(0x100, 0x000001b3);

/***** member functions *****/

public:
	static TUint32 FNV1a_Hash32(const TDesC8& aDataIn);
	//static TInt64 FNV1a_Hash64(const TDesC8& aDataIn);

};		

#endif	// __FNV_H__
