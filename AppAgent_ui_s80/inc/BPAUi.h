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

FILE NAME	:	BPAUi.h

DESCRIPTION	:	This file defines the user interface class in Symbian
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
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __BPA_UI_H__
#define __BPA_UI_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <eikappui.h>
#include "bpa.h"
#include "BPA_Globals.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/

class CBPAView;

/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CBPAUi
*/

class CBPAUi : public CEikAppUi
{

public:

/*!
  @function ConstructL

  @discussion Perform the second phase construction of a CBPAUi object
  this needs to be public due to the way the framework constructs the AppUi
*/

	void ConstructL();

/*!
  @function CBPAUi

  @discussion Perform the first phase of two phase construction.
  This needs to be public due to the way the framework constructs the AppUi
*/

	CBPAUi();

/*!
  @function ~CBPAUi

  @discussion Destroy the object and release all memory objects
*/

	~CBPAUi();

public: // from CAknAppUi

/*!
  @function HandleCommandL

  @discussion Handle user menu selections
  @param aCommand the enumerated code for the option selected
*/

	void HandleCommandL(TInt aCommand);

/*!
  @function DynInitMenuPaneL

  @discussion Which of the receiver operations are available and set menu
  accordingly
  @param aResourceId resource ID of the menu pane to be initialised
  @param aMenuPane the in-memory representation of the menu pane
*/

	void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
	
	void SendFileL();
	void ReceiveFileL();
	
	void PrintBTAddr();
	
	TBool ParseMIMEData(const TDesC8* aDataIn, TDesC8*& aHeaderOut, TDesC8*& aDataOut,
					   TMediaFileTypes& aFileTypeOut);
	
	void PrepareFileHeaderL(const TMediaFileTypes aFileType, const TUint aFileSize,
							TDesC8*& aHdrOut, TDesC8*& aDestOut);

private:

/*! @var iAppView The application view */

	CBPAView* iAppView;
	CBPA *iBPA;
	TUint number;
	TGlobalConfigParams iConfigParams;
};

//-----------------------------------------------------------------------------

#endif // __BPA_UI_H__
