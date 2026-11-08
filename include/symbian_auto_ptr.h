/*
 * Copyright (c) 1997-1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/*
 * Adapted from the SGI auto_ptr implementation by Sander van der wal, mBrain Software.
 *
 * A number of declarations have been commented out because
 * - they don't work for the GCC2.7.2 compiler, as used for Release 5
 * - I don't know exactly what they are doing
 *
 * Fortunately the most important things are there.
 *
 */

#ifndef __AUTO_PTR_H__
#define __AUTO_PTR_H__  


#ifndef __E32BASE_H__
#include <e32base.h>
#endif


template<class Y> struct auto_ptr_ref
{
  Y* iPtr;
  auto_ptr_ref(Y* aPtr) : iPtr(aPtr) {}
};


template<class X> class auto_ptr
{
public:  
  typedef X element_type;

  auto_ptr(X* aPtr = 0): iPtr(aPtr)
  {
    CleanupStack::PushL(TCleanupItem(Close, (void*)this));
  }  
  
  auto_ptr(auto_ptr& aPtr): iPtr(aPtr.release())
  {
    CleanupStack::PushL(TCleanupItem(Close, (void*)this));
  }

  // MS VC++ 6.0 thinks this is identical to the one above
  // GCC 2.7.2 (for ER5) hates this declaration
  // GCC 2.9-psion-98r2 (V6 Nokia 9200 Series) think this is ok
  //template <class Y> auto_ptr(auto_ptr<Y>& aPtr): iPtr(aPtr.release())
  //{
  //  CleanupStack::PushL(TCleanupItem(Close, (void*)this));
  //}
  
  auto_ptr<X>& operator=(auto_ptr<X>& aRhs)
  {
    if (&aRhs != this)
    {
      delete iPtr;
      iPtr = aRhs.release();
    }
    return (*this); 
  }

  // MS VC++ 6.0 thinks this is identical to the one above
  // GCC 2.7.2 (for ER5) hates this declaration
  // GCC 2.9-psion-98r2 (V6 Nokia 9200 Series) think this is ok
  //template <class Y> auto_ptr& operator=(auto_ptr<Y>& aRhs)
  //{
  //  if (aRhs.get() != this->get()) {
  //    delete iPtr;
  //    iPtr = aRhs.release();
  //  }
  //  return *this;
  //}

  ~auto_ptr() 
  { 
    CleanupStack::Pop();
    delete iPtr;
  }

  X& operator *() const { return *iPtr; }
  X* operator ->() const { return iPtr; }
   
  X* get() const { return iPtr; }

  X* release()
 	{ 
    X* result = iPtr;
    iPtr = 0;
		return result; 
  }

  void reset(X* aPtr = 0) {
    if (aPtr != iPtr) {
      delete iPtr;
      iPtr = aPtr;
    }
  }


  //auto_ptr(auto_ptr_ref<X> aRef): iPtr(aRef.iPtr)
  //{
  //  CleanupStack::PushL(TCleanupItem(Close, (void*) this));
  //}

  //auto_ptr& operator=(auto_ptr_ref<X> aRef)
  //{
  //  if (aRef.iPtr != this->get()) {
  //    delete iPtr;
  //    iPtr = aRef.iPtr;
  //  }
  //  return *this;
  //}

  // MS VC++ 6.0 thinks this is ok
  // GCC 2.7.2 (for ER5) hates this declaration
  // GCC 2.9-psion-98r2 (V6 Nokia 9200 Series) thinks this is ok
  //template <class Y> operator auto_ptr_ref<Y>() 
  //  { return auto_ptr_ref<Y>(this->release()); }
  
  // MS VC++ 6.0 thinks this is ok
  // GCC 2.7.2 (for ER5) hates this declaration
  // GCC 2.9-psion-98r2 (V6 Nokia 9200 Series) think this is ok
  //template <class Y> operator auto_ptr<Y>()
  //  { return auto_ptr<Y>(this->release()); }

private:
  static void Close(void* aPtr)
  { 
    auto_ptr<X>* self = (auto_ptr<X>*)aPtr;
    delete self->iPtr;
  }

private:
  X* iPtr;  
};

#endif // __AUTO_PTR_H__
