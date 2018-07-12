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

FILE NAME	:	BPAApplication.h

DESCRIPTION	:	This file defines the top level class in Symbian Document/View
				style application architecture for BPA application.

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

#ifndef __BPA_APPLICATION_H__
#define __BPA_APPLICATION_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <eikapp.h>

/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CBPAApplication

  @discussion An instance of CBPAApplication is the application part of the
  AVKON application framework for the Bundle Protocol Agent application
  */

class CBPAApplication : public CEikApplication
{

public:  // from CEikApplication

/*!
  @function AppDllUid

  @discussion Returns the application DLL UID value
  @result the UID of this Application/Dll
*/

	TUid AppDllUid() const;

protected:  // from CEikApplication

/*!
  @function CreateDocumentL

  @discussion Create a CApaDocument object and return a pointer to it
  @result a pointer to the created document
*/

	CApaDocument* CreateDocumentL();

};

//-----------------------------------------------------------------------------

#endif // __BPA_APPLICATION_H__
