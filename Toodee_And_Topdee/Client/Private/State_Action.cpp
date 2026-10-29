#include "State_Action.h"

CState_Action::CState_Action()
{
}

HRESULT CState_Action::Initialize(void* pArg)
{
    PLAYERSTATE_DESC* pDesc = static_cast<PLAYERSTATE_DESC*>(pArg);
    if (pDesc == nullptr)
        return E_FAIL;

    m_eState = pDesc->eState;

    m_iMaxAnimCount = pDesc->iMaxAnimCount;
    m_fAnimDelay = 0.02f;

    return S_OK;
}

void CState_Action::Enter(CPlayer* pPlayer)
{
    m_iCurrentAnimCount = 0;
    m_fAnimTime = 0.f;
}

void CState_Action::HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta)
{
    if(iInputData & ENUM_CLASS(KEYINPUT::KEY_Z))
    {
        pPlayer->Action();
    }

    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        if(pPlayer->CanMoveInAction())
            pPlayer->Move(iInputData, fTimeDelta);

        if (!pPlayer->InAction())
            if (FAILED(pPlayer->Change_State(PLAYERSTATE::MOVE)))
                MSG_BOX(TEXT("Failed Change State : MOVE"));
    }

    if (iInputData == 0)
    {
        if (!pPlayer->InAction())
        {
            pPlayer->Idle();
            if (FAILED(pPlayer->Change_State(PLAYERSTATE::IDLE)))
                MSG_BOX(TEXT("Failed Change State : IDLE"));
        }
    }

}

void CState_Action::Update(CPlayer* pPlayer, _float fTimeDelta)
{
    if (!pPlayer->InAction())
    {
        pPlayer->Idle();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::IDLE)))
            MSG_BOX(TEXT("Failed Change State : IDLE"));
    }

    Check_GlobalTransition(pPlayer);
}

void CState_Action::UpdateAnim(_float fTimeDelta)
{
    Update_OnceAnim(fTimeDelta);
}

void CState_Action::Exit(CPlayer* pPlayer)
{
}


CState_Action* CState_Action::Create(void* pArg)
{
    CState_Action* pInstance = new CState_Action();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Created Failed : CState_Action"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Action::Free()
{
    __super::Free();
}

