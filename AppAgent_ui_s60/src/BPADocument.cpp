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

FILE NAME	:	BPADocument.cpp

DESCRIPTION	:	This file implements the document class in Symbian
				Document/View style application architecture for BPA
				application.

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

#include "BPAUi.h"
#include "BPADocument.h"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

// Standard Symbian construction sequence

CBPADocument* CBPADocument::NewL(CEikApplication& aApp)
{
	CBPADocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CBPADocument* CBPADocument::NewLC(CEikApplication& aApp)
{
	CBPADocument* self = new (ELeave) CBPADocument(aApp);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

//-----------------------------------------------------------------------------

void CBPADocument::ConstructL()
{
	// no implementation required
}

//-----------------------------------------------------------------------------

CBPADocument::CBPADocument(CEikApplication& aApp)
: CEikDocument(aApp)
{
	// no implementation required
}

//-----------------------------------------------------------------------------

CBPADocument::~CBPADocument()
{
	// no implementation required
}

//-----------------------------------------------------------------------------

CEikAppUi* CBPADocument::CreateAppUiL()
{
	// Create the application user interface, and return a pointer to it
	
	CEikAppUi* appUi = new (ELeave) CBPAUi;
	return appUi;
}

//-----------------------------------------------------------------------------
