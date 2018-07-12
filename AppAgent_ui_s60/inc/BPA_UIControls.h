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

FILE NAME	:	BPA_UIControls.h

DESCRIPTION	:	This file defines UI control classes, used in many places in UI.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	May 19, 2006

MODIFICATION:	May 19, 2006

HISTORY		:
				19-05-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __BPA_CONTOLS_H__
#define __BPA_CONTOLS_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <eikappui.h>
#include <eikdialg.h>
#include "BPA_Globals.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/

class CBPAUi;

/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

class CFileNameDialog
{

public:
	
	static TBool RunDlgLD(TDesC** aFileName, TMediaFileTypes& aFileType);
};

//-----------------------------------------------------------------------------

class CSendFileOptionsDialog : public CEikDialog
{

public:
	
	static TBool RunDlgLD(HBufC** aFrom, HBufC** aTo, HBufC** aSubject, const TDesC8& aLocalEID);
	
private:

	CSendFileOptionsDialog(HBufC** aFrom, HBufC** aTo, HBufC** aSubject, const TDesC8& aLocalEID);
	
	// from CEikDialog
	void PreLayoutDynInitL();
	TBool OkToExitL(TInt aKeyCode);
	
private:

	// uses
	HBufC** iFrom;
	HBufC** iTo;
	HBufC** iSubject;
	HBufC* iLocalEID;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


#endif	// __BPA_CONTOLS_H__
