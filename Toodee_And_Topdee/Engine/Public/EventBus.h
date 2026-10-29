#pragma once
#include "Base.h"
#include "Event.h"

BEGIN(Engine)

class CEventBus final : public CBase
{
public:
    typedef function<void(const CEvent&)> Callback;

private:
    CEventBus();
    virtual ~CEventBus() = default;

public:
    HRESULT Initialize(_uint iNumLevels);

public:
    template <typename EventType>
    void Subscribe(_uint iSubscribeLevel,EVENT_KEY eEventKey, function<void(const EventType&)> Callback) {
        if (iSubscribeLevel >= m_iNumLevels)
            return;

        auto Wrapper = [Callback = move(Callback)](const CEvent& Event) {
            Callback(static_cast<const EventType&>(Event));
            };
        m_Subscribers[iSubscribeLevel][eEventKey].push_back(move(Wrapper));
    }

    void Publish(_uint iSubscribeLevel, EVENT_KEY eEventKey, const CEvent& Event) {
        if (iSubscribeLevel >= m_iNumLevels)
            return;

        auto iter = m_Subscribers[iSubscribeLevel].find(eEventKey);
        if (iter == m_Subscribers[iSubscribeLevel].end())
            return;

        for (auto CallBack : iter->second) {
            CallBack(Event);
        }
    }

    void Clear(_uint iClearLevel);

private:
    _uint									    m_iNumLevels = {};
    unordered_map<EVENT_KEY, vector<Callback>>* m_Subscribers = { nullptr };

public:
    static CEventBus* Create(_uint iNumLevels);
    virtual void Free() override;
};

END