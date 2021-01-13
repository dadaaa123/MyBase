// MonitorAdapter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MonitorAdap.h"
 
int main() {
  MonitorAdapter m_monitorAdapter;  //��ʾ��
  VEC_MONITORMODE_INFO vecMointorListInfo;
  m_monitorAdapter.GetAllDisplayMode(vecMointorListInfo);

  int nWidth = 0, nHeight = 0, nFreq = 0, nBits = 0;
  VEC_MONITORMODE_INFO::iterator itBeg = vecMointorListInfo.begin();
  for (int i = 0; i < vecMointorListInfo.size(); i++) {
    //�õ���ǰ��ʾ���ֱ��� ˢ���� ɫλ
    m_monitorAdapter.GetCurrentReselotion(itBeg->szDevice, nWidth, nHeight,
                                          nFreq, nBits);
    itBeg++;
    nWidth = 0, nHeight = 0, nFreq = 0, nBits = 0;
  }

  return 0;
}