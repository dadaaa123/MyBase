#pragma once
#include <windows.h>

/* string consisting of several Asian characters */
//const wchar_t* wcsString = L"\u9580\u961c\u9640\u963f\u963b\u9644";
const wchar_t* wcsString = L"ÖÐ¹ú I Love China";
// LPTSTR wcsString = L"OnlyAsciiCharacters";

char* encode(const wchar_t* wstr, unsigned int codePage) {
  int sizeNeeded =
      WideCharToMultiByte(codePage, 0, wstr, -1, NULL, 0, NULL, NULL);
  char* encodedStr = new char[sizeNeeded];
  WideCharToMultiByte(codePage, 0, wstr, -1, encodedStr, sizeNeeded, NULL,
                      NULL);
  return encodedStr;
}

wchar_t* decode(const char* encodedStr, unsigned int codePage) {
  int sizeNeeded = MultiByteToWideChar(codePage, 0, encodedStr, -1, NULL, 0);
  wchar_t* decodedStr = new wchar_t[sizeNeeded];
  MultiByteToWideChar(codePage, 0, encodedStr, -1, decodedStr, sizeNeeded);
  return decodedStr;
}

void code_convert() {
  char* str = encode(wcsString, CP_UTF8);  // UTF-8 encoding
  wchar_t* wstr = decode(str, CP_UTF8);
  // If the wcsString is UTF-8 encodable, then this comparison will result to
  // true. (As i remember some of the Chinese dialects cannot be UTF-8 encoded
  bool ok = memcmp(wstr, wcsString, sizeof(wchar_t) * wcslen(wcsString)) == 0;
  delete str;
  delete wstr;

  str = encode(wcsString, 20127);  // US-ASCII (7-bit) encoding
  wstr = decode(str, 20127);
  // If there were non-ascii characters existing on wcsString,
  // we cannot return back, since some of the data is lost
  ok = memcmp(wstr, wcsString, sizeof(wchar_t) * wcslen(wcsString)) == 0;
  delete str;
  delete wstr;
}