#ifndef ROUTEDUMP_H_INCLUDED
#define ROUTEDUMP_H_INCLUDED 1

#include <es_sock.h>
#include <f32file.h>
#include <charconv.h>
#include <in_sock.h>

typedef struct TCapsMaskEntry {
  TUint32 iMask;
  const TText* iName;
} TCapsMaskEntry;


class CInfoDump : public CBase {
 public:
  ~CInfoDump();
  void DumpInfotoFileL(const TDesC& FileName);
  void DumpSerInfotoFileL(const TDesC& FileName);
  void TestCommConfigL(const TDesC& FileName);
  void DumpCPrefInfotoFileL(const TDesC& FileName);
  void DumpAppInfoL(const TDesC& FileName);
  void DumpProcInfoL(const TDesC& FileName);
  CInfoDump();
  void ConstructL();
  void AddMRouterDefaultRouteL(const TDesC& FileName);

 protected:
  void PushRoute();
  void BeginTable(const TDesC& name);
  void BeginRow();
  void FieldValue(const TDesC& name, const TDesC& value);
  void FieldValue(const TDesC& name, const TInetAddr& addr);
  void FieldValue(const TDesC& name, const TInt i);
  void FieldValue(const TDesC& name, const TUid uid);
  void CapsValue(const TDesC &name, const TInt val, const TCapsMaskEntry* skel);
  void DumpInterface(TSoInetInterfaceInfo &cif);
  void DumpRoute(TSoInetRouteInfo &rif);
  void EndRow();
  void EndTable();

 private:
  RSocketServ rs;

  TInt state;
  RFs	iFs;
  RFs&	Fs() { return iFs; }
  RFile	iFile;
  TBuf<2048> iValue;
  TBuf8<2048> iBuf;
  CCnvCharacterSetConverter* iCC;
  CCnvCharacterSetConverter& CC() { return *iCC; }

};

#endif
