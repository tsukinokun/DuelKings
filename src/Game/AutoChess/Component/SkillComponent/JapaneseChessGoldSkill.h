//---------------------------------------------------------------------------
//!	@file	JapaneseChessGoldSkill.h
//! @brief	金将のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(JapaneseChessGoldSkill);
class Piece;    // 前方宣言
class JapaneseChessGoldSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(JapaneseChessGoldSkill, u8"金将のスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    JapaneseChessGoldSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    //---------------------------------------------------------
    // スキルオブジェクト生成処理
    //! @param position [in] 生成位置
    //! @param owner    [in] 所有者ピース
    //---------------------------------------------------------
    void CreateSkillObject(const float3& position, std::shared_ptr<Piece> owner);

private:
    const std::array<int, 3> DAMAGE_VALUES_ = {200, 250, 300};    //ダメージ量
};

CEREAL_REGISTER_TYPE(JapaneseChessGoldSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JapaneseChessGoldSkill)
