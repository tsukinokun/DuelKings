//---------------------------------------------------------------------------
//!	@file	SkillObjectBase.h
//! @brief	スキルのオブジェクトの基底クラス
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//前方宣言
class ComponentEffect;
class Agent;
USING_PTR(SkillObjectBase);

class SkillObjectBase : public Object
{
public:
    BP_OBJECT_DECL(SkillObjectBase, u8"スキルのオブジェクトの基底クラス")

    //@
    bool Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // エフェクトを設定
    //! @param effect_name エフェクト名
    //! @return 自分自身のshared_ptr
    //---------------------------------------------------------------------------
    std::shared_ptr<SkillObjectBase> SetEffect(const std::string_view effect_name);

    //---------------------------------------------------------------------------
    // スキル使用者のオーナーエージェントを設定する
    //! @param  agent_ptr [in] スキル使用者のオーナーエージェントのポインタ
    //! @return 自分自身のshared_ptr
    //---------------------------------------------------------------------------
    std::shared_ptr<SkillObjectBase> SetSkillOwner(const std::shared_ptr<Agent> agent_ptr);

    //---------------------------------------------------------------------------
    // スキルの再生速度を設定する
    //! @param play_speed 再生速度
    //---------------------------------------------------------------------------
    std::shared_ptr<SkillObjectBase> SetEffectPlaySpeed(float play_speed);

protected:
    std::weak_ptr<ComponentEffect> effect_component_;    //!< エフェクトコンポーネント
    std::weak_ptr<Agent>           owner_agent_ptr_;     //!< スキル使用者の名前
};
