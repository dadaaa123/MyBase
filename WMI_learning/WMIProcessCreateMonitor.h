#pragma once

#include <tchar.h>
#include <Shlwapi.h>
#include <comdef.h>
#include <wbemidl.h>
#include <windows.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Shlwapi.lib")

int WMIProcessCreateMonitor() {
  HRESULT hRet = S_OK;

  // 初始化COM组件
  hRet = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  if (FAILED(hRet)) {
    cout << "初始化COM库组件失败。错误码：" << hRet << endl;
    return hRet;
  }

  IWbemLocator *pIWbemLocator = NULL;

  hRet = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
                          IID_IWbemLocator, (LPVOID *)&pIWbemLocator);
  if (FAILED(hRet)) {
    cout << "创建IWbemLocator对象失败！错误码：" << hRet << endl;
    CoUninitialize();
    return hRet;
  }

  IWbemServices *pIWbemServices = NULL;

  bstr_t strNetwoekResource("ROOT\\CIMV2");

  hRet = pIWbemLocator->ConnectServer(strNetwoekResource, NULL, NULL, NULL, 0,
                                      NULL, NULL, &pIWbemServices);
  if (FAILED(hRet)) {
    cout << "" << hRet << endl;
    pIWbemLocator->Release();
    CoUninitialize();
    return hRet;
  }

  hRet = CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
                           NULL, RPC_C_AUTHN_LEVEL_CALL,
                           RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
  if (FAILED(hRet)) {
    cout << "" << endl;
    pIWbemServices->Release();
    pIWbemLocator->Release();
    CoUninitialize();
    return hRet;
  }

  bstr_t strQueryLanguage("WQL");
  bstr_t strQuery(
      "SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA "
      "'Win32_Process'");

  //监控进程关闭
  //SELECT * FROM __InstanceDeletionEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process' 

  IEnumWbemClassObject *pIEnumWbemClassObject = NULL;

  hRet = pIWbemServices->ExecNotificationQuery(
      strQueryLanguage, strQuery,
      WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL,
      &pIEnumWbemClassObject);

  if (SUCCEEDED(hRet)) {
    do {
      ULONG uReturned = 0;
      IWbemClassObject *pIWbemClassObject = NULL;

      hRet = pIEnumWbemClassObject->Next(WBEM_INFINITE, 1, &pIWbemClassObject,
                                         &uReturned);

      if (SUCCEEDED(hRet) && pIWbemClassObject) {
        VARIANT vtInstanceObject;
        hRet = pIWbemClassObject->Get(L"TargetInstance", 0,
                                      &vtInstanceObject, NULL, NULL);

        if (SUCCEEDED(hRet) && vtInstanceObject.vt == VT_UNKNOWN &&
            vtInstanceObject.punkVal != NULL) {
          IWbemClassObject *pTargetInstance =
              (IWbemClassObject *)vtInstanceObject.punkVal;

          VARIANT vtProcessID, vtExecutablePath;

          // 获取进程ID
          hRet = pTargetInstance->Get(_T("ProcessID"), 0, &vtProcessID, NULL,
                                      NULL);
          if (SUCCEEDED(hRet)) {
            // 获取进程名
            hRet = pTargetInstance->Get(_T("Name"), 0, &vtExecutablePath, NULL,
                                        NULL);
            if (SUCCEEDED(hRet)) {
              wchar_t pName[MAX_PATH] = {0};
              wsprintf(pName, L"%s", vtExecutablePath.bstrVal);
              _wcsupr_s(pName, MAX_PATH);

              wcout << L"ProcessName:" << pName << L"  ProcessId:"
                    << vtProcessID.ulVal << " is started..." << endl;
            }
          }

          vtInstanceObject.punkVal->Release();
        }
      }

    } while (TRUE);
  }

  pIWbemServices->Release();
  pIWbemLocator->Release();
  CoUninitialize();

  return 0;
}