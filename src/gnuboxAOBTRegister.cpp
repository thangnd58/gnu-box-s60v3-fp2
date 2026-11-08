//
//	gnuboxAOLoader.cpp - Gnubox Active Object Loader cpp file.
//	The "AO Loader" is used to ensure that loading of CommDB
//	doesn't block screen drawing.
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

#include "gnuboxPhone.h"

#ifdef BTSELECTAO

#include <btextnotifiers.h> // for Bluetooth notifier

#include "gnuboxContainer.h"
#include "debug.h"

//#include <eiklabel.h>  // for example label control
//#include <coemain.h>   // for iCoeEnv definition
//#include <coecntrl.h>  // for ControlEnv() inline

CGnuboxAOBTRegister::CGnuboxAOBTRegister() : CActive(EPriorityStandard)
  /**
     CGnuboxAOBTRegister::CGnuboxAOBTRegister()
     Default constructor.
  */
{
}

CGnuboxAOBTRegister *CGnuboxAOBTRegister::NewL(CGnuboxContainer *aParent)
  /**
     CGnuboxAOBTRegister *CGnuboxAOBTRegister::NewL(CGnuboxContainer *aParent)
  */
{
  CGnuboxAOBTRegister *c=new (ELeave) CGnuboxAOBTRegister;
  CleanupStack::PushL(c);
  c->ConstructL(aParent);
  CleanupStack::Pop();
  CActiveScheduler::Add(c);
  return c;
}

void CGnuboxAOBTRegister::ConstructL(CGnuboxContainer *aParent)
  /**
     CGnuboxAOBTRegister::ConstructL(CGnuboxContainer *aParent)
  */
{
  iParent = aParent;
  User::LeaveIfError(iNot.Connect());
}

CGnuboxAOBTRegister::~CGnuboxAOBTRegister()
  /**
     CGnuboxAOBTRegister::~CGnuboxAOBTRegister
  */
{
  if(IsActive())
    Cancel();
  iNot.Close();
}

void CGnuboxAOBTRegister::StartDialog(TBTProfile aProfile)
  /**
     CGnuboxAOBTRegister::Fire()
     CGnuboxAOBTRegister operates like a one-shot.  Could have used
     a CAsyncOneShot here instead but this at least illustrates
     how to do one using an active object for yielding control
     within a thread (ie. cooperative multitasking).
  */
{
  if (IsActive()) {
    Cancel();
  }
  FLOG(_L("CGnuboxAOBTRegister::StartDialog"));
  iProfile=aProfile;
  // Do not filter by UUID or DeviceClass on S60v3 so that vetera-bridge (ESP32)
  // and all Bluetooth devices are always shown in the device picker dialog:
  // selPckg().SetUUID(aProfile);
  // TBTDeviceClass deviceClass((TUint16) EMajorServiceNetworking ,0 ,0);
  // selPckg().SetDeviceClass(deviceClass);
  TBuf<150> b;
  b.Format(_L("Select: devcl valid %d uid valid %d "),selPckg().IsValidDeviceClass(),selPckg().IsValidUUID());
  FLOG(b);
  iNot.CancelNotifier(KDeviceSelectionNotifierUid);
  iNot.StartNotifierAndGetResponse(iStatus,KDeviceSelectionNotifierUid,selPckg,pckg);
  SetActive();
}

void CGnuboxAOBTRegister::RunL()
  /**
     CGnuboxAOBTRegister::RunL()
  */
{
  FLOG(_L("CGnuboxAOBTRegister::RunL - user selected BT device"));
  // Currently we're leaving if the user presses Cancel...
  FPrint(_L("istatus %d valid addr %d valid name %d name %S"),iStatus.Int(),!!pckg().IsValidBDAddr(),!!pckg().IsValidDeviceName(),&pckg().DeviceName());
  LogLeaveIfErrorL(iStatus.Int(),"BTselect");
  FLOG(_L("X"));
  iNot.CancelNotifier(KDeviceSelectionNotifierUid);
  FLOG(_L("Y"));
  iParent->SetupCSYL(pckg(),iProfile);
}

void CGnuboxAOBTRegister::DoCancel()
  /**
     CGnuboxAOBTRegister::DoCancel()
  */
{
  iNot.CancelNotifier(KDeviceSelectionNotifierUid); 
}

#endif
