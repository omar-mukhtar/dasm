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

FILE NAME	:	SchedularTimer.h

DESCRIPTION	:	This file defines SchedularTimer class, which specializes
				CTimer class to generate scheduled events.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	April 2, 2006

MODIFICATION:	April 2, 2006

HISTORY		:
				02-04-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __SCHDULAR_TIMER_H__
#define __SCHDULAR_TIMER_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32base.h>
#include "EventNotifier.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CSchedularTimer

  @discussion
  
*/

class CSchedularTimer : public CTimer
{

/***** member data *****/

private:

	/*! @var iTimerPeriod */
	TTimeIntervalMicroSeconds32 iTimerPeriod;
	
	/*! @var iObserver */
	MEventNotifier* iObserver;


/***** member functions *****/

public:

/*!
	@function SetPeriod();
	@discussion
	@param
	@result
*/

	void SetPeriod(TInt32 aNewPeriod);

/*!
	@function SetPeriod();
	@discussion
	@param
	@result
*/

	TInt32 GetPeriod();

/*!
	@function ~CSchedularTimer
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	~CSchedularTimer();

/*!
	@function NewL
	@discussion constructs the object
	objects.
	@param
	@result
*/

	static CSchedularTimer* NewL(const TInt32 aTimerPeriod, MEventNotifier* aObvserver);

/*!
	@function NewLC
	@discussion constructs the object
	objects.
	@param
	@result
*/

	static CSchedularTimer* NewLC(const TInt32 aTimerPeriod, MEventNotifier* aObvserver);

protected:

/*!
	@function ConstructL
	@discussion 2nd-phase constructor to allocate resources for members
	objects. Must have to be called from derived classes
	@param
	@result
*/

	void ConstructL();

/*!
	@function RunL
	@discussion
	@param
	@result
*/

	void RunL();


private:

/*!
	@function CCLA
	@discussion constructs the object and allocates memory to member automatic
	objects.
	@param
	@result
*/

	CSchedularTimer(const TInt32 aTimerPeriod, MEventNotifier* aObvserver);


};

#endif // __SCHDULAR_TIMER_H__