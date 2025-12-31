//---------------------------------------------------------------------------
//!	@file	ComponentActiveSkill.h
//! @brief	アクティブスキルのベースコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/SkillData/SkillData.h>
USING_PTR(ComponentActiveSkill);

class ComponentActiveSkill : public Component
{
public:
    BP_COMPONENT_DECL(ComponentActiveSkill, u8"アクティブスキルのベースコンポーネントクラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------
    //  スキル発動可能かを返す関数
    //!  @return スキル発動可能ならtrue、不可ならfalse
    //---------------------------------------------------------
    bool CanActivate() const;

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    virtual void Activate();

    //---------------------------------------------------------
    // MPを増加させる関数
    //! @param amount [in] 増加させるMP量
    //---------------------------------------------------------
    void AddMP(int amount);

    //---------------------------------------------------------
    // MPを減少させる関数
    //! @param amount [in] 減少させるMP量
    //---------------------------------------------------------
    void RemoveMP(int amount);

    //---------------------------------------------------------
    // MPを取得する関数
    //! @retval 現在のスキルMP
    //---------------------------------------------------------
    int GetMP() const;

    //----------------------------------------------------------
    // スキル名を取得する関数
    //! @retval スキル名
    //----------------------------------------------------------
    const std::string& GetSkillName();

    //----------------------------------------------------------
    // スキルの説明文を取得する関数
    //! @retval スキル説明文
    //----------------------------------------------------------
    const std::string& GetSkillDescription();

    //----------------------------------------------------------
    // スキルアイコンのキーを取得する関数
    //! @retval スキルアイコンのキー
    //----------------------------------------------------------
    const std::string& GetSkillIconKey();

    //----------------------------------------------------------
    // 参照するマスターデータを設定する関数
    //! @param master [in] 参照マスターデータ
    //----------------------------------------------------------
    void SetMasterData(const SkillData* master);

protected:
    SkillData skill_data_;               //!< スキルデータ
    int       mp_             = 0;       //!< スキルMP
    float     cool_doen_time_ = 0.0f;    //!< クールダウン時間
};

CEREAL_REGISTER_TYPE(ComponentActiveSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentActiveSkill)
