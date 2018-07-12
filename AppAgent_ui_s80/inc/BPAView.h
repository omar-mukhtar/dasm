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

FILE NAME	:	BPAView.h

DESCRIPTION	:	This file defines the view class in Symbian	Document/View style
				 application architecture for BPA application.

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

#ifndef __BPA_VIEW_H__
#define __BPA_VIEW_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <coemain.h>
#include <coecntrl.h>
#include <eiktxlbx.h>
#include <eiktxlbm.h>
#include <gdi.h>

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CBPAView
  */

class CBPAView : public CCoeControl
{
public:

/*!
  @function NewL

  @discussion Create a CBPAView object, which will draw itself to aRect
  @param aRect the rectangle this view will be drawn to
  @result a pointer to the created instance of CBPAView
  */
    static CBPAView* NewL(const TRect& aRect);

/*!
  @function NewLC

  @discussion Create a CBPAView object, which will draw itself to aRect
  @param aRect the rectangle this view will be drawn to
  @result a pointer to the created instance of CBPAView
  */
    static CBPAView* NewLC(const TRect& aRect);

/*!
  @function ~CBPAView

  @discussion Destroy the object and release all memory objects
  */
    ~CBPAView();

public:  // from CCoeControl

/*!
  @function CountComponentControls

  @discussion Return the number of component controls
  @result the number of component controls
  */
   TInt CountComponentControls() const;

private:

/*!
  @function ConstructL

  @discussion  Perform the second phase construction of a CBPAView object
  @param aRect the rectangle this view will be drawn to
  */
    void ConstructL(const TRect& aRect);

/*!
  @function CBPAView

  @discussion Perform the first phase of two phase construction
  */
    CBPAView();
    void Draw(const TRect& /* aRect */) const;

public:

/*!
	@function PrintMsg
	@discussion Prints a text message in client area of window
	*/
	
	void PrintMsg(const TDesC16&);
	void PrintMsg(const TDesC8&);
	
/*!
	@function PrintMsg
	@discussion Prints a text message in client area of window
	*/
	
	void CBPAView::AppendLine(const TDesC& aMsg);

private:	// data members
	
	CDesCArraySeg *iDataArray;
	const TInt MAX_CHAR_PER_LINE;
	const TInt MAX_LINES;
};


#endif // __BPA_VIEW_H__