//---------------------------------------------------------------------------
//!	@file	GameContext.cpp
//! @brief	マスターデータ群
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "GameContext.h"
//------------------------------------------------------
//! @brief 駒・シナジーのマスターデータを読み込む
//------------------------------------------------------
bool GameContext::LoadRepositories(const std::string& piece_path, const std::string& synergy_path, const std::string& skill_path)
{
    bool piece_loaded   = piece_repo_.LoadFromJson(piece_path);
    bool synergy_loaded = synergy_repo_.LoadFromJson(synergy_path);
    bool skill_loaded   = skill_repo_.LoadFromJson(skill_path);
    return piece_loaded && synergy_loaded && skill_loaded;
}

//--------------------------------------------------
//! @brief 駒定義リポジトリへの参照を取得
//--------------------------------------------------
const PieceRepository& GameContext::GetPieceRepository() const
{
    return piece_repo_;
}

//--------------------------------------------------
//! @brief シナジー定義リポジトリへの参照を取得
//--------------------------------------------------
const SynergyRepository& GameContext::GetSynergyRepository() const
{
    return synergy_repo_;
}

//--------------------------------------------------
//! @brief スキル定義リポジトリへの参照を取得
//--------------------------------------------------
const SkillRepository& GameContext::GetSkillRepository() const
{
    return skill_repo_;
}
