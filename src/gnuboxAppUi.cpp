//
//	gnuboxAppUi.cpp - main Gnubox AppUi cpp file.
//
//  Gnubox is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  Gnubox is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Gnubox; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//	Copyright (C) 2002 Mal Minhas.
//
//	Revision history:
//	=================
//	28/08/02:	First version of Gnubox started.

#ifdef __SERIES60__
#include <avkon.hrh>
#else //UIQ
#include <eikenv.h>
#endif
#include "gnuboxAppUi.h"
#include "gnuboxContainer.h"
//#include "gnubox.rsg"
#include "gnubox.hrh"
#include "cdb.h"
#include "routedump.h"
#include "gnuboxPhone.h"
#include "debug.h"

void CGnuboxAppUi::ConstructL()
/**
	CGnuboxAppUi::ConstructL()
*/
	{
	BaseConstructL();
	// Need to set right mode after parsing CommDB.
	//iMode=TConnectionMode());
        iMode=PhoneCfg::GetSettingsWFallback(GetDocument()->RetrieveRecordL(_L("Mode")));
	iAppContainer = new (ELeave) CGnuboxContainer;
	//CleanupStack::PushL(iAppContainer);
	iAppContainer->ConstructL(ClientRect(), this);
	//CleanupStack::Pop();
	AddToStackL(iAppContainer);
	
	}

CGnuboxAppUi::~CGnuboxAppUi()
/**
	CGnuboxAppUi::~CGnuboxAppUi()
	Destructor
*/
	{
	if(iAppContainer)
		{
		RemoveFromStack(iAppContainer);
		delete iAppContainer;
		}
   }

void CGnuboxAppUi::DynInitMenuPaneL(
	TInt /*aResourceId*/,CEikMenuPane* /*aMenuPane*/)
/**
	CGnuboxAppUi::DynInitMenuPaneL(TInt aResourceId,
	CEikMenuPane* aMenuPane)
	This function is called by the EIKON framework just
	before it displays a menu pane. Its default implementation
	is empty, and by overriding it, the application can set the
	state of menu items dynamically according
	to the state of application data.
*/
	{
	}

TKeyResponse CGnuboxAppUi::HandleKeyEventL(
	const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
/**
	CGnuboxAppUi::HandleKeyEventL(
	 const TKeyEvent& aKeyEvent,TEventCode aType)
*/
	{
	return EKeyWasNotConsumed;
	}

void CGnuboxAppUi::HandleCommandL(TInt aCommand)
/**
	CGnuboxAppUi::HandleCommandL(TInt aCommand)
*/
{
  switch (aCommand)
    {
#ifdef __SERIES60__
    case EAknSoftkeyBack:
#endif
    case EEikCmdExit:
      Exit();
      break;
      /*    case EGnuboxCmdAppInfraredDirect:
      if (iMode != EInfraredDirect)
        iAppContainer->SwitchModeL(EInfraredDirect);
      else
        INFOMSG(_L("Already is Direct Infrared"));
      break;
    case EGnuboxCmdAppBluetoothDirectSer:
      iAppContainer->SwitchModeL(EBluetoothSerial);
      break;
    case EGnuboxCmdAppBluetoothDirectLan:
      iAppContainer->SwitchModeL(EBluetoothLANAccess);
      break;
    case EGnuboxCmdAppBluetoothDirectMR:
      iAppContainer->SwitchModeL(EBluetoothMR);
      break;
    case EGnuboxCmdAppCableDirectMR:
      iAppContainer->SwitchModeL(ECableDirect);
      break;
    case EGnuboxCmdAppBluetoothDirectDun:
      iAppContainer->SwitchModeL(EBluetoothDUN);
      break;
    case EGnuboxCmdAppInfraredDialup:
      if (iMode != EInfraredDialup)
        iAppContainer->SwitchModeL(EInfraredDialup);
      else
        INFOMSG(_L("Already is Dialup Infrared"));
    break;
    case EGnuboxCmdAppCSD_GPRS:
      if (iMode != ECSD_GPRS)
        iAppContainer->SwitchModeL(ECSD_GPRS);
      else
        INFOMSG(_L("Already is default CSD/GPRS"));
      break;
*/
    case EGnuboxCmdAppDumpCommDb:
      {
        INFOMSG(_L("Allow 2min for work"));
        CCommDbDump *cd = CCommDbDump::NewL();
        CleanupStack::PushL(cd);
        cd->DumpDBtoFileL(_L("" DEBUGPATH "commdb.txt"));
        CleanupStack::PopAndDestroy(1);
        INFOMSG(_L("Done. See " DEBUGPDISP "commdb.txt"));
        break;
      }
    case EGnuboxCmdAppDumpCommDbNew:
      {
        INFOMSG(_L("Allow 2min for work"));
        CCommDbDump *cd = CCommDbDump::NewL();
        CleanupStack::PushL(cd);
        cd->DumpDBtoFileXL(_L("" DEBUGPATH "commdb.txt"));
        CleanupStack::PopAndDestroy();
        INFOMSG(_L("Done. See " DEBUGPDISP "commdb.txt"));
        break;
      }
    case EGnuboxCmdAppDumpInfo:
      {
        INFOMSG(_L("Allow 1min for work"));
        CInfoDump cd;
        cd.ConstructL();
        cd.DumpInfotoFileL(_L("" DEBUGPATH "route.txt"));
        INFOMSG(_L("Done. See " DEBUGPDISP "route.txt"));
        break;
      }
    case EGnuboxCmdAppDumpCommInfo:
      {
        INFOMSG(_L("Allow 1min for work"));
        CInfoDump cd;
        cd.ConstructL();
        cd.DumpSerInfotoFileL(_L("" DEBUGPATH "comm.txt"));
        INFOMSG(_L("Done. See " DEBUGPDISP "comm.txt"));
        break;
      }
    case EGnuboxCmdAppCommTest: {
      CInfoDump cd;
      INFOMSG(_L("Allow 1min for work."));
      cd.ConstructL();
      cd.TestCommConfigL(_L("" DEBUGPATH "ctest.txt"));
      INFOMSG(_L("Done. See " DEBUGPDISP "ctest.txt"));
      break;
    }
    case EGnuboxCmdAppDumpAppInfo: {
      CInfoDump cd;
      INFOMSG(_L("Allow 2min for work."));
      cd.ConstructL();
      cd.DumpAppInfoL(_L("" DEBUGPATH "ainfo.txt"));
      INFOMSG(_L("Done. See " DEBUGPDISP "ainfo.txt"));
      break;
    }
    case EGnuboxCmdAppCommPref: {
      CInfoDump cd;
      INFOMSG(_L("Allow 1min for work."));
      cd.ConstructL();
      LogTrapIgnoreError(cd.DumpCPrefInfotoFileL(_L("" DEBUGPATH "cpref.txt")),"action");
      INFOMSG(_L("Done. See " DEBUGPDISP "cpref.txt"));
      break;
    }
    case EGnuboxCmdAppStartConn: 
      {
        iAppContainer->BringUpIfL();
        break;
      }
    case EGnuboxCmdAppSetupScript: 
      {
        iAppContainer->SetupScriptL();
        INFOMSG(_L("done."));
        break;
      }
    case EGnuboxCmdAppCopyFromGPRS: 
      {
        iAppContainer->CopyFromGPRSL();
        INFOMSG(_L("done."));
        break;
      }
    case EGnuboxCmdAppWriteModemInit: 
      {
        iAppContainer->CopyScriptToInitStringL();
        INFOMSG(_L("done."));
        break;
      }
    case EGnuboxCmdAppFixupModemEntry: 
      {
        iAppContainer->FixupModemEntryL();
        break;
      }
    case EGnuboxCmdAppChangeConnPref: {
      iAppContainer->ChangeConnPrefL();
      break;
    }
    case EGnuboxCmdAppRevertConnPref: {
      iAppContainer->RevertConnPrefL();
      break;
    }
    case EGnuboxCmdAppSetGPRS: {
      iAppContainer->SetToGPRSL();
      break;
    }
    
    case EGnuboxCmdAppSetModemID: 
      {
        iAppContainer->SetDataModemL(TRUE);
        break;
      }
    case EGnuboxCmdAppUnsetModemID: 
      {
        iAppContainer->SetDataModemL(FALSE);
        break;
      }
    case EGnuboxCmdAppSetModemD2: 
      {
        iAppContainer->SetModemL(EData,2);
        break;
      }
    case EGnuboxCmdAppSetModemD6: 
      {
        iAppContainer->SetModemL(EData,6);
        break;
      }
    case EGnuboxCmdAppSetModemP2: 
      {
        iAppContainer->SetModemL(EPhone,2);
        break;
      }
    case EGnuboxCmdAppSetModemP6: 
      {
        iAppContainer->SetModemL(EPhone,6);
        break;
      }
    case EGnuboxCmdAddMRouterDefRoute: 
      {
        INFOMSG(_L("Allow 1min for work"));
        CInfoDump cd;
        cd.ConstructL();
        cd.AddMRouterDefaultRouteL(_L("" DEBUGPATH "nroute.txt"));
        INFOMSG(_L("Done. See " DEBUGPDISP "nroute.txt"));
        break;
      }
    case EGnuboxCmdDumpProcInfo: 
      {
        INFOMSG(_L("Allow 1min for work"));
        CInfoDump cd;
        cd.ConstructL();
        cd.DumpProcInfoL(_L("" DEBUGPATH "proc.txt"));
        INFOMSG(_L("Done. See " DEBUGPDISP "proc.txt"));
        break;
      }
    case EGnuboxCmdAppAddRecords: 
      {
        iAppContainer->CreateCommDbRecords();
        break;
      }
    case EGnuboxCmdAppDelRecords: 
      {
        iAppContainer->DeleteCommDbRecords();
        break;
      }
    case EGnuboxCmdAppSetTelNo: {
      iAppContainer->QueryTelNoL();
      break;
    }
    default:
      const TCfgEntry * ce = PhoneCfg::GetSettings(aCommand);
      if (ce) {
        iAppContainer->SwitchModeL(ce);
      }
      break;
    }
}
