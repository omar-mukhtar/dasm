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

FILE NAME	:	BPRouter.h

DESCRIPTION	:	This file defines BPRouter class, which maintains and updates
				routing records. It also processes routing quries.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	May 12, 2006

MODIFICATION:	May 12, 2006

HISTORY		:
				22-05-2006 : File created
				

******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __BPROUTER_H__
#define __BPROUTER_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32base.h>
#include "BPA_Globals.h"
#include "EventNotifier.h"
#include "SchedularTimer.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CBPRouter

  @discussion
  
*/

class CBPRouter : public CBase, public MEventNotifier
{

/***** local data-types *****/

	/*! @var iConfigParams */
	const TGlobalConfigParams* const iConfigParams;

/***** member data *****/

private:

	/*! @var iRoutesFile*/
	TFileName iRoutesFile;
	
	/*! @var iRoutingRecordsQ */
	CArrayPtrSeg<CEIDLookUpRecord>* iRoutingRecordsQ;
	
	CSchedularTimer* iTimer;
	
/***** member functions *****/

public:

/*!
	@function PerformRouteQuery
	@discussion
	@param
	@result
*/

	void PerformRouteQuery(CEIDLookUpRecord& aRouteRecord);

/*!
	@function NewL
	@discussion Construct an object of class
	@param
	@result
*/

	static CBPRouter* NewL(const TInt32 aTimerPeriod, const TFileName& aRoutesFile,
						   const TGlobalConfigParams& aConfig);

/*!
	@function NewLC
	@discussion Construct an object of class
	@param
	@result
*/

	static CBPRouter* NewLC(const TInt32 aTimerPeriod, const TFileName& aRoutesFile,
							const TGlobalConfigParams& aConfig);

/*!
	@function ~CBPRouter
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	~CBPRouter();

private:

/*!
	@function EventHandler
	@discussion from MEventNotifier
	@param
	@result
*/

	void EventHandler(TAny* aLPVOID = NULL);

/*!
	@function ConstructL
	@discussion 2nd phase constructor
	@param
	@result
*/

	void ConstructL(const TInt32 aTimerPeriod, const TFileName& aRoutesFile);

/*!
	@function CBPRouter
	@discussion default constructor
	@param
	@result
*/

	CBPRouter(const TGlobalConfigParams& aConfig);

/*!
	@function UpdateRoutingRecords
	@discussion Updates routing records, a heavey operation
	@param
	@result
*/

	void UpdateRoutingRecordsL();
	
	void AddEntryL(TPtr8 aEntry);
	
	TInt FindEntry(const TDesC8* aValue);
	
};

#endif	// __BPROUTER_H__