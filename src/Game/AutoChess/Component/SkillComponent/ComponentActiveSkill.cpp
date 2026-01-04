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
    mp_             = 0;                                  // 発動したらMPをリセット
    cool_doen_time_ = skill_data_.max_cool_doen_time_;    // 発動したらクールダウンをリセット
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
//! @brief MPを減少させる関数
//---------------------------------------------------------
void ComponentActiveSkill::RemoveMP(int amount)
{
    mp_ -= amount;
    if(mp_ < 0) {
        mp_ = 0;    // MPの下限は0
    }
}

//---------------------------------------------------------
//! @brief MPを取得する関数
//---------------------------------------------------------
int ComponentActiveSkill::GetMP() const
{
    return mp_;
}

//----------------------------------------------------------
//! @brief スキル名を取得する関数
//----------------------------------------------------------
const std::string& ComponentActiveSkill::GetSkillName()
{
    return skill_data_.skill_name_;
}

//----------------------------------------------------------
//! @brief スキルの説明文を取得する関数
//----------------------------------------------------------
const std::string& ComponentActiveSkill::GetSkillDescription()
{
    return skill_data_.skill_description_;
}

//----------------------------------------------------------
//! @brief スキルアイコンのキーを取得する関数
//----------------------------------------------------------
const std::string& ComponentActiveSkill::GetSkillIconKey()
{
    return skill_data_.skill_icon_key_;
}

//----------------------------------------------------------
//! @brief 参照するマスターデータを設定する関数
//----------------------------------------------------------
void ComponentActiveSkill::SetMasterData(const SkillData* master)
{
    // マスターデータが存在する場合、スキルデータをコピーする
    if(master) {
        skill_data_ = *master;
    }
}

//----------------------------------------------------------
//! @brief マスターデータを返す関数
//----------------------------------------------------------
const SkillData& ComponentActiveSkill::GetMasterData() const
{
    return skill_data_;
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
