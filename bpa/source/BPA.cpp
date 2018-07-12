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

FILE NAME	:	BPA.cpp

DESCRIPTION	:	This file implements BPA class, which handles the core
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
			Include Files
-----------------------------------------------------------------------------*/

#include <f32file.h>
#include <s32strm.h>
#include <s32file.h>
#include <libc\stdlib.h>
#include "bpa.h"

/*-----------------------------------------------------------------------------
			Globals
-----------------------------------------------------------------------------*/

_LIT8(K_NULL_EID, "dtn:none");
_LIT(K_Y2K, "20000000:000000.000000");
//_LIT(K_EPOCH, "19700000:000000.000000");
	
/*-----------------------------------------------------------------------------
			Class Implementations
-----------------------------------------------------------------------------*/

TBool CBPA::Push(TDesC8* aDataIn, TDesC8* aDestEID, const TUint32 aID,
				 TUint32 aAABundleTxOptions, TUint8 aBSRRFlags, TUint32 aLifeTime)
{
	if((NULL == aDataIn) || (NULL == aDestEID))
		return EFalse;
	
	TBPInterfaceRecord tempRecord;
	
	tempRecord.iMsgType = E_AA_NEW_PAYLOAD_TX;
	tempRecord.iAppTag = aID;
	tempRecord.iAABundleTxOptions = aAABundleTxOptions;
	tempRecord.iBSRRFlags = aBSRRFlags;
	tempRecord.iLifeTime = aLifeTime;
	
	TRAPD(error, iTxQ->AppendL(tempRecord));
	
	if(KErrNone == error)
	{
		iTxQ->At(iTxQ->Count()-1).iPayloadBuffer = aDataIn;
		iTxQ->At(iTxQ->Count()-1).iDestEID = aDestEID;
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}

//-----------------------------------------------------------------------------

TBool CBPA::Pop(TAppAgentMsgTypes& aMsgTypeOut, TDesC8*& aDataOut, TUint32& aIDOut)
{
	if(iRxQ->Count() < 1)
		return EFalse;
	
	aMsgTypeOut = iRxQ->At(0).iMsgType;
	aIDOut = iRxQ->At(0).iAppTag;
	aDataOut = iRxQ->At(0).iPayloadBuffer;
	
	iRxQ->At(0).iPayloadBuffer = NULL;
	iRxQ->At(0).FreeMem();
	iRxQ->Delete(0);
	
	return ETrue;
}

//-----------------------------------------------------------------------------

void CBPA::StartCLAL(const TCLAType aCLAType)
{
	CCLA* ptrCLA = NULL;
	TCLARecord tempCLARecord;
	
	if(E_TCP_CLA == aCLAType)
	{
		ptrCLA = FindCLARef(E_TCP_CLA);
		
		if(NULL != ptrCLA)
			User::Leave(KErrAlreadyExists);
		
		ptrCLA = CTCPCLA::NewLC(K_DEFAULT_TIME_PERIOD, 50, 0, 1, 1, *iConfigParams);
		
		tempCLARecord.iCLAType = aCLAType;
		iCLAQ->AppendL(tempCLARecord);
		iCLAQ->At(iCLAQ->Count()-1).iPtrCLA = ptrCLA;
		CleanupStack::Pop(ptrCLA);
				
		ptrCLA->StartListening(iTCPListeningPort);
		
	}
	
	else if(E_BT_RFCOMM_CLA == aCLAType)
	{
		ptrCLA = FindCLARef(E_BT_RFCOMM_CLA);
		
		if(NULL != ptrCLA)
			User::Leave(KErrAlreadyExists);
		
		ptrCLA = CBTRFCommCLA::NewLC(K_DEFAULT_TIME_PERIOD, 50, 0, 1, 1, *iConfigParams);
		
		tempCLARecord.iCLAType = aCLAType;
		iCLAQ->AppendL(tempCLARecord);
		iCLAQ->At(iCLAQ->Count()-1).iPtrCLA = ptrCLA;
		CleanupStack::Pop(ptrCLA);
				
		User::LeaveIfError(ptrCLA->StartListening(iBTListeningChannel));
	}
}

//-----------------------------------------------------------------------------

TBool CBPA::GetFragmentationPolicy()
{
	return iFragmentationPolicy;
}

//-----------------------------------------------------------------------------

void CBPA::SetFragmentationPolicy(const TBool aFlag)
{
	iFragmentationPolicy = aFlag;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetCustodyAcceptancePolicy()
{
	return iCustodyAcceptancePolicy;
}

//-----------------------------------------------------------------------------

void CBPA::SetCustodyAcceptancePolicy(const TBool aFlag)
{
	iCustodyAcceptancePolicy = aFlag;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetHeaderErrorStatusPolicy()
{
	return iHeaderErrorStatusPolicy;
}

//-----------------------------------------------------------------------------

void CBPA::SetHeaderErrorStatusPolicy(const TBool aFlag)
{
	iHeaderErrorStatusPolicy = aFlag;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetHeaderErrorDiscardPolicy()
{
	return iHeaderErrorDiscardPolicy;
}

//-----------------------------------------------------------------------------

void CBPA::SetHeaderErrorDiscardPolicy(const TBool aFlag)
{
	iHeaderErrorDiscardPolicy = aFlag;
}

//-----------------------------------------------------------------------------

TUint8 CBPA::GetCOSPriority()
{
	return iCOSPriority;
}

//-----------------------------------------------------------------------------

void CBPA::SetCOSPriority(const TUint8 aCOSPriority)
{
	iCOSPriority = aCOSPriority;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetSRRCustodyAcceptance()
{
	return iSRRCustodyAcceptance;
}

//-----------------------------------------------------------------------------

void CBPA::SetSRRCustodyAcceptance(const TBool aFlag)
{
	iSRRCustodyAcceptance = aFlag;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetSRRBundleForwarding()
{
	return iSRRBundleForwarding;
}

//-----------------------------------------------------------------------------

void CBPA::SetSRRBundleForwarding(const TBool aFlag)
{
	iSRRBundleForwarding = aFlag;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetSRRBundleReception()
{
	return iSRRBundleReception;
}

//-----------------------------------------------------------------------------

void CBPA::SetSRRBundleReception(const TBool aFlag)
{
	iSRRBundleReception = aFlag;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetSRRBundleDelivery()
{
	return iSRRBundleDelivery;
}

//-----------------------------------------------------------------------------

void CBPA::SetSRRBundleDelivery(const TBool aFlag)
{
	iSRRBundleDelivery = aFlag;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetSRRBundleDeletion()
{
	return iSRRBundleDeletion;
}

//-----------------------------------------------------------------------------

void CBPA::SetSRRBundleDeletion(const TBool aFlag)
{
	iSRRBundleDeletion = aFlag;
}

//-----------------------------------------------------------------------------

TBool CBPA::GetSRRAppAck()
{
	return iSRRAppAck;
}

//-----------------------------------------------------------------------------

void CBPA::SetSRRAppAck(const TBool aFlag)
{
	iSRRAppAck = aFlag;
}

//-----------------------------------------------------------------------------

const TDesC8& CBPA::GetLocalEID()
{
	return *iLocalEID;
}

//-----------------------------------------------------------------------------

TInt CBPA::SetLocalEID(const TDesC8& aLocalEID)
{
	delete iLocalEID;
	
	iLocalEID = HBufC8::New(aLocalEID.Size());
	
	if(NULL != iLocalEID)
	{
		iLocalEID->Des().Copy(aLocalEID);
		iLocalEID->Des().TrimAll();
		return KErrNone;
	}
	else
		return KErrNoMemory;
}

//-----------------------------------------------------------------------------

const TDesC8& CBPA::GetReportToEID()
{
	return *iReportToEID;
}

//-----------------------------------------------------------------------------

TInt CBPA::SetReportToEID(const TDesC8& aReportToEID)
{
	delete iReportToEID;
	
	iReportToEID = HBufC8::New(aReportToEID.Size());
	
	if(NULL != iReportToEID)
	{
		iReportToEID->Des().Copy(aReportToEID);
		iReportToEID->Des().TrimAll();
		return KErrNone;
	}
	else
		return KErrNoMemory;
}

//-----------------------------------------------------------------------------

TUint32 CBPA::GetDefaultLifeTime()
{
	return iDefaultLifeTime;
}

//-----------------------------------------------------------------------------

void CBPA::SetDefaultLifeTime(const TUint32 aDefaultLifeTime)
{
	iDefaultLifeTime = aDefaultLifeTime;
}

//-----------------------------------------------------------------------------

void CBPA::DoBPATxOperations()
{
	TInt lp_indx;
	
	for(lp_indx = 0; lp_indx < iProcessingQ->Count(); lp_indx++)
	{
		if(E_BPA_DATA_NEW_OUTGOING == iProcessingQ->At(lp_indx)->iRecordStatus)
		{
			TDesC8* ptrEidStr = NULL;
			TRAPD(error, ptrEidStr = iProcessingQ->At(lp_indx)->iBundle->GetDestinationEID()->EIDStrL());
			if(KErrNone == error)
			{
				iProcessingQ->At(lp_indx)->iResolverRecord->iStrEID = ptrEidStr;
				iProcessingQ->At(lp_indx)->iRecordStatus = E_BPA_DATA_DEST_RESOLVING;
			}
			else
				continue;	// memory may not be availalbe... try next time
		}
		else if(E_BPA_DATA_DEST_RESOLVING == iProcessingQ->At(lp_indx)->iRecordStatus)
		{
			iBPRouter->PerformRouteQuery(*(iProcessingQ->At(lp_indx)->iResolverRecord));
			iProcessingQ->At(lp_indx)->iRecordStatus = E_BPA_DATA_PROCESSING;
		}
		else if(E_BPA_DATA_PROCESSING == iProcessingQ->At(lp_indx)->iRecordStatus)
		{
			ExecuteTxStateMachine(*(iProcessingQ->At(lp_indx)));
		}
		else if((E_BPA_DATA_TRANSMITTED_OK == iProcessingQ->At(lp_indx)->iRecordStatus) ||
				(E_BPA_DATA_TRANSMITTED_NOK == iProcessingQ->At(lp_indx)->iRecordStatus))
		{
			ExecuteTxStateMachine(*(iProcessingQ->At(lp_indx)));
		}
	}
}

//-----------------------------------------------------------------------------

void CBPA::DoBPARxOperations()
{
	TInt lp_indx;
	
	for(lp_indx = 0; lp_indx < iProcessingQ->Count(); lp_indx++)
	{
		if(E_BPA_DATA_NEW_INCOMING == iProcessingQ->At(lp_indx)->iRecordStatus)
		{
			if(K_RC_NONE == iProcessingQ->At(lp_indx)->iRetentionConstraints)
			{
				TDesC8* ptrEidStr = NULL;
				TRAPD(error, ptrEidStr = iProcessingQ->At(lp_indx)->iBundle->GetDestinationEID()->EIDStrL());
				if(KErrNone == error)
				{
					// if by chance it has some older copy
					delete iProcessingQ->At(lp_indx)->iResolverRecord->iStrEID;
					iProcessingQ->At(lp_indx)->iResolverRecord->iStrEID = ptrEidStr;
				}
				iBPRouter->PerformRouteQuery(*(iProcessingQ->At(lp_indx)->iResolverRecord));
				// note a bundle could be destined for local EID, so Router may not have a reply
				// so don't check it like in TxOps
				Sec_4_6(*(iProcessingQ->At(lp_indx)));
			}
		}
	}
}

//-----------------------------------------------------------------------------

void CBPA::DoBPAAuxillaryOperations()
{
	CBundle *tempBundle = NULL;
	TInt lp_indx;
	TInt error;
	
	// 1. Get new requests from TxQ and hand over for processing
	
	if(iTxQ->Count() > 0)
	{
		TRAP(error, tempBundle = SynthesizeBundleL(iTxQ->At(0)));
		
		if(KErrNone == error)
		{
			TRAPD(error2, HandoverNewBundleForProcessingL(tempBundle, iTxQ->At(0).iAppTag, ETrue));
			if(KErrNone != error2)
			{
				// memory may not be available, try next time
				delete tempBundle;
			}
			else
			{
				iTxQ->At(0).FreeMem();
				iTxQ->Delete(0);
			}
			
			tempBundle = NULL;
		}
		else if(KErrArgument == error)
		{
			// transmission request has some invlaid options... delete record, and inform App Agent
			TBPInterfaceRecord tempRecord;
			tempRecord.iAppTag = iTxQ->At(0).iAppTag;
			tempRecord.iMsgType = E_AA_TX_STAT;
			TRAP(error, iRxQ->AppendL(tempRecord));
			if(KErrNone == error)
			{
				_LIT8(K_ERR_DES, "Invalid options for request identified by ");
				TBuf8<80>  tempStr(K_ERR_DES);
				tempStr.AppendNum(static_cast<TUint>(tempRecord.iAppTag));
				TDesC8* ptrStr = tempStr.Alloc();
				if(NULL == ptrStr)
				{
					iRxQ->At(iRxQ->Count()-1).FreeMem();
					iRxQ->Delete(iRxQ->Count()-1);
				}
				else
				{
					User::Beep(1000, TTimeIntervalMicroSeconds32(1000000));
					iRxQ->At(iRxQ->Count()-1).iPayloadBuffer = ptrStr;
					iTxQ->At(0).FreeMem();
					iTxQ->Delete(0);
				}
			}
			// else try again
		}
		else if(KErrNoMemory != error)
		{
			// unknown error, delete record
			iTxQ->At(0).FreeMem();
			iTxQ->Delete(0);
		}
		// else memory may not be available, try later
	}
	
	// 2. Get new ready bundles from ProcessingQ and put payload into RxQ
	
	for(lp_indx = 0; lp_indx < iProcessingQ->Count(); lp_indx++)
	{
		if(E_BPA_DATA_RECEIVED == iProcessingQ->At(lp_indx)->iRecordStatus)
		{
			TBPInterfaceRecord tempRecord;
			tempRecord.iAppTag = iProcessingQ->At(lp_indx)->iAppTag;
			TRAP(error, iRxQ->AppendL(tempRecord));
			if(KErrNone == error)
			{
				TDesC8* tempPayload = NULL;
				TBool isBSR;
				
				TRAP(error, ExtractInformationInBundlePayloadL(*(iProcessingQ->At(lp_indx)), tempPayload, isBSR));
				iRxQ->At(iRxQ->Count()-1).iMsgType = isBSR ? E_AA_BSR : E_AA_NEW_PAYLOAD_RX;

				if(NULL == tempPayload)
				{
					iRxQ->At(iRxQ->Count()-1).FreeMem();
					iRxQ->Delete(iRxQ->Count()-1);
				}
				else
				{
					User::Beep(1000, TTimeIntervalMicroSeconds32(1000000));
					iRxQ->At(iRxQ->Count()-1).iPayloadBuffer = tempPayload;
					iProcessingQ->At(lp_indx)->iRecordStatus = E_BPA_DATA_TO_BE_DISCARDED;
				}
			}
			break;
		}
	}
	
	// 3. remove to-be-discarded bundles
	
	for(lp_indx = 0; lp_indx < iProcessingQ->Count(); lp_indx++)
	{
		if(E_BPA_DATA_TO_BE_DISCARDED == iProcessingQ->At(lp_indx)->iRecordStatus)
		{
			__ASSERT_ALWAYS((K_RC_NONE == iProcessingQ->At(lp_indx)->iRetentionConstraints), User::InfoPrint(_L("BPA: some error in discarding logic...")));
			delete iProcessingQ->At(lp_indx);
			iProcessingQ->Delete(lp_indx);
		}
	}
	
	iProcessingQ->Compress();
	
	// 4. get new bundles from CLA Q's
	__ASSERT_ALWAYS((NULL == tempBundle), User::Panic(_L("Some memory leak in BPA core..."), 420));
	TDesC8* ptrDataOut;
	TUint32 bundleIDOut;
	TUint32 ackedLenOut;
	TBundleStatus bundleStatusOut;
	
	for(lp_indx = 0; lp_indx < iCLAQ->Count(); lp_indx++)
	{
		if(iCLAQ->At(lp_indx).iPtrCLA->Pop(ptrDataOut, bundleIDOut, bundleStatusOut, ackedLenOut))
		{
			if(E_BUNDLE_RECEIVED_OK == bundleStatusOut)
			{
				TRAP(error, tempBundle = CBundle::NewL(*ptrDataOut));
				
				if(KErrNone == error)
				{
					// bundle ID is of no use now, just store it if ever needed
					TRAPD(error2, HandoverNewBundleForProcessingL(tempBundle, bundleIDOut, EFalse));
					if(KErrNone != error2)
					{
						delete tempBundle;
					}
					
					// in either case...
					tempBundle = NULL;
				}
			}
			else if(E_BUNDLE_SENT_NOK == bundleStatusOut)
			{
				if(iConfigParams->iLogLevel & K_LOG_BPA)
				{
					User::InfoPrint(_L("BPA: Bundle couldn't be sent successfully!"));
				}
				// TODO: how to handle partial transfers?
				TBool status = (ackedLenOut > 0) ? ETrue : EFalse;
				TRAP(error, AcknowledgeBundleTxL(bundleIDOut, status));
				// can't help (no memory type) error at this stage... just ignore
			}
			else if(E_BUNDLE_SENT_OK == bundleStatusOut)
			{
				if(iConfigParams->iLogLevel & K_LOG_BPA)
				{
					User::InfoPrint(_L("BPA: Bundle transmitted successfully!"));
				}
				
				TRAP(error, AcknowledgeBundleTxL(bundleIDOut, ETrue));
				// can't help (no memory type) error at this stage... just ignore
			}
			
			// in either case...
			delete ptrDataOut;
			ptrDataOut = NULL;
		}
	}
}

//-----------------------------------------------------------------------------

CBPA* CBPA::NewL(const TInt32 aTimerPeriod, const TGlobalConfigParams& aConfig)
{
	CBPA* self = CBPA::NewLC(aTimerPeriod, aConfig);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CBPA* CBPA::NewLC(const TInt32 aTimerPeriod, const TGlobalConfigParams& aConfig)
{
	CBPA* self = new (ELeave) CBPA(aConfig);
	CleanupStack::PushL(self);
	self->ConstructL(aTimerPeriod);
	return self;
}

//-----------------------------------------------------------------------------

CBPA::~CBPA()
{
	TInt lp_indx;
	
	delete iLocalEID;
	delete iReportToEID;
	
	for(lp_indx = 0; lp_indx < iTxQ->Count(); lp_indx++)
	{
		iTxQ->At(lp_indx).FreeMem();
	}
	iTxQ->Reset();
	delete iTxQ;
	
	for(lp_indx = 0; lp_indx < iRxQ->Count(); lp_indx++)
	{
		iRxQ->At(lp_indx).FreeMem();
	}
	iRxQ->Reset();
	delete iRxQ;
	
	iProcessingQ->ResetAndDestroy();
	delete iProcessingQ;
	
	for(lp_indx = 0; lp_indx < iCLAQ->Count(); lp_indx++)
	{
		iCLAQ->At(lp_indx).FreeMem();
	}
	iCLAQ->Reset();
	delete iCLAQ;
	
	delete iTimer;
	delete iBPRouter;
}

//-----------------------------------------------------------------------------

void CBPA::EventHandler(TAny* aLPVOID)
{
	// BPA performs 3 operatiosn periodically...
	
	// 1. Process new messages to be transmitted and hand over to CLA(s)
	DoBPATxOperations();
	
	// 2. Process incoming bundles
	DoBPARxOperations();
	
	// 3. Get new bundles from CLA(s) and put in RxQ
	DoBPAAuxillaryOperations();
}

//-----------------------------------------------------------------------------

void CBPA::ConstructL(const TInt32 aTimerPeriod)
{
	LoadConfigFileL(_L("C:\\data\\dtn\\config\\bpa_config.txt"));
	
	iTxQ = new (ELeave) CArrayFixSeg<TBPInterfaceRecord>(8);
	iRxQ = new (ELeave) CArrayFixSeg<TBPInterfaceRecord>(8);
	iProcessingQ = new (ELeave) CArrayPtrSeg<CBPARecord>(16);
	iCLAQ = new  (ELeave) CArrayFixSeg<TCLARecord>(8);
	
	iBPRouter = CBPRouter::NewL(90000000, _L("C:\\data\\dtn\\config\\routes.txt"), *iConfigParams);
	
	iTimer = CSchedularTimer::NewL(aTimerPeriod, this);
}

//-----------------------------------------------------------------------------

CBPA::CBPA(const TGlobalConfigParams& aConfig) : iConfigParams(&aConfig)
{
	iFragmentationPolicy = EFalse;
	iCustodyAcceptancePolicy = EFalse;
	iHeaderErrorStatusPolicy = ETrue;
	iHeaderErrorDiscardPolicy = ETrue;
	iCOSPriority = 0x01;
	iSRRBundleReception = ETrue;
	iSRRCustodyAcceptance = ETrue;
	iSRRBundleForwarding = EFalse;
	iSRRBundleDelivery = ETrue;
	iSRRBundleDeletion = EFalse;
	iSRRAppAck = EFalse;
	iLocalEID = NULL;
	iReportToEID = NULL;
	iDefaultLifeTime = 1800;
	iTxQ = NULL;
	iRxQ = NULL;
	iProcessingQ = NULL;
	iCLAQ = NULL;
	iBPRouter = NULL;
	iTimer = NULL;
	
}

//-----------------------------------------------------------------------------

CBundle* CBPA::SynthesizeBundleL(TBPInterfaceRecord& aBPInterfaceRecord)
{
	if(aBPInterfaceRecord.iAABundleTxOptions & K_AABTR_TRANSFER_CUSTODY)
	{
		if(aBPInterfaceRecord.iAABundleTxOptions & K_AABTR_SRC_NODE_ACCEPT_CUSTODY)
		{
			if(!iCustodyAcceptancePolicy)
				User::Leave(KErrArgument);
		}
	}
	
	CEID *tempEid = NULL;
	TUint8 temp8;
	
	// create empty bundle object
	CBundle *tempBundle = CBundle::NewLC();
	
	// generate destination EID
	tempEid = CEID::NewL(*(aBPInterfaceRecord.iDestEID));
	
	// set primary header processing flags
	
	temp8 = 0x0;
	
	temp8 &= (~K_PHPF_BUNDLE_IS_FRAGMENT);		// not a fragment
	
	temp8 &= (~K_PHPF_ADU_IS_ADMIN_REC);		// not an admin record
	
	if(aBPInterfaceRecord.iAABundleTxOptions & K_AABTR_FRAGMENTATION_ALLOWED)
		temp8 &= (~K_PHPF_BUNDLE_NO_FRAGMENTATION);
	else
		temp8 |= K_PHPF_BUNDLE_NO_FRAGMENTATION;
	
	if(aBPInterfaceRecord.iAABundleTxOptions & K_AABTR_TRANSFER_CUSTODY)
		temp8 |= K_PHPF_BUNDLE_CUSTODY_RQSTD;
	else
		temp8 &= (~K_PHPF_BUNDLE_CUSTODY_RQSTD);
	
	temp8 |= K_PHPF_DEST_EID_SINGLETON;			// always a singleton EID node
	
	tempBundle->SetPrimaryHeaderProcFlags(temp8);
	
	// set COS flags
	tempBundle->SetCOSFlags(iCOSPriority);
	
	// set SRR flags
	
	tempBundle->SetSRRFlags(aBPInterfaceRecord.iBSRRFlags);
	
	// set destination EID
	tempBundle->SetDestinationEID(tempEid);
	// ownership transferred
	tempEid = NULL;
	
	// set source EID
	tempEid = CEID::NewL(*iLocalEID);
	tempBundle->SetSourceEID(tempEid);
	// ownership transferred
	
	// set report-to EID
	if(*iLocalEID != *iReportToEID)
	{
		tempEid = NULL;
		tempEid = CEID::NewL(*iReportToEID);
			
	}
	tempBundle->SetReportToEID(tempEid);
	// ownership transferred
	tempEid = NULL;
	
	// set custodian EID
	// Since we are bundle originator, it has to be "dtn:none"
	TBuf8<8> nullEID(K_NULL_EID);
	tempEid = CEID::NewL(nullEID);
	tempBundle->SetCustodianEID(tempEid);
	// ownership transferred
	tempEid = NULL;
	
	// set creation time stamp
	tempBundle->SetCreationTimeStamp(GenerateDTNTime());
	
	// set lifetime
	tempBundle->SetLifeTime(aBPInterfaceRecord.iLifeTime);
	
	// set payload header type
	tempBundle->SetPayloadHeaderType(K_BUNDLE_PAYLOAD_HEADER_TYPE);
	
	// set payload header proc flags
	// TODO: on what basis these flags are set?
	tempBundle->SetPayloadHeaderProcFlags(0x0);
	
	tempBundle->SetPayload(aBPInterfaceRecord.iPayloadBuffer);
	
	CleanupStack::Pop(tempBundle);
	
	return tempBundle;
}

//-----------------------------------------------------------------------------

void CBPA::ExecuteTxStateMachine(CBPARecord& aBPARecord)
{
	if(K_RC_FORWARD_PENDING & aBPARecord.iRetentionConstraints)
		Sec_4_4(aBPARecord);
	else
		Sec_4_2(aBPARecord);
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_2(CBPARecord& aBPARecord)
{
	// processing starts from beginning

	// STEP 1
	// has already been performed while generating outbound bundle...
	
	// STEP 2
	// outbound bundle already created, set retention constraint
	aBPARecord.iRetentionConstraints |= K_RC_DISPATCH_PENDING;
	
	// STEP 3
	Sec_4_3(aBPARecord);

}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_3(CBPARecord& aBPARecord)
{
	// STEP 1
	if(aBPARecord.iBundle->GetDestinationEID()->Compare(*iLocalEID))
		Sec_4_7(aBPARecord);
	
	// STEP 2
	else
		Sec_4_4(aBPARecord);
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_4(CBPARecord& aBPARecord)
{
	TInt error;
	TUint8 reasonCode;
	CCLA* ptrCLA = NULL;
	
	if(aBPARecord.iRetentionConstraints & K_RC_DISPATCH_PENDING)
	{
		// STEP 1
		aBPARecord.iRetentionConstraints &= (~K_RC_DISPATCH_PENDING);
		aBPARecord.iRetentionConstraints |= K_RC_FORWARD_PENDING;
		
		// STEP 2
		TBool fwdCntrIndct = IsForwardingContraindicated(aBPARecord, reasonCode, ptrCLA);
		
		// STEP 3
		if(fwdCntrIndct)
		{
			Sec_4_4_1(aBPARecord, reasonCode);
			return;			// skip next steps
		}
		
		// STEP 4
		if(aBPARecord.iBundle->GetPrimaryHeaderProcFlags() & K_PHPF_BUNDLE_CUSTODY_RQSTD)
			Sec_4_10(aBPARecord);
		
		// STEP 5
		TDesC8* tempBuff;
		TRAP(error, aBPARecord.iBundle->GenerateBundleMessageL(tempBuff));
		if(KErrNone != error)
		{
			// most probably, memory was not available...
			// to try again later, reset states...
			aBPARecord.iRecordStatus = E_BPA_DATA_DEST_RESOLVING;
			aBPARecord.iRetentionConstraints &= (~K_RC_FORWARD_PENDING);
			return;
		}
		
		if(ptrCLA->Push(tempBuff, static_cast<TSockAddr*>(aBPARecord.iResolverRecord->iReply), aBPARecord.iBundle->UniqueID32()))
		{
			aBPARecord.iResolverRecord->iReply = NULL;
			aBPARecord.iRecordStatus = E_BPA_DATA_TRANSMITTING;
			return;
		}
		else
		{
			// most probably, memory was not available...
			// to try again later, reset states...
			aBPARecord.iRecordStatus = E_BPA_DATA_DEST_RESOLVING;
			aBPARecord.iRetentionConstraints &= (~K_RC_FORWARD_PENDING);
			return;
		}
	}
	
	if(aBPARecord.iRetentionConstraints & K_RC_FORWARD_PENDING)
	{
		if(E_BPA_DATA_TRANSMITTED_OK == aBPARecord.iRecordStatus)
		{
			// STEP 6
			if(aBPARecord.iBundle->GetSRRFlags() & K_SRR_BUNDLE_FORWARDING)
			{
				TUint8 temp8 = K_SRR_BUNDLE_FORWARDING;
				TRAP(error, GenerateBSRL(aBPARecord, temp8, K_SRRC_NO_INFO));
				// just ignore error
			}
			
			if(aBPARecord.iRetentionConstraints & K_RC_CUSTODY_ACCEPTED)
			{
				aBPARecord.iRecordStatus = E_BPA_DATA_CUSTODY_ACCEPTED;
			}
			else
			{
				aBPARecord.iRecordStatus = E_BPA_DATA_TO_BE_DISCARDED;
			}
			
			if(aBPARecord.iBundle->GetSourceEID()->Compare(*iLocalEID))
			{
				// bundle was originated from this node
				AcknowledgeAppAgentPayloadTxL(aBPARecord, ETrue);
			}
			
		}
		else if(E_BPA_DATA_TRANSMITTED_NOK == aBPARecord.iRecordStatus)
		{
			if(aBPARecord.iRetentionConstraints & K_RC_CUSTODY_ACCEPTED)
			{
				aBPARecord.iRecordStatus = E_BPA_DATA_DEST_RESOLVING;
			}
			else
			{
				aBPARecord.iRecordStatus = E_BPA_DATA_TO_BE_DISCARDED;
				
				if(aBPARecord.iBundle->GetSourceEID()->Compare(*iLocalEID))
				{
					// bundle was originated from this node
					AcknowledgeAppAgentPayloadTxL(aBPARecord, EFalse);
				}
			}
		}
		
		aBPARecord.iRetentionConstraints &= (~K_RC_FORWARD_PENDING);
	}
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_4_1(CBPARecord& aBPARecord, TUint8 aReasonCode)
{
	// STEP 1
	// we always declare Failure in case of forwarding is contraindicated
	
	// STEP 2
	Sec_4_4_2(aBPARecord, aReasonCode);
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_4_2(CBPARecord& aBPARecord, TUint8 aReasonCode)
{
	// STEP 1
	if(aBPARecord.iBundle->GetPrimaryHeaderProcFlags() & K_PHPF_BUNDLE_CUSTODY_RQSTD)
	{
		TRAPD(error, GenerateCustodySignalL(aBPARecord, EFalse, aReasonCode));
		// just ignore error
	}
	
	// STEP 2
	Sec_4_13(aBPARecord, aReasonCode);
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_6(CBPARecord& aBPARecord)
{
	TInt error;
	
	// STEP 1
	aBPARecord.iRetentionConstraints |= K_RC_DISPATCH_PENDING;
	
	// STEP 2
	if(aBPARecord.iBundle->GetSRRFlags() & K_SRR_BUNDLE_RECEPTION)
	{
		TUint8 temp8 = K_SRR_BUNDLE_RECEPTION;
		TRAP(error, GenerateBSRL(aBPARecord, temp8, K_SRRC_NO_INFO));
		// just ignore error
	}
	
	// STEP 3: ambiguous step (if header can't be processed how to get EID?)
	
	// STEP 4
	if(aBPARecord.iBundle->GetPrimaryHeaderProcFlags() & K_PHPF_BUNDLE_CUSTODY_RQSTD)
	{
		TInt indx_pos = FindSimilarBundle(aBPARecord.iBundle);
		if(indx_pos >= 0)
		{
			if((iProcessingQ->At(indx_pos)->iRecordStatus != E_BPA_DATA_TO_BE_DISCARDED) &&
			   (iProcessingQ->At(indx_pos)->iRecordStatus != E_BPA_DATA_TO_BE_DELETED) &&
			   (iProcessingQ->At(indx_pos)->iRetentionConstraints & K_RC_CUSTODY_ACCEPTED))
			{
				TRAP(error, GenerateCustodySignalL(aBPARecord, EFalse, K_CSRC_REDUNDANT_RECEPTION));
				// just ignore error
				
				aBPARecord.iRetentionConstraints &= (~K_RC_DISPATCH_PENDING);
				
				return;
			}
		}
	}
	
	// STEP 5
	Sec_4_3(aBPARecord);
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_7(CBPARecord& aBPARecord)
{
	TInt error;
	
	// STEP 1
	if(aBPARecord.iBundle->IsFragment())
	{
		Sec_4_9(aBPARecord);
		
		if(!aBPARecord.iBundle->IsFragment())
		{
			aBPARecord.iRetentionConstraints |= K_RC_REASSEMBLY_PENDING;
			return;		// skip all other steps
		}
	}
	
	// STEP 2
	if(aBPARecord.iBundle->IsAdminRecord())
	{
		if(aBPARecord.iBundle->GetAdminRecord()->IsCustodySignal())
		{
			HandleCustodySignal(aBPARecord.iBundle->GetAdminRecord());
			aBPARecord.iRecordStatus = E_BPA_DATA_TO_BE_DISCARDED;
		}
		else
		{
			aBPARecord.iRecordStatus = E_BPA_DATA_RECEIVED;
		}
	}
	else
	{
		// actual delivery to App Agent will be performed in Aux. Ops.
	
		// STEP 3
		if(aBPARecord.iBundle->GetSRRFlags() & K_SRR_BUNDLE_DELIVERY)
		{
			TUint8 temp8 = K_SRR_BUNDLE_DELIVERY;
			TRAP(error, GenerateBSRL(aBPARecord, temp8, K_SRRC_NO_INFO));
			// just ignore error
		}
		if(aBPARecord.iBundle->GetPrimaryHeaderProcFlags() & K_PHPF_BUNDLE_CUSTODY_RQSTD)
		{
			TRAP(error, GenerateCustodySignalL(aBPARecord, ETrue, 0));
			// just ignore error
		}
		
		aBPARecord.iRecordStatus = E_BPA_DATA_RECEIVED;
	}
	
	aBPARecord.iRetentionConstraints &= (~K_RC_DISPATCH_PENDING);
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_9(CBPARecord& aBPARecord)
{
	TInt lp_indx;
	TUint32 frgmntOffsetArg, frgmntOffsetLoop;
	TBool found = EFalse;
	
	// locate previous bundle fragment
	
	for(lp_indx = 0; lp_indx < iProcessingQ->Count(); lp_indx++)
	{
		// reassembly is done for incoming bundles only
		if(iProcessingQ->At(lp_indx)->iRecordStatus == E_BPA_DATA_NEW_INCOMING)
		{
			// previous one too is a fragment
			if(iProcessingQ->At(lp_indx)->iBundle->IsFragment())
			{
				// it has same source EID
				if(iProcessingQ->At(lp_indx)->iBundle->GetSourceEID()->Compare(aBPARecord.iBundle->GetSourceEID()))
				{
					// it has same creation time stamp
					if(iProcessingQ->At(lp_indx)->iBundle->GetCreationTimeStamp() == aBPARecord.iBundle->GetCreationTimeStamp())
					{
						// check validity for re-assembly
						TUint32 payloadSizeLoop;
						
						iProcessingQ->At(lp_indx)->iBundle->GetFragmentOffset(frgmntOffsetLoop);
						aBPARecord.iBundle->GetFragmentOffset(frgmntOffsetArg);
						
						iProcessingQ->At(lp_indx)->iBundle->GetPayloadLen(payloadSizeLoop);
						
						if((frgmntOffsetArg > frgmntOffsetLoop) &&
						   ((frgmntOffsetLoop + payloadSizeLoop) == frgmntOffsetArg))
						{
							// we're the later "half" AND both fragments are successive
							found = ETrue;
							break;
						}
					}
				}
			}
		}
	}
	
	if(found == EFalse)
		return;
	
	// add "former" fragment's payload to the one of "later" fragment
	TDesC8* payloadBuffArg = NULL;
	TDesC8* payloadBuffLoop = NULL;
	
	aBPARecord.iBundle->GetPayload(payloadBuffArg);
	if(NULL == payloadBuffArg)
		return;
	
	iProcessingQ->At(lp_indx)->iBundle->GetPayload(payloadBuffLoop);
	
	if(NULL == payloadBuffLoop)
	{
		delete payloadBuffArg;
		return;		// try next time
	}
	
	HBufC8* payloadMerged = HBufC8::New(payloadBuffArg->Size() + payloadBuffLoop->Size());
	if(NULL == payloadMerged)
	{
		delete payloadBuffArg;
		delete payloadBuffLoop;
		return;		// try next time
	}
	
	payloadMerged->Des().Append(*payloadBuffLoop);
	payloadMerged->Des().Append(*payloadBuffArg);
	
	delete payloadBuffArg;
	delete payloadBuffLoop;
	
	TUint32 payloadMergedSize = static_cast<TUint32>(payloadMerged->Size());
	aBPARecord.iBundle->SetPayload(payloadMerged);
	delete payloadMerged;
	aBPARecord.iBundle->SetFragmentOffset(frgmntOffsetLoop);
	
	TUint32 totalADULen;
	aBPARecord.iBundle->GetTotalADULen(totalADULen);
	
	// this fragment should be deleted
	iProcessingQ->At(lp_indx)->iRetentionConstraints = K_RC_NONE;
	iProcessingQ->At(lp_indx)->iRecordStatus = E_BPA_DATA_TO_BE_DELETED;
	
	if(totalADULen == payloadMergedSize)
	{
		aBPARecord.iRetentionConstraints &= (~K_RC_REASSEMBLY_PENDING);
		TUint8 flag = aBPARecord.iBundle->GetPrimaryHeaderProcFlags();
		flag &= (~K_PHPF_BUNDLE_IS_FRAGMENT);
		aBPARecord.iBundle->SetPrimaryHeaderProcFlags(flag);
	}
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_10(CBPARecord& aBPARecord)
{
	if(aBPARecord.iRetentionConstraints & K_RC_CUSTODY_ACCEPTED)
		return;
	
	if(iCustodyAcceptancePolicy)
		Sec_4_10_1(aBPARecord);
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_10_1(CBPARecord& aBPARecord)
{
	TInt error;
	
	// assign new custodian EID 
	CEID* tempEid = NULL;
	TRAP(error, tempEid = CEID::NewL(*iLocalEID));
	
	if(error != KErrNone)
	{
		return;		// can't help such error at this stage
	}
	
	aBPARecord.iRetentionConstraints |= K_RC_CUSTODY_ACCEPTED;
	
	if(aBPARecord.iBundle->GetSRRFlags() & K_SRR_CUSTODY_ACCEPTANCE)
	{
		TUint8 temp8 = K_SRR_CUSTODY_ACCEPTANCE;
		TRAP(error, GenerateBSRL(aBPARecord, temp8, K_SRRC_NO_INFO));
	}
	
	TRAP(error, GenerateCustodySignalL(aBPARecord, ETrue, 0));
	// just ignore error
	
	aBPARecord.iBundle->SetCustodianEID(tempEid);
	tempEid = NULL;
	
	// TODO: put bundle in persistent storage
}

//-----------------------------------------------------------------------------

void CBPA::Sec_4_13(CBPARecord& aBPARecord, TUint8 aReasonCode)
{
	// STEP 1
	if(aBPARecord.iRetentionConstraints & K_RC_CUSTODY_ACCEPTED)
	{
		// all retention constraints will be removed by this function...
		// TODO: remove from consistant storage...
	}
	
	if(aBPARecord.iBundle->GetSRRFlags() & K_SRR_BUNDLE_DELETION)
	{
		TUint8 temp8 = K_SRR_BUNDLE_DELETION;
		
		TRAPD(error, GenerateBSRL(aBPARecord, temp8, aReasonCode));
		// just ignore error
	}
	
	// in case of deletion of bundle originated from local node, notify App-Agent
	if(aBPARecord.iBundle->GetSourceEID()->Compare(*iLocalEID))
	{
		// bundle was originated from this node
		AcknowledgeAppAgentPayloadTxL(aBPARecord, EFalse);
	}
	
	aBPARecord.iRecordStatus = E_BPA_DATA_TO_BE_DISCARDED;
	aBPARecord.iRetentionConstraints = K_RC_NONE;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

TBool CBPA::IsForwardingContraindicated(CBPARecord& aBPARecord, TUint8& aReasonOut,
										CCLA*& ptrCLAOut)
{
	// this is decided according to Table-5 in BPSpec-04
	
	aReasonOut = K_SRRC_NO_INFO;
	
	if(IsBundleExpired(aBPARecord.iBundle))
	{
		aReasonOut = K_SRRC_LIFETIME_EXPIRED;
		return ETrue;
	}
	
	if(E_RESOLVED_NULL == aBPARecord.iResolverRecord->iLUType)
	{
		aReasonOut = K_SRRC_NO_KNOWN_ROUTE_TO_DEST;
		return ETrue;
	}
	else if(E_RESOLVED_INET_TCP == aBPARecord.iResolverRecord->iLUType)
	{
		ptrCLAOut = FindCLARef(E_TCP_CLA);
	}
	else if(E_RESOLVED_BT_RF_COMM == aBPARecord.iResolverRecord->iLUType)
	{
		ptrCLAOut = FindCLARef(E_BT_RFCOMM_CLA);
	}
	
	if(NULL == ptrCLAOut)
	{
		aReasonOut = K_SRRC_NO_KNOWN_ROUTE_TO_DEST;
		return ETrue;
	}
	
	return EFalse;
}

//-----------------------------------------------------------------------------

TBool CBPA::IsBundleExpired(const CBundle* aBundle)
{
	TTime now;
	TTimeIntervalSeconds secondsElapsed;
	TUint32 delta;
	TUint diff;
	
	now.UniversalTime();
	now.SecondsFrom(TTime(K_Y2K), secondsElapsed);
	
	// a minor clock skew can be horrible... (could result in small -ve number
	// which results in a very large +ve difference
	
	diff = static_cast<TUint>(secondsElapsed.Int()) - aBundle->GetCreationTimeStamp().High();
	
	// so use abs value of signed difference
	delta = static_cast<TUint32>( abs( static_cast<TInt>(diff) ) );
	
	if(delta > aBundle->GetLifeTime())
		return ETrue;
	else
		return EFalse;
}

//-----------------------------------------------------------------------------

TInt CBPA::FindSimilarBundle(const CBundle* aBundle)
{
	TInt lp_indx;
	TInt indx_pos = -1;
	
	for(lp_indx = 0; lp_indx < iProcessingQ->Count(); lp_indx++)
	{
		if(iProcessingQ->At(lp_indx)->iBundle->CompareBundle(aBundle))
		{
			indx_pos = lp_indx;
			break;
		}
	}
	
	return indx_pos;
}

//-----------------------------------------------------------------------------

void CBPA::HandoverNewBundleForProcessingL(CBundle* aBundle, TUint32 aAppTag, TBool aIsOutGoing)
{
	CBPARecord* tempRecord = new (ELeave) CBPARecord;
	CleanupStack::PushL(tempRecord);
	
	CEIDLookUpRecord* tempRoutingRecord = new (ELeave) CEIDLookUpRecord;
	CleanupStack::PushL(tempRoutingRecord);
		
	if(aIsOutGoing)
		tempRecord->iRecordStatus = E_BPA_DATA_NEW_OUTGOING;
	else
		tempRecord->iRecordStatus = E_BPA_DATA_NEW_INCOMING;
	
	tempRecord->iAppTag = aAppTag;
	
	iProcessingQ->AppendL(tempRecord);
	
	iProcessingQ->At(iProcessingQ->Count()-1)->iBundle = aBundle;
	iProcessingQ->At(iProcessingQ->Count()-1)->iResolverRecord = tempRoutingRecord;
	
	CleanupStack::Pop(tempRoutingRecord);
	CleanupStack::Pop(tempRecord);
}

//-----------------------------------------------------------------------------

void CBPA::LoadConfigFileL(const TFileName& aConfigFileName)
{
	const TInt MAX_LINE_SIZE =   128 +	// name + delimiters
								2048;	// value
										// TOTOAL: 2176 bytes
	const TChar lineDelimiter = ';';
	
	HBufC8* bufLine = NULL;
	RFs fileServer;
	RFileReadStream configFileStream;
	
	bufLine = HBufC8::NewLC(MAX_LINE_SIZE);
	TPtr8 ptrBuf = bufLine->Des();
	
	User::LeaveIfError(fileServer.Connect());
	CleanupClosePushL(fileServer);
	
	User::LeaveIfError(configFileStream.Open(fileServer, aConfigFileName, EFileRead));
	configFileStream.PushL();
	
	while(ETrue)	// loop will break when function leaves
	{
		TRAPD(error, configFileStream.ReadL(ptrBuf, lineDelimiter));
		if(KErrEof == error)
		{
			break;
		}
		ptrBuf.TrimAll();
		ptrBuf.Delete(ptrBuf.Size()-1, 1);		// get rid of delimiter ';'
		
		// parse the value
		ProcessConfigParamL(ptrBuf);
	}
	
	configFileStream.Pop();
	configFileStream.Close();
	
	CleanupStack::PopAndDestroy();			//	fileServer
	CleanupStack::PopAndDestroy(bufLine);	//	bufLine

}

//-----------------------------------------------------------------------------

void CBPA::ProcessConfigParamL(TPtr8 aConfigParam)
{
	const TChar tokenDelimiter = '=';
	
	TInt pos;
	
	// first value (eid)
	pos = aConfigParam.Locate(tokenDelimiter);
	User::LeaveIfError(pos);
	
	if(aConfigParam.Left(pos) == _L8("local_eid"))
	{
		User::LeaveIfError( SetLocalEID( aConfigParam.Mid(pos+1) ) );
	}
	else if(aConfigParam.Left(pos) == _L8("report_to_eid"))
	{
		User::LeaveIfError( SetReportToEID( aConfigParam.Mid(pos+1) ) );
	}
	else if(aConfigParam.Left(pos) == _L8("tcp_listening_port"))
	{
		TLex8 lex(aConfigParam.Mid(pos+1));
		User::LeaveIfError(lex.Val(iTCPListeningPort, EDecimal));
	}
	else if(aConfigParam.Left(pos) == _L8("bt_listening_channel"))
	{
		TLex8 lex(aConfigParam.Mid(pos+1));
		User::LeaveIfError(lex.Val(iBTListeningChannel, EDecimal));
	}
	else if(aConfigParam.Left(pos) == _L8("bundle_tx_retry_interval"))
	{
		TInt tmp32;
		TLex8 lex(aConfigParam.Mid(pos+1));
		User::LeaveIfError(lex.Val(tmp32));
		iBundleTxRetryInterval = tmp32;
	}
	else
	{
		// ignore it?
	}
}

//-----------------------------------------------------------------------------

CCLA* CBPA::FindCLARef(const TCLAType aCLAType)
{
	CCLA* ptrCLA = NULL;
	
	TInt lp_indx;
	
	for(lp_indx = 0; lp_indx < iCLAQ->Count(); lp_indx++)
	{
		if(aCLAType == iCLAQ->At(lp_indx).iCLAType)
		{
			ptrCLA = iCLAQ->At(lp_indx).iPtrCLA;
			break;
		}
	}
	
	return ptrCLA;
}

//-----------------------------------------------------------------------------

void CBPA::AcknowledgeAppAgentPayloadTxL(CBPARecord& aBPARecord, const TBool aSentStatus)
{
	HBufC8* tempBuff = HBufC8::NewLC(50);
	
	TBPInterfaceRecord tempRecord;
	
	tempRecord.iAppTag = aBPARecord.iAppTag;
	tempRecord.iMsgType = E_AA_TX_STAT;
	
	if(aSentStatus == EFalse)
	{
		tempBuff->Des().Copy(_L8("Data couldn't be sent!"));
	}
	else
	{
		tempBuff->Des().Copy(_L8("Data sent successfully!"));
	}
	
	iRxQ->AppendL(tempRecord);
	
	iRxQ->At(iRxQ->Count()-1).iPayloadBuffer = static_cast<TDesC8*>(tempBuff);
	
	CleanupStack::Pop(tempBuff);
}

//-----------------------------------------------------------------------------

void CBPA::AcknowledgeBundleTxL(const TUint32 aBundleID, const TBool aSentStatus)
{
	TInt lp_indx;
	
	for(lp_indx = 0; lp_indx < iProcessingQ->Count(); lp_indx++)
	{ 
		if(aBundleID == iProcessingQ->At(lp_indx)->iBundle->UniqueID32())
		{
			if(aSentStatus)
			{
				iProcessingQ->At(lp_indx)->iRecordStatus = E_BPA_DATA_TRANSMITTED_OK;
			}
			else
			{
				iProcessingQ->At(lp_indx)->iRecordStatus = E_BPA_DATA_TRANSMITTED_NOK;
			}
			
			break;
		}
	}
}

//-----------------------------------------------------------------------------

void CBPA::ExtractInformationInBundlePayloadL(CBPARecord& aBPARecord, TDesC8*& aPayloadOut,
												 TBool& aIsBSROut)
{
	HBufC8* tempBuff = NULL;
	
	aPayloadOut = NULL;
	
	if(aBPARecord.iBundle->IsAdminRecord())
	{
		if(aBPARecord.iBundle->GetAdminRecord()->IsBSR())
		{
			tempBuff = HBufC8::NewLC(4096);
			TTime creationTime(K_Y2K);
			TBuf<80> bufTime;
			TUint8 status;
			TTimeIntervalSeconds creationTimeDelta;
			
			tempBuff->Des().Append(_L("BSR, for bundle originated from \""));
			tempBuff->Des().Append(aBPARecord.iBundle->GetAdminRecord()->GetAssociatedBundleSourceEID()->SchemeDes());
			tempBuff->Des().Append(':');
			tempBuff->Des().Append(aBPARecord.iBundle->GetAdminRecord()->GetAssociatedBundleSourceEID()->SSPDes());
			tempBuff->Des().Append(_L("\" at "));
			creationTimeDelta = aBPARecord.iBundle->GetAdminRecord()->GetAssociatedBundleCreationTimeStamp().High();
			creationTime += creationTimeDelta;
			creationTime.FormatL(bufTime, KDateTimeFormatStringShort);
			tempBuff->Des().Append(bufTime);
			tempBuff->Des().Append(_L(", reports that bundle "));
			
			status = aBPARecord.iBundle->GetAdminRecord()->GetStatus();
			
			if(status & K_SRR_BUNDLE_RECEPTION)
			{
				tempBuff->Des().Append(_L("was received by, "));
			}
			if(status & K_SRR_CUSTODY_ACCEPTANCE)
			{
				tempBuff->Des().Append(_L(", custody of, was accepted by, "));
			}
			if(status & K_SRR_BUNDLE_FORWARDING)
			{
				tempBuff->Des().Append(_L(" was forwarded by, "));
			}
			if(status & K_SRR_BUNDLE_DELIVERY)
			{
				tempBuff->Des().Append(_L(" was delivered by, "));
			}
			if(status & K_SRR_BUNDLE_DELETION)
			{
				TUint8 reason = aBPARecord.iBundle->GetAdminRecord()->GetReasonCode();
				tempBuff->Des().Append(_L(" was deleted, due to "));
				
				if(K_SRRC_NO_INFO == reason)
					tempBuff->Des().Append(_L("unspecified reason, by, "));
				else if(K_SRRC_LIFETIME_EXPIRED == reason)
					tempBuff->Des().Append(_L("expiry, by, "));
				else if(K_SRRC_FORWARDED_OVER_UNIDIR_LINK == reason)
					tempBuff->Des().Append(_L("forwarding over unidirectional link, by, "));
				else if(K_SRRC_TRANSMISSION_CANCELLED == reason)
					tempBuff->Des().Append(_L("transmission cancellation, by, "));
				else if(K_SRRC_DEPLETED_STORAGE == reason)
					tempBuff->Des().Append(_L("depleted storage, by, "));
				else if(K_SRRC_DEST_EID_UNINTELLIGIBLE == reason)
					tempBuff->Des().Append(_L("unintelligible destination EID, by, "));
				else if(K_SRRC_NO_KNOWN_ROUTE_TO_DEST == reason)
					tempBuff->Des().Append(_L("no known route to destination, by, "));
				else if(K_SRRC_NO_TIMELY_CONTACT_WITH_NEXT_NODE == reason)
					tempBuff->Des().Append(_L("no timely contact with next node, by, "));
				else if(K_SRRC_HEADER_UNINTELLIGIBLE == reason)
					tempBuff->Des().Append(_L("unintelligible header, by, "));
				else
					tempBuff->Des().Append(_L("unknown reason, by, "));
			}
			if(status & K_SRR_APP_ACK)
			{
				tempBuff->Des().Append(_L("was acknowledged by application of, "));
			}
			
			tempBuff->Des().Append(aBPARecord.iBundle->GetSourceEID()->SchemeDes());
			tempBuff->Des().Append(':');
			tempBuff->Des().Append(aBPARecord.iBundle->GetSourceEID()->SSPDes());
			tempBuff->Des().Append('.');
			
			aPayloadOut = tempBuff;
			aIsBSROut = ETrue;
			CleanupStack::Pop(tempBuff);
		}
	}
	else
	{
		aIsBSROut = EFalse;
		aBPARecord.iBundle->GetPayload(aPayloadOut);
	}
}

//-----------------------------------------------------------------------------

CAdminRecord* CBPA::ComposeBSRL(CBPARecord& aBPARecord, TUint8 aSRRFlags, TUint8 aReasonCode)
{
	TUint8 temp8;
	TInt64 temp64;
	CSDNV* tmpSDNV = NULL;
	
	CAdminRecord* adminRec = CAdminRecord::NewLC();
	
	temp8 = 0;
	temp8 |= K_ART_STATUS_REPORT;
	if(aBPARecord.iBundle->IsFragment())
	{
		temp8 |= K_ARF_FOR_FRAGMENT;
	}
	
	adminRec->SetRecordTypesFlags(temp8);
	
	adminRec->SetStatus(aSRRFlags);
	adminRec->SetReasonCode(aReasonCode);
	
	if(aBPARecord.iBundle->IsFragment())
	{
		tmpSDNV = CSDNV::NewLC();
		aBPARecord.iBundle->GetFragmentOffset(tmpSDNV);
		adminRec->SetFragmentOffset(tmpSDNV);
		CleanupStack::Pop(tmpSDNV);				// ownership transferred
		tmpSDNV = NULL;
		
		tmpSDNV = CSDNV::NewLC();
		aBPARecord.iBundle->GetPayloadLen(tmpSDNV);
		adminRec->SetFragmentLength(tmpSDNV);
		CleanupStack::Pop(tmpSDNV);				// ownership transferred
		tmpSDNV = NULL;
	}
	
	temp64 = GenerateDTNTime();
	
	if(aSRRFlags & K_SRR_BUNDLE_RECEPTION)
		adminRec->SetAssociatedBundleReceiptTimeStamp(temp64);
	if(aSRRFlags & K_SRR_CUSTODY_ACCEPTANCE)
		adminRec->SetAssociatedBundleCustodyAcceptanceTimeStamp(temp64);
	if(aSRRFlags & K_SRR_BUNDLE_FORWARDING)
		adminRec->SetAssociatedBundleForwardingTimeStamp(temp64);
	if(aSRRFlags & K_SRR_BUNDLE_DELIVERY)
		adminRec->SetAssociatedBundleDeliveryTimeStamp(temp64);
	if(aSRRFlags & K_SRR_BUNDLE_DELETION)
		adminRec->SetAssociatedBundleDeletionTimeStamp(temp64);
	if(aSRRFlags & K_SRR_APP_ACK)
		adminRec->SetAssociatedBundleAcknowledgementTimeStamp(temp64);
	
	adminRec->SetAssociatedBundleCreationTimeStamp(aBPARecord.iBundle->GetCreationTimeStamp());
	
	CEID* tmpEID = CEID::NewLC(aBPARecord.iBundle->GetSourceEID());
	adminRec->SetAssociatedBundleSourceEID(tmpEID);
	CleanupStack::Pop(tmpEID);				// ownership transferred
	
	CleanupStack::Pop(adminRec);
	
	return adminRec;
}

//-----------------------------------------------------------------------------

TInt64 CBPA::GenerateDTNTime(void)
{
	TTime time;
	TInt64 timeStamp;
	TTimeIntervalSeconds deltaSec;
	time.UniversalTime();
	User::LeaveIfError(time.SecondsFrom(TTime(K_Y2K), deltaSec));
	timeStamp.Set(static_cast<TUint>(deltaSec.Int()), User::TickCount());
	
	return timeStamp;
}

//-----------------------------------------------------------------------------

void CBPA::GenerateBSRL(CBPARecord& aBPARecord, TUint8 aSRRFlags, TUint8 aReasonCode)
{
	CAdminRecord* tmpAdminRec = NULL;
	
	tmpAdminRec = ComposeBSRL(aBPARecord, aSRRFlags, aReasonCode);
	CleanupStack::PushL(tmpAdminRec);
	
	CBundle *tmpBundle = ComposeAdminRecordBundleL(aBPARecord, tmpAdminRec);
	CleanupStack::Pop(tmpAdminRec);
	CleanupStack::PushL(tmpBundle);
	
	HandoverNewBundleForProcessingL(tmpBundle, 0, ETrue);
	
	CleanupStack::Pop(tmpBundle);
}

//-----------------------------------------------------------------------------

CBundle* CBPA::ComposeAdminRecordBundleL(CBPARecord& aBPARecord, CAdminRecord* aAdminRecord)
{
	CEID *tmpEid = NULL;
	TUint8 temp8;
	
	// create empty bundle object
	CBundle *tmpBundle = CBundle::NewLC();
	
	// set primary header processing flags
	
	temp8 = 0x0;
	
	temp8 &= (~K_PHPF_BUNDLE_IS_FRAGMENT);		// not a fragment
	temp8 |= K_PHPF_ADU_IS_ADMIN_REC;			// an admin record
	temp8 |= K_PHPF_BUNDLE_NO_FRAGMENTATION;	// no fragmentation allowed for BSR
	temp8 &= (~K_PHPF_BUNDLE_CUSTODY_RQSTD);	// no custody transfer for BSR
	temp8 |= K_PHPF_DEST_EID_SINGLETON;			// always a singleton EID node
	
	tmpBundle->SetPrimaryHeaderProcFlags(temp8);
	
	// set COS flags
	tmpBundle->SetCOSFlags(iCOSPriority);
	
	// set SRR flags
	tmpBundle->SetSRRFlags(K_SRR_NO_REPORTS);	// no BSRs for BSR
	
	// set destination EID
	if(aAdminRecord->IsCustodySignal())
		tmpEid = CEID::NewL(aBPARecord.iBundle->GetCustodianEID());
	else
		tmpEid = CEID::NewL(aBPARecord.iBundle->GetReportToEID());
	
	tmpBundle->SetDestinationEID(tmpEid);
	tmpEid = NULL;								// ownership transferred
	
	// set source EID
	tmpEid = CEID::NewL(*iLocalEID);
	tmpBundle->SetSourceEID(tmpEid);
	tmpEid = NULL;								// ownership transferred
	
	TBuf8<8> nullEID(K_NULL_EID);
	
	// set report-to EID
	tmpEid = CEID::NewL(nullEID);
	tmpBundle->SetReportToEID(tmpEid);
	
	// set custodian EID
	tmpBundle->SetCustodianEID(tmpEid);
	tmpEid = NULL;								// ownership transferred
	
	// set creation time stamp
	tmpBundle->SetCreationTimeStamp(GenerateDTNTime());
	
	// set lifetime
	tmpBundle->SetLifeTime(iDefaultLifeTime);
	
	// set payload header type
	tmpBundle->SetPayloadHeaderType(K_BUNDLE_PAYLOAD_HEADER_TYPE);
	
	// set payload header proc flags
	tmpBundle->SetPayloadHeaderProcFlags(0x0);
	
	tmpBundle->SetAdminRecord(aAdminRecord);
	
	CleanupStack::Pop(tmpBundle);
	
	return tmpBundle;
}

//-----------------------------------------------------------------------------

void CBPA::GenerateCustodySignalL(CBPARecord& aBPARecord, TBool aCustodySucceeded, TUint8 aReasonCode)
{
	CAdminRecord* tmpAdminRec = NULL;
	
	if(aBPARecord.iBundle->GetCustodianEID()->Compare(K_NULL_EID))
		return;
	
	aReasonCode = ((aReasonCode >= 0x04) && (aReasonCode <= 0x08)) ? aReasonCode : K_CSRC_NO_INFO; 
	
	tmpAdminRec = ComposeCustodySignalL(aBPARecord, aCustodySucceeded, aReasonCode);
	CleanupStack::PushL(tmpAdminRec);
	
	CBundle *tmpBundle = ComposeAdminRecordBundleL(aBPARecord, tmpAdminRec);
	CleanupStack::Pop(tmpAdminRec);
	CleanupStack::PushL(tmpBundle);
	
	HandoverNewBundleForProcessingL(tmpBundle, 0, ETrue);
	
	CleanupStack::Pop(tmpBundle);
}

//-----------------------------------------------------------------------------

CAdminRecord* CBPA::ComposeCustodySignalL(CBPARecord& aBPARecord, TBool aCustodySucceeded, TUint8 aReasonCode)
{
	TUint8 temp8;
	TInt64 temp64;
	CSDNV* tmpSDNV = NULL;
	
	CAdminRecord* adminRec = CAdminRecord::NewLC();
	
	temp8 = 0;
	temp8 |= K_ART_CUSTODY_SIGNAL;
	if(aBPARecord.iBundle->IsFragment())
	{
		temp8 |= K_ARF_FOR_FRAGMENT;
	}
	
	adminRec->SetRecordTypesFlags(temp8);
	
	temp8 = aReasonCode;
	if(aCustodySucceeded)
		temp8 |= 0x80;
	
	adminRec->SetStatus(temp8);
	
	if(aBPARecord.iBundle->IsFragment())
	{
		tmpSDNV = CSDNV::NewLC();
		aBPARecord.iBundle->GetFragmentOffset(tmpSDNV);
		adminRec->SetFragmentOffset(tmpSDNV);
		CleanupStack::Pop(tmpSDNV);				// ownership transferred
		tmpSDNV = NULL;
		
		tmpSDNV = CSDNV::NewLC();
		aBPARecord.iBundle->GetPayloadLen(tmpSDNV);
		adminRec->SetFragmentLength(tmpSDNV);
		CleanupStack::Pop(tmpSDNV);				// ownership transferred
		tmpSDNV = NULL;
	}
	
	temp64 = GenerateDTNTime();
	
	adminRec->SetCustodySignalTimeStamp(temp64);
	
	adminRec->SetAssociatedBundleCreationTimeStamp(aBPARecord.iBundle->GetCreationTimeStamp());
	
	CEID* tmpEID = CEID::NewLC(aBPARecord.iBundle->GetSourceEID());
	adminRec->SetAssociatedBundleSourceEID(tmpEID);
	CleanupStack::Pop(tmpEID);				// ownership transferred
	
	CleanupStack::Pop(adminRec);
	
	return adminRec;
}

//-----------------------------------------------------------------------------

void CBPA::HandleCustodySignal(CAdminRecord* aAdminRecord)
{
	TUint8 temp8;
	TInt lp_indx;
	TInt pos = -1;
	TBool matched = EFalse;
	TUint32 tempOffset;
	
	for(lp_indx = 0; lp_indx < iProcessingQ->Count(); lp_indx++)
	{
		matched = ETrue;
		
		if(!iProcessingQ->At(lp_indx)->iBundle->GetSourceEID()->Compare(aAdminRecord->GetAssociatedBundleSourceEID()))
			matched = EFalse;
		
		if(iProcessingQ->At(lp_indx)->iBundle->GetCreationTimeStamp() != aAdminRecord->GetAssociatedBundleCreationTimeStamp())
			matched = EFalse;
		
		iProcessingQ->At(lp_indx)->iBundle->GetFragmentOffset(tempOffset);
		
		if(NULL != aAdminRecord->GetFragmentOffset())
		{
			if(tempOffset != aAdminRecord->GetFragmentOffset()->GetNumericVal())
				matched = EFalse;
		}
		
		if(matched)
		{
			pos = lp_indx;
			break;
		}
	}
	
	if(!matched)
		return;		// might be some hanging around signal
	
	temp8 = aAdminRecord->GetStatus();
	
	if(temp8 & 0x80)
	{
		iProcessingQ->At(pos)->iRetentionConstraints &= (~K_RC_CUSTODY_ACCEPTED);
		
		if(E_BPA_DATA_CUSTODY_ACCEPTED == iProcessingQ->At(pos)->iRecordStatus)
			iProcessingQ->At(pos)->iRecordStatus = E_BPA_DATA_TO_BE_DISCARDED;
		// else might be in some other transition, so leave as it is...
		
		// TODO: remove from consistant storage...
	}
	else
	{
		if((K_CSRC_DEPLETED_STORAGE == temp8) || (K_CSRC_NO_KNOWN_ROUTE_TO_DEST == temp8))
		{
			// retry again
			iProcessingQ->At(pos)->iRecordStatus = E_BPA_DATA_DEST_RESOLVING;
		}
		else
		{
			if(E_BPA_DATA_CUSTODY_ACCEPTED == iProcessingQ->At(pos)->iRecordStatus)
			{
				// release custody
				iProcessingQ->At(pos)->iRetentionConstraints &= (~K_RC_CUSTODY_ACCEPTED);
				iProcessingQ->At(pos)->iRecordStatus = E_BPA_DATA_TO_BE_DISCARDED;
			}
			// else might be in some other transition, so leave as it is...
		}
	}
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
