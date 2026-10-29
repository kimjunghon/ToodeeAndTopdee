#include "PlayerState.h"

CPlayerState::CPlayerState()
{
}


void CPlayerState::Free()
{
	__super::Free();
}

void CPlayerState::Request_ChangeState(CPlayer* pPlayer, PLAYERSTATE eState)
{
	if (eState == m_eState)
		return;

	pPlayer->Change_State(eState);
}

// Clear / Stop 은 어떤 상태에서든 넘어갈 수 있는 공통 전이 (Clear 우선)
_bool CPlayerState::Check_GlobalTransition(CPlayer* pPlayer)
{
	if (pPlayer->CanClear())
	{
		if (FAILED(pPlayer->Change_State(PLAYERSTATE::CLEAR)))
			MSG_BOX(TEXT("Failed Change State : CLEAR"));
		return true;
	}

	if (pPlayer->CanActive() == false)
	{
		if (FAILED(pPlayer->Change_State(PLAYERSTATE::STOP)))
			MSG_BOX(TEXT("Failed Change State : STOP"));
		return true;
	}

	return false;
}

// 반복 재생 애니메이션 (Idle, Move, Swim)
void CPlayerState::Update_LoopAnim(_float fTimeDelta)
{
	if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
	{
		m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount;
		m_fAnimTime = 0.f;
	}
	else
		m_fAnimTime += fTimeDelta;
}

// 한 번만 재생하고 마지막 프레임에서 멈추는 애니메이션 (Action, Clear, Dead)
void CPlayerState::Update_OnceAnim(_float fTimeDelta)
{
	if (m_iCurrentAnimCount >= m_iMaxAnimCount - 1)
		return;

	if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
	{
		m_iCurrentAnimCount++;
		m_fAnimTime = 0.f;
	}
	else
		m_fAnimTime += fTimeDelta;
}
