//
//	gnuboxContainer.cpp - Gnubox main AppUi container cpp file.
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
#include <AknQueryDialog.h>
#include <aknnotewrappers.h>
#include <gnubox.rsg>
#endif

#if defined(__UIQ__)||defined(__SERIES90__)
#include <eikenv.h>
#include <eikdialg.h>
#include <eikedwin.h>
#endif



#include <in_sock.h>
#include <faxdefn.h>
#include <cdbtemp.h>
#include <cdbpreftable.h>
#include <commsdattypesv1_1.h>

using namespace CommsDat;

#include "gnuboxContainer.h"
#include "gnuboxPhone.h" //for phone specific constants
#include "debug.h"
#include "cdb.h"
#include "gnubox.rsg"

#ifdef BTSELECTUIQ
//includes for UIQ BT REGISTER
#include <QBTselectdlg.h>
#endif 

#include <eiklabel.h>  // for example label control
#include <coemain.h>   // for iCoeEnv definition
#include <coecntrl.h>  // for ControlEnv() inline
#include <btextnotifiers.h> // for Bluetooth notifier

#ifdef NOBTREGISTRY
#include "btdefcommport.h"
#include <btmanclient.h>
#else
#include <btregistry.h>  // for accessing BT registry
#endif

#include <bttypes.h>

#include <btmanclient.h>
#include <d32comm.h>
#include <cdbstore.h>
#if !defined(__SY70S__) && !defined(__SY80__)
#include <AgentClient.h>
#endif
#include <cdbover.h>

//commdb connpref for getting up the connection
#if defined(__SY70S__) || defined(__SY80__)
#include <commdbconnpref.h>
#endif

/**
   Information about CommDb
   ========================
   CommDb provides system-wide storage for communications-related
   settings. It holds information about Internet Access Providers
   (IAPs), Internet Service Providers (ISPs), GPRS, modems, locations,
   charge-cards, proxies and WAP settings

   CommDb stores the settings in a series of tables in a
   relational database, using the DBMS architecture.
   DBMS is a client/server architecture and provides for safe
   access by multiple clients.

   The CommDb API has two key concepts:
   <b>communications database</b>, and <b>table view</b>.

   The communications database interface allows clients to access
   the database, open its tables, and perform transactions. It is
   provided by CCommsDatabase and its base class CCommsDatabaseBase.

   Most database settings are read and written to by opening the
   appropriate table. Table access is provided by CCommsDbTableView.

   It is possible for several instances of most of the tables
   (e.g. where multiple IAPs have been set up) to exist in the database.
   The Connected preferences table sets a ranking by which such records
   are used. That table is accessed through its own view
   class CCommsDbConnectionPrefTableView.

   A view is simply a subset of the records (or rows) within a table,
   selected according to defined criteria. Tables, records, rows, columns,
   and so on, are DBMS concepts.

   Note that you can open two kinds of CCommsDatabase in 6.0 - EDatabaseTypeIAP
   and EDatabaseTypeISP.  The latter is deprecated in 6.1.

   For a database of type EDatabaseTypeIAP,
   tables are arranged as follows (from /commdb/scdb/cdbdata.cpp):

   LOCAL_D TTable const KTableList[]=
   {
   {IAP, KIAPTable},
   {DIAL_OUT_ISP, KDialOutISPTable},
   {DIAL_IN_ISP, KDialInISPTable},
   {WAP_ACCESS_POINT, KWAPAccessPointTable},
   {WAP_IP_BEARER, KWAPIPBearerTable},
   {WAP_SMS_BEARER, KWAPSMSBearerTable},
   {OUTGOING_GPRS, KGPRSTable},
   {INCOMING_GPRS, KGPRSTable},
   {MODEM, KModemTable},
   {LOCATION, KLocationTable},
   {CHARGECARD, KChargecardTable},
   {PROXIES, KProxiesTable},
   {CONNECTION_PREFERENCES, KConnectionPreferencesTable},
   {AGENT, KAgentTable},
   {GLOBAL_SETTINGS, KGlobalSettings},
   {BT_DEVICE_TABLE, KCBTDeviceTable},
   {BT_DEFAULT_TABLE, KCBTDefaultServiceTable},
   {DEFAULT_GPRS, KDefaultGPRSTable},
   {BT_SECURITY_TABLE, KCBTSecurityTable},
   {ACCESS_TYPE_TABLE, KAccessTypeTable},
   {0}
   };

   Note that the various defines here are in /commdb/inc/cdbcols.h:

   // tables...
   #define IAP												_S("IAP")
   #define DIAL_OUT_ISP									_S("DialOutISP")
   #define MODEM											_S("Modem")
   ...

   // DIAL_OUT_ISP table columns
   #define ISP_DESCRIPTION									_S("Description")					// text field for app to use to describe ISP
   #define ISP_TYPE										_S("Type")
   #define ISP_DEFAULT_TEL_NUM								_S("DefaultTelNum")
   ...

   // modem table columns
   #define MODEM_PORT_NAME									_S("PortName")
   #define MODEM_TSY_NAME									_S("TSYName")
   #define MODEM_CSY_NAME									_S("CSYName")
   #define MODEM_HANDSHAKING								_S("Handshaking")
   ...

   // Bluetooth table columns

   #define BT_DEV_ADD				_S("DeviceAddress")
   #define BT_DEV_DFLT_NAME		_S("DeviceDefaultName")
   #define BT_DEV_FRIENDLY_NAME	_S("DeviceFriendlyName")
   #define BT_DEVICE_CLASS			_S("DeviceClass")
   #define BT_LINKKEY_SIZE			_S("DeviceLinkKeySize")
   #define BT_LINKKEY				_S("DeviceLinkKey")
   #define BT_DEV_STATE			_S("DeviceStatus")
   #define BT_DEVICE_GLOBAL_SECURITY _S("DeviceGlobalSecurity")
   //
   #define BT_DFLT_SER_NAME		_S("DefaultServicename")
   #define BT_DFLT_SER_UUID		_S("DefaultServiceUID")
   #define BT_DFLT_DEV_ADD			_S("DefaultDeviceAddress")
   #define BT_SEC_UID				_S("BTSecurityUID")
   #define BT_SEC_DEVICE_SECURITY	_S("BTSecurityDevice")
   #define BT_DFLT_VIRTUAL_PORT    _S("DefaultVirtualPort")
   #define BT_DFLT_SECURITY_LEVEL  _S("DefaultSecurityLevel")

   Tables we are interested in are DIAL_OUT_ISP () and MODEM ().
   From (from /commdb/scdb/cdbdata.cpp):

   LOCAL_D TColumn const KDialOutISPTable[]=
   {
   {ISP_DESCRIPTION,EDbColText,0},
   ...
   {ISP_DEFAULT_TEL_NUM,EDbColText,0},
   ...
   {ISP_TYPE,EDbColUint32,0}
   ...
   {ISP_IF_NAME,EDbColText,TDbCol::ENotNull},
   ...
   };

   LOCAL_D TColumn const KModemTable[]=
   {
   ...
   {MODEM_PORT_NAME,EDbColText,0},
   {MODEM_TSY_NAME,EDbColText,0},
   {MODEM_CSY_NAME,EDbColText,0},
   ...
   {MODEM_HANDSHAKING,EDbColUint32,0},
   ...
   };

   LOCAL_D TColumn const KCBTDeviceTable[]=
   {
   ...
   {BT_DEV_ADD,EDbColText8,TDbCol::ENotNull},
   {BT_DEV_DFLT_NAME,EDbColLongText16,0},
   {BT_DEV_FRIENDLY_NAME,EDbColLongText16,0},
   {BT_DEVICE_CLASS,EDbColUint32,0},
   {BT_LINKKEY,EDbColText8,0},
   {BT_DEVICE_GLOBAL_SECURITY, EDbColUint32,0},
   {BT_DEV_STATE, EDbColUint32, TDbCol::ENotNull},
   ...
   };

   LOCAL_D TColumn const KCBTDefaultServiceTable[]=
   {
   ...	  	{BT_DFLT_SER_NAME,EDbColText16,0},
   {BT_DFLT_SER_UUID,EDbColText8,TDbCol::ENotNull},
   {BT_DFLT_VIRTUAL_PORT,EDbColUint32,TDbCol::ENotNull},
   {BT_DFLT_DEV_ADD,EDbColText8,TDbCol::ENotNull},
   {BT_DFLT_SECURITY_LEVEL,EDbColUint8,0},
   ...
   };

   LOCAL_D TColumn const KCBTSecurityTable[]=
   {
   ...
   {BT_DEV_ADD,EDbColText8,TDbCol::ENotNull},
   {BT_SEC_DEVICE_SECURITY,EDbColUint32,0},
   {BT_SEC_UID,EDbColUint32,0},
   ...
   };

   Note that you don't actually directly manipulate the BT fields in CommDB.
   The BTRegistry is your friend in this regard.  It acts as a facade over
   the CommDB.  Fine sentiment.  Now all we need is someone to do the same thing
   for the networking bits ;-)

   @TODO: Add more comments here on use cases for each option and IOP/testing.
*/

void CGnuboxContainer::ConstructL(const TRect& aRect, CGnuboxAppUi* aParent)
  /**
     CGnuboxContainer::ConstructL(const TRect& aRect)
     EPOC two phased constructor.
     Initialise to default settings.
  */
{
  FLOG(_L("CGnuboxContainer::ConstructL"));
  CreateWindowL();

  iConnected = EFalse;
  iParent = aParent;

  // This will add further text but it might take time so
  // we've hived it off to a separate active object...
  iLoader = CGnuboxAOLoader::NewL(this);
  //This will do the user interaction wait for bluetooth device selection
#ifdef BTSELECTAO
  iBTRegister = CGnuboxAOBTRegister::NewL(this);
#else
  //no more need due to the UIQ BT device dialog is synchronous
  //maybe re-enable it for series 60 backport
#endif
  iLabel = new (ELeave) CEikLabel;
  iLabel->SetContainerWindowL(*this);

  // Set up the screen text with whatever the default bearer is...
  SetAndDrawScreenTextL(iParent->GetMode());  // MALM

  SetRect(aRect);

  ActivateL();
}

CGnuboxContainer::~CGnuboxContainer()
  /**
     Destructor
  */
{
  FLOG(_L("CGnuboxContainer::~CGnuboxContainer"));
  if (iConnected) {
    iConn.Close();
    iSockServ.Close();
    iConnected = EFalse;
  }
  delete iLabel;
  iTextLines.Close();
  delete iLoader;
#ifdef BTSELECTAO
  delete iBTRegister;
#endif
}

void CGnuboxContainer::SizeChanged()
  /**
     CGnuboxContainer::SizeChanged()
     Called by framework when the view size is changed.
     @TODO: Add control resize code etc here needed to resize label properly
  */
{
  FLOG(_L("CGnuboxContainer::SizeChanged"));
  iLabel->SetExtent(TPoint(10,5),iLabel->MinimumSize());
}

TInt CGnuboxContainer::CountComponentControls() const
  /**
     CGnuboxContainer::CountComponentControls() const
  */
{
  return 1; // return nbr of controls inside this container
}

CCoeControl* CGnuboxContainer::ComponentControl(TInt aIndex) const
  /**
     CGnuboxContainer::ComponentControl(TInt aIndex) const
  */
{
  switch ( aIndex )
    {
    case 0:
      return iLabel;
    default:
      return NULL;
    }
}

void CGnuboxContainer::Draw(const TRect& aRect) const
  /**
     CGnuboxContainer::Draw(const TRect& aRect) const
     Note that this redraws itself in different ways
     depending on what the current bearer is.
  */
{
  FLOG(_L("CGnuboxContainer::Draw"));
  DrawControls(aRect);
}

void CGnuboxContainer::HandleControlEventL(
                                           CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
  /**
     CGnuboxContainer::HandleControlEventL(
     CCoeControl* aControl,TCoeEvent aEventType)
     We are not interested in handling control events in
     this program (?).
  */
{
  // TODO: Add your control event handler code here
  FLOG(_L("CGnuboxContainer::HandleControlEventL"));
}


static TInt CommsDatDeleteWithRetry(CMDBSession& aSession, CMDBRecordBase* aRec)
{
  TInt err = KErrNone;
  for (TInt retry = 0; retry < 25; ++retry) {
    TRAP(err, aRec->DeleteL(aSession));
    if (err != KErrLocked && err != KErrInUse && err != KErrServerBusy) {
      break;
    }
    User::After(100000);
  }
  return err;
}

static TInt CommsDatStoreWithRetry(CMDBSession& aSession, CMDBRecordBase* aRec)
{
  TInt err = KErrNone;
  aRec->ClearAttributes(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);
  for (TInt retry = 0; retry < 25; ++retry) {
    TRAP(err, aRec->StoreL(aSession));
    if (err != KErrLocked && err != KErrInUse && err != KErrServerBusy) {
      break;
    }
    User::After(100000);
  }
  return err;
}

static TInt CommsDatModifyWithRetry(CMDBSession& aSession, CMDBRecordBase* aRec)
{
  TInt err = KErrNone;
  for (TInt retry = 0; retry < 25; ++retry) {
    TRAP(err, aRec->ModifyL(aSession));
    if (err != KErrLocked && err != KErrInUse && err != KErrServerBusy) {
      break;
    }
    User::After(100000);
  }
  return err;
}

static TInt StoreNamedRecordCleanL(CMDBSession& aSession, TMDBElementId aTableId, const TDesC& aName, CCDRecordBase* aNewRec, TUint32& aOutId)
{
  aSession.SetAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);
  CCDRecordBase* findRec = static_cast<CCDRecordBase*>(CCDRecordBase::RecordFactoryL(aTableId));
  CleanupStack::PushL(findRec);
  findRec->iRecordName.SetL(aName);
  TBool found = EFalse;
  TRAP_IGNORE(found = findRec->FindL(aSession));

  TInt err = KErrNone;
  if (found && findRec->RecordId() > 0) {
    aNewRec->SetRecordId(findRec->RecordId());
    aNewRec->SetElementId(findRec->ElementId());
    err = CommsDatModifyWithRetry(aSession, aNewRec);
    if (err != KErrNone) {
      CommsDatDeleteWithRetry(aSession, findRec);
      aNewRec->SetRecordId(KCDNewRecordRequest);
      err = CommsDatStoreWithRetry(aSession, aNewRec);
    }
  } else {
    aNewRec->SetRecordId(KCDNewRecordRequest);
    err = CommsDatStoreWithRetry(aSession, aNewRec);
  }
  CleanupStack::PopAndDestroy(findRec);
  aSession.ClearAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);

  aOutId = static_cast<TUint32>(aNewRec->RecordId());
  return err;
}

void CGnuboxContainer::SetModemL(TModemName m,TUint32 id) {
  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);
  dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete);
  CCDGlobalSettingsRecord* globalRec = static_cast<CCDGlobalSettingsRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdGlobalSettingsRecord));
  CleanupStack::PushL(globalRec);
  globalRec->SetRecordId(1);
  TRAPD(err, globalRec->LoadL(*dbSession));
  if (err == KErrNone) {
    if (m == EData) {
      globalRec->iModemForDataAndFax = id;
    } else if (m == EPhone) {
      globalRec->iModemForPhoneServicesAndSMS = id;
    }
    err = CommsDatModifyWithRetry(*dbSession, globalRec);
  }
  CleanupStack::PopAndDestroy(2, dbSession);
}


void CGnuboxContainer::SetDataModemL(TBool dir) {
  INFOMSG(_L("Switching DataAndFax modem"));
  FLOG(_L("**** CGnuboxContainer::SetDataModemL ****"));

  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);
  dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete);

  TUint32 modemID = 0;
  if (dir) {
    CCDModemBearerRecord* modemRec = static_cast<CCDModemBearerRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
    CleanupStack::PushL(modemRec);
    modemRec->iRecordName.SetL(KModemName);
    TBool found = EFalse;
    TRAP_IGNORE(found = modemRec->FindL(*dbSession));
    if (!found) {
      CleanupStack::PopAndDestroy(2, dbSession);
      INFOMSG(_L("Modem not found. Create records first."));
      User::Leave(KErrNotFound);
    }
    modemID = static_cast<TUint32>(modemRec->RecordId());
    CleanupStack::PopAndDestroy(modemRec);
  }

  CCDGlobalSettingsRecord* globalRec = static_cast<CCDGlobalSettingsRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdGlobalSettingsRecord));
  CleanupStack::PushL(globalRec);
  globalRec->SetRecordId(1);
  TRAPD(err, globalRec->LoadL(*dbSession));
  if (err == KErrNone) {
    if (!dir) {
      modemID = globalRec->iModemForPhoneServicesAndSMS;
    }
    globalRec->iModemForDataAndFax = modemID;
    err = CommsDatModifyWithRetry(*dbSession, globalRec);
  }
  CleanupStack::PopAndDestroy(2, dbSession);

  TBuf<40> iValue;
  iValue.Format(_L("Data modem ID %u"), modemID);
  INFOMSG(iValue);
  FLOG(iValue);
}

void CGnuboxContainer::SwitchModeL(const TCfgEntry * aBearer)
  /**
     CGnuboxContainer::SwitchModeL(TConnectionMode aMode)
     Invoked whenever the user has decided to switch from
     one underlying connection mode to another.
  */
{
  INFOMSG(TPtrC(aBearer->iDispName));
  TBool okToUpdate=UpdateCommDbL(aBearer);
  if (okToUpdate)
    {// we are ok to switch mode
      iParent->SetMode(aBearer);
		
      ////Don't do the StoreRecord here - do it in Loader RunL

      // Then change the container control text strings to
      // ensure their correct updating on redraw
      SetAndDrawScreenTextL(aBearer);
    }
}

void CGnuboxContainer::DrawControls(const TRect& aRect) const
  /**
     CGnuboxContainer::DrawControls() const
     Main point at which AppUi controls are drawn
     Note that for 7650 width=176 and height=144
     @TODO: Need to scale the drawing of controls.
  */
{
  CWindowGc& gc = SystemGc();
  gc.SetPenStyle(CGraphicsContext::ENullPen);
  gc.SetBrushColor(iParent->GetMode()->iColor);
  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  gc.DrawRect(aRect);
  // Now we can draw screen elements
  //TPoint startpoint(0,iH*KHeightScalingFactor*3/5-40);
  TPoint startpoint(KXOffset,20);
  //TPoint endpoint(iW*KWidthScalingFactor,iH*KHeightScalingFactor*3/5-40);
  TPoint endpoint((KWidth-KXOffset),20);
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  TSize penSizeThicker(1,1);
  gc.SetPenSize(penSizeThicker);
  gc.SetPenColor(KRgbBlack);
  gc.DrawLine(startpoint,endpoint);

  //TFontSpec fs(_L("Arial"),20*7); // 9pt Arial
  TFontSpec fs(_L("Arial"),20*4); // 8pt Arial
  //TFontSpec fs(_L("Arial"),20*11); // 11pt Arial
  //TFontSpec fs(_L("Arial"),20*24); // 24pt Arial
  fs.iFontStyle=
    TFontStyle(EPostureUpright,EStrokeWeightNormal,EPrintPosNormal);
  CFont* fontUsed=ControlEnv()->CreateScreenFontL(fs);
  gc.UseFont(fontUsed);

  TRect textRect(KXOffset,23,(KWidth-KXOffset),KHeight);
  gc.DrawRect(textRect); // just to see our limits...

  // Now draw the lines of text - you must do this per redraw
  for(TInt count = 0; count < iTextLines.Count(); count++)
    gc.DrawText(iTextLines[count],TPoint(KTextOffset,35+(count*KLineGap)));

  gc.DiscardFont();
  ControlEnv()->ReleaseScreenFont(fontUsed);
  //
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);
}

void CGnuboxContainer::SetAndDrawScreenTextL(const TCfgEntry * aBearer)
  /**
     CGnubox::SetAndDrawScreenTextL(TConnectionMode aMode)
     This function does two things:
     1) Changes the main label text
     2) Updates screen with new CommDb settings
     To do the latter, it invokes the iLoader active object.
  */
{
  FLOG(_L("CGnuboxContainer::SetScreenTextL"));
  TBuf<22> txtBuf;
  txtBuf.FillZ();
  txtBuf = TPtrC(aBearer->iDispName);
  txtBuf.SetMax();
  iLabel->SetTextL(txtBuf);
  iTextLines.Close();
  User::LeaveIfError(iTextLines.Append(_L("Loading CommDb")));
  User::LeaveIfError(iTextLines.Append(_L("settings...")));
  iLoader->Fire();
}

void CGnuboxContainer::ReadTextFieldL(CCommsDbTableView* aTable, const TDesC& aRecord)
  /**
     CGnuboxContainer::ReadTextFieldL(CCommsDbTableView* aTable, const TDesC& aRecord)
     Reads a particular field and appends it to iTextLines.
     @TODO: Need to add scrolling text window support - migrate to CTextView?
  */
{
  TBuf<KCommsDbSvrMaxFieldLength> printBuffer;
  TBuf<KCommsDbSvrMaxFieldLength-10> recBuffer;

  printBuffer=_L(" ");
  printBuffer.Append(aRecord);
  printBuffer.Append(_L(": "));
  TRAPD(readErr, aTable->ReadTextL(aRecord,recBuffer));
  if (readErr == KErrUnknown)
    {
      recBuffer.Zero();
    }
  else
    {
      User::LeaveIfError(readErr);
    }

  // Need to store the value in dummyBuffer too...
  //If the array is already at the maximum size, delete the first line of text
  if (iTextLines.Count() >= KMaxTextLines)
    {
      iTextLines.Remove(0);
    }
  if (printBuffer.Length() + recBuffer.Length() > KMaxLineLength)
    {
      iTextLines.Append(printBuffer);
      TBuf<KCommsDbSvrMaxFieldLength> tempBuffer=_L("  ");
      tempBuffer.Append(recBuffer);
      iTextLines.Append(tempBuffer);
    }
  else
    {
      printBuffer.Append(recBuffer);
      iTextLines.Append(printBuffer);
    }

  if ((iParent->GetMode()->iID == CFGID_SAVE))
    {// Need to store these away because they are important
      // @TODO: Only do the following if it is different from current record...
      TBuf<KCommsDbSvrMaxFieldLength-10> currentBuffer;
      iParent->GetDocument()->RetrieveRecordL(aRecord,currentBuffer);
      if (currentBuffer != recBuffer)
        iParent->GetDocument()->StoreRecordL(aRecord,recBuffer);
    }
}

void CGnuboxContainer::LoadCommDbSettingsL()
  /**
     CGnuboxContainer::LoadCommDbSettingsL()
     This function loads the relevant CommDb settings

     Note that we store a couple of tables.
     Namely the MODEM table and the DIAL_OUT_ISP table.
     @TODO: Need to be pulling out the "Nokia CSD" IAP settings
     @TODO: There is a bug here relating to DIAL_OUT_ISP - what if we're GPRS??!!
  */
{
  FLOG(_L("**** CGnuboxContainer::LoadCommDbSettingsL ****"));

  // UPDATE THE CURRENT MODE IN - need to do this to pick up changes
  const TCfgEntry* mode=iParent->GetMode();
  FLOG(_L("1"));
  iParent->GetDocument()->StoreRecordL(KModeTxt,TInt(mode->iID));
  FLOG(_L("2"));
  // Ensure member iCommDb is never left dangling
  iCommDb = NULL;
  FLOG(_L("3"));
  CCommsDatabase* commDb=CCommsDatabase::NewL(EDatabaseTypeIAP);
  commDb->ShowHiddenRecords();
  FLOG(_L("4"));
  CleanupStack::PushL(commDb);

  // DIAL_OUT_ISP TABLE
  CCommsDbTableView* table=commDb->OpenTableLC(TPtrC(DIAL_OUT_ISP));
  FPrint(_L("5a ptr=0x%x"),table);
  //  FLOG(_L("5a"));
  TBuf<350> tableName;
  FLOG(_L("5b"));
#ifdef __MO_A92x__
  //ugly hack to work around problems
  tableName.Copy(TPtrC(DIAL_OUT_ISP));
#else
  table->GetTableName(tableName);
#endif
  FLOG(_L("5b2"));
  FLOG(tableName);
  FLOG(_L("5c"));
  iTextLines.Append(tableName);
  FLOG(_L("5d"));
  TBool found=FindName(table, KISPName);
  FLOG(_L("5"));
  if (!found) {
    TBuf<200> msg;
    msg.Format(_L("ISP '%S' not found"),&KISPName);
    iTextLines.Append(msg);
    CleanupStack::PopAndDestroy(2); // table, commDb
    DrawNow();
    return;
  }
  TInt ret;
  TRAP(ret, ReadTextFieldL(table,TPtrC(ISP_DESCRIPTION)));
  FLOG(_L("6"));
  if (ret!=KErrNone) {
    TBuf<20> msg;
    msg.Format(_L("No desc %d"), ret);
    iTextLines.Append(msg);
  }

  //ReadTextFieldL(table,TPtrC(ISP_TYPE));
  TRAP(ret, ReadTextFieldL(table,TPtrC(ISP_DEFAULT_TEL_NUM)));
  FLOG(_L("7"));
  if (ret!=KErrNone) {
    TBuf<20> msg;
    msg.Format(_L("No telnum %d"), ret);
    iTextLines.Append(msg);
  }
  CleanupStack::PopAndDestroy();  // table

  // MODEM
  table=commDb->OpenTableLC(TPtrC(MODEM_TABLE));
  FLOG(_L("8a"));
#ifdef  __MO_A92x__
  tableName.Copy(TPtrC(MODEM_TABLE));  
#else
  table->GetTableName(tableName);
#endif

  FLOG(_L("8"));
  FLOG(tableName);
  FLOG(_L("8b1"));
  iTextLines.Append(tableName);
  FLOG(_L("8b"));
  found=FindName(table, KModemName);
  if (!found) {
    TBuf<200> msg;
    msg.Format(_L("Modem '%S' not found"),&KModemName);
    iTextLines.Append(msg);
    CleanupStack::PopAndDestroy(2); // table, commDb
    DrawNow();
    return;
  }
  FLOG(_L("9"));
  TRAP_IGNORE(ReadTextFieldL(table,TPtrC(MODEM_PORT_NAME)));
  TRAP_IGNORE(ReadTextFieldL(table,TPtrC(MODEM_CSY_NAME)));
  TRAP_IGNORE(ReadTextFieldL(table,TPtrC(MODEM_TSY_NAME)));
  CleanupStack::PopAndDestroy();  // table

  FLOG(_L("a"));
  // GET RID OF COMMDB HANDLE
  CleanupStack::PopAndDestroy();  // commDb
  iCommDb = NULL;

  // **** The (re)drawing bit ****

  //TRect textRect(KXOffset,23,(KWidth-KXOffset),KHeight);
  //SetRect(textRect);
  FLOG(_L("b"));
  DrawNow();
  FLOG(_L("c"));
}

TBool CGnuboxContainer::FindName(CCommsDbTableView* table, const TDesC& name)
{
  TBool found=EFalse;
  while (!found && table->GotoNextRecord()==KErrNone) {
    TBuf<100> n;
    table->ReadTextL(TPtrC(COMMDB_NAME), n);
    if (!n.Compare(name) ) found=ETrue;
  }
  return found;
}


void CGnuboxContainer::SetupScriptL() {
  FLOG(_L("**** CGnuboxContainer::SetupScriptL ****"));
  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);
  dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete);

  CCDDialOutISPRecord* ispRec = static_cast<CCDDialOutISPRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
  CleanupStack::PushL(ispRec);
  ispRec->iRecordName.SetL(KISPName);
  TBool found = EFalse;
  TRAP_IGNORE(found = ispRec->FindL(*dbSession));
  if (!found) {
    CleanupStack::PopAndDestroy(2, dbSession);
    CreateCommDbRecords();
    return;
  }

  ispRec->iLoginScript.SetL(_L("CHARMAP [windows-1252]\nLOOP 10\n{\nSEND \"CLIENT\"+<0x0d>\nWAIT 3\n{\n\"SERVER\" OK\n}\n}\nEXIT KErrNoAnswer$\n\nOK:\nEXIT\n"));
  ispRec->iUseLoginScript = ETrue;
  ispRec->iDialResolution = EFalse;
  ispRec->iPromptForLogin = EFalse;
  ispRec->iIfPromptForAuth = EFalse;
  ispRec->iDisablePlainTextAuth = EFalse;
  ispRec->iEnableIpHeaderComp = EFalse;
  ispRec->iEnableLcpExtension = EFalse;
  ispRec->iEnableSwComp = EFalse;
  ispRec->iIpAddrFromServer = ETrue;
  ispRec->iIpDnsAddrFromServer = ETrue;

  TInt err = CommsDatModifyWithRetry(*dbSession, ispRec);
  if (err != KErrNone) {
    TInt savedId = ispRec->RecordId();
    CommsDatDeleteWithRetry(*dbSession, ispRec);
    dbSession->ClearAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);
    ispRec->SetRecordId(savedId);
    err = CommsDatStoreWithRetry(*dbSession, ispRec);
  }
  LogLeaveIfErrorL(err, "setup script");

  CleanupStack::PopAndDestroy(2, dbSession);
  INFOMSG(_L("RAS login script set."));
}


void CGnuboxContainer::CopyScriptToInitStringL() {
  FLOG(_L("**** CGnuboxContainer::CopyScriptToInitStringL ****"));
  CCommsDatabase * iCommDb=CCommsDatabase::NewL(EDatabaseTypeIAP);

  CleanupStack::PushL(iCommDb);
  
  RFs iFs;
  iFs.Connect();
  CCnvCharacterSetConverter *iCC=CCnvCharacterSetConverter::NewL();
  CleanupStack::PushL(iCC);
  iCC->PrepareToConvertToOrFromL(KCharacterSetIdentifierIso88591, iFs);


  CCommsDbTableView* table=iCommDb->OpenViewMatchingTextLC(TPtrC(DIAL_OUT_ISP),TPtrC(COMMDB_NAME),KISPName);
  LogLeaveIfErrorL(table->GotoFirstRecord(),"goto first record");
  HBufC *bs;
  FLOG(_L("a"));
  bs=table->ReadLongTextLC(TPtrC(ISP_LOGIN_SCRIPT));
  FLOG(_L("b"));  
  LogLeaveIfErrorL(table->UpdateRecord(),"updaterecord");
  FLOG(_L("c"));
  TBuf8<256> b;
  iCC->ConvertFromUnicode(b,*bs);
  FLOG(_L("c2"));
  LogTrapLeaveIfErrorL(table->WriteTextL(TPtrC(ISP_INIT_STRING),b),"set init string");
  FLOG(_L("5"));
  LogTrapLeaveIfErrorL(table->WriteBoolL(TPtrC(ISP_USE_LOGIN_SCRIPT),FALSE),"setuseloginscript");
  FLOG(_L("d"));
  LogLeaveIfErrorL(table->PutRecordChanges(),"put record changes");
  FLOG(_L("e"));
  CleanupStack::PopAndDestroy();//iCC          
  CleanupStack::PopAndDestroy();//Hbuf          
  CleanupStack::PopAndDestroy();//view        
  CleanupStack::PopAndDestroy();//iCommDb
  iFs.Close();
}

void CGnuboxContainer::ChangeConnPrefL() {
  FLOG(_L("**** CGnuboxContainer::ChangeConnPrefL ****"));
  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);
  dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete);

  CCDIAPRecord* iapRec = static_cast<CCDIAPRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
  CleanupStack::PushL(iapRec);
  iapRec->iRecordName.SetL(KISPName);
  TBool found = EFalse;
  TRAP_IGNORE(found = iapRec->FindL(*dbSession));
  if (!found) {
    CleanupStack::PopAndDestroy(2, dbSession);
    INFOMSG(_L("IAP Bt not found. Create records first."));
    User::Leave(KErrNotFound);
  }
  TUint32 iapid = static_cast<TUint32>(iapRec->RecordId());
  CleanupStack::PopAndDestroy(iapRec);

  CCDConnectionPrefsRecord* prefRec = static_cast<CCDConnectionPrefsRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdConnectionPrefsRecord));
  CleanupStack::PushL(prefRec);
  prefRec->iDirection = ECommDbConnectionDirectionOutgoing;
  prefRec->iRanking = 1;
  found = EFalse;
  TRAP_IGNORE(found = prefRec->FindL(*dbSession));
  prefRec->iDirection = ECommDbConnectionDirectionOutgoing;
  prefRec->iRanking = 1;
  prefRec->iBearerSet = KCommDbBearerCSD;
  prefRec->iDialogPref = ECommDbDialogPrefDoNotPrompt;
  prefRec->iDefaultIAP = iapid;

  TInt err = KErrNone;
  if (found) {
    err = CommsDatModifyWithRetry(*dbSession, prefRec);
  } else {
    dbSession->ClearAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);
    prefRec->SetRecordId(KCDNewRecordRequest);
    err = CommsDatStoreWithRetry(*dbSession, prefRec);
  }
  CleanupStack::PopAndDestroy(2, dbSession);

  INFOMSG(_L("Done. Now Accesspoint Bt should be the default."));
  FLOG(_L("Done."));
}

void CGnuboxContainer::RevertConnPrefL() {
  FLOG(_L("**** CGnuboxContainer::ChangeConnPrefL ****"));
  CCommsDatabase * iCommDb=CCommsDatabase::NewL(EDatabaseTypeIAP);
  CleanupStack::PushL(iCommDb);
  iCommDb->ShowHiddenRecords();
  FLOG(_L("Treating connpref entry"));

  //first find out IAP Id of 'Bt'

  CCommsDbTableView * view = iCommDb->OpenViewMatchingTextLC(TPtrC(IAP),TPtrC(IAP_SERVICE_TYPE),TPtrC(OUTGOING_GPRS));
  LogLeaveIfErrorL(view->GotoFirstRecord(),"load IAP entry");
  TUint32 iapid;
  LogTrapLeaveIfErrorL(view->ReadUintL(TPtrC(COMMDB_ID),iapid),"read iap ID");
  CleanupStack::PopAndDestroy(); //view

  //open connpref table on the default outgoing record
  CCommsDbConnectionPrefTableView *preftbl=iCommDb->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
  LogLeaveIfErrorL(preftbl->GotoFirstRecord(),"pref->goto first");

  //update record
  CCommsDbConnectionPrefTableView::TCommDbIapBearer br;
  br.iBearerSet=KCommDbBearerPSD;
  br.iIapId=iapid;
  TCommDbDialogPref prf=ECommDbDialogPrefPrompt;
  LogTrapLeaveIfErrorL(preftbl->UpdateBearerL(br,FALSE),"update bearer");
  LogTrapLeaveIfErrorL(preftbl->UpdateDialogPrefL(prf),"update dialogpref");

  //cleanup and notify user.
  CleanupStack::PopAndDestroy(); //preftbl
  CleanupStack::PopAndDestroy(); //commsdb

  INFOMSG(_L("Done. Now a GPRS accesspoint should be the default, and asks for accesspoint."));
  FLOG(_L("Done."));
  

}


void CGnuboxContainer::FixupModemEntryL() {
  // No-op on S60v3 CommsDat: CCommsDbTemplateRecord with pseudo-column "Hidden"
  // corrupts template record 0 in CentralRepository 0xCCCCCC00 and causes KErrLocked (-22).
  FLOG(_L("**** CGnuboxContainer::FixupModemEntryL (skipped on S60v3 CommsDat) ****"));
}



//! does the actual update in the commdb.
/**

   \param aTelNum is written to DIAL_OUT_ISP row KISPName column ISP_DEFAULT_TEL_NUM
   \param aDesc is written to DIAL_OUT_ISP row KISPName column ISP_DESCRIPTION
   \param aPort is written to MODEM_TABLE row KModemName column MODEM_PORT_NAME
   \param aCsy is written to MODEM_TABLE row KModemName column MODEM_CSY_NAME
   \param aTsy is written to MODEM_TABLE row KModemName column MODEM_TSY_NAME

   PITFALL: if you want to set something to empty string, you may have
   to rubout its default value. This is currently provided for aDescr
   and aTelNum.

*/
void CGnuboxContainer::WriteCommDbL(const TDesC & aTelNum, const TDesC& aDesc, const TDesC& aPort, const TDesC& aCsy, const TDesC& aTsy) {
  FLOG(_L("**** CGnuboxContainer::WriteCommDbL ****"));
  iCommDb = NULL;
  if (iLoader && iLoader->IsActive()) {
    iLoader->Cancel();
  }

  // First ensure all required records (ModemBearer, IAP, WAPAccessPoint) exist;
  // if any are missing (e.g. from a previously interrupted setup), create them all cleanly.
  {
    CMDBSession* checkSession = CMDBSession::NewLC(KCDVersion1_1);
    checkSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete);

    CCDModemBearerRecord* mRec = static_cast<CCDModemBearerRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
    CleanupStack::PushL(mRec);
    mRec->iRecordName.SetL(KModemName);
    TBool mFound = EFalse;
    TRAP_IGNORE(mFound = mRec->FindL(*checkSession));
    CleanupStack::PopAndDestroy(mRec);

    CCDIAPRecord* iapCheck = static_cast<CCDIAPRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
    CleanupStack::PushL(iapCheck);
    iapCheck->iRecordName.SetL(KISPName);
    TBool iapFound = EFalse;
    TRAP_IGNORE(iapFound = iapCheck->FindL(*checkSession));
    CleanupStack::PopAndDestroy(iapCheck);

    CCDWAPAccessPointRecord* wapCheck = static_cast<CCDWAPAccessPointRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
    CleanupStack::PushL(wapCheck);
    wapCheck->iRecordName.SetL(KWAPName);
    TBool wapFound = EFalse;
    TRAP_IGNORE(wapFound = wapCheck->FindL(*checkSession));
    CleanupStack::PopAndDestroy(wapCheck);

    CleanupStack::PopAndDestroy(checkSession);
    if (!mFound || !iapFound || !wapFound) {
      CreateCommDbRecords();
    }
  }

  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);

  // 1. Update or replace 'Bt' in DialOutISP
  CCDDialOutISPRecord* ispRec = static_cast<CCDDialOutISPRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
  CleanupStack::PushL(ispRec);
  ispRec->iRecordName.SetL(KISPName);
  if (aDesc.Length() > 0) {
    ispRec->iDescription.SetL(aDesc);
  } else {
    ispRec->iDescription.SetL(_L("Direct 2box"));
  }
  if (aTelNum.Length() > 0) {
    ispRec->iDefaultTelNum.SetL(aTelNum);
  } else {
    ispRec->iDefaultTelNum.SetL(KNullDesC);
  }
  ispRec->iType = EIspTypeInternetAndWAP;
  ispRec->iDialResolution = EFalse;
  ispRec->iUseLoginScript = ETrue;
  ispRec->iLoginScript.SetL(_L("CHARMAP [windows-1252]\nLOOP 10\n{\nSEND \"CLIENT\"+<0x0d>\nWAIT 3\n{\n\"SERVER\" OK\n}\n}\nEXIT KErrNoAnswer$\n\nOK:\nEXIT\n"));
  ispRec->iPromptForLogin = EFalse;
  ispRec->iLoginName.SetL(KNullDesC);
  ispRec->iLoginPass.SetL(KNullDesC);
  ispRec->iDisplayPct = EFalse;
  ispRec->iIfParams.SetL(KNullDesC);
  ispRec->iIfNetworks.SetL(_L("ip"));
  ispRec->iIfPromptForAuth = EFalse;
  ispRec->iIfAuthName.SetL(KNullDesC);
  ispRec->iIfAuthPass.SetL(KNullDesC);
  ispRec->iIfAuthRetries = 0;
  ispRec->iIfCallbackEnabled = EFalse;
  ispRec->iIfCallbackType = 0;
  ispRec->iIfCallbackInfo.SetL(KNullDesC8);
  ispRec->iCallbackTimeout = 0;
  ispRec->iIpAddrFromServer = ETrue;
  ispRec->iIpAddr.SetL(_L("0.0.0.0"));
  ispRec->iIpNetMask.SetL(_L("0.0.0.0"));
  ispRec->iIpGateway.SetL(_L("0.0.0.0"));
  ispRec->iIpDnsAddrFromServer = ETrue;
  ispRec->iIpNameServer1.SetL(_L("0.0.0.0"));
  ispRec->iIpNameServer2.SetL(_L("0.0.0.0"));
  ispRec->iIp6DnsAddrFromServer = ETrue;
  ispRec->iIp6NameServer1.SetL(_L("0:0:0:0:0:0:0:0"));
  ispRec->iIp6NameServer2.SetL(_L("0:0:0:0:0:0:0:0"));
  ispRec->iEnableIpHeaderComp = EFalse;
  ispRec->iEnableLcpExtension = EFalse;
  ispRec->iDisablePlainTextAuth = EFalse;
  ispRec->iEnableSwComp = EFalse;
  ispRec->iBearerName = 0;
  ispRec->iBearerSpeed = 0;
  ispRec->iBearerCe = 0;
  ispRec->iInitString.SetL(KNullDesC8);
  ispRec->iBearerType = 0;
  ispRec->iChannelCoding = 0;
  ispRec->iAiur = 0;
  ispRec->iRequestedTimeSlots = 0;
  ispRec->iMaximumTimeSlots = 0;
  ispRec->iBearerService = 0;
  ispRec->iBearerProtocol = 0;
  ispRec->iUseEdge = EFalse;
  TUint32 ispid = 0;
  LogLeaveIfErrorL(StoreNamedRecordCleanL(*dbSession, KCDTIdDialOutISPRecord, KISPName, ispRec, ispid), "writecommdb->isp");
  CleanupStack::PopAndDestroy(ispRec);

  // 2. Update or replace 'GnuBoxModem' in ModemBearer
  CCDModemBearerRecord* modemRec = static_cast<CCDModemBearerRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
  CleanupStack::PushL(modemRec);
  modemRec->iRecordName.SetL(KModemName);
  modemRec->iTsyName.SetL(aTsy);
  modemRec->iCsyName.SetL(aCsy);
  modemRec->iPortName.SetL(aPort);
  modemRec->iNifName.SetL(TPtrC(MODEMENTRY_IF_NAME));
  modemRec->iBearerAgent.SetL(_L("nullagt.agt"));
  modemRec->iBearerTechnology = KCommDbBearerCSD;
  modemRec->iRate = EBps115200;
  modemRec->iDataBits = EData8;
  modemRec->iStopBits = EStop1;
  modemRec->iParity = EParityNone;
  modemRec->iHandshaking = 4; // KConfigObeyCTS (no PSTN DCD required for Bluetooth RFCOMM)
  modemRec->iFaxClassPref = EClassAuto;
  modemRec->iSpeakerPref = EModemSpeakerSettingNever;
  modemRec->iSpeakerVolPref = EModemSpeakerVolumeQuiet;
  modemRec->iMinSignalLevel = 0;
  modemRec->iCommRole = 0;
  modemRec->iSirSettings = 0;
  modemRec->iSpecialRate = 0;
  modemRec->iXonChar = 0;
  modemRec->iXoffChar = 0;
  modemRec->iLastSocketActivityTimeout = 1800000;
  modemRec->iLastSocketClosedTimeout = 1800000;
  modemRec->iLastSessionClosedTimeout = 1800000;
  modemRec->iModemInitString.SetL(_L8("AT"));
  modemRec->iDataInitString.SetL(_L8("AT"));
  modemRec->iFaxInitString.SetL(_L8("AT"));
  TUint32 modemid = 0;
  LogLeaveIfErrorL(StoreNamedRecordCleanL(*dbSession, KCDTIdModemBearerRecord, KModemName, modemRec, modemid), "writecommdb->modem");
  CleanupStack::PopAndDestroy(modemRec);

  // 3. Ensure IAP 'Bt' points to DIAL_OUT_ISP and modemid
  TRAP_IGNORE({
    dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete);
    CCDIAPRecord* iapRec = static_cast<CCDIAPRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
    CleanupStack::PushL(iapRec);
    iapRec->iRecordName.SetL(KISPName);
    if (iapRec->FindL(*dbSession)) {
      iapRec->iService = ispid;
      iapRec->iServiceType.SetL(TPtrC(DIAL_OUT_ISP));
      iapRec->iBearer = modemid;
      iapRec->iBearerType.SetL(TPtrC(MODEM_TABLE));
      CommsDatModifyWithRetry(*dbSession, iapRec);
    }
    CleanupStack::PopAndDestroy(iapRec);
  });

  CleanupStack::PopAndDestroy(dbSession);
  FLOG(_L("Updated CommDB"));
} 





TBool CGnuboxContainer::UpdateCommDbL(const TCfgEntry * aBearer)
  /**
     TBool CGnubox::UpdateCommDbL(TConnectionMode aMode)
     Here we update the CommDb MODEM record to reflect the
     change in connection mode we have solicited.
     Currently there are five options:
     1) Direct PPP connection over Infrared
     2) Direct PPP connection over Bluetooth
     3) Dialup PPP connection over Infrared
     4) Dialup PPP connection over Bluetooth
     5) Default PPP over CSD/GPRS phone stack

     @TODO: Need to work out how to restore CSD/GPRS settings
  */
{
  FLOG(_L("**** CGnuboxContainer::UpdateCommDbL ****"));

  if (aBearer->iSetBT) {
    TRAPD(err,SelectBluetoothDeviceL(aBearer->iBTProfile));
    if (err != 0) {
      FLOG(_L("Failed to select BT"));
      return FALSE;
    } else {
      FLOG(_L("Selected BT"));                  
    }
  }
  if (aBearer->iEmptyTelNo) {
    WriteCommDbL(KDefaultTelNum,TPtrC(aBearer->iDescr),TPtrC(aBearer->iPortName),
               TPtrC(aBearer->iCSY),TPtrC(aBearer->iTSY));
  } else {
    TBuf<20> telnum;
    iParent->GetDocument()->RetrieveRecordL(KISPTelNumTxt,telnum);	
    FPrint(_L("Setting telnum to >%S<"),&telnum);
    WriteCommDbL(telnum,TPtrC(aBearer->iDescr),TPtrC(aBearer->iPortName),
               TPtrC(aBearer->iCSY),TPtrC(aBearer->iTSY));
  }

  return TRUE;
}


void CGnuboxContainer::CopyFromGPRSL() {
  FLOG(_L("Container::CopyFromGPRSL()"));
  //  TInt err;
  CCommsDatabase * iCommDb=CCommsDatabase::NewL(EDatabaseTypeIAP);
  CleanupStack::PushL(iCommDb);
  FLOG(_L("1"));
  CCommsDbTableView * view = iCommDb->OpenViewMatchingTextLC(TPtrC(OUTGOING_WCDMA),TPtrC(COMMDB_NAME),KISPName);
  FLOG(_L("2"));  
  LogLeaveIfErrorL(view->GotoFirstRecord(),"loading gprs record");
  FLOG(_L("3"));

  CCommsDbTableView * doview = iCommDb->OpenViewMatchingTextLC(TPtrC(DIAL_OUT_ISP),TPtrC(COMMDB_NAME),KISPName);
  FLOG(_L("4"));
  TInt doerr = doview->GotoFirstRecord();
  if (doerr == KErrNotFound) {
    TUint32 ispid = 0;
    LogLeaveIfErrorL(doview->InsertRecord(ispid), "dialoutisp->insert");
    LogTrapLeaveIfErrorL(doview->WriteTextL(TPtrC(COMMDB_NAME), KISPName), "dialoutisp->name");
  } else {
    LogLeaveIfErrorL(doerr, "loading dialoutisp record");
    LogLeaveIfErrorL(doview->UpdateRecord(), "dialout->start update");
  }
  FLOG(_L("5"));
  FLOG(_L("6"));
#define COPYBOOL( fn, tn, desc ) {TBool val; LogTrapLeaveIfErrorL(view->ReadBoolL(TPtrC(fn),val),"read " desc); LogTrapLeaveIfErrorL(doview->WriteBoolL(TPtrC(tn),val), "write " desc); }

#define COPYTEXT( fn, tn, desc ) {TBuf16<256> val; LogTrapLeaveIfErrorL(view->ReadTextL(TPtrC(fn),val),"read " desc); LogTrapLeaveIfErrorL(doview->WriteTextL(TPtrC(tn),val), "write " desc); }
  FLOG(_L("7a"));
  COPYBOOL(GPRS_HEADER_COMPRESSION, ISP_ENABLE_IP_HEADER_COMP, "header compression");
  FLOG(_L("7b"));
  COPYBOOL(GPRS_IF_PROMPT_FOR_AUTH, ISP_PROMPT_FOR_LOGIN, "prompt for login");
  FLOG(_L("7c"));
  COPYTEXT(GPRS_IF_AUTH_NAME,ISP_IF_AUTH_NAME, "login name1");
  COPYTEXT(GPRS_IF_AUTH_NAME,ISP_LOGIN_NAME, "login name2");
  FLOG(_L("7d"));
  COPYTEXT(GPRS_IF_AUTH_PASS,ISP_IF_AUTH_PASS, "passwd1");
  COPYTEXT(GPRS_IF_AUTH_PASS,ISP_LOGIN_PASS, "passwd2");
  FLOG(_L("7e"));
  COPYTEXT(GPRS_IP_NETMASK,ISP_IP_NETMASK, "netmask");
  FLOG(_L("7f"));
  COPYTEXT(GPRS_IP_GATEWAY,ISP_IP_GATEWAY, "gw");
  FLOG(_L("7g"));
  COPYBOOL(GPRS_IP_ADDR_FROM_SERVER,ISP_IP_ADDR_FROM_SERVER, "ipaddrfromserver");
  FLOG(_L("7h"));
  COPYBOOL(GPRS_IP_DNS_ADDR_FROM_SERVER,ISP_IP_DNS_ADDR_FROM_SERVER, "dnsaddrfromserver");
  FLOG(_L("7i"));
  COPYTEXT(GPRS_IP_NAME_SERVER1,ISP_IP_NAME_SERVER1,"dns1");
  FLOG(_L("7j"));
  COPYTEXT(GPRS_IP_NAME_SERVER2,ISP_IP_NAME_SERVER2,"dns2");
  FLOG(_L("7k"));
  COPYBOOL(GPRS_ENABLE_LCP_EXTENSIONS,ISP_ENABLE_LCP_EXTENSIONS,"enablelcpext");
  FLOG(_L("7l"));
  COPYBOOL(GPRS_DISABLE_PLAIN_TEXT_AUTH,ISP_DISABLE_PLAIN_TEXT_AUTH,"disableplaintextauth");
  FLOG(_L("7m"));

  LogLeaveIfErrorL(doview->PutRecordChanges(),"dialout->update record");
  FLOG(_L("8"));
  CleanupStack::PopAndDestroy(); //view1
  CleanupStack::PopAndDestroy(); //view2

  CleanupStack::PopAndDestroy(); //iCommDb
  FLOG(_L("9"));
  INFOMSG(_L("Settings copied."));
}

void CGnuboxContainer::SetToGPRSL() {
  FLOG(_L("Container::SetToGPRSL()"));

  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);
  dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);

  // 1. Find or create OutgoingGprsRecord 'Bt'
  CCDOutgoingGprsRecord* gprsRec = static_cast<CCDOutgoingGprsRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));
  CleanupStack::PushL(gprsRec);
  gprsRec->iRecordName.SetL(KISPName);
  TBool gprsFound = EFalse;
  TRAP_IGNORE(gprsFound = gprsRec->FindL(*dbSession));

  TUint32 gprsid = 0;
  if (!gprsFound) {
    gprsRec->SetRecordId(KCDNewRecordRequest);
    gprsRec->iGPRSAPN.SetL(_L("internet"));
    gprsRec->iGPRSPDPType = 0; // IPv4
    gprsRec->iGPRSPDPAddress.SetL(_L("0.0.0.0"));
    gprsRec->iGPRSReqPrecedence = 0;
    gprsRec->iGPRSReqDelay = 0;
    gprsRec->iGPRSReqReliability = 0;
    gprsRec->iGPRSReqPeakThroughput = 0;
    gprsRec->iGPRSReqMeanThroughput = 0;
    gprsRec->iGPRSMinPrecedence = 0;
    gprsRec->iGPRSMinDelay = 0;
    gprsRec->iGPRSMinReliability = 0;
    gprsRec->iGPRSMinPeakThroughput = 0;
    gprsRec->iGPRSMinMeanThroughput = 0;
    gprsRec->iGPRSDataCompression = EFalse;
    gprsRec->iGPRSHeaderCompression = EFalse;
    gprsRec->iGPRSUseEdge = EFalse;
    gprsRec->iGPRSAnonymousAccess = EFalse;
    gprsRec->iGPRSIfNetworks.SetL(_L("ip"));
    gprsRec->iGPRSIfPromptForAuth = EFalse;
    gprsRec->iGPRSIfAuthName.SetL(KNullDesC);
    gprsRec->iGPRSIfAuthPass.SetL(KNullDesC);
    gprsRec->iGPRSIfAuthRetries = 0;
    gprsRec->iGPRSIPNetMask.SetL(_L("0.0.0.0"));
    gprsRec->iGPRSIPGateway.SetL(_L("0.0.0.0"));
    gprsRec->iGPRSIPAddrFromServer = ETrue;
    gprsRec->iGPRSIPAddr.SetL(_L("0.0.0.0"));
    gprsRec->iGPRSIPDNSAddrFromServer = ETrue;
    gprsRec->iGPRSIPNameServer1.SetL(_L("0.0.0.0"));
    gprsRec->iGPRSIPNameServer2.SetL(_L("0.0.0.0"));
    gprsRec->iGPRSEnableLCPExtension = EFalse;
    gprsRec->iGPRSDisablePlainTextAuth = EFalse;
    LogLeaveIfErrorL(CommsDatStoreWithRetry(*dbSession, gprsRec), "gprs->store");
    gprsid = static_cast<TUint32>(gprsRec->RecordId());
  } else {
    gprsid = static_cast<TUint32>(gprsRec->RecordId());
  }
  CleanupStack::PopAndDestroy(gprsRec);

  // 2. Set IAP 'Bt' service type to OUTGOING_WCDMA so it appears in Destinations / AP list
  CCDIAPRecord* iapRec = static_cast<CCDIAPRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
  CleanupStack::PushL(iapRec);
  iapRec->iRecordName.SetL(KISPName);
  TBool iapFound = EFalse;
  TRAP_IGNORE(iapFound = iapRec->FindL(*dbSession));
  if (!iapFound) {
    CleanupStack::PopAndDestroy(2, dbSession);
    INFOMSG(_L("IAP 'Bt' not found. Run Create records first."));
    User::Leave(KErrNotFound);
  }

  iapRec->iServiceType.SetL(TPtrC(OUTGOING_WCDMA));
  iapRec->iService = gprsid;
  LogLeaveIfErrorL(CommsDatModifyWithRetry(*dbSession, iapRec), "iap->modify gprs");
  CleanupStack::PopAndDestroy(iapRec);

  CleanupStack::PopAndDestroy(dbSession);

  INFOMSG(_L("Done. 'Bt' is GPRS AP (Visible in Access Points)."));
  TRAP_IGNORE({
    CAknInformationNote* note = new (ELeave) CAknInformationNote(ETrue);
    note->ExecuteLD(_L("Set to GPRS OK! Access Point 'Bt' visible."));
  });
  FLOG(_L("SetToGPRSL complete: IAP 'Bt' is OUTGOING_WCDMA"));
}


void CGnuboxContainer::BringUpIfL() {
  FLOG(_L("Container::BringUpIF()"));

  if (iConnected) {
    iConn.Close();
    iSockServ.Close();
    iConnected = EFalse;
  }

  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);
  TUint32 iapid = 0;
  // Look up IAP 'Bt'
  CCDIAPRecord* iapRec = static_cast<CCDIAPRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
  CleanupStack::PushL(iapRec);
  iapRec->iRecordName.SetL(KISPName);
  TBool found = EFalse;
  TRAP_IGNORE(found = iapRec->FindL(*dbSession));
  if (found) {
    iapid = static_cast<TUint32>(iapRec->RecordId());
  }
  CleanupStack::PopAndDestroy(iapRec);
  CleanupStack::PopAndDestroy(dbSession);

  if (iapid == 0) {
    INFOMSG(_L("IAP 'Bt' not found. Run Create records first."));
    User::Leave(KErrNotFound);
  }

  TBuf<100> b;
  b.Format(_L("Connecting 'Bt' (iap %d)..."), iapid);
  FLOG(b);
  INFOMSG(b);

  LogLeaveIfErrorL(iSockServ.Connect(), "RSocketServ open");
  TInt connErr = iConn.Open(iSockServ);
  if (connErr != KErrNone) {
    iSockServ.Close();
    LogLeaveIfErrorL(connErr, "RConnection open");
  }

  TCommDbConnPref aPref;
  aPref.SetIapId(iapid);
  aPref.SetBearerSet(KCommDbBearerCSD | KCommDbBearerPSD);
  aPref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);

  TInt err = iConn.Start(aPref);
  if (err != KErrNone) {
    iConn.Close();
    iSockServ.Close();
    TBuf<100> eb;
    eb.Format(_L("Open conn: Error %d (iap %d)"), err, iapid);
    FLOG(eb);
    INFOMSG(eb);
    TRAP_IGNORE({
      CAknInformationNote* note = new (ELeave) CAknInformationNote(ETrue);
      note->ExecuteLD(eb);
    });
    User::Leave(err);
  }

  iConnected = ETrue;
  TBuf<128> okMsg;
  okMsg.Format(_L("PPP UP! Testing 192.168.7.1:80..."));
  INFOMSG(okMsg);

  // Quick HTTP probe directly over iConn
  RSocket testSock;
  TInt sockErr = testSock.Open(iSockServ, KAfInet, KSockStream, KProtocolInetTcp, iConn);
  if (sockErr == KErrNone) {
    TInetAddr destAddr;
    destAddr.SetAddress(INET_ADDR(192, 168, 7, 1));
    destAddr.SetPort(80);
    TRequestStatus stat;
    testSock.Connect(destAddr, stat);
    User::WaitForRequest(stat);
    if (stat.Int() == KErrNone) {
      _LIT8(KReq, "GET / HTTP/1.0\r\nHost: 192.168.7.1\r\n\r\n");
      testSock.Write(KReq, stat);
      User::WaitForRequest(stat);
      okMsg.Format(_L("PPP UP & HTTP OK! Web responded."));
    } else {
      okMsg.Format(_L("PPP UP! HTTP test err %d"), stat.Int());
    }
    testSock.Close();
  } else {
    okMsg.Format(_L("PPP UP! Socket open err %d"), sockErr);
  }

  FLOG(okMsg);
  INFOMSG(okMsg);
  TRAP_IGNORE({
    CAknInformationNote* note = new (ELeave) CAknInformationNote(ETrue);
    note->ExecuteLD(okMsg);
  });
}


void CGnuboxContainer::CreateCommDbRecords() {
  FLOG(_L("Container::CreateCommDbRecords()"));
  iCommDb = NULL;
  if (iLoader && iLoader->IsActive()) {
    iLoader->Cancel();
  }

  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);

  // Clean up any broken template record 0 or orphan "Default Modem" / "BTCOMM::5" records
  // left behind by an earlier CCommsDatabase / FixupModemEntryL run
  TRAP_IGNORE({
    dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);
    for (TInt pass = 0; pass < 4; ++pass) {
      CCDModemBearerRecord* orphan = static_cast<CCDModemBearerRecord*>(
          CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
      CleanupStack::PushL(orphan);
      orphan->iRecordName.SetL(_L("Default Modem"));
      if (orphan->FindL(*dbSession)) {
        CommsDatDeleteWithRetry(*dbSession, orphan);
        CleanupStack::PopAndDestroy(orphan);
      } else {
        CleanupStack::PopAndDestroy(orphan);
        break;
      }
    }
    for (TInt pass = 0; pass < 4; ++pass) {
      CCDModemBearerRecord* orphan = static_cast<CCDModemBearerRecord*>(
          CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
      CleanupStack::PushL(orphan);
      orphan->iPortName.SetL(_L("BTCOMM::5"));
      if (orphan->FindL(*dbSession)) {
        CommsDatDeleteWithRetry(*dbSession, orphan);
        CleanupStack::PopAndDestroy(orphan);
      } else {
        CleanupStack::PopAndDestroy(orphan);
        break;
      }
    }
    // Clean up any old OutgoingGprs record named 'Bt' (removes any leftover GPRS causing -4180)
    for (TInt pass = 0; pass < 4; ++pass) {
      CCDOutgoingGprsRecord* oldGprs = static_cast<CCDOutgoingGprsRecord*>(
          CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));
      CleanupStack::PushL(oldGprs);
      oldGprs->iRecordName.SetL(KISPName);
      if (oldGprs->FindL(*dbSession)) {
        CommsDatDeleteWithRetry(*dbSession, oldGprs);
        CleanupStack::PopAndDestroy(oldGprs);
      } else {
        CleanupStack::PopAndDestroy(oldGprs);
        break;
      }
    }
    dbSession->ClearAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);
  });

  // 1. ModemBearer ('GnuBoxModem')
  FLOG(_L("Treating modem entry"));
  CCDModemBearerRecord* modemRec = static_cast<CCDModemBearerRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
  CleanupStack::PushL(modemRec);
  modemRec->iRecordName.SetL(KModemName);
  modemRec->iPortName.SetL(_L("BTCOMM::5"));
  modemRec->iCsyName.SetL(_L("BTCOMM"));
  modemRec->iTsyName.SetL(KNullDesC);
  modemRec->iNifName.SetL(TPtrC(MODEMENTRY_IF_NAME));
  modemRec->iBearerAgent.SetL(_L("nullagt.agt"));
  modemRec->iBearerTechnology = KCommDbBearerCSD;
  modemRec->iRate = EBps115200;
  modemRec->iDataBits = EData8;
  modemRec->iStopBits = EStop1;
  modemRec->iParity = EParityNone;
  modemRec->iHandshaking = 4;
  modemRec->iFaxClassPref = EClassAuto;
  modemRec->iSpeakerPref = EModemSpeakerSettingNever;
  modemRec->iSpeakerVolPref = EModemSpeakerVolumeQuiet;
  modemRec->iMinSignalLevel = 0;
  modemRec->iCommRole = 0;
  modemRec->iSirSettings = 0;
  modemRec->iSpecialRate = 0;
  modemRec->iXonChar = 0;
  modemRec->iXoffChar = 0;
  modemRec->iLastSocketActivityTimeout = 1800000;
  modemRec->iLastSocketClosedTimeout = 1800000;
  modemRec->iLastSessionClosedTimeout = 1800000;
  modemRec->iModemInitString.SetL(_L8("AT"));
  modemRec->iDataInitString.SetL(_L8("AT"));
  modemRec->iFaxInitString.SetL(_L8("AT"));
  TUint32 modemid = 0;
  LogLeaveIfErrorL(StoreNamedRecordCleanL(*dbSession, KCDTIdModemBearerRecord, KModemName, modemRec, modemid), "modem->store");
  CleanupStack::PopAndDestroy(modemRec);

  // 2. DialOutISP ('Bt') - Required by GnuBox UI (DumpIspL) and standard Symbian dialup
  FLOG(_L("Treating DialOutISP entry"));
  CCDDialOutISPRecord* ispRec = static_cast<CCDDialOutISPRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
  CleanupStack::PushL(ispRec);
  ispRec->iRecordName.SetL(KISPName);
  ispRec->iDescription.SetL(_L("Direct 2box"));
  ispRec->iDefaultTelNum.SetL(_L("111"));
  ispRec->iType = EIspTypeInternetAndWAP;
  ispRec->iDialResolution = EFalse;
  ispRec->iUseLoginScript = ETrue;
  ispRec->iLoginScript.SetL(_L("CHARMAP [windows-1252]\nLOOP 10\n{\nSEND \"CLIENT\"+<0x0d>\nWAIT 3\n{\n\"SERVER\" OK\n}\n}\nEXIT KErrNoAnswer$\n\nOK:\nEXIT\n"));
  ispRec->iPromptForLogin = EFalse;
  ispRec->iLoginName.SetL(KNullDesC);
  ispRec->iLoginPass.SetL(KNullDesC);
  ispRec->iDisplayPct = EFalse;
  ispRec->iIfParams.SetL(KNullDesC);
  ispRec->iIfNetworks.SetL(_L("ip"));
  ispRec->iIfPromptForAuth = EFalse;
  ispRec->iIfAuthName.SetL(KNullDesC);
  ispRec->iIfAuthPass.SetL(KNullDesC);
  ispRec->iIfAuthRetries = 0;
  ispRec->iIfCallbackEnabled = EFalse;
  ispRec->iIfCallbackType = 0;
  ispRec->iIfCallbackInfo.SetL(KNullDesC8);
  ispRec->iCallbackTimeout = 0;
  ispRec->iIpAddrFromServer = ETrue;
  ispRec->iIpAddr.SetL(_L("0.0.0.0"));
  ispRec->iIpNetMask.SetL(_L("0.0.0.0"));
  ispRec->iIpGateway.SetL(_L("0.0.0.0"));
  ispRec->iIpDnsAddrFromServer = ETrue;
  ispRec->iIpNameServer1.SetL(_L("0.0.0.0"));
  ispRec->iIpNameServer2.SetL(_L("0.0.0.0"));
  ispRec->iIp6DnsAddrFromServer = ETrue;
  ispRec->iIp6NameServer1.SetL(_L("0:0:0:0:0:0:0:0"));
  ispRec->iIp6NameServer2.SetL(_L("0:0:0:0:0:0:0:0"));
  ispRec->iEnableIpHeaderComp = EFalse;
  ispRec->iEnableLcpExtension = EFalse;
  ispRec->iDisablePlainTextAuth = EFalse;
  ispRec->iEnableSwComp = EFalse;
  ispRec->iBearerName = 0;
  ispRec->iBearerSpeed = 0;
  ispRec->iBearerCe = 0;
  ispRec->iInitString.SetL(KNullDesC8);
  ispRec->iBearerType = 0;
  ispRec->iChannelCoding = 0;
  ispRec->iAiur = 0;
  ispRec->iRequestedTimeSlots = 0;
  ispRec->iMaximumTimeSlots = 0;
  ispRec->iBearerService = 0;
  ispRec->iBearerProtocol = 0;
  ispRec->iUseEdge = EFalse;
  TUint32 ispid = 0;
  LogLeaveIfErrorL(StoreNamedRecordCleanL(*dbSession, KCDTIdDialOutISPRecord, KISPName, ispRec, ispid), "isp->store");
  CleanupStack::PopAndDestroy(ispRec);

  // 3. Network ('Bt')
  FLOG(_L("Treating Network entry"));
  CCDNetworkRecord* netRec = static_cast<CCDNetworkRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdNetworkRecord));
  CleanupStack::PushL(netRec);
  netRec->iRecordName.SetL(KISPName);
  TUint32 networkid = 0;
  LogLeaveIfErrorL(StoreNamedRecordCleanL(*dbSession, KCDTIdNetworkRecord, KISPName, netRec, networkid), "network->store");
  CleanupStack::PopAndDestroy(netRec);

  // 4. Find default Location ID
  TUint32 defloc = 2;
  TRAP_IGNORE({
    dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete);
    CCDGlobalSettingsRecord* globalRec = static_cast<CCDGlobalSettingsRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdGlobalSettingsRecord));
    CleanupStack::PushL(globalRec);
    globalRec->SetRecordId(1);
    globalRec->LoadL(*dbSession);
    if (TUint32(globalRec->iLocationForDataAndFax) > 0) {
      defloc = globalRec->iLocationForDataAndFax;
    }
    CleanupStack::PopAndDestroy(globalRec);
    dbSession->ClearAttributeMask(ECDHidden | ECDNoWriteButDelete);
  });

  // 5. IAP ('Bt') - DIAL_OUT_ISP / GnuBoxModem (ModemBearer)
  FLOG(_L("Treating IAP entry 'Bt'"));
  CCDIAPRecord* iapRec = static_cast<CCDIAPRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
  CleanupStack::PushL(iapRec);
  iapRec->iRecordName.SetL(KISPName);
  iapRec->iService = ispid;
  iapRec->iServiceType.SetL(TPtrC(DIAL_OUT_ISP));
  iapRec->iBearer = modemid;
  iapRec->iBearerType.SetL(TPtrC(MODEM_TABLE));
  iapRec->iNetwork = networkid;
  iapRec->iNetworkWeighting = 0;
  iapRec->iLocation = defloc;
  TUint32 iapid = 0;
  LogLeaveIfErrorL(StoreNamedRecordCleanL(*dbSession, KCDTIdIAPRecord, KISPName, iapRec, iapid), "iap->store");
  CleanupStack::PopAndDestroy(iapRec);

  // 6. WAPAccessPoint ('Bt')
  FLOG(_L("Treating WAP AccessPoint entry"));
  CCDWAPAccessPointRecord* wapRec = static_cast<CCDWAPAccessPointRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
  CleanupStack::PushL(wapRec);
  wapRec->iRecordName.SetL(KWAPName);
  wapRec->iWAPCurrentBearer.SetL(TPtrC(WAP_IP_BEARER));
  wapRec->iWAPStartPage.SetL(_L("http://"));
  TUint32 wapid = 0;
  LogLeaveIfErrorL(StoreNamedRecordCleanL(*dbSession, KCDTIdWAPAccessPointRecord, KWAPName, wapRec, wapid), "wap->store");
  CleanupStack::PopAndDestroy(wapRec);

  // 7. WAPIPBearer (linked to wapid & iapid)
  FLOG(_L("Treating WAP IPBearer entry"));
  dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);
  for (TInt pass = 0; pass < 4; ++pass) {
    CCDWAPIPBearerRecord* findWapIp = static_cast<CCDWAPIPBearerRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdWAPIPBearerRecord));
    CleanupStack::PushL(findWapIp);
    findWapIp->iWAPAccessPointId = wapid;
    TBool foundWapIp = EFalse;
    TRAP_IGNORE(foundWapIp = findWapIp->FindL(*dbSession));
    if (foundWapIp) {
      CommsDatDeleteWithRetry(*dbSession, findWapIp);
      CleanupStack::PopAndDestroy(findWapIp);
    } else {
      CleanupStack::PopAndDestroy(findWapIp);
      break;
    }
  }
  dbSession->ClearAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);

  CCDWAPIPBearerRecord* wapIpRec = static_cast<CCDWAPIPBearerRecord*>(
      CCDRecordBase::RecordFactoryL(KCDTIdWAPIPBearerRecord));
  CleanupStack::PushL(wapIpRec);
  wapIpRec->iRecordName.SetL(KWAPName);
  wapIpRec->iWAPAccessPointId = wapid;
  wapIpRec->iWAPGatewayAddress.SetL(_L("0.0.0.0"));
  wapIpRec->iWAPIAP = iapid;
  wapIpRec->iWAPProxyPort = 0;
  wapIpRec->iWAPProxyLoginName.SetL(KNullDesC);
  wapIpRec->iWAPProxyLoginPass.SetL(KNullDesC);
  wapIpRec->iWAPSecurity = EFalse;
  wapIpRec->iWAPWSPOption = EWapWspOptionConnectionOriented;
  wapIpRec->SetRecordId(KCDNewRecordRequest);
  LogLeaveIfErrorL(CommsDatStoreWithRetry(*dbSession, wapIpRec), "wapip->store");
  CleanupStack::PopAndDestroy(wapIpRec);

  CleanupStack::PopAndDestroy(dbSession);

  // Also set Data/Fax modem and default AP automatically (ignoring non-fatal errors)
  TRAP_IGNORE(SetDataModemL(ETrue));
  TRAP_IGNORE(ChangeConnPrefL());

  INFOMSG(_L("Done. Access Point 'Bt' created."));
  TRAP_IGNORE({
    CAknInformationNote* note = new (ELeave) CAknInformationNote(ETrue);
    note->ExecuteLD(_L("Records created OK!"));
  });
  SetAndDrawScreenTextL(iParent->GetMode());
}


void CGnuboxContainer::DeleteCommDbRecords() {
  FLOG(_L("Container::DeleteCommDbRecords()"));
  iCommDb = NULL;
  if (iLoader && iLoader->IsActive()) {
    iLoader->Cancel();
  }

  CMDBSession* dbSession = CMDBSession::NewLC(KCDVersion1_1);
  dbSession->SetAttributeMask(ECDHidden | ECDNoWriteButDelete | ECDProtectedWrite);

  // Delete WAP AccessPoint & WAP IPBearer
  for (TInt pass = 0; pass < 8; ++pass) {
    CCDWAPAccessPointRecord* wapRec = static_cast<CCDWAPAccessPointRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
    CleanupStack::PushL(wapRec);
    wapRec->iRecordName.SetL(KWAPName);
    TBool found = EFalse;
    TRAP_IGNORE(found = wapRec->FindL(*dbSession));
    if (found) {
      TUint32 wapid = static_cast<TUint32>(wapRec->RecordId());
      CCDWAPIPBearerRecord* wapIpRec = static_cast<CCDWAPIPBearerRecord*>(
          CCDRecordBase::RecordFactoryL(KCDTIdWAPIPBearerRecord));
      CleanupStack::PushL(wapIpRec);
      wapIpRec->iWAPAccessPointId = wapid;
      TBool foundIp = EFalse;
      TRAP_IGNORE(foundIp = wapIpRec->FindL(*dbSession));
      if (foundIp) {
        CommsDatDeleteWithRetry(*dbSession, wapIpRec);
      }
      CleanupStack::PopAndDestroy(wapIpRec);
      CommsDatDeleteWithRetry(*dbSession, wapRec);
      CleanupStack::PopAndDestroy(wapRec);
    } else {
      CleanupStack::PopAndDestroy(wapRec);
      break;
    }
  }

  const TMDBElementId tables[] = {
    KCDTIdIAPRecord,
    KCDTIdNetworkRecord,
    KCDTIdOutgoingGprsRecord,
    KCDTIdDialOutISPRecord,
    KCDTIdLANServiceRecord
  };
  _LIT(KBtDirectName, "BtDirect");
  const TDesC* namesToClean[] = { &KISPName, &KBtDirectName };
  for (TUint n = 0; n < sizeof(namesToClean) / sizeof(namesToClean[0]); ++n) {
    for (TUint i = 0; i < sizeof(tables) / sizeof(tables[0]); ++i) {
      for (TInt pass = 0; pass < 8; ++pass) {
        CCDRecordBase* rec = static_cast<CCDRecordBase*>(CCDRecordBase::RecordFactoryL(tables[i]));
        CleanupStack::PushL(rec);
        rec->iRecordName.SetL(*namesToClean[n]);
        TBool found = EFalse;
        TRAP_IGNORE(found = rec->FindL(*dbSession));
        if (found) {
          CommsDatDeleteWithRetry(*dbSession, rec);
          CleanupStack::PopAndDestroy(rec);
        } else {
          CleanupStack::PopAndDestroy(rec);
          break;
        }
      }
    }
  }

  for (TInt pass = 0; pass < 8; ++pass) {
    CCDModemBearerRecord* modemRec = static_cast<CCDModemBearerRecord*>(
        CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
    CleanupStack::PushL(modemRec);
    modemRec->iRecordName.SetL(KModemName);
    TBool found = EFalse;
    TRAP_IGNORE(found = modemRec->FindL(*dbSession));
    if (found) {
      CommsDatDeleteWithRetry(*dbSession, modemRec);
      CleanupStack::PopAndDestroy(modemRec);
    } else {
      CleanupStack::PopAndDestroy(modemRec);
      break;
    }
  }

  CleanupStack::PopAndDestroy(dbSession);

  INFOMSG(_L("Done."));
  SetAndDrawScreenTextL(iParent->GetMode());
}


void CGnuboxContainer::SelectBluetoothDeviceL(TBTProfile aProfile)
  /**
     CGnuboxContainer::SelectBluetoothDeviceL()
     Deals with Bluetooth device selection.
  */
{
  FLOG(_L("CGnuboxContainer::SelectBluetoothDevice"));
  
#ifdef BTSELECTAO
  iBTRegister->StartDialog(aProfile);
  //All the job was moved to the Active Object gnuboxAOBTRegister
#else
  //UIQ BT select dialog
  TBTDevAddr remoteDevAddr(0);
  TBTDeviceName remoteDevName;
  TBTDeviceClass remoteDevClass;
  TInt serviceFilter = -1;
  CQBTUISelectDialog* btSelectDialog = new(ELeave) CQBTUISelectDialog
    (remoteDevAddr, remoteDevName, remoteDevClass,
     CQBTUISelectDialog::EQBTDeviceFilterAll, serviceFilter);
  if(btSelectDialog->LaunchSingleSelectDialogLD()) {
    TBTDeviceResponseParams p;
    p.SetDeviceAddress(remoteDevAddr);
    p.SetDeviceClass(remoteDevClass);
    p.SetDeviceName(remoteDevName);
    SetupCSYL(p,aProfile);
  } else {
    //we leave if user presses cancel
    User::Leave(0);
  }
#endif
}

void CGnuboxContainer::SetupCSYL(const TBTDeviceResponseParams &resp, TUint32 aProfile)
{
  FLOG(_L("0"));
  iDevAddr = resp.BDAddr();
  iDevName = resp.DeviceName();
  FLOG(_L("1"));

  //#ifndef NOBTREGISTRY
  TBTCommPortSettings	settings;
  //#endif

  FLOG(_L("2"));
  TBuf<150> b;
  FLOG(_L("3"));
  TBuf<12> addrAsText;
  addrAsText.Zero();
  addrAsText.Format(_L("%02x%02x%02x%02x%02x%02x"),
                    iDevAddr[0], iDevAddr[1], iDevAddr[2], iDevAddr[3], iDevAddr[4], iDevAddr[5]);
  FLOG(_L("3b"));
  b.Zero();
  b.Format(_L("Connecting to %S"),&iDevName);
  FLOG(_L("4"));
  TInt butret;
#ifdef __SERIES60__
  CAknQueryDialog* dlg;
  dlg = CAknQueryDialog::NewL( CAknQueryDialog::ENoTone );
  butret=!dlg->ExecuteLD( R_GNUBOX_ENCRYPT_CONFIRM_QUERY );

#else //UIQ
  TRequestStatus s;
  RNotifier n;
  User::LeaveIfError(n.Connect());
  FLOG(_L("X"));
  n.Notify(b,_L("Do you want to require encryption?"),_L("Yes"),_L("No"),butret,s);
  FLOG(_L("Y"));
  User::WaitForRequest(s);
  FLOG(_L("Z"));
  n.Close();
#endif


#ifdef NOBTREGISTRY
  //we need to emulate BT registry api
  //we have second version BT registry

  RBTRegServ iRegistryServer;
  LogLeaveIfErrorL(iRegistryServer.Connect(),"BT registry server connect");
  CleanupClosePushL(iRegistryServer);
  RBTCommPortSettings iGlobalSettings;
  LogLeaveIfErrorL(iGlobalSettings.Open(iRegistryServer),"BT RegServer settings open"); 
  CleanupClosePushL(iGlobalSettings);
#else
  CBTRegistry* registry=CBTRegistry::NewLC();
  //  CleanupStack::PushL(registry);

#endif

  settings.SetPort(5);
  settings.SetBTAddr(iDevAddr);
  settings.SetName(iDevName);
  TUUID srvuid(aProfile);
  settings.SetUUID(srvuid);

#ifdef NOBTREGISTRY
  if (butret==0) {
    settings.SetNoSecurity();
    settings.SetAuthenticationAndEncryption();
  } else {
    // True NoSecurity (level 0) when user answers "No" to encryption prompt,
    // matching vetera-bridge 'btsec off' / gateway-initiated 'btsec auto'
    settings.SetNoSecurity();
  }
#else

  /*
  settings.SetUUID(TUint(aProfile));  // The service - e.g. DUNP=0x1103
  settings.SetPort(5);				// n where BTCOMM::n
  settings.SetBTAddr(iDevAddr);
  */
  if (butret==0) {
    settings.SetNoSecurity();
    settings.SetEncryption();
  } else {
    settings.SetNoSecurity();
    settings.SetAuthentication();
  }
#endif

  FLOG(addrAsText);
  FLOG(_L("BTCOMM CSY Configured\n"));

#ifdef NOBTREGISTRY
  TRequestStatus status;
  // Primary GnuBox port (BTCOMM::5)
  settings.SetPort(5);
  iGlobalSettings.Modify(settings, status);
  User::WaitForRequest(status);
  LogLeaveIfErrorL(status.Int(),"Update BT registry");

  // Also update BTCOMM::0 (Auto), BTCOMM::3 (vetera-bridge LAP ch3),
  // and BTCOMM::4 (vetera-bridge SPP/DUN ch4) so no stale BTCOMM entry remains
  const TUint32 extraPorts[] = { 0, 3, 4 };
  for (TInt i = 0; i < 3; ++i) {
    settings.SetPort(extraPorts[i]);
    iGlobalSettings.Modify(settings, status);
    User::WaitForRequest(status);
  }
  CleanupStack::PopAndDestroy(2); // iGlobalSettings, iRegistryServer

#else
  TRAP(err,registry->SetDefaultCommPort(settings));
  LogLeaveIfErrorL(err,"Setting BT registry");
  CleanupStack::PopAndDestroy(); //registry

#endif	
  FLOG(_L("BTCOMM CSY Configured\n"));
  INFOMSG(_L("Set BT registry here"));
  SetAndDrawScreenTextL(iParent->GetMode());
}


#ifdef __UIQ__
class CTextQueryDialog: public CEikDialog {
private:
  TDes& iText;
public:
  CTextQueryDialog(TDes& aText):iText(aText) {}

  void PreLayoutDynInitL() {
    CEikEdwin* edwin;
    edwin=STATIC_CAST(CEikEdwin*, Control(EDialogControlIdText));
    edwin->SetTextL(&iText);
  }

  TBool OkToExitL(TInt) {
    CEikEdwin* edwin;
    edwin=STATIC_CAST(CEikEdwin*, Control(EDialogControlIdText));
    edwin->GetText(iText);
    return ETrue;
  }

};


#endif

void CGnuboxContainer::QueryTelNoL() {
#ifdef ENABLESETTELNO  
  TBuf<32> telnum;
  iParent->GetDocument()->RetrieveRecordL(KISPTelNumTxt,telnum);
#ifdef __SERIES60__
  CAknTextQueryDialog* dlg = new (ELeave) CAknTextQueryDialog(telnum,CAknQueryDialog::ENoTone);
  if (! dlg->ExecuteLD(R_DIALOG_TELNUM_QUERY)) {
    return;
  }
#elif defined(__UIQ__)
  CEikDialog *dlg = new (ELeave) CTextQueryDialog(telnum);
  if (! dlg->ExecuteLD(R_DIALOG_TELNUM_QUERY)) {
    return;
  }
  TBuf<100> pr;
  pr.Format(_L("entered %S"),&telnum);
  INFOMSG(pr);
  FLOG(pr);
#elif defined(__SERIES90__)

#endif                   
  iParent->GetDocument()->StoreRecordL(KISPTelNumTxt,telnum);
  const TCfgEntry * ce = PhoneCfg::GetSettings(EGnuboxCmdAppCSD_GPRS);
  SwitchModeL(ce);
#endif

}
