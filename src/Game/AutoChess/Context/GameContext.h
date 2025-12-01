//---------------------------------------------------------------------------
//!	@file	GameContext.h
//! @brief	マスターデータ群
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <Game/AutoChess/Piece/PieceData/PieceRepository.h>
#include <Game/AutoChess/Synergy/SynergyRepository.h>
#include <Game/AutoChess/Component/SkillComponent/SkillData/SkillRepository.h>
class GameContext
{
public:
    //--------------------------------------------------
    // マスターデータを読み込む
    //! @param piece_path 駒定義JSONのパス
    //! @param synergy_path シナジー定義JSONのパス
    //! @param skill_path スキル定義JSONのパス
    //! @return 成功時 true、失敗時 false
    //--------------------------------------------------
    bool LoadRepositories(const std::string& piece_path, const std::string& synergy_path, const std::string& skill_path);

    //--------------------------------------------------
    // 駒定義リポジトリへの参照を取得
    //! @return 駒定義リポジトリへの参照
    //--------------------------------------------------
    const PieceRepository& GetPieceRepository() const;

    //--------------------------------------------------
    // シナジー定義リポジトリへの参照を取得
    //! @return シナジー定義リポジトリへの参照
    //--------------------------------------------------
    const SynergyRepository& GetSynergyRepository() const;

    //--------------------------------------------------
    // スキル定義リポジトリへの参照を取得
    //! @return スキル定義リポジトリへの参照
    //--------------------------------------------------
    const SkillRepository& GetSkillRepository() const;

private:
    PieceRepository   piece_repo_;      //!< 駒定義リポジトリ
    SynergyRepository synergy_repo_;    //!< シナジー定義リポジトリ
    SkillRepository   skill_repo_;      //!< スキル定義リポジトリ
};
