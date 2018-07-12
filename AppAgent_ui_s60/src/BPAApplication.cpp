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

FILE NAME	:	BPAApplication.cpp

DESCRIPTION	:	This file implements the top level class in Symbian
				Document/View style application architecture for BPA
				application. It also contains the DLL entry point functions.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	December 28, 2005

MODIFICATION:	December 28, 2005

HISTORY		:
				28-12-2005 : File created
				

******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "BPAApplication.h"
#include "BPADocument.h"


/*-----------------------------------------------------------------------------
			Global Functions
-----------------------------------------------------------------------------*/

// Symbian DLL entry point, return that everything is ok

GLDEF_C TInt E32Dll(TDllReason)
{
	return KErrNone;
}

//-----------------------------------------------------------------------------

// Create a BPA application, and return a pointer to it

EXPORT_C CApaApplication* NewApplication()
{
    return (new CBPAApplication);
}

/*-----------------------------------------------------------------------------
			Globals / Constants
-----------------------------------------------------------------------------*/

// TODO: need to get new valid ID from ForumNokia?
const TUid K_BPA_v_1_0_ID = {0x10005B8B};

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

CApaDocument* CBPAApplication::CreateDocumentL()
{
	// Create an BTPointToPoint document, and return a pointer to it
	
	CApaDocument* document = CBPADocument::NewL(*this);
	return document;
}

//-----------------------------------------------------------------------------

TUid CBPAApplication::AppDllUid() const
{
    // Return the UID for the BTPointToPoint application
    return K_BPA_v_1_0_ID;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
