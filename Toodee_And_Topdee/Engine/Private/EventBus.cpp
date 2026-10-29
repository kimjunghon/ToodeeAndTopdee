#include "EventBus.h"

CEventBus::CEventBus()
{
}

HRESULT CEventBus::Initialize(_uint iNumLevels)
{
    m_iNumLevels = iNumLevels;
    
    m_Subscribers = new unordered_map<EVENT_KEY, vector<Callback>>[iNumLevels];
    return S_OK;
}

void CEventBus::Clear(_uint iClearLevel)
{
    if (iClearLevel >= m_iNumLevels)
        return;

    m_Subscribers[iClearLevel].clear();
}

CEventBus* CEventBus::Create(_uint iNumLevels)
{
    CEventBus* pInstance = new CEventBus();

    if (FAILED(pInstance->Initialize(iNumLevels)))
    {
        MSG_BOX(TEXT("Failed Created : CEventBus "));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CEventBus::Free()
{
    __super::Free();

    Safe_Delete_Array(m_Subscribers);
}
