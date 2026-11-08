// btdefcommport.h
//
// Copyright (c) 1999-2005 Symbian Ltd.  All rights reserved.
//

#ifndef BTDEFCOMMPORT_H__
#define BTDEFCOMMPORT_H__

#include <e32std.h>
#include <e32base.h>
#include <bttypes.h>
#include <s32mem.h>
#include <btmanclient.h>

// security levels for the default service
static const TUint8 KBTDefaultServiceSecurityNone                   = 0;
static const TUint8 KBTDefaultServiceSecurityAuthenticateOnly       = 1;
static const TUint8 KBTDefaultServiceSecurityAuthenticateAndEncrypt = 2;
static const TUint32 KCommHighUnit=63;
/**
	Wraps up a Service TUUID and a port number.
	This info can then be saved by the registry and retrieved when needed.

**/
class TBTCommPortSettings
	{
public:
    IMPORT_C TBTCommPortSettings(const TDesC& aName, TUUID aUUID,TUint32 aBTCommPort, TUint8 aSecurityLevel, TBTDevAddr aBDAddr);
    IMPORT_C TBTCommPortSettings();
    IMPORT_C const TDesC& Name() const;	///< Returns the name of the service.
    IMPORT_C TUUID UUID() const;			///< Returns the unique id of the service.
    IMPORT_C TBTDevAddr BDAddr() const;		///< Returns the address of the default device associated with the service.
    IMPORT_C TUint32 Port() const;
    IMPORT_C TBool IsSecuritySet() const;
    IMPORT_C TBool IsEncryptionSet() const;
    IMPORT_C TBool IsAuthenticationOnlySet() const;
    IMPORT_C void SetName(const TDesC& aName);	///< Sets the name of the service.
    IMPORT_C void SetUUID(TUUID aUUID);			///< Sets the unique id of the service.
    IMPORT_C void SetPort(TUint32 aBTCommPort);
    IMPORT_C void SetAuthentication();
    IMPORT_C void SetAuthenticationAndEncryption(); // enforces authentication as well
    IMPORT_C void SetNoSecurity(); // resets the above
    IMPORT_C void SetBTAddr(TBTDevAddr aBDAddr);	///< Sets the address of the default device associated with the service.

	 TUint8 SecurityLevel() const; //for use by internal clients
	 void SetSecurityLevel(TUint8 aLevel);  //for use by internal clients
private:
	TBuf<32>	iName;		    // The name of the service   ..FIXME the buf size is there a constant in commdb ?
	TUUID		iUUID;			// The UUID of the service
	TUint32		iPort;          // The port no of the virtual serial port opened by the legacy app
	TUint8		iSecurityLevel; // for that default service (i.e 0,1,2 = none,auth,encrypt+auth)
	TBTDevAddr	iBDAddr;	    // The address of the default device
	};

typedef TPckgBuf<TBTCommPortSettings> TBTCommPortSettingsPckg;
class RBTCommPortSettings : public RBTManSubSession
/**
    Subsession on RegistryServer
    This subsession allows the virtual serial ports to be updated, modified, examined etc.
 **/
    {
    public:
    IMPORT_C RBTCommPortSettings();
    IMPORT_C TInt Open(RBTRegServ& aSession);   // creates the subsession server side
    IMPORT_C TInt Get(TBTCommPortSettings& aSettings);  //synchronous
    IMPORT_C void Modify(const TBTCommPortSettings& aSettings, TRequestStatus& aStatus); // may add if not present
    IMPORT_C void Delete(const TBTCommPortSettings& aSettings, TRequestStatus& aStatus);
    IMPORT_C void Close();
private:
    TPckgBuf<TBTCommPortSettings>   iPortPckg;
    };

#endif
