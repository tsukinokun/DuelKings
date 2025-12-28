//---------------------------------------------------------------------------
//!	@file	SkillObjectBase.cpp
//! @brief	スキルのオブジェクトの基底クラス
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "SkillObjectBase.h"
#include <System/Component/ComponentEffect.h>

//---------------------------------------------------------------------------------
//!	@brief 初期化
//---------------------------------------------------------------------------------
bool SkillObjectBase::Init()
{
    __super::Init();

    SetName(u8"Skill");

    //---------------------------------------------------------------------------------
    // エフェクトコンポーネント
    //---------------------------------------------------------------------------------
    effect_component_ = AddComponent<ComponentEffect>();

    //---------------------------------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------------------------------
    {
        auto update = [this]() {
            if(auto eff = effect_component_.lock()) {
                if(eff->IsPlaying())
                    return;
            }

            // Effect終了で自分ごと消去
            Scene::Object::Release(SharedThis());
        };
        SetProc("update", update, ProcTiming::Update, ProcPriority::NORMAL);
    }
    return true;
}

//---------------------------------------------------------------------------
//!	@brief エフェクトを設定
//---------------------------------------------------------------------------
std::shared_ptr<SkillObjectBase> SkillObjectBase::SetEffect(const std::string_view effect_name, bool is_loop)
{
    if(auto eff = effect_component_.lock()) {
        eff->Load(effect_name);
        eff->Play(is_loop);    // ロードと同時にエフェクトを再生
    }
    return dynamic_pointer_cast<SkillObjectBase>(shared_from_this());
}

//---------------------------------------------------------------------------
//!	@brief スキル使用者の名前を設定する
//---------------------------------------------------------------------------
std::shared_ptr<SkillObjectBase> SkillObjectBase::SetSkillOwner(const std::shared_ptr<Agent> agent_ptr)
{
    owner_agent_ptr_ = agent_ptr;
    return dynamic_pointer_cast<SkillObjectBase>(shared_from_this());
}

//---------------------------------------------------------------------------
//!	@brief スキルの再生速度を設定する
//---------------------------------------------------------------------------
std::shared_ptr<SkillObjectBase> SkillObjectBase::SetEffectPlaySpeed(float play_speed)
{
    if(auto eff = effect_component_.lock()) {
        eff->SetPlaySpeed(play_speed);
    }
    return dynamic_pointer_cast<SkillObjectBase>(shared_from_this());
}
