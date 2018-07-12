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

FILE NAME	:	BPA.h

DESCRIPTION	:	This file defines BPA class, which handles the core
				functionality of bundle protocol.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	May 12, 2006

MODIFICATION:	May 12, 2006

HISTORY		:
				12-05-2006 : File created
				

******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __BPA_H__
#define __BPA_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32base.h>
#include "TCPCLA.h"
#include "BTRFCommCLA.h"
#include "bundle.h"
#include "BPA_Globals.h"
#include "BPRouter.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CBPA

  @discussion
  
*/

class CBPA : public CBase, public MEventNotifier
{

/***** local data-types *****/

private:

	enum TBPABundleStatus
	{
		E_BPA_DATA_FROZEN,
		E_BPA_DATA_NEW_OUTGOING,
		E_BPA_DATA_DEST_RESOLVING,
		E_BPA_DATA_PROCESSING,
		E_BPA_DATA_TO_BE_DISCARDED,
		E_BPA_DATA_TO_BE_DELETED,
		E_BPA_DATA_TRANSMITTING,
		E_BPA_DATA_TRANSMITTED_OK,
		E_BPA_DATA_TRANSMITTED_NOK,
		E_BPA_DATA_CUSTODY_ACCEPTED,	// when all other transitions are over (i.e. successfully transmitted)
		E_BPA_DATA_NEW_INCOMING,
		E_BPA_DATA_RECEIVED
	};
	
	struct TBPInterfaceRecord
	{
		TAppAgentMsgTypes iMsgType;
		TUint32 iAppTag;
		TDesC8* iPayloadBuffer;
		TDesC8* iDestEID;
		TUint32 iAABundleTxOptions;
		TUint32 iLifeTime;
		TUint8 iBSRRFlags;
		
		TBPInterfaceRecord()
		{
			iMsgType = E_AA_NONE;
			iAppTag = 0;
			iPayloadBuffer = NULL;
			iDestEID = NULL;
			iAABundleTxOptions = K_AABTR_NONE;
		}
		
		void FreeMem()
		{
			delete iPayloadBuffer;
			iPayloadBuffer = NULL;
			delete iDestEID;
			iDestEID = NULL;
		}
		
		~TBPInterfaceRecord()
		{
			FreeMem();
		}
	};
	
	
	class CBPARecord : public CBase
	{
	
	public:
	
		TUint32 iAppTag;
		TBPABundleStatus iRecordStatus;
		CBundle* iBundle;
		CEIDLookUpRecord *iResolverRecord;
		TUint16 iRetentionConstraints;
		TTime iLastRetryInstance;
		
		CBPARecord()
		{
			iAppTag = 0;
			iBundle = NULL;
			iResolverRecord = NULL;
			iRetentionConstraints = K_RC_NONE;
			iLastRetryInstance = TInt64(0);
		}
		
		~CBPARecord()
		{
			delete iBundle;
			iBundle = NULL;
			delete iResolverRecord;
			iResolverRecord = NULL;
		}
	};
	
	struct TCLARecord
	{
		TCLAType iCLAType;
		CCLA* iPtrCLA;
		
		TCLARecord()
		{
			iPtrCLA = NULL;
		}
		
		void FreeMem()
		{
			delete iPtrCLA;
			iPtrCLA = NULL;
		}
		
		~TCLARecord()
		{
			FreeMem();
		}
	};
	
	

/***** member data *****/

private:

	/*! @var iFragmentationPolicy*/
	TBool iFragmentationPolicy;
	
	/*! @var iCustodyAcceptancePolicy*/
	TBool iCustodyAcceptancePolicy;
	
	/*! @var iHeaderErrorStatusPolicy*/
	TBool iHeaderErrorStatusPolicy;
	
	/*! @var iHeaderErrorDiscardPolicy*/
	TBool iHeaderErrorDiscardPolicy;
	
	/*! @var iCOSPriority*/
	TUint8 iCOSPriority;
	
	/*! @var iSRRBundleReception*/
	TBool iSRRBundleReception;
	
	/*! @var iSRRCustodyAcceptance*/
	TBool iSRRCustodyAcceptance;
	
	/*! @var iSRRBundleForwarding*/
	TBool iSRRBundleForwarding;
	
	/*! @var iSRRBundleDelivery*/
	TBool iSRRBundleDelivery;
	
	/*! @var iSRRBundleDeletion*/
	TBool iSRRBundleDeletion;
	
	/*! @var iSRRAppAck*/
	TBool iSRRAppAck;
	
	/*! @var iLocalEID*/
	HBufC8* iLocalEID;
	
	/*! @var iReportToEID*/
	HBufC8* iReportToEID;
	
	/*! @var iDefaultLifeTime*/
	TUint32 iDefaultLifeTime;
	
	/*! @var iTxQ */
	CArrayFixSeg<TBPInterfaceRecord>* iTxQ;
	
	/*! @var iRxQ */
	CArrayFixSeg<TBPInterfaceRecord>* iRxQ;
	
	/*! @var iProcessingQ */
	CArrayPtrSeg<CBPARecord>* iProcessingQ;
	
	/*! @var iCLAQ */
	CArrayFixSeg<TCLARecord>* iCLAQ;
	
	/*! @var iTimer */
	CSchedularTimer* iTimer;
	
	/*! @var iTCPListeningPort */
	TUint16 iTCPListeningPort;
	
	/*! @var iBTListeningChannel */
	TUint8 iBTListeningChannel;
	
	CBPRouter* iBPRouter;
	
	/*! @var iConfigParams */
	const TGlobalConfigParams* const iConfigParams;
	
	/*! @var iBTListeningChannel */
	TTimeIntervalSeconds iBundleTxRetryInterval;
	

/***** member functions *****/

public:

/*!
	@function Push()
	@discussion Pushes a new outgoing payload in TxQ
	@param
	@result
*/

	TBool Push(TDesC8* aDataIn, TDesC8* aDestEID, const TUint32 aID,
			   TUint32 aAABundleTxOptions, TUint8 aBSRRFlags, TUint32 aLifeTime);

/*!
	@function Pop()
	@discussion Pops a new incoming payload from RxQ
	@param
	@result
*/

	TBool Pop(TAppAgentMsgTypes& aMsgTypeOut, TDesC8*& aDataOut, TUint32& aIDOut);

/*!
	@function StartCLAL()
	@discussion Starts a requested CLA service
	@param
	@result
*/

	void StartCLAL(const TCLAType aCLAType);

/*!
	@function GetFragmentationPolicy()
	@discussion
	@param
	@result
*/

	TBool GetFragmentationPolicy();

/*!
	@function SetFragmentationPolicy()
	@discussion
	@param
	@result
*/

	void SetFragmentationPolicy(const TBool aFlag);

/*!
	@function GetCustodyAcceptancePolicy()
	@discussion
	@param
	@result
*/

	TBool GetCustodyAcceptancePolicy();

/*!
	@function SetCustodyAcceptancePolicy()
	@discussion
	@param
	@result
*/

	void SetCustodyAcceptancePolicy(const TBool aFlag);

/*!
	@function GetHeaderErrorStatusPolicy()
	@discussion
	@param
	@result
*/

	TBool GetHeaderErrorStatusPolicy();

/*!
	@function SetHeaderErrorStatusPolicy()
	@discussion
	@param
	@result
*/

	void SetHeaderErrorStatusPolicy(const TBool aFlag);

/*!
	@function GetHeaderErrorDiscardPolicy()
	@discussion
	@param
	@result
*/

	TBool GetHeaderErrorDiscardPolicy();

/*!
	@function SetHeaderErrorDiscardPolicy()
	@discussion
	@param
	@result
*/

	void SetHeaderErrorDiscardPolicy(const TBool aFlag);

/*!
	@function GetCOSPriority()
	@discussion
	@param
	@result
*/

	TUint8 GetCOSPriority();

/*!
	@function SetCOSPriority()
	@discussion
	@param
	@result
*/

	void SetCOSPriority(const TUint8 aCOSPriority);

/*!
	@function GetSRRCustodyAcceptance()
	@discussion
	@param
	@result
*/

	TBool GetSRRCustodyAcceptance();

/*!
	@function SetSRRCustodyAcceptance()
	@discussion
	@param
	@result
*/

	void SetSRRCustodyAcceptance(const TBool aFlag);

/*!
	@function GetSRRBundleForwarding()
	@discussion
	@param
	@result
*/

	TBool GetSRRBundleForwarding();

/*!
	@function SetSRRBundleForwarding()
	@discussion
	@param
	@result
*/

	void SetSRRBundleForwarding(const TBool aFlag);
	
/*!
	@function GetSRRBundleReception()
	@discussion
	@param
	@result
*/

	TBool GetSRRBundleReception();

/*!
	@function SetSRRBundleReception()
	@discussion
	@param
	@result
*/

	void SetSRRBundleReception(const TBool aFlag);

/*!
	@function GetSRRBundleDelivery()
	@discussion
	@param
	@result
*/

	TBool GetSRRBundleDelivery();

/*!
	@function SetSRRBundleDelivery()
	@discussion
	@param
	@result
*/

	void SetSRRBundleDelivery(const TBool aFlag);

/*!
	@function GetSRRBundleDeletion()
	@discussion
	@param
	@result
*/

	TBool GetSRRBundleDeletion();

/*!
	@function SetSRRBundleDeletion()
	@discussion
	@param
	@result
*/

	void SetSRRBundleDeletion(const TBool aFlag);

/*!
	@function GetSRRAppAck()
	@discussion
	@param
	@result
*/

	TBool GetSRRAppAck();

/*!
	@function SetSRRAppAck()
	@discussion
	@param
	@result
*/

	void SetSRRAppAck(const TBool aFlag);

/*!
	@function GetLocalEID()
	@discussion
	@param
	@result
*/

	const TDesC8& GetLocalEID();

/*!
	@function SetLocalEID()
	@discussion
	@param
	@result
*/

	TInt SetLocalEID(const TDesC8& aLocalEID);

/*!
	@function GetReportToEID()
	@discussion
	@param
	@result
*/

	const TDesC8& GetReportToEID();

/*!
	@function SetReportToEID()
	@discussion
	@param
	@result
*/

	TInt SetReportToEID(const TDesC8& aLocalEID);

/*!
	@function GetDefaultLifeTime()
	@discussion
	@param
	@result
*/

	TUint32 GetDefaultLifeTime();

/*!
	@function SetDefaultLifeTime()
	@discussion
	@param
	@result
*/

	void SetDefaultLifeTime(const TUint32 aDefaultLifeTime);

/*!
	@function DoBPATxOperations()
	@discussion
	@param
	@result
*/

	void DoBPATxOperations();

/*!
	@function DoBPARxOperations()
	@discussion
	@param
	@result
*/

	void DoBPARxOperations();

/*!
	@function DoBPAAuxillaryOperations()
	@discussion
	@param
	@result
*/

	void DoBPAAuxillaryOperations();

/*!
	@function NewL
	@discussion Construct an object of class
	@param
	@result
*/

	static CBPA* NewL(const TInt32 aTimerPeriod, const TGlobalConfigParams& aConfig);

/*!
	@function NewLC
	@discussion Construct an object of class
	@param
	@result
*/

	static CBPA* NewLC(const TInt32 aTimerPeriod, const TGlobalConfigParams& aConfig);

/*!
	@function ~CBPA
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	~CBPA();

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

	void ConstructL(const TInt32 aTimerPeriod);

/*!
	@function CBPA
	@discussion default constructor
	@param
	@result
*/

	CBPA(const TGlobalConfigParams& aConfig);

/*!
	@function SynthesizeBundleL
	@discussion from MEventNotifier
	@param
	@result
*/

	CBundle* SynthesizeBundleL(TBPInterfaceRecord& aRecordRef);

/*!
	@function ExecuteTxStateMachine
	@discussion
	@param
	@result
*/

	void ExecuteTxStateMachine(CBPARecord& aRecordRef);

/*!
	@function IsForwardingContraindicated
	@discussion
	@param
	@result
*/

	TBool IsForwardingContraindicated(CBPARecord& aRecordRef, TUint8& aReasonOut,
									  CCLA*& ptrCLAOut);

/*!
	@function IsBundleExpired
	@discussion
	@param
	@result
*/

	TBool IsBundleExpired(const CBundle* aBundle);
	
/*!
	@function FindSimilarBundle
	@discussion
	@param
	@result
*/

	TInt FindSimilarBundle(const CBundle* aBundle);
	
/*!
	@function HandoverNewBundle
	@discussion
	@param
	@result
*/

	void HandoverNewBundleForProcessingL(CBundle* aBundle, TUint32 aAppTag, TBool aIsOutGoing);
	
	// These functions carry out specific opreation as described in BPSPEC-04
	void Sec_4_2(CBPARecord& aBPARecord);
	void Sec_4_3(CBPARecord& aBPARecord);
	void Sec_4_4(CBPARecord& aBPARecord);
	void Sec_4_4_1(CBPARecord& aBPARecord, TUint8 aReasonCode);
	void Sec_4_4_2(CBPARecord& aBPARecord, TUint8 aReasonCode);
	void Sec_4_6(CBPARecord& aBPARecord);
	void Sec_4_7(CBPARecord& aBPARecord);
	void Sec_4_9(CBPARecord& aBPARecord);
	void Sec_4_10(CBPARecord& aBPARecord);
	void Sec_4_10_1(CBPARecord& aBPARecord);
	void Sec_4_10_2(CBPARecord& aBPARecord);
	void Sec_4_11(CBPARecord& aBPARecord);
	void Sec_4_12(CBPARecord& aBPARecord);
	void Sec_4_13(CBPARecord& aBPARecord, TUint8 aReasonCode);

/*!
	@function LoadConfigFileL()
	@discussion
	@param
	@result
*/

	void LoadConfigFileL(const TFileName& aConfigFileName);

/*!
	@function ProcessConfigParam()
	@discussion
	@param
	@result
*/
	void ProcessConfigParamL(TPtr8 aConfigParam);
	
/*!
	@function FindCLARef()
	@discussion
	@param
	@result
*/
	
	CCLA* FindCLARef(const TCLAType aCLAType);

/*!
	@function AcknowledgeBundleTxL()
	@discussion
	@param
	@result
*/

	void AcknowledgeBundleTxL(const TUint32 aBundleID, const TBool aSentStatus);
	
/*!
	@function AcknowledgeAppAgentPayloadTxL()
	@discussion
	@param
	@result
*/

	void AcknowledgeAppAgentPayloadTxL(CBPARecord& aBPARecord, const TBool aSentStatus);

/*!
	@function ExtractInformationInBundlePayloadL()
	@discussion
	@param
	@result
*/
	void ExtractInformationInBundlePayloadL(CBPARecord& aBPARecord, TDesC8*& aPayloadOut,
											TBool& aIsBSROut);
/*!
	@function GenerateBSRL()
	@discussion
	@param
	@result
*/
	void GenerateBSRL(CBPARecord& aBPARecord, TUint8 aSRRFlags, TUint8 aReasonCode);

/*!
	@function ComposeBSRL()
	@discussion
	@param
	@result
*/
	CAdminRecord* ComposeBSRL(CBPARecord& aBPARecord, TUint8 aSRRFlags, TUint8 aReasonCode);

/*!
	@function GenerateDTNTime()
	@discussion
	@param
	@result
*/
	TInt64 GenerateDTNTime(void);

/*!
	@function ComposeAdminRecordBundleL()
	@discussion
	@param
	@result
*/
	CBundle* ComposeAdminRecordBundleL(CBPARecord& aBPARecord, CAdminRecord* aAdminRecord);
	
/*!
	@function GenerateCustodySignalL()
	@discussion
	@param
	@result
*/
	void GenerateCustodySignalL(CBPARecord& aBPARecord, TBool aCustodySucceeded, TUint8 aReasonCode);

/*!
	@function ComposeCustodySignalL()
	@discussion
	@param
	@result
*/
	CAdminRecord* ComposeCustodySignalL(CBPARecord& aBPARecord, TBool aCustodySucceeded, TUint8 aReasonCode);

/*!
	@function HandleCustodySignal()
	@discussion
	@param
	@result
*/
	void HandleCustodySignal(CAdminRecord* aAdminRecord);
	
};


#endif	// __BPA_H__
