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
/*******************************************************************************

FILE NAME	:	SDNV.h

DESCRIPTION	:	This file defines SDNV class, which manipulates Self-Deliminating
				Numeric Values.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	January 08, 2006

MODIFICATION:	January 08, 2006

HISTORY		:
				08-01-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __SDNV_H__
#define __SDNV_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32base.h>
#include <s32mem.h>

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CSDNV

  @discussion
  
*/

class CSDNV : public CBase
{

/***** member data *****/

private:

	/*! @var iNumericVal */
	TUint32 iNumericVal;
	
	/*! @var iTempBuff */
	HBufC8* iTempBuff;
	
	/*! @var iTempArray */
	// stores bit strings temporarily; should be reset after use
	CArrayFixFlat<TUint8> *iTempArray;

/***** member functions *****/

public:

/*!
	@function ExtractSdnvL()
	@discussion
	@param
	@result
*/

	void ExtractSDNValL(RDesReadStream& aStream);


/*!
	@function GetNumericVal()
	@discussion
	@param
	@result
*/

	TUint32 GetNumericVal() const;

/*!
	@function SetNumericVal()
	@discussion
	@param
	@result
*/

	void SetNumericVal(const TUint32 aVal);

/*!
	@function BitStrLen()
	@discussion calculates the totoal bits needed for SDNV string
	@param
	@result
*/

	TUint32 BitStrLen();

/*!
	@function ByteStrLen()
	@discussion calculates the totoal bits needed for SDNV string
	@param
	@result
*/

	TUint32 ByteStrLen();

/*!
	@function Update()
	@discussion
	@param
	@result
*/

	void UpdateL(const TUint8 aByte);


/*!
	@function DesL()
	@discussion
	@param
	@result
*/

	const TDesC8& DesL();

/*!
	@function NewL
	@discussion Construct an object of class
	@param
	@result
*/

	static CSDNV* NewL(const TUint32 aInitVal = 0);

/*!
	@function NewLC
	@discussion Construct an object of class
	@param
	@result
*/

	static CSDNV* NewLC(const TUint32 aInitVal = 0);

/*!
	@function ~CSDNV
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	~CSDNV();

private:

/*!
	@function ComputeNum()
	@discussion This function must not be be externally.
	As it uses internal resource, which may be unallocated at times.
	@param
	@result
*/

	void ComputeNum();

/*!
	@function ConstructL
	@discussion second phase constructer
	objects.
	@param
	@result
*/

	void ConstructL();

/*!
	@function CSDNV
	@discussion constructs the object and allocates memory to member automatic
	objects.
	@param
	@result
*/

	CSDNV(const TUint32 aInitVal = 0);

};


#endif // __SDNV_H__

