//---------------------------------------------------------------------------
//!	@file	ComponentActiveSkill.cpp
//! @brief	スキルのベースコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ComponentActiveSkill::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // クールダウンを現象させる処理
    //---------------------------------------------------------
    auto time_proc = [this]() {
        // クールダウン時間を減少させる
        if(cool_doen_time_ > 0.0f) {
            cool_doen_time_ -= GetDeltaTime();
        }
    };
    SetProc("cooldown_time", time_proc, ProcTiming::Update, ProcPriority::NORMAL);
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ComponentActiveSkill::Activate()
{
    mp_             = 0;       // 発動したらMPをリセット
    cool_doen_time_ = 0.0f;    // 発動したらクールダウンをリセット
}

//---------------------------------------------------------
//! @brief MPを増加させる関数
//---------------------------------------------------------
void ComponentActiveSkill::AddMP(int amount)
{
    mp_ += amount;
    if(mp_ > 100) {
        mp_ = 100;    // MPの上限は100
    }
}

//---------------------------------------------------------
//! @brief MPを取得する関数
//---------------------------------------------------------
int ComponentActiveSkill::GetMP() const
{
    return mp_;
}

//---------------------------------------------------------
//  スキル発動可能かを返す関数
//!  @return スキル発動可能ならtrue、不可ならfalse
//---------------------------------------------------------
bool ComponentActiveSkill::CanActivate() const
{
    // スキルMPが100以上かつクールダウン時間が0以下なら発動可能
    return (mp_ >= 100) && (cool_doen_time_ <= 0.0f);
}
