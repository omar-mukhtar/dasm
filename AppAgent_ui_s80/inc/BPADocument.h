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

FILE NAME	:	BPADocument.h

DESCRIPTION	:	This file defines the document class in Symbian Document/View
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

#ifndef __BPA_DOCUMENT_H__
#define __BPA_DOCUMENT_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <eikdoc.h>

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/

class CBPAUi;
//TODO: can't include header file for this?
class CEikApplication;

/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CBTPointToPointDocument

  @discussion An instance of class CBTPointToPointDocument is the Document part of the AVKON
  application framework for the BTPointToPoint example application
*/

class CBPADocument : public CEikDocument
{

public:

/*!
  @function NewL

  @discussion Construct a CBPADocument for the AVKON application aApp
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CBPADocument
*/

	static CBPADocument* NewL(CEikApplication& aApp);

/*!
  @function NewLC

  @discussion Construct a CBPADocument for the AVKON application aApp
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CBPADocument
*/

	static CBPADocument* NewLC(CEikApplication& aApp);

/*!
  @function ~CBPADocument

  @discussion Destroy the object and release all memory objects
*/

	~CBPADocument();

public: // from CEikDocument

/*!
  @function CreateAppUiL

  @discussion Create a CBPAUi object and return a pointer to it
  @result a pointer to the created instance of the AppUi created
*/

	CEikAppUi* CreateAppUiL();

private:

/*!
  @function ConstructL

  @discussion Perform the second phase construction of a CBPADocument object
*/

	void ConstructL();

/*!
  @function CBPADocument

  @discussion Perform the first phase of two phase construction
  @param aApp application creating this document
*/

	CBPADocument(CEikApplication& aApp);

};

//-----------------------------------------------------------------------------

#endif // __BPA_DOCUMENT_H__
