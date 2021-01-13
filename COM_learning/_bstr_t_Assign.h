#pragma once

// _bstr_t_Assign.cpp

#include <comdef.h>
#include <stdio.h>

int _bstr_t_Assign() {
  // creates a _bstr_t wrapper
  _bstr_t bstrWrapper;

  bstrWrapper.operator const wchar_t *
  // creates BSTR and attaches to it
  bstrWrapper = "some text";
  wprintf_s(L"bstrWrapper = %s\n", static_cast<wchar_t*>(bstrWrapper));

  // bstrWrapper releases its BSTR
  BSTR bstr = bstrWrapper.Detach();
  wprintf_s(L"bstrWrapper = %s\n", static_cast<wchar_t*>(bstrWrapper));
  // "some text"
  wprintf_s(L"bstr = %s\n", bstr);

  bstrWrapper.Attach(SysAllocString(OLESTR("SysAllocedString")));
  wprintf_s(L"bstrWrapper = %s\n", static_cast<wchar_t*>(bstrWrapper));

  // assign a BSTR to our _bstr_t
  bstrWrapper.Assign(bstr);
  wprintf_s(L"bstrWrapper = %s\n", static_cast<wchar_t*>(bstrWrapper));

  // done with BSTR, do manual cleanup
  SysFreeString(bstr);

  // resuse bstr
  bstr = SysAllocString(OLESTR("Yet another string"));
  // two wrappers, one BSTR
  _bstr_t bstrWrapper2 = bstrWrapper;

  *bstrWrapper.GetAddress() = bstr;

  // bstrWrapper and bstrWrapper2 do still point to BSTR
  bstr = 0;
  wprintf_s(L"bstrWrapper = %s\n", static_cast<wchar_t*>(bstrWrapper));
  wprintf_s(L"bstrWrapper2 = %s\n", static_cast<wchar_t*>(bstrWrapper2));

  // new value into BSTR
  _snwprintf_s(bstrWrapper.GetBSTR(), 100, bstrWrapper.length(),
               L"changing BSTR");
  wprintf_s(L"bstrWrapper = %s\n", static_cast<wchar_t*>(bstrWrapper));
  wprintf_s(L"bstrWrapper2 = %s\n", static_cast<wchar_t*>(bstrWrapper2));

  return 0;
}
