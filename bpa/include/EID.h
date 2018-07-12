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

FILE NAME	:	EID.h

DESCRIPTION	:	This file defines EID class, which manipulates EID's.

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

#ifndef __EID_H__
#define __EID_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32base.h>

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CEID

  @discussion
  
*/

class CEID : public CBase
{

/***** member data *****/

private:

	/*! @var iBuffScheme*/
	HBufC8 *iBuffScheme;
	
	/*! @var iBuffSSP*/
	HBufC8 *iBuffSSP;
	

/***** member functions *****/

public:

/*!
	@function Length()
	@discussion returns total length of EID
	@param
	@result
*/

	TUint16 Length() const;

/*!
	@function SchemeLength()
	@discussion
	@param
	@result
*/

	TUint16 SchemeLength(void) const;

/*!
	@function SSPLength()
	@discussion
	@param
	@result
*/

	TUint16 SSPLength(void) const;

/*!
	@function SchemeDes()
	@discussion
	@param
	@result
*/

	TDesC8& SchemeDes(void) const;

/*!
	@function SSPDes()
	@discussion
	@param
	@result
*/

	TDesC8& SSPDes(void) const;

/*!
	@function SetScheme()
	@discussion
	@param
	@result
*/

	void SetScheme(TDesC8* aScheme);


/*!
	@function SetSSP()
	@discussion
	@param
	@result
*/

	void SetSSP(TDesC8* aSSP);

public:

/*!
	@function NewL
	@discussion Construct an object of class
	@param
	@result
*/

	static CEID* NewL();

	static CEID* NewL(const TDesC8& aStrEID);
	
	static CEID* NewL(const CEID* aEID);

/*!
	@function NewLC
	@discussion Construct an object of class
	@param
	@result
*/

	static CEID* NewLC();

	static CEID* NewLC(const TDesC8& aStrEID);

	static CEID* NewLC(const CEID* aEID);

/*!
	@function ~CSDNV
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	~CEID();

/*!
	@function Compare()
	@discussion Compares this EID with another EID
	@param
	@result
*/

	TBool Compare(const TDesC8& aStrEID) const;
	
	TBool Compare(const CEID* aEID) const;

/*!
	@function EIDStrLC()
	@discussion generates a complete URI string and transfers ownership
	@param
	@result
*/

	TDesC8* EIDStrLC() const;
	
/*!
	@function EIDStrL()
	@discussion generates a complete URI string and transfers ownership
	@param
	@result
*/

	TDesC8* EIDStrL() const;



private:

/*!
	@function CSDNV
	@discussion constructs the object and allocates memory to member automatic
	objects.
	@param
	@result
*/

	CEID();

/*!
	@function ConstructL
	@discussion Performs second phase construction of this object
	@param aMessage the message to be sent to the remote machine
	@result
*/

	void ConstructL(const TDesC8& aStrEID);
	
	void ConstructL(const CEID* aEID);

};


#endif // __EID_H__

