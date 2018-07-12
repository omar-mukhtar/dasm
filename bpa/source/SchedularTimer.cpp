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

FILE NAME	:	SchedularTimer.cpp

DESCRIPTION	:	This file implements SchedularTimer class, which specializes
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
			Include Files
-----------------------------------------------------------------------------*/

#include "bpa_globals.h"
#include "SchedularTimer.h"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

void CSchedularTimer::SetPeriod(const TInt32 aNewPeriod)
{
	iTimerPeriod = (aNewPeriod > 0) ? aNewPeriod : K_DEFAULT_TIME_PERIOD;
}

//-----------------------------------------------------------------------------

TInt32 CSchedularTimer::GetPeriod()
{
	return iTimerPeriod.Int();
}

//-----------------------------------------------------------------------------

CSchedularTimer::~CSchedularTimer()
{
	Cancel();
	// observer object has not to be deleted due to uses-a relationship
}

//-----------------------------------------------------------------------------

CSchedularTimer* CSchedularTimer::NewL(const TInt32 aTimerPeriod, MEventNotifier* aObvserver)
{
	CSchedularTimer* self = NewLC(aTimerPeriod, aObvserver);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CSchedularTimer* CSchedularTimer::NewLC(const TInt32 aTimerPeriod, MEventNotifier* aObvserver)
{
	CSchedularTimer* self = new (ELeave) CSchedularTimer(aTimerPeriod, aObvserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

//-----------------------------------------------------------------------------

void CSchedularTimer::ConstructL()
{
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
	After(iTimerPeriod);
}

//-----------------------------------------------------------------------------

void CSchedularTimer::RunL()
{
	if(KErrNone == iStatus.Int())
	{
		iObserver->EventHandler();
	}
	After(iTimerPeriod);
}

//-----------------------------------------------------------------------------

CSchedularTimer::CSchedularTimer(const TInt32 aTimerPeriod, MEventNotifier* aObvserver)
	: CTimer(EPriorityStandard)
{
	SetPeriod(aTimerPeriod);
	iObserver = aObvserver;
}

//-----------------------------------------------------------------------------
