#include "routedump.h"
#include "debug.h"
#include <in_iface.h>
#include <c32comm.h>
#include <d32comm.h>
#include "gnuboxPhone.h"

#ifndef SERIES60
//#include <mrouterclient.h>
#endif

#include <e32std.h>
#include <eikenv.h>

#include <cdbcols.h>
#include "cdb.h"
#include <cdbpreftable.h>
#include <apgcli.h>

CInfoDump::CInfoDump()
{
  rs.Connect(3);

}

void CInfoDump::ConstructL()
{
  iFs.Connect();
  iCC=CCnvCharacterSetConverter::NewL();
  iCC->PrepareToConvertToOrFromL(KCharacterSetIdentifierIso88591, iFs);
  state=CCnvCharacterSetConverter::KStateDefault;
}

CInfoDump::~CInfoDump()
{
  iFile.Close();
  iFs.Close();
  rs.Close();
}


void CInfoDump::FieldValue(const TDesC& name, const TDesC& value)
{
  CC().ConvertFromUnicode(iBuf, name);
  iFile.Write(iBuf);
  iFile.Write(_L8(" = "));
  CC().ConvertFromUnicode(iBuf, value);
  iFile.Write(iBuf);
  iFile.Write(_L8("\n"));
}

void CInfoDump::FieldValue(const TDesC& name, const TInetAddr& addr) {
  TBuf<100> nb;
  addr.Output(nb);
  FieldValue(name,nb);
}

void CInfoDump::FieldValue(const TDesC& name, const TInt i) {
  TBuf<100> nb;
  nb.Format(_L("%d"),i);
  FieldValue(name,nb);
}

void CInfoDump::FieldValue(const TDesC& name, const TUid uid) {
  TBuf<100> nb;
  nb.Format(_L("0x%x"),uid);
  FieldValue(name,nb);
}



void CInfoDump::CapsValue(const TDesC &name, const TInt val, const TCapsMaskEntry* skel) {
  CC().ConvertFromUnicode(iBuf, name);
  iFile.Write(iBuf);
  iFile.Write(_L8(" = "));
  if (!val) { 
    iFile.Write(_L8("<empty mask>"));
  } else {
    while(skel->iMask) {
      if (val & skel->iMask) {
        CC().ConvertFromUnicode(iBuf, TPtrC(skel->iName));
        iFile.Write(iBuf);
        iFile.Write(_L8(" "));
      }
      skel++;
    }
  }
  iFile.Write(_L8("\n"));
}

#define M(x) { x , _S( # x ) }

static const TCapsMaskEntry RATE_CAPS[] = {
  M(KCapsBps50),
  M(KCapsBps75),
  M(KCapsBps110),
  M(KCapsBps134),
  M(KCapsBps150),
  M(KCapsBps300),
  M(KCapsBps600),
  M(KCapsBps1200),
  M(KCapsBps1800),
  M(KCapsBps2000),
  M(KCapsBps2400),
  M(KCapsBps3600),
  M(KCapsBps4800),
  M(KCapsBps7200),
  M(KCapsBps9600),
  M(KCapsBps19200),
  M(KCapsBps57600),
  M(KCapsBps115200),
  M(KCapsBps230400),
  M(KCapsBps460800),
  M(KCapsBps1152000),
  M(KCapsBps4000000),
  M(KCapsBpsSpecial),
  { 0 }
};

void CInfoDump::DumpInterface(TSoInetInterfaceInfo &cif) {
  iFile.Write(_L8("INTERFACE\n\n"));
  FieldValue(_L("Name"),cif.iName);
  FieldValue(_L("Tag"),cif.iTag);
  iValue.Format(_L("%u"),cif.iState);
  FieldValue(_L("State"),iValue);
  iValue.Format(_L("%u"),cif.iSpeedMetric);
  FieldValue(_L("Speed"),iValue);
  iValue.Format(_L("%u"),cif.iFeatures);
  FieldValue(_L("Features"),iValue);
  FieldValue(_L("Address"),cif.iAddress);
  FieldValue(_L("Netmask"),cif.iNetMask);
  FieldValue(_L("Broadcast"),cif.iBrdAddr);
  FieldValue(_L("Gateway"),cif.iDefGate);
  FieldValue(_L("DNS1"),cif.iNameSer1);
  FieldValue(_L("DNS2"),cif.iNameSer2);
  iFile.Write(_L8("\n\n"));
}

void CInfoDump::DumpRoute(TSoInetRouteInfo &rif) {
  iFile.Write(_L8("ROUTE\n\n"));
  iValue.Format(_L("%u"),rif.iType);
  FieldValue(_L("Type"),iValue);
  iValue.Format(_L("%u"),rif.iState);
  FieldValue(_L("State"),iValue);
  iValue.Format(_L("%u"),rif.iMetric);
  FieldValue(_L("Metric"),iValue);
  FieldValue(_L("Destination"),rif.iDstAddr);
  FieldValue(_L("Netmask"),rif.iNetMask);
  FieldValue(_L("IFAddress"),rif.iIfAddr);
  FieldValue(_L("Gateway"),rif.iGateway);
  iFile.Write(_L8("\n\n"));
}





void CInfoDump::DumpCPrefInfotoFileL(const TDesC& FileName)
{
  User::LeaveIfError(iFile.Replace(Fs(), FileName, EFileWrite));
  CleanupClosePushL(iFile);

  CCommsDatabase * iCommDb=CCommsDatabase::NewL(EDatabaseTypeIAP);
  iCommDb->ShowHiddenRecords();
  
  CleanupStack::PushL(iCommDb);

  CCommsDbConnectionPrefTableView* view = iCommDb->OpenConnectionPrefTableLC();
  
  CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref cpr;
  TInt err;
  for (err=view->GotoFirstRecord();err!=KErrNotFound;err=view->GotoNextRecord()) {
    LogLeaveIfErrorL(err,"connpref goto");
    LogTrapLeaveIfErrorL(view->ReadConnectionPreferenceL(cpr),"connpref read");
    iFile.Write(_L8("\n\nCONNECTION PREF\n\n"));
    FieldValue(_L("Rank"),cpr.iRanking);
    FieldValue(_L("Direction"),cpr.iDirection);
    FieldValue(_L("DialogPref"),cpr.iDialogPref);
    FieldValue(_L("BearerSet"),cpr.iBearer.iBearerSet);
    FieldValue(_L("IAP"),cpr.iBearer.iIapId);
    iFile.Write(_L8("\n"));    
  }
  
  CleanupStack::PopAndDestroy(); //view
  CleanupStack::PopAndDestroy(); //iCommsDb

  CleanupStack::PopAndDestroy(); //iFile
}

void CInfoDump::DumpInfotoFileL(const TDesC& FileName)
{
  User::LeaveIfError(iFile.Replace(Fs(), FileName, EFileWrite));
  CleanupClosePushL(iFile);

  RSocket socket;
  User::LeaveIfError(socket.Open(rs,KAfInet,KSockStream,KProtocolInetTcp));
  /*TInetAddr addr;
  addr.SetAddress(INET_ADDR(127,0,0,1));
  addr.SetPort(5843);
  socket.Bind(addr);*/
  TPckgBuf<TSoInetInterfaceInfo> iif;
  User::LeaveIfError(socket.SetOpt(KSoInetEnumInterfaces,KSolInetIfCtrl)); 
  TInt ret;
  while ((ret=socket.GetOpt(KSoInetNextInterface,KSolInetIfCtrl,iif))==KErrNone) {
    DumpInterface(iif());
  }

  TPckgBuf<TSoInetRouteInfo> irt;
  User::LeaveIfError(socket.SetOpt(KSoInetEnumRoutes,KSolInetRtCtrl));
  while ((ret=socket.GetOpt(KSoInetNextRoute,KSolInetRtCtrl,irt))==KErrNone) {
    DumpRoute(irt());
  }
  socket.Close();
  CleanupStack::PopAndDestroy(); //iFile
}

void CInfoDump::AddMRouterDefaultRouteL(const TDesC& FileName) {
  User::LeaveIfError(iFile.Replace(Fs(), FileName, EFileWrite));
  CleanupClosePushL(iFile);
  RSocket socket;
  User::LeaveIfError(socket.Open(rs,KAfInet,KSockStream,KProtocolInetTcp));
  iFile.Write(_L8("1"));

  //search for address and gateway
  TPckgBuf<TSoInetRouteInfo> irt;
  TPckgBuf<TSoInetRouteInfo> nrt;
  nrt().iIfAddr=TInetAddr(KInetAddrNone,0);
  nrt().iGateway=TInetAddr(KInetAddrNone,0);
  User::LeaveIfError(socket.SetOpt(KSoInetEnumRoutes,KSolInetRtCtrl));
  TInt ret;
  while ((ret=socket.GetOpt(KSoInetNextRoute,KSolInetRtCtrl,irt))==KErrNone) {
    if ((irt().iNetMask.Address()==INET_ADDR(255,254,0,0))||(irt().iNetMask.Address()==INET_ADDR(0,0,0,0))) {
      //this has the if address
      nrt().iIfAddr=irt().iIfAddr;
    }
  }
  iFile.Write(_L8("2"));
  if (nrt().iIfAddr.Address()==KInetAddrNone) {
    iFile.Write(_L8("3"));
    User::Leave(KErrNotFound);
    //    INFOMSG(_L("Route not found. Check that mRouter connection is up!")); 
    //return;
  }
  nrt().iDstAddr=TInetAddr(KInetAddrAny,KInetPortAny);
  nrt().iNetMask=TInetAddr(KInetAddrMaskAll,KInetPortAny);;
  nrt().iGateway=TInetAddr(INET_ADDR(169,254,1,68),0);
  nrt().iState=ERtReady;
  nrt().iType=ERtUser;
  nrt().iMetric=0;
  DumpRoute(nrt());
  iFile.Write(_L8("4"));
  TInt err=socket.SetOpt(KSoInetAddRoute,KSolInetRtCtrl,nrt);
  iValue.Format(_L("Err %d "),err);
  FieldValue(_L(""),iValue);
  iFile.Write(_L8("5"));
  socket.Close();
  iFile.Write(_L8("6"));
  //DumpInfotoFileL(FileName);
  CleanupStack::PopAndDestroy(); //iFile
}






void CInfoDump::DumpSerInfotoFileL(const TDesC& FileName)
{
  User::LeaveIfError(iFile.Replace(Fs(), FileName, EFileWrite));
  CleanupClosePushL(iFile);

  iFile.Write(_L8("1"));


  RCommServ cserv;
  User::LeaveIfError(cserv.Connect());
  iFile.Write(_L8("2"));
  for(unsigned int i=0;CSYInfo::CSYFILES[i];++i) {
    TInt ret=cserv.LoadCommModule(TPtrC(CSYInfo::CSYFILES[i]));
    if (ret!=KErrNone) {
      FPrint(_L("Loading comm module %s: error %d"),CSYInfo::CSYFILES[i],ret);
    }
  }
  /*
#ifdef __S60FP2__
  User::LeaveIfError(cserv.LoadCommModule(_L("btcomm")));
  User::LeaveIfError(cserv.LoadCommModule(_L("dataport")));
  User::LeaveIfError(cserv.LoadCommModule(_L("ecacm")));
  User::LeaveIfError(cserv.LoadCommModule(_L("ecuart")));
  User::LeaveIfError(cserv.LoadCommModule(_L("ircomm")));
#else //UIQ
  User::LeaveIfError(cserv.LoadCommModule(_L("ecuart")));
  User::LeaveIfError(cserv.LoadCommModule(_L("btcomm")));
  User::LeaveIfError(cserv.LoadCommModule(_L("btdun")));
  User::LeaveIfError(cserv.LoadCommModule(_L("ircomm")));
  User::LeaveIfError(cserv.LoadCommModule(_L("ecmux")));
#endif
  */
  iFile.Write(_L8("3"));
  TInt nump;
  User::LeaveIfError(cserv.NumPorts(nump));
  iFile.Write(_L8("4\n"));
  FieldValue(_L("Num Modules"),nump);
  for (int j=0;j<nump;j++) {
    TBuf<150> modn;
    TSerialInfo si;
    User::LeaveIfError(cserv.GetPortInfo(j,modn,si));
    iFile.Write(_L8("\n\nMODULE\n"));
    FieldValue(_L("NAME"),modn);
    iFile.Write(_L8("\n"));
    iFile.Write(_L8("5\n"));
    FieldValue(_L("PortName"),si.iName);
    FieldValue(_L("PortDesc"),si.iDescription);
    FieldValue(_L("LowUnit"),si.iLowUnit);
    FieldValue(_L("HighUnit"),si.iHighUnit);
  }
  {
    RComm cpt;
    iFile.Write(_L8("a"));  
    TInt ret=cpt.Open(cserv,_L("COMM::0"),ECommPreemptable);
    if (ret!=KErrNone) {
      if (ret==KErrLocked) {
        iFile.Write(_L8("=locked."));
      } else {
        FieldValue(_L("Error"),ret);
      }
    } else { //no error -- get caps
      iFile.Write(_L8("b"));  
      TCommCaps2 caps;
      cpt.Caps(caps);  
      FieldValue(_L("Supp rate"),caps().iRate);
      CapsValue(_L("Supp rate"),caps().iRate,RATE_CAPS);
      FieldValue(_L("Supp databits"),caps().iDataBits);
      FieldValue(_L("Supp StopBits"),caps().iStopBits);
      FieldValue(_L("Supp Parity"),caps().iParity);
      FieldValue(_L("Supp Handshake"),caps().iHandshake);
      FieldValue(_L("Supp Signals"),caps().iSignals);
      FieldValue(_L("Supp Fifo"),caps().iFifo);
      FieldValue(_L("Supp SIR"),caps().iSIR);
    }
  }

  {
    RComm cpt;
    iFile.Write(_L8("a"));  
    TInt ret=cpt.Open(cserv,_L("BTCOMM::5"),ECommPreemptable);
    if (ret!=KErrNone) {
      if (ret==KErrLocked) {
        iFile.Write(_L8("=locked."));
      } else {
        FieldValue(_L("Error"),ret);
      }
    } else { //no error -- get caps
      iFile.Write(_L8("b"));  
      TCommCaps2 caps;
      cpt.Caps(caps);  
      FieldValue(_L("Supp rate"),caps().iRate);
      CapsValue(_L("Supp rate"),caps().iRate,RATE_CAPS);
      FieldValue(_L("Supp databits"),caps().iDataBits);
      FieldValue(_L("Supp StopBits"),caps().iStopBits);
      FieldValue(_L("Supp Parity"),caps().iParity);
      FieldValue(_L("Supp Handshake"),caps().iHandshake);
      FieldValue(_L("Supp Signals"),caps().iSignals);
      FieldValue(_L("Supp Fifo"),caps().iFifo);
      FieldValue(_L("Supp SIR"),caps().iSIR);
    }
  }
  
  {
    RComm cpt;
    iFile.Write(_L8("A"));  
    TInt ret=cpt.Open(cserv,_L("ACM::0"),ECommPreemptable);
    if (ret!=KErrNone) {
      if (ret==KErrLocked) {
        iFile.Write(_L8("=locked."));
      } else {
        FieldValue(_L("Error"),ret);
      }
    } else { //no error -- get caps
      iFile.Write(_L8("b"));  
      TCommCaps2 caps;
      cpt.Caps(caps);  
      FieldValue(_L("Supp rate"),caps().iRate);
      CapsValue(_L("Supp rate"),caps().iRate,RATE_CAPS);
      FieldValue(_L("Supp databits"),caps().iDataBits);
      FieldValue(_L("Supp StopBits"),caps().iStopBits);
      FieldValue(_L("Supp Parity"),caps().iParity);
      FieldValue(_L("Supp Handshake"),caps().iHandshake);
      FieldValue(_L("Supp Signals"),caps().iSignals);
      FieldValue(_L("Supp Fifo"),caps().iFifo);
      FieldValue(_L("Supp SIR"),caps().iSIR);
    }
  }
  //mrouter info
  /*  RmRouterClient mcl;
  mcl.ConnectL(TRUE);
  TmRConfig cfg;
  cfg.iBearer=EmR_USB;
  mcl.MRConfigL(&cfg);
  iFile.Write(_L8("MROUTER USB CONFIG\n"));
  FieldValue(_L("bearer"),cfg.iBearer);
  FieldValue(_L("bearerunit"),cfg.iBearerUnit);
  FieldValue(_L("bearerspeed"),cfg.iBearerSpeed);
  
  TUSBListenStatus ls;
  mcl.USBListenStatusL(&ls);
  
  FieldValue(_L("\nusb listen status"),ls);
  
  TBTListenStatus bls;
  mcl.BTListenStatusL(&bls);

  FieldValue(_L("\nbt listen status"),bls);

  */

  cserv.Close();
  CleanupStack::PopAndDestroy(); //iFile
}


void CInfoDump::TestCommConfigL(const TDesC& FileName)
{
  FLOG(_L("CINfoDump::TestCommConfigL"));
  LogLeaveIfErrorL(iFile.Replace(Fs(), FileName, EFileWrite),"file replace");
  CleanupClosePushL(iFile);

  iFile.Write(_L8("1"));
  FLOG(_L("1"));

  RCommServ cserv;
  LogLeaveIfErrorL(cserv.Connect(),"cserv.connect");
  iFile.Write(_L8("2"));
  FLOG(_L("2"));

  LogLeaveIfErrorL(cserv.LoadCommModule(_L("btcomm")),"load btcomm module");
  iFile.Write(_L8("3"));
  FLOG(_L("3"));
  
  RComm port;
  LogLeaveIfErrorL(port.Open(cserv,_L("BTCOMM::5"),ECommExclusive),"open port");
  iFile.Write(_L8("4"));
  FLOG(_L("4"));

  TRequestStatus aSt;
  port.Write(aSt,10000000,_L8("~mRouter - Are you there?~"));
  iFile.Write(_L8("5"));
  FLOG(_L("5"));
  
  User::WaitForRequest(aSt);
  iFile.Write(_L8("6"));
  FieldValue(_L("Error"),aSt.Int());
  FPrint(_L("6  %d"),aSt.Int());
  TBuf8<200> lb;
  port.Read(aSt,20000000,lb);
  iFile.Write(_L8("7"));
  FLOG(_L("7"));
  User::WaitForRequest(aSt);
  iFile.Write(_L8("8 "));  
  FieldValue(_L("Error"),aSt.Int());
  FPrint(_L("8 %d"), aSt.Int());
  iFile.Write(lb);
  FLOG(_L("9"));
  port.Close();
  cserv.Close();
  FLOG(_L("Closed"));
  CleanupStack::PopAndDestroy(); //iFile
  FLOG(_L("finish"));
}


void CInfoDump::DumpProcInfoL(const TDesC& FileName)
{
  LogLeaveIfErrorL(iFile.Replace(Fs(), FileName, EFileWrite),"replace file");
  CleanupClosePushL(iFile);
  FLOG(_L("1"));

  TFindProcess fp;
  FLOG(_L("2"));

  TInt ret;
  TFullName pname;
  while (true) {
    ret=fp.Next(pname);
    if (ret==KErrNotFound) 
      break;
    LogLeaveIfErrorL(ret,"find next");
    iFile.Write(_L8("PROCESS"));
    FieldValue(_L("Name"),pname);
  }

  TFindThread ft;
  FLOG(_L("4"));
  
  while (true) {
    ret=ft.Next(pname);
    if (ret==KErrNotFound) 
      break;
    LogLeaveIfErrorL(ret,"find next");
    iFile.Write(_L8("THREAD"));
    FieldValue(_L("Name"),pname);
  }

  CleanupStack::PopAndDestroy(); //iFile

}

void CInfoDump::DumpAppInfoL(const TDesC& FileName)
{
  LogLeaveIfErrorL(iFile.Replace(Fs(), FileName, EFileWrite),"replace file");
  CleanupClosePushL(iFile);
  FLOG(_L("1"));
  RApaLsSession rs;
  LogLeaveIfErrorL(rs.Connect(),"APA srv connect");
  FLOG(_L("2"));

  LogLeaveIfErrorL(rs.GetAllApps(),"get all apps");
  FPrint(_L("3 %d"),sizeof(TApaAppInfo));

  CArrayFixFlat<TApaAppInfo> * apl = new CArrayFixFlat<TApaAppInfo>(5);
  CleanupStack::PushL(apl);
  FLOG(_L("3.5"));
  apl->SetReserveL(10);
  FLOG(_L("4"));
  while (true) {
    TApaAppInfo *caif;
    LogTrapLeaveIfErrorL(caif=&apl->ExtendL(),"extend base");
    FPrint(_L("4.5 0x%x"),caif);
    TInt ret = rs.GetNextApp(*caif);
    FPrint(_L("5 %d"),ret);
    if (ret==RApaLsSession::ENoMoreAppsInList) {
      break;
    } 
    LogLeaveIfErrorL(ret,"get next app");
  }
  FLOG(_L("6"));
  FieldValue(_L("App count"),apl->Count());
  FLOG(_L("7"));
  CArrayFixFlat<TSize> *ics  = new CArrayFixFlat<TSize>(2);
  CleanupStack::PushL(ics);
  for (TInt i=0;i<apl->Count();++i) {
    ics->Delete(0,ics->Count());
    FLOG(_L("8"));
    TApaAppInfo &caif=apl->At(i);
    FLOG(_L("9"));
    TInt err=rs.GetAppIconSizes(caif.iUid,*ics);
    FLOG(_L("a"));
    iFile.Write(_L8("APPLICATION\n\n"));
    FieldValue(_L("Caption"),caif.iCaption);
    FieldValue(_L("Full Name"),caif.iFullName);
    FieldValue(_L("UID"),caif.iUid);
    if (err!=KErrNone) {
      FieldValue(_L("Iconsize get error"),err);
    } else
      for(TInt j=0;j<ics->Count();++j) {
        FieldValue(_L("Icon Width"),ics->At(j).iWidth);
        FieldValue(_L("Icon Height"),ics->At(j).iHeight);
      }
    iFile.Write(_L8("\n\n"));
    FLOG(_L("b"));
  }

  FLOG(_L("c"));
  CleanupStack::PopAndDestroy(); //ics
  CleanupStack::PopAndDestroy(); //apl
  CleanupStack::PopAndDestroy(); //iFile
}
