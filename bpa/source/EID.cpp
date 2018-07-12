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

FILE NAME	:	EID.cpp

DESCRIPTION	:	This file implements EID class, which manipulates EID's.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	January 30, 2006

MODIFICATION:	January 30, 2006

HISTORY		:
				30-01-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "eid.h"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

TUint16 CEID::Length() const
{
	return static_cast<TUint16>(iBuffScheme->Length() + iBuffSSP->Length());
}

//-----------------------------------------------------------------------------

TUint16 CEID::SchemeLength(void) const
{
	return static_cast<TUint16>(iBuffScheme->Length());
}

//-----------------------------------------------------------------------------

TUint16 CEID::SSPLength(void) const
{
	return static_cast<TUint16>(iBuffSSP->Length());
}

//-----------------------------------------------------------------------------

TDesC8& CEID::SchemeDes(void) const
{
	return *iBuffScheme;
}

//-----------------------------------------------------------------------------

TDesC8& CEID::SSPDes(void) const
{
	return *iBuffSSP;
}

//-----------------------------------------------------------------------------

void CEID::SetScheme(TDesC8* aScheme)
{
	delete iBuffScheme;
	iBuffScheme = static_cast<HBufC8*>(aScheme);
}

//-----------------------------------------------------------------------------

void CEID::SetSSP(TDesC8* aSSP)
{
	delete iBuffSSP;
	iBuffSSP = static_cast<HBufC8*>(aSSP);
}

//-----------------------------------------------------------------------------

CEID* CEID::NewL()
{
	CEID *self = NewLC();
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CEID* CEID::NewL(const TDesC8& aStrEID)
{
	CEID *self = NewLC(aStrEID);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CEID* CEID::NewL(const CEID* aEID)
{
	CEID *self = NewLC(aEID);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CEID* CEID::NewLC()
{
	CEID* self = new (ELeave) CEID();
    CleanupStack::PushL(self);
    return self;
}

//-----------------------------------------------------------------------------

CEID* CEID::NewLC(const TDesC8& aStrEID)
{
	CEID* self = new (ELeave) CEID();
    CleanupStack::PushL(self);
    self->ConstructL(aStrEID);
    return self;
}

//-----------------------------------------------------------------------------

CEID* CEID::NewLC(const CEID* aEID)
{
	CEID* self = new (ELeave) CEID();
    CleanupStack::PushL(self);
    self->ConstructL(aEID);
    return self;
}

//-----------------------------------------------------------------------------

CEID::~CEID()
{
	delete iBuffScheme;
	delete iBuffSSP;
}

//-----------------------------------------------------------------------------

CEID::CEID()
{
	iBuffScheme = NULL;
	iBuffSSP = NULL;
}

//-----------------------------------------------------------------------------

void CEID::ConstructL(const TDesC8& aStrEID)
{
	// parses the string to generate EID
	
	TInt posColon = aStrEID.Locate(':');
	
	if((KErrNotFound == posColon) || (0 == posColon) || (aStrEID.Size()-1 == posColon))
	{
		// Either no colon, or no Scheme or no SSP present
		User::Leave(KErrArgument);
	}
	
	iBuffScheme = aStrEID.Left(posColon).AllocL();
	iBuffSSP = aStrEID.Mid(posColon+1).AllocL();
}

//-----------------------------------------------------------------------------

void CEID::ConstructL(const CEID* aEID)
{
	iBuffScheme = aEID->SchemeDes().AllocL();
	iBuffSSP = aEID->SSPDes().AllocL();
}

//-----------------------------------------------------------------------------

TBool CEID::Compare(const TDesC8& aStrEID) const
{
	TInt posColon = aStrEID.Locate(':');
	
	if((KErrNotFound == posColon) || (0 == posColon) || (aStrEID.Size()-1 == posColon))
	{
		// Either no colon, or no Scheme or no SSP present
		return EFalse;
	}
	
	if(aStrEID.Left(posColon) != (*iBuffScheme))
		return EFalse;
	
	if(aStrEID.Mid(posColon+1) != (*iBuffSSP))
		return EFalse;
	
	return ETrue;
}

//-----------------------------------------------------------------------------

TBool CEID::Compare(const CEID* aEID) const
{
	if(*iBuffScheme != aEID->SchemeDes())
		return EFalse;
		
	if(*iBuffSSP != aEID->SSPDes())
		return EFalse;
	
	return ETrue;
}

//-----------------------------------------------------------------------------

TDesC8* CEID::EIDStrL() const
{
	TDesC8* str = EIDStrLC();
	CleanupStack::Pop(str);
	return str;
}

//-----------------------------------------------------------------------------

TDesC8* CEID::EIDStrLC() const
{
	TInt len = static_cast<TInt>(Length());
	HBufC8* str = HBufC8::NewLC(len+1);
	
	str->Des().Append(SchemeDes());
	str->Des().Append(':');
	str->Des().Append(SSPDes());
	
	return static_cast<TDesC8*>(str);
}

//-----------------------------------------------------------------------------
