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

FILE NAME	:	TCPCLA.cpp

DESCRIPTION	:	This file implements TCPCLA class, which specializes Convergence
				Layer Adapter's operation for TCP.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	April 03, 2006

MODIFICATION:	April 03, 2006

HISTORY		:
				03-04-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "BPA_Globals.h"
#include "tcpcla.h"
#include "tcpconnection.h"
#include "sdnv.h"

/*-----------------------------------------------------------------------------
			Class Implementations
-----------------------------------------------------------------------------*/

CTCPCLA* CTCPCLA::NewL(const TUint32 aTimerPeriod, const TInt32 aKeepAliveInterval,
					   const TUint32 aPartialAckLen, const TUint8 aFlags, const TUint8 aMaxRetries,
					   const TGlobalConfigParams& aConfig)
{
	CTCPCLA* self = CTCPCLA::NewLC(aTimerPeriod, aKeepAliveInterval, aPartialAckLen, aFlags, aMaxRetries, aConfig);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CTCPCLA* CTCPCLA::NewLC(const TUint32 aTimerPeriod, const TInt32 aKeepAliveInterval,
						const TUint32 aPartialAckLen, const TUint8 aFlags, const TUint8 aMaxRetries,
						const TGlobalConfigParams& aConfig)
{
	CTCPCLA* self = new (ELeave) CTCPCLA(aKeepAliveInterval, aPartialAckLen, aFlags, aMaxRetries, aConfig);
	CleanupStack::PushL(self);
	self->ConstructL(aTimerPeriod);
	return self;
}

//-----------------------------------------------------------------------------

CTCPCLA::~CTCPCLA()
{
	// all other resources would be freed by base class, including cancelling AO requests
	
	// so that listening socket associated with interface is closed first
	StopListening();
	
	iInterfaceMngr.Stop();
	iInterfaceMngr.Close();
}

//-----------------------------------------------------------------------------

TBool CTCPCLA::StartListening(TUint32 aPort)
{
	// check if already listening
	if(IsListening())
		return EFalse;
	
	TInt retVal;
	
	// open socket
	retVal = iListeningSocket.Open(iSockServer, KAfInet, KSockStream,
								   KProtocolInetTcp, iInterfaceMngr);
	if(KErrNone != retVal)
		return EFalse;
	
	// bind to a port
	retVal = iListeningSocket.SetLocalPort(aPort);
	if(KErrNone != retVal)
		return EFalse;
	
	// start listening
	retVal = iListeningSocket.Listen(5);	// common value in berkely-type sockets
	if(KErrNone != retVal)
		return EFalse;
	
	// create a new blank socket
	iTempSock = new RSocket;
	if(NULL == iTempSock)
		return EFalse;
	retVal = iTempSock->Open(iSockServer);
	if(KErrNone != retVal)
		return EFalse;
	
	// call accept
	iListeningSocket.Accept(*iTempSock, iStatus);
	SetActive();
	
	iListeningStatus = ETrue;
	
	return ETrue;
}

//-----------------------------------------------------------------------------

void CTCPCLA::EventHandler(TAny* aLPVOID)
{
	// TCP CLA performs 3 operatiosn periodically...
	
	// 1. check for new bundles to be sent
	DoCLATxOperations();
	
	// 2. read incoming bundles
	DoCLARxOperations();
	
	// 3. manage connections; send CLA protocol auxillary messages; etc.
	DoCLAAuxillaryOperations();
	
}

//-----------------------------------------------------------------------------

void CTCPCLA::ConstructL(const TUint32 aTimerPeriod)
{
	CCLA::ConstructL(aTimerPeriod);
	
	// We need to activate an IAP, so that listening socket can receive
	// connections. Otherwise it would remain deactive unless 1st bundle is sent
	User::LeaveIfError(iInterfaceMngr.Open(iSockServer));
	User::LeaveIfError(iInterfaceMngr.Start());
}

//-----------------------------------------------------------------------------

void CTCPCLA::DoCancel()
{
	iListeningSocket.CancelAll();
	// timer cancellation would be performed by it's destrcutor
}

//-----------------------------------------------------------------------------

void CTCPCLA::RunL()
{
	if(KErrNone == iStatus.Int())
	{
		// TODO: we need to check whether there's an already open connection from
		// same source?
		
		// socket has been associated with remote node, unless ownership
		// transfers to CConnection object, push TempSock to make it leave safe
		///CleanupClosePushL(*iTempSock);
		// No need to push socket... if function leaves, it will be closed in RunError fn.
		// This is to avoid tempsock ownership transfers to two places: ClnStk & Conn. obj
		
		
		CConnectionInfo *pConnInfo = new (ELeave) CTCPConnectionInfo;
		CleanupStack::PushL(pConnInfo);
		
		// in this case, we surely ain't connection initiator
		pConnInfo->iConnectionInitiator = EFalse;
		
		// We don't know about connection type yet; may be other side wants to
		// be receiver;
		pConnInfo->iConType = CConnectionInfo::E_CON_INIT;
		
		// No need to set following ptr to cleanup-stack, as it's holder object
		// is already safe...
		pConnInfo->iSockInfo = new (ELeave) TInetAddr;
		// Get remote IP and store here... this info would be available even if
		// socket is closed here after
		iTempSock->RemoteName(*(static_cast<TInetAddr*>(pConnInfo->iSockInfo)));
		
		// No need to set following ptr to cleanup-stack, as it's holder object
		// is already safe...
		pConnInfo->iConnectionPtr = CTCPConnection::NewL(iTempSock, *iConfigParams);
		pConnInfo->iConnectionPtr->ConnectedActivate();	// start receiving
		///CleanupStack::Pop();		// iTempSock ownership transferred to Conn.
		iTempSock = NULL;
		
		// Push ConnInfo into array and remove pointer from cleanup-stack
		// due to ownership transfer
		iConnectionQ->AppendL(pConnInfo);
		CleanupStack::Pop(pConnInfo);
		ResetTempSockL();
		
		// every thing went ok, so accept next connection request...
		iListeningSocket.Accept(*iTempSock, iStatus);
		SetActive();
	}
	else
	{
		// some error occurred, try again
				
		iTempSock->Close();		// just in case...
		User::LeaveIfError(iTempSock->Open(iSockServer));
		iListeningSocket.Accept(*iTempSock, iStatus);
		SetActive();
	}
}

//-----------------------------------------------------------------------------

TInt CTCPCLA::RunError(TInt aError)
{
	// TODO: perhaps a better idea would be to switch active object so that 
	// other active objects could release some resources. In that case, a
	// small state machine would be needed to remember cause of error
	
	// There are two possible reasons for error:
	
	// 1. if memory was not allocated for tempSock, then try to get some
	if(NULL != iTempSock)
	{
		iTempSock->Close();		// just in case...
	}
	else
	{
		while(NULL != iTempSock)
		{
			User::After(200);
			iTempSock = new RSocket;
		}
	}
	
	// 2. if sock object couldn't be opened, then try again after some time (200 uSec)
	
	TInt retVal;
	
	while(ETrue)
	{
		User::After(200);
		retVal = iTempSock->Open(iSockServer);
		if(KErrNone == retVal)
			break;
	}
	
	iListeningSocket.Accept(*iTempSock, iStatus);
	SetActive();
	return KErrNone;
}

//-----------------------------------------------------------------------------

CTCPCLA::CTCPCLA(const TInt32 aKeepAliveInterval, const TUint32 aPartialAckLen,
				 const TUint8 aFlags, const TUint8 aMaxRetries, const TGlobalConfigParams& aConfig)
		: CCLA(aMaxRetries), iConfigParams(&aConfig)
{
	iKeepAliveInterval = aKeepAliveInterval;
	iPartialAckLen = aPartialAckLen;
	iFlags = aFlags;
	iVersion = 0x2;
}

//-----------------------------------------------------------------------------

void CTCPCLA::ResetTempSockL()
{
	iTempSock = new (ELeave) RSocket;
	User::LeaveIfError(iTempSock->Open(iSockServer));
}

//-----------------------------------------------------------------------------

void CTCPCLA::DoCLATxOperations()
{
	// hands over all bundles over to an appropriate connection
	
	CConnectionInfo* pConnInfo;
	TInt lp_indx;
	
	for(lp_indx = 0; lp_indx < iTxQ->Count(); lp_indx++)
	{
		// check if there's a new bundle at index pos?
		if(E_BUNDLE_NEW == iTxQ->At(lp_indx).iBundleStatus)
		{
			// locate a matching connection
			pConnInfo = LocateConnection(iTxQ->At(lp_indx).iSockInfo,
										 CConnectionInfo::E_CON_SENDER);
			if(NULL == pConnInfo)
			{
				if(iTxQ->At(lp_indx).iRetriesCount > iMaxRetries)
				{
					TBundleInfo bundleInfo;
					bundleInfo.iBundleStatus = E_BUNDLE_SENT_NOK;
					bundleInfo.iBundleID = iTxQ->At(lp_indx).iBundleID;
					TRAPD(error, iRxQ->AppendL(bundleInfo));	// can't help afford such error at this stage
					if(KErrNone == error)
					{
						iTxQ->At(lp_indx).FreeMem();
						iTxQ->Delete(lp_indx);
					}
					// else try next time
					
					continue;	// in either case, move on to next bundle...
				}
				
				iTxQ->At(lp_indx).iRetriesCount++;
				
				// connection doesn't exist, open a new one
				TRAPD(error, CreateConnectionL(iTxQ->At(lp_indx).iSockInfo));
				// we'll wait until socket connects successfully
				continue;	// whether or not it was successful, move to next
			}
			else	// connection does exist
			{
				if(!pConnInfo->IsReady())
				{
					// wait until CLA parameters are negotiated
					continue;	// move on to next bundle
				}
				else if(!pConnInfo->iConnectionPtr->IsConnected())
				{
					// connection exists but is closed;
					// some how this infor has not propagated to CLA yet
					// other wise previous condition would have detected it
					continue;	// just wait until it gets synchronized
				}
				else	// deliver the bundle
				{
					// compose CLA header
					HBufC8* ptrBuff = NULL;
					TRAPD(error, ComposeCLADataUnitL(iTxQ->At(lp_indx), ptrBuff));
					if(KErrNone == error)
					{
						CConnection::TConnectionOperation conOp;
						conOp.iOpCode = CConnection::E_COC_TX_DATA;
						conOp.iData = ptrBuff;
						conOp.iTransactionID = ++iTransactionID;
						
						if(pConnInfo->iConnectionPtr->Push(conOp))
						{
							// handed over to connection successfully; change state
							iTxQ->At(lp_indx).iBundleStatus = E_BUNDLE_SENDING;
							iTxQ->At(lp_indx).iTransactionID = conOp.iTransactionID;
							conOp.iData = NULL;
							// log tx operation time
							(static_cast<CTCPConnectionInfo*>(pConnInfo))->iLastTxMoment.UniversalTime();
						}
						// else let's try next time
						// as soon as conOp goes out of scope, it'll delete ptrBuff too
					}
					else
					{
						continue;	// move on to next bundle
					}
				}	// end deliver the bundle...
			}		// end connection does exist...
		}			// end bundle new...
	}				// end loop
}

//-----------------------------------------------------------------------------

void CTCPCLA::DoCLARxOperations()
{
	CTCPConnectionInfo* pConnInfo;
	TInt lp_indx;
	CConnection::TConnectionOperation conOp;
	
	for(lp_indx = 0; lp_indx < iConnectionQ->Count(); lp_indx++)
	{
		// we've to check every connection, as even closed ones may have data.
		pConnInfo = static_cast<CTCPConnectionInfo*>(iConnectionQ->At(lp_indx));
		
		// delete previous copy
		delete conOp.iData;
		conOp.iData = NULL;
		// pick top most element from FIFO of each connection
		if(pConnInfo->iConnectionPtr->Pop(conOp))
		{
			switch(conOp.iOpCode)
			{
				case CConnection::E_COC_CONNECTED_OK:
					if(CConnectionInfo::E_CON_INIT == pConnInfo->iConType)
					{
						// this is time to send contact header
						if(!SendContactHeader(pConnInfo))
						{
							//actual termination would be performed in auxillary operations
							pConnInfo->iConType = CConnectionInfo::E_CON_TERMINATED;
						}
					}
					else
					{
						// this is state machine violation, needs to be fixed
						User::Panic(_L("Must be in INIT state, just after TCP connection established"), 420);
					}
					break;
				
				case CConnection::E_COC_CONNECTED_NOK:
					if(CConnectionInfo::E_CON_INIT == pConnInfo->iConType)
					{
						//actual termination would be performed in auxillary operations
						pConnInfo->iConType = CConnectionInfo::E_CON_TERMINATED;
					}
					else
					{
						// this is state machine violation, needs to be fixed
						User::Panic(_L("Must be in INIT state, just after TCP connection established"), 420);
					}
					break;
				
				case CConnection::E_COC_TX_STAT_OK:
					if(CConnectionInfo::E_CON_INIT == pConnInfo->iConType)
					{
						// contact header must have been transmitted successfully
						if(conOp.iTransactionID != 0)
						{
							// verify it was contact header, not a bundle
							User::Panic(_L("In INIT state, no bundle must have been sent"), 420);
						}
						else
						{
							if(!pConnInfo->iConnectionInitiator)
							{
								// If we're not conn. initiator, then contact header exchange should have been
								// completed by both the sides. Change state to enable data exhange
								if(pConnInfo->iFlags & K_FLAG_I_AM_RCVR)
								{
									// initiator party decided to be a sender
									pConnInfo->iConType = CConnectionInfo::E_CON_RECEIVER;
								}
								else
								{
									// initiator party decided to be a receiver
									pConnInfo->iConType = CConnectionInfo::E_CON_SENDER;
								}
								
								// Now that connection is established & params negotiated, initialize timers
								// otherwise, first time diff. would be too large
								pConnInfo->iLastTxMoment.UniversalTime();
								pConnInfo->iLastRxMoment.UniversalTime();
							}
							// else we've sent contact header first, so let's wait until other party 
							// sends its parameters as well, and change conn. state there
						}
					}
					else if(CConnectionInfo::E_CON_SENDER == pConnInfo->iConType)
					{
						// we must have sent some bundle or other message
						if(conOp.iTransactionID != 0)
						{
						 	// must be for a bundle sent, check whether we need to wait for CLA-ACK or not
							if(!(pConnInfo->iFlags & K_FLAG_REQ_ACK))
							{
								// no need to wait for CLA-ACK, declare bundle to be sent successfully
								TInt index = LocateBundleInfoRecord(iTxQ, conOp.iTransactionID);
								if(index >= 0)
								{
									if(!AcknowledgeBundleTransmission(index, ETrue, 0))
									{
										continue;	// we'll try later
									}
									// else proceed normally
								}
								else
								{
									if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
									{
										User::InfoPrint(_L("DoCLARxOperations :: invalid msg token from connection..."));
									}
								}
							}
							// else wait until CLA ACK arrives, and proceed normally
							// although connection is terminated but CLA-ACK may be in Q
						}
						// else could be for a KeepAlive etc., proceed normally
					}
					else if(CConnectionInfo::E_CON_RECEIVER == pConnInfo->iConType)
					{
						if(conOp.iTransactionID != 0)
						{
							// must be for a CLA-ACK for a received message
							AcknowledgeBundleReception(conOp.iTransactionID, ETrue);
						}
						// else could be for a KeepAlive etc., proceed normally
					}
					else if(CConnectionInfo::E_CON_TERMINATED == pConnInfo->iConType)
					{
						// if it's associated with a bundle...
						if(conOp.iTransactionID != 0)
						{
							if(pConnInfo->iFlags & K_FLAG_I_AM_RCVR)
							{
								// must be for a CLA-ACK for a received message
								AcknowledgeBundleReception(conOp.iTransactionID, ETrue);
							}
							else
							{
								if(!(pConnInfo->iFlags & K_FLAG_REQ_ACK))
								{
									// no need to wait for CLA-ACK, declare bundle to be sent successfully
									TInt index = LocateBundleInfoRecord(iTxQ, conOp.iTransactionID);
									if(index >= 0)
									{
										if(!AcknowledgeBundleTransmission(index, ETrue, 0))
										{
											continue;	// we'll try later
										}
										// else proceed normally
									}
									else
									{
										if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
										{
											User::InfoPrint(_L("DoCLARxOperations :: invalid msg token from connection..."));
										}
									}
								}
								// else wait until CLA ACK arrives, and proceed normally
								// although connection is terminated but CLA-ACK may be in Q
							}
						}
						// else might be for a KeepAlive or other status msg; proceed normally
					}
					break;
				
				case CConnection::E_COC_TX_STAT_NOK:
					if(CConnectionInfo::E_CON_INIT == pConnInfo->iConType)
					{
						// verify it was for contact header, not a bundle
						if(conOp.iTransactionID != 0)
						{
							User::Panic(_L("In INIT state, no bundle must have been sent"), 420);
						}
						else
						{
							// now that contact header was not transmitted successfully,
							// terminate connection
							pConnInfo->iConnectionPtr->Disconnect();
							pConnInfo->iConType = CConnectionInfo::E_CON_TERMINATED;
						}
					}
					else if(CConnectionInfo::E_CON_SENDER == pConnInfo->iConType)
					{
						// we must have sent some bundle or other message
						if(conOp.iTransactionID != 0)
						{
						 	// must be for a bundle sent
							TInt index = LocateBundleInfoRecord(iTxQ, conOp.iTransactionID);
							if(index >= 0)
							{
								if(!AcknowledgeBundleTransmission(index, EFalse, 0))
								{
									continue;	// we'll try later
								}
								// else proceed normally
							}
							else
							{
								if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
								{
									User::InfoPrint(_L("DoCLARxOperations :: invalid msg token from connection..."));
								}
							}
						}
						// else could be for a KeepAlive etc., proceed normally
					}
					else if(CConnectionInfo::E_CON_RECEIVER == pConnInfo->iConType)
					{
						if(conOp.iTransactionID != 0)
						{
							// must be for a CLA-ACK for a received message
							AcknowledgeBundleReception(conOp.iTransactionID, EFalse);
						}
						// else could be for a KeepAlive etc., proceed normally
					}
					else if(CConnectionInfo::E_CON_TERMINATED == pConnInfo->iConType)
					{
						// if it's associated with a bundle...
						if(conOp.iTransactionID != 0)
						{
							if(pConnInfo->iFlags & K_FLAG_I_AM_RCVR)
							{
								// must be for a CLA-ACK for a received message
								AcknowledgeBundleReception(conOp.iTransactionID, EFalse);
							}
							else
							{
								TInt index = LocateBundleInfoRecord(iTxQ, conOp.iTransactionID);
								if(index >= 0)
								{
									if(!AcknowledgeBundleTransmission(index, EFalse, 0))
									{
										continue;	// we'll try later
									}
									// else proceed normally
								}
								else
								{
									if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
									{
										User::InfoPrint(_L("DoCLARxOperations :: invalid msg token from connection..."));
									}
								}
							}
						}
						// else might be for a KeepAlive or other status msg; proceed normally
					}
					break;
				
				case CConnection::E_COC_RX_DATA:
					// there are are 2 possiblities
					// 1. it must be contact header, if it's E_CON_INIT state
					if(CConnectionInfo::E_CON_INIT == pConnInfo->iConType)
					{
						if(HandleContactHeaderMsg(pConnInfo, conOp.iData))
						{
							if(pConnInfo->iConnectionInitiator)
							{
								// If we're conn. initiator, then contact header exchange should have been
								// completed by both the sides. Change state to enable data exhange
								if(pConnInfo->iFlags & K_FLAG_I_AM_RCVR)
								{
									// initiator party decided to be a sender
									pConnInfo->iConType = CConnectionInfo::E_CON_RECEIVER;
								}
								else
								{
									// initiator party decided to be a receiver
									pConnInfo->iConType = CConnectionInfo::E_CON_SENDER;
								}
								// Now that connection is established & params negotiated, initialize timers
								// otherwise, first time diff. would be too large
								pConnInfo->iLastTxMoment.UniversalTime();
								pConnInfo->iLastRxMoment.UniversalTime();
							}
							
							else
							{
								// we'll decide when our contact header would be transmitted successfully
								if(!SendContactHeader(pConnInfo))
								{
									pConnInfo->iConnectionPtr->Disconnect();
									pConnInfo->iConType = CConnectionInfo::E_CON_TERMINATED;
								}
							}
						}
						else	// error in contact header processing
						{
							pConnInfo->iConnectionPtr->Disconnect();
							pConnInfo->iConType = CConnectionInfo::E_CON_TERMINATED;
						}
					}
					
					// 2. not a contact header, but some otehr message
					else
					{
						if(NULL == pConnInfo->iFragmentBuffer)
						{
							pConnInfo->iFragmentBuffer = conOp.iData;
						}
						else
						{
							HBufC8 *tmp = NULL;
							TInt newSize =  pConnInfo->iFragmentBuffer->Size() + conOp.iData->Size();
							tmp = pConnInfo->iFragmentBuffer->ReAlloc(newSize);
							if(NULL != tmp)
							{
								__ASSERT_ALWAYS((tmp->Des().MaxSize() >= newSize), User::Panic(_L("TCP CLA"), K_BPA_PAN_MEM_EXP));
								pConnInfo->iFragmentBuffer = tmp;
								pConnInfo->iFragmentBuffer->Des().Append(*(conOp.iData));
							}
							else
							{
								continue;	//try next time
							}
						}
						
						conOp.iData = NULL;		// ownership transferred
					
						// it will help in determinign if connection has been active
						pConnInfo->iLastRxMoment.UniversalTime();
						
						if(!ProcessIncomingMsg(pConnInfo))
							continue;
					}
					
					break;
				
				case CConnection::E_COC_CLOSED:
					// What ever the state of connection is, declare it as terminated
					// actual termination would be performed in auxillary operations
					pConnInfo->iConType = CConnectionInfo::E_CON_TERMINATED;
					break;
				
				default:
					User::Panic(_L("TCPCLA: invalid op code from TCP connection"), 420);
			}
		
			// this would delete data as well, if any
			pConnInfo->iConnectionPtr->PopOut();			
		}	// end msg popped
		
		else
		{
			// memory may not be available to pop a msg
			if(NULL != pConnInfo->iFragmentBuffer)
			{
				// some old fragment needs processing
				if(!ProcessIncomingMsg(pConnInfo))
					continue;
			}
		}
	}		// end for loop
}

//-----------------------------------------------------------------------------

void CTCPCLA::DoCLAAuxillaryOperations()
{
	// Here we perform connection management operations:
	
	CTCPConnectionInfo* pConnInfo = NULL;
	TInt lp_indx;
	TTime now;
	TTimeIntervalSeconds deltaTime;
	
	for(lp_indx = 0; lp_indx < iConnectionQ->Count(); lp_indx++)
	{
		pConnInfo = static_cast<CTCPConnectionInfo*>(iConnectionQ->At(lp_indx));
		
		if(pConnInfo->IsReady())
		{
			// 1. check if there has been no Tx activity for a while; send KeepAlive
			now.UniversalTime();
			now.SecondsFrom(pConnInfo->iLastTxMoment, deltaTime);
			if((deltaTime > pConnInfo->iKeepAliveInterval) && (pConnInfo->iKeepAliveInterval.Int() != 0))
			{
				HBufC8* msgBuff = HBufC8::New(1);
				if(NULL == msgBuff)
					continue;
				TPtr8 ptrBuff = msgBuff->Des();
				TUint8 tempByte = K_MSG_TYPE_KEEP_ALIVE;
				ptrBuff.Append(&tempByte, 1);
				
				CConnection::TConnectionOperation conOp;
				conOp.iOpCode = CConnection::E_COC_TX_DATA;
				conOp.iData = msgBuff;
				// no transaction ID needed
								
				if(pConnInfo->iConnectionPtr->Push(conOp))
				{
					conOp.iData = NULL;
					pConnInfo->iLastTxMoment.UniversalTime();
				}
				// else try next time
			}
			
			// 2. check if there has been no Rx activity for a while
			// terminate connection
			now.UniversalTime();
			now.SecondsFrom(pConnInfo->iLastRxMoment, deltaTime);
			TTimeIntervalSeconds tempSec = (pConnInfo->iKeepAliveInterval).Int()*2;
			if(deltaTime > tempSec)
			{
				HBufC8* msgBuff = HBufC8::New(1);
				if(NULL == msgBuff)
					continue;
				TPtr8 ptrBuff = msgBuff->Des();
				TUint8 tempByte = K_MSG_TYPE_SHUT_DOWN;
				ptrBuff.Append(&tempByte, 1);
				
				CConnection::TConnectionOperation conOp;
				conOp.iOpCode = CConnection::E_COC_TX_DATA;
				conOp.iData = msgBuff;
				// no transaction ID is needed
		
				if(pConnInfo->iConnectionPtr->Push(conOp))
				{
					conOp.iData = NULL;
					// no one should send more data from now onwards
					pConnInfo->iConType = CConnectionInfo::E_CON_TERMINATED;
					// we'll close connection actually when other side sends same msg
				}
				// else try next time
			}
		}
		
		else if(CConnectionInfo::E_CON_TERMINATED == pConnInfo->iConType)
		{
			if(pConnInfo->iConnectionPtr->IsRxQEmpty() && pConnInfo->iConnectionPtr->IsTxQEmpty())
			{
				if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
				{
					User::InfoPrint(_L("DoCLAAuxillaryOperations... deleting a connection"));
				}
				
				// delete connection
				delete pConnInfo;
				iConnectionQ->Delete(lp_indx);
			}
		}
		
	}
}

//-----------------------------------------------------------------------------

CConnectionInfo* CTCPCLA::LocateConnection(TSockAddr* aAddr,
										   const CConnectionInfo::TConnectionType aConType)
{
	CConnectionInfo* pConnInfo = NULL;
	TInetAddr *tempAddrIn = static_cast<TInetAddr*>(aAddr);
	TInetAddr *tempAddr;
	// find an already existing sender-connection
	for(TInt lp_indx = 0; lp_indx < iConnectionQ->Count(); lp_indx++)
	{
		pConnInfo = iConnectionQ->At(lp_indx);
		// match based on IP address only (as remote port can be arbitrary) and
		// connection type
		tempAddr = static_cast<TInetAddr*>(pConnInfo->iSockInfo);
		
		if( (tempAddr->Match(*tempAddrIn)) &&
			((aConType == pConnInfo->iConType) ||
			 (CConnectionInfo::E_CON_INIT == pConnInfo->iConType) ||
			 (CConnectionInfo::E_CON_TERMINATED == pConnInfo->iConType)) )
		{
			break;
		}
		else
		{
			pConnInfo = NULL;
		}
	}
	
	return pConnInfo;
}

//-----------------------------------------------------------------------------

CConnectionInfo* CTCPCLA::CreateConnectionL(TSockAddr* aAddr)
{
	// If this time, a connection can't be created, we leave silently
	// to try next time. 
	
	CConnectionInfo *pConnInfo = new (ELeave) CTCPConnectionInfo;
	CleanupStack::PushL(pConnInfo);
	
	RSocket *tmpSock = new (ELeave) RSocket;
	CleanupClosePushL(*tmpSock);
	
	User::LeaveIfError(tmpSock->Open(iSockServer, KAfInet, KSockStream,
									 KProtocolInetTcp, iInterfaceMngr) );
	
	
	// in this case, we surely are connection initiator
	pConnInfo->iConnectionInitiator = ETrue;
		
	// No need to set following pointers to cleanup-stack, as their holder
	// object is already safe...
	
	// Make a copy of remote IP here (as we know the destination)...
	// this info would be available even if socket is closed here after
	pConnInfo->iSockInfo = static_cast<TSockAddr*>(new (ELeave) TInetAddr(*aAddr));
	pConnInfo->iConnectionPtr = CTCPConnection::NewL(tmpSock, *iConfigParams);
	CleanupStack::Pop();		// tmpSock ownership transferred
	
	// TODO: confirm that this sock object won't ever be modified by this call
	pConnInfo->iConnectionPtr->ConnectTo(pConnInfo->iSockInfo);
	
	// Push ConnInfo into array and remove pointer from cleanup-stack
	// due to ownership transfer
	iConnectionQ->AppendL(pConnInfo);
	CleanupStack::Pop(pConnInfo);
	
	return pConnInfo;
}

//-----------------------------------------------------------------------------

void CTCPCLA::ComposeCLADataUnitL(TBundleInfo& aBundleInfo, HBufC8*& aDataOut)
{
	TUint32 len = static_cast<TUint32>(aBundleInfo.iBundleBuff->Size());
	CSDNV *lenStr;
	lenStr = CSDNV::NewLC(len);
	
	TInt totLen = static_cast<TInt>(lenStr->ByteStrLen()) + 
				  static_cast<TInt>(len) + K_MSG_TYPE_LEN + K_BUNDLE_ID_LEN;
	
	HBufC8 *tempBuff = HBufC8::NewLC(totLen);
	TPtr8 ptrBuff = tempBuff->Des();
	
	// append opcode
	TUint8 tempByte = K_MSG_TYPE_BUNDLE_DATA;
	ptrBuff.Append(&tempByte, 1);
	
	// append bundle id
	TUint32 tempId = ByteOrder::Swap32(aBundleInfo.iBundleID);
	TUint8* ptrId = reinterpret_cast<TUint8*>(&tempId);
	ptrBuff.Append(ptrId, 4);
	
	// append SDNV len
	const TDesC8& tempRef = lenStr->DesL();
	ptrBuff.Append(tempRef);
	
	// append bundle
	ptrBuff.Append(*(aBundleInfo.iBundleBuff));
	
	CleanupStack::Pop(tempBuff);
	CleanupStack::PopAndDestroy(lenStr);
	aDataOut = tempBuff;
}

//-----------------------------------------------------------------------------

void CTCPCLA::ComposeACKMsgL(const TBundleInfo& aBundleInfo, const TUint32 aAckedLen, HBufC8*& aDataOut)
{
	CSDNV *lenStr = CSDNV::NewLC(aAckedLen);
	
	TInt totLen = static_cast<TInt>(lenStr->ByteStrLen()) + 
				  K_MSG_TYPE_LEN + K_BUNDLE_ID_LEN;
	
	HBufC8 *tempBuff = HBufC8::NewLC(totLen);
	TPtr8 ptrBuff = tempBuff->Des();
	
	// append opcode
	TUint8 tempByte = K_MSG_TYPE_BUNDLE_ACK;
	ptrBuff.Append(&tempByte, 1);
	
	// append bundle id
	TUint32 tempId = ByteOrder::Swap32(aBundleInfo.iBundleID);
	TUint8* ptrId = reinterpret_cast<TUint8*>(&tempId);
	ptrBuff.Append(ptrId, 4);
	
	// append SDNV len
	const TDesC8& tempRef = lenStr->DesL();
	ptrBuff.Append(tempRef);
	
	CleanupStack::Pop(tempBuff);
	CleanupStack::PopAndDestroy(lenStr);
	aDataOut = tempBuff;
}

//-----------------------------------------------------------------------------

TBool CTCPCLA::ComposeContactHeader(const CTCPConnectionInfo *aConInfo, HBufC8*& aDataOut)
{
	TUint8* ptrByteArray = NULL;
	
	// We must re-calculate the flags depending upon who is connection initiator
	TUint8 tempFlags = iFlags;
	if(aConInfo->iConnectionInitiator)
	{
		// at the moment, we can olny be sender, as an initiator
		tempFlags = tempFlags & (~K_FLAG_I_AM_RCVR);	// reset 3rd bit
		// initialize flags in conn info
		(const_cast<CTCPConnectionInfo*>(aConInfo))->iFlags = tempFlags;
	}
	else
	{
		// in this case, we've no right to decide about that...
		// always set this bit to 0 (as per specs)
		tempFlags = tempFlags & (~K_FLAG_I_AM_RCVR);	// reset 3rd bit
	}
	
	HBufC8* tempHeader;
	tempHeader = HBufC8::New(K_CONTACT_HEADER_SIZE);
	if(NULL == tempHeader)
		return EFalse;
	
	TPtr8 ptrHeader = tempHeader->Des();
	
	ptrHeader.Append(K_MAGIC_STR);
	
	ptrHeader.Append(&iVersion, 1);
	
	ptrHeader.Append(&tempFlags, 1);
	
	TUint16 temp16 = ByteOrder::Swap16(static_cast<TUint16>(iKeepAliveInterval.Int()));
	ptrByteArray = reinterpret_cast<TUint8*>(&temp16);
	ptrHeader.Append(ptrByteArray, 2);
	
	TUint32 temp32 = ByteOrder::Swap32(iPartialAckLen);
	ptrByteArray = reinterpret_cast<TUint8*>(&temp32);
	ptrHeader.Append(ptrByteArray, 4);
	
	aDataOut = tempHeader;
	
	return ETrue;
}

//-----------------------------------------------------------------------------

void CTCPCLA::HandleBundleDataMsgL(CTCPConnectionInfo* pConnInfo)
{
	if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
	{
		User::InfoPrint(_L("HandleBundleDataMsgL... start"));
	}
	
	// To handle fragmentation and aggregation, we return normally
	
	// Incoming data is always from fragment buffer
	const TDesC8* aDataIn = pConnInfo->iFragmentBuffer;
	
	if(aDataIn->Size() < 6)
	{
		// type code, bundle id and at least 1 byte of SDNV is expected
		User::Leave(KErrNone);
	}
	
	// extract bundle id
	TUint32 bundleId = BigEndian::Get32(aDataIn->Mid(1,4).Ptr());
	
	// extract bundle length
	CSDNV* bundleLen = CSDNV::NewLC();
	
	TInt index = 5;		// start of bundle length field
	TUint8 temp8;
	
	while(ETrue)
	{
		temp8 = (*aDataIn)[index++];
		bundleLen->UpdateL(temp8);
		
		if(!(temp8 & 0x80))
		{
			break;			// MSB 0 detected... end of SDNV
		}
		else if(index >= aDataIn->Size())
		{
			// may be incomplete chunk (if SDNV is not corrupt), try next time
			User::Leave(KErrNone);
		}
		else if((index-5) > K_SDNV_MAX_STRBYTE_LEN)
		{
			if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
			{
				User::InfoPrint(_L("Too long SDNV... bundle msg could be corrupt!"));
			}
			User::Leave(KErrTooBig);
		}
	}
	
	// extract remaining part from buffer
	TPtrC8 ptrData = aDataIn->Mid(index);
	
	// check if we have full bundle including
	if(bundleLen->GetNumericVal() > static_cast<TUint32>(ptrData.Size()))
	{
		User::Leave(KErrNone);	// TODO: how to send partial ack?
	}
	
	// else there's definitely complete data, at least (may be more, so extract exact part)
	ptrData.Set(aDataIn->Mid(index, bundleLen->GetNumericVal()));
	
	// could be more data, so remember last location
	index += ptrData.Size();
	
	// create a buffer large enough to hold bundle
	HBufC8* bundleData = HBufC8::NewLC(bundleLen->GetNumericVal());
	TPtr8 ptrBuff = bundleData->Des();
	ptrBuff.Append(ptrData);
	
	HBufC8* extraData = NULL;
	if(index < aDataIn->Size())
	{
		TPtrC8 ptrExtra = aDataIn->Mid(index);
		extraData = HBufC8::NewLC(ptrExtra.Size());
		extraData->Des().Append(ptrExtra);
	}
	
	// create a bundle info record
	TBundleInfo bundleInfo;
	bundleInfo.iBundleStatus = E_BUNDLE_RECEIVING;
	bundleInfo.iSockInfo = NULL;
	bundleInfo.iBundleID = bundleId;
	
	// send ACK
	if(pConnInfo->iFlags & K_FLAG_REQ_ACK)
	{
		HBufC8* tempAck = NULL;
		ComposeACKMsgL(bundleInfo, bundleLen->GetNumericVal(), tempAck);
		
		CConnection::TConnectionOperation conOp;
		conOp.iOpCode = CConnection::E_COC_TX_DATA;
		conOp.iData = tempAck;
		conOp.iTransactionID = ++iTransactionID;
		
		if(!pConnInfo->iConnectionPtr->Push(conOp))
		{
			User::Leave(KErrNone);	// let's try next time
		}
		// else
		conOp.iData = NULL;
		// log tx operation time
		pConnInfo->iLastTxMoment.UniversalTime();
		
		// we'll confirm reception status when ACK is sent successfully
		
		bundleInfo.iTransactionID = conOp.iTransactionID;
	}
	else	// otherwise do it right now
	{
		bundleInfo.iBundleStatus = E_BUNDLE_RECEIVED_OK;
	}
	
	iRxQ->AppendL(bundleInfo);
	// append data only if record is successfully added
	iRxQ->At(iRxQ->Count()-1).iBundleBuff = bundleData;
	
	delete pConnInfo->iFragmentBuffer;
	if(NULL == extraData)
	{
		pConnInfo->iFragmentBuffer = NULL;
	}
	else
	{
		pConnInfo->iFragmentBuffer = extraData;
		CleanupStack::Pop(extraData);
	}
	
	// after ownership transfer, pointer is save now
	CleanupStack::Pop(bundleData);
	
	CleanupStack::PopAndDestroy(bundleLen);
	
	// If bundle has not been received completely, even then we've to return
	// to shorten CLA operations per iteration / event.
	if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
	{
		User::InfoPrint(_L("HandleBundleDataMsgL... end"));
	}
}

//-----------------------------------------------------------------------------

void CTCPCLA::HandleBundleAckMsgL(CTCPConnectionInfo* pConnInfo)
{
	if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
	{
		User::InfoPrint(_L("HandleBundleAckMsgL... start"));
	}
	
	// To handle fragmentation and aggregation, we return normally
	
	// Incoming data is always from fragment buffer
	const TDesC8* aDataIn = pConnInfo->iFragmentBuffer;
	
	if(aDataIn->Size() < 6)
	{
		// type code, bundle id and at least 1 byte of SDNV is expected
		User::Leave(KErrNone);
	}
	
	// extract bundle id
	TUint32 bundleId = BigEndian::Get32(aDataIn->Mid(1,4).Ptr());
	
	// extract bundle length
	CSDNV* bundleLen = CSDNV::NewLC();
	
	TInt index = 5;		// start of bundle length field
	TUint8 temp8;
	
	while(ETrue)
	{
		temp8 = (*aDataIn)[index++];
		bundleLen->UpdateL(temp8);
		
		if(!(temp8 & 0x80))
		{
			break;			// MSB 0 detected... end of SDNV
		}
		else if(index >= aDataIn->Size())
		{
			// may be incomplete chunk (if SDNV is not corrupt), try next time
			User::Leave(KErrNone);
		}
		else if((index-5) > K_SDNV_MAX_STRBYTE_LEN)
		{
			if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
			{
				User::InfoPrint(_L("Too long SDNV... bundle msg could be corrupt!"));
			}
			User::Leave(KErrTooBig);
		}
	}
	
	HBufC8* extraData = NULL;
	if(index < aDataIn->Size())
	{
		TPtrC8 ptrExtra = aDataIn->Mid(index);
		extraData = HBufC8::NewLC(ptrExtra.Size());
		extraData->Des().Append(ptrExtra);
	}
	
	// locate matching bundle in TxQ
	index = LocateBundleInfoRecord(iTxQ, bundleId);
	
	if(index >= 0)
	{
		if(static_cast<TUint32>(iTxQ->At(index).iBundleBuff->Size()) == bundleLen->GetNumericVal())
		{
			if(!AcknowledgeBundleTransmission(index, ETrue, bundleLen->GetNumericVal()))
			{
				User::Leave(KErrCancel);
			}
			// else proceed normally
		}
		else
		{
			// TODO: check if partial length is OK?
			if(!AcknowledgeBundleTransmission(index, EFalse, bundleLen->GetNumericVal()))
			{
				User::Leave(KErrCancel);
			}
			// else proceed normally
		}
	}
	else
	{
		if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
		{
			User::InfoPrint(_L("HandleBundleAckMsgL :: invalid msg token from connection..."));
		}
	}
	
	delete pConnInfo->iFragmentBuffer;
	if(NULL == extraData)
	{
		pConnInfo->iFragmentBuffer = NULL;
	}
	else
	{
		pConnInfo->iFragmentBuffer = extraData;
		CleanupStack::Pop(extraData);
	}
	
	CleanupStack::PopAndDestroy(bundleLen);
	
	if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
	{
		User::InfoPrint(_L("HandleBundleAckMsgL... end"));
	}
}

//-----------------------------------------------------------------------------

void CTCPCLA::HandleKeepAliveMsgL(CTCPConnectionInfo* pConnInfo)
{
	if(iConfigParams->iLogLevel & K_LOG_TCP_CLA)
	{
		User::InfoPrint(_L("HandleKeepAliveMsg..."));
	}
	// To handle fragmentation and aggregation, we return normally
	
	// Incoming data is always from fragment buffer
	const TDesC8* aDataIn = pConnInfo->iFragmentBuffer;
	
	// We've already logged the time of reception, so no other processing
	// is required
	
	// check if other msgs are merged after this
	
	TInt index = 1;		// just a type code
	
	HBufC8* extraData = NULL;
	if(index < aDataIn->Size())
	{
		TPtrC8 ptrExtra = aDataIn->Mid(index);
		extraData = HBufC8::NewLC(ptrExtra.Size());
		extraData->Des().Append(ptrExtra);
	}
	
	delete pConnInfo->iFragmentBuffer;
	if(NULL == extraData)
	{
		pConnInfo->iFragmentBuffer = NULL;
	}
	else
	{
		pConnInfo->iFragmentBuffer = extraData;
		CleanupStack::Pop(extraData);
	}
	
}

//-----------------------------------------------------------------------------

void CTCPCLA::HandleShutDownMsgL(CTCPConnectionInfo* pConnInfo)
{
	// To handle fragmentation and aggregation, we return normally
	
	// Incoming data is always from fragment buffer
	const TDesC8* aDataIn = pConnInfo->iFragmentBuffer;
	
	// check if other msgs are merged after this
	
	TInt index = 1;		// just a type code
	
	HBufC8* extraData = NULL;
	if(index < aDataIn->Size())
	{
		TPtrC8 ptrExtra = aDataIn->Mid(index);
		extraData = HBufC8::NewLC(ptrExtra.Size());
		extraData->Des().Append(ptrExtra);
	}
	
	delete pConnInfo->iFragmentBuffer;
	if(NULL == extraData)
	{
		pConnInfo->iFragmentBuffer = NULL;
	}
	else
	{
		pConnInfo->iFragmentBuffer = extraData;
		CleanupStack::Pop(extraData);
	}
	
	// should Send a sutdown message to other side as well
	// NOTE: current ref. implementation closes its side of connection,
	// immidiately after sendign SD msg. So it's useless to send SD msg in return
	
	// just disconnect
	pConnInfo->iConnectionPtr->Disconnect();
	pConnInfo->iConType = CConnectionInfo::E_CON_TERMINATED;
	
}

//-----------------------------------------------------------------------------

TBool CTCPCLA::SendContactHeader(CTCPConnectionInfo* pConnInfo)
{
	HBufC8* contactHeader = NULL;
	
	if(ComposeContactHeader(pConnInfo, contactHeader))
	{
		CConnection::TConnectionOperation conOp;
		conOp.iOpCode = CConnection::E_COC_TX_DATA;
		// TODO: this data is not associated with a bundle, so how to match
		// it when response arrives?
		conOp.iData = contactHeader;
		
		if(pConnInfo->iConnectionPtr->Push(conOp))
		{
			conOp.iData = NULL;		// before conOp goes out of scope
			return ETrue;			// wait until other party also sends parameters
		}
		else
		{
			// as soon as conOp goes out of scope, it'll delete contact header too
			return EFalse;
		}
	}
	
	return EFalse;	
}

//-----------------------------------------------------------------------------

TBool CTCPCLA::HandleContactHeaderMsg(CTCPConnectionInfo* pConnInfo, const TDesC8* aDataIn)
{
	// TOOD: what if contact header msg is segmented by TCP, or is merged
	// with other segment? Probably put this processing into a function, to deal that?

	TUint32 temp32;
	TTimeIntervalSeconds tempSec;
	TUint8 temp8;
	
	// match magic string
	if(0 != aDataIn->Left(K_MAGIC_LEN).Compare(K_MAGIC_STR))
	{
		return EFalse;
	}
	
	// validate version
	if(iVersion > (*aDataIn)[4])
	{
		return EFalse;
	}
	
	// extract flags
	temp8 = (*aDataIn)[5];
	// recalculate flags by comparing values of both sides
	
	if((temp8 & K_FLAG_REQ_ACK) && (iFlags & K_FLAG_REQ_ACK))
		pConnInfo->iFlags = pConnInfo->iFlags | K_FLAG_REQ_ACK;
	else
		pConnInfo->iFlags = pConnInfo->iFlags & (~K_FLAG_REQ_ACK);
	
	if((temp8 & K_FLAG_REQ_REAC_FRAG) && (iFlags & K_FLAG_REQ_REAC_FRAG))
		pConnInfo->iFlags = pConnInfo->iFlags | K_FLAG_REQ_REAC_FRAG;
	else
		pConnInfo->iFlags = pConnInfo->iFlags & (~K_FLAG_REQ_REAC_FRAG);
	
	if(!pConnInfo->iConnectionInitiator)
	{
		// if we're not connection initiatior, it's other party's right to decide
		if(temp8 & K_FLAG_I_AM_RCVR)
			pConnInfo->iFlags = pConnInfo->iFlags & (~K_FLAG_I_AM_RCVR);
		else
			pConnInfo->iFlags = pConnInfo->iFlags | K_FLAG_I_AM_RCVR;
	}
	else	// else we've to decide what role we prefer
	{
		// at the moment, we always would like to be sender
		//TODO: implement how to become a receiver in this way?
		pConnInfo->iFlags = pConnInfo->iFlags & (~K_FLAG_I_AM_RCVR);
	}
	
	// extract 16-bit keep-alive interval
	tempSec = static_cast<TInt>(BigEndian::Get16(aDataIn->Mid(6,2).Ptr()));
	// recalcualte parameters
	if(0 == iKeepAliveInterval.Int())
		pConnInfo->iKeepAliveInterval = 0;
	else if(iKeepAliveInterval < tempSec)
		pConnInfo->iKeepAliveInterval = iKeepAliveInterval;
	else
		pConnInfo->iKeepAliveInterval = tempSec;
	
	// extract partial ack length
	temp32 = BigEndian::Get32(aDataIn->Mid(8,4).Ptr());
	
	if(0 == iPartialAckLen)
		pConnInfo->iPartialAckLen = 0;
	else if(iPartialAckLen < temp32)
		pConnInfo->iPartialAckLen = iPartialAckLen;
	else
		pConnInfo->iPartialAckLen = temp32;
	
	// TODO: if connection initiator is receiver, then it would transmit
	// more info (see TCP CLA draft sec. 3.3. Handle that info here
	
	return ETrue;
}

//-----------------------------------------------------------------------------
			
//-----------------------------------------------------------------------------

TBool CTCPCLA::AcknowledgeBundleTransmission(const TInt aIndex, const TBool aTransferStatus,
											 TUint32 aAckedLen)
{
	// Acked-Len denotes number of bytes received by the peer. In case of failure,
	// if it's 0, either we don't have this information (ACKs are disabled),
	// or peer has received nothing...
	// In case of success, this field should be ignored by BPA, as it can still be 0
	// if ACKs are disabled...
	
	TBundleInfo bundleInfo;
			
	if(aTransferStatus)
	{
		bundleInfo.iBundleStatus = E_BUNDLE_SENT_OK;
	}
	else
	{
		bundleInfo.iBundleStatus = E_BUNDLE_SENT_NOK;
	}
	
	bundleInfo.iBundleID = iTxQ->At(aIndex).iBundleID;
	bundleInfo.iAckedLen = aAckedLen;
	
	TRAPD(error, iRxQ->AppendL(bundleInfo));	// can't help afford such error at this stage
	
	if(KErrNone == error)
	{
		iTxQ->At(aIndex).FreeMem();
		iTxQ->Delete(aIndex);
		return ETrue;
	}
	else
	{
		// try next time
		return EFalse;
	}
	
	// no match found, could be some hanging-around status report
	// ignore and proceed normally
	return ETrue;
}

//-----------------------------------------------------------------------------

void CTCPCLA::AcknowledgeBundleReception(const TTransactionID aTransactionID, const TBool aTransferStatus)
{
	// find corresponding bundle in RxQ
	TInt lp_indx;
	
	for(lp_indx = 0; lp_indx < iRxQ->Count(); lp_indx++)
	{
		if(aTransactionID == iRxQ->At(lp_indx).iTransactionID)
		{
			if(aTransferStatus)
				iRxQ->At(lp_indx).iBundleStatus = E_BUNDLE_RECEIVED_OK;
			else
				iRxQ->At(lp_indx).iBundleStatus = E_BUNDLE_RECEIVED_NOK;
			
			break;
		}
	}
	
	// no match found, could be some hanging-around status report
	// ignore and proceed normally
}

//-----------------------------------------------------------------------------

TBool CTCPCLA::ProcessIncomingMsg(CTCPConnectionInfo* pConnInfo)
{
	TUint8 temp8;
	
	TInt trapErr = KErrNone;
				
	// extract message type code
	temp8 = (*(pConnInfo->iFragmentBuffer))[0];
						
	switch(temp8)
	{
		case K_MSG_TYPE_BUNDLE_DATA:
			TRAP(trapErr, HandleBundleDataMsgL(pConnInfo));
			break;
		case K_MSG_TYPE_BUNDLE_ACK:
			TRAP(trapErr, HandleBundleAckMsgL(pConnInfo));
			break;
		case K_MSG_TYPE_KEEP_ALIVE:
			TRAP(trapErr, HandleKeepAliveMsgL(pConnInfo));
			break;
		case K_MSG_TYPE_SHUT_DOWN:
			TRAP(trapErr, HandleShutDownMsgL(pConnInfo));
			break;
		default:
			User::Panic(_L("Some thing wrong with incoming CLA data segments"), 420);
			break;
	}
	
	if(KErrTooBig == trapErr)
	{
		// this won't ever help, must have to get rid of current chunks
		// as we can't determine the boundaries
		delete pConnInfo->iFragmentBuffer;
		pConnInfo->iFragmentBuffer = NULL;
		return ETrue;
	}
	else if(KErrNone != trapErr)	// KErrCancel or KErrNoMemory etc.
	{
		return EFalse;		// try next time 
	}
	
	return ETrue;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
