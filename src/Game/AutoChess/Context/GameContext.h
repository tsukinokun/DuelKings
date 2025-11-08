//---------------------------------------------------------------------------
//!	@file	GameContext.h
//! @brief	マスターデータ群
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <Game/AutoChess/Piece/PieceData/PieceRepository.h>
#include <Game/AutoChess/Synergy/SynergyRepository.h>
class GameContext
{
public:
    //--------------------------------------------------
    // マスターデータを読み込む
    //! @param piece_path 駒定義JSONのパス
    //! @param synergy_path シナジー定義JSONのパス
    //! @return 成功時 true、失敗時 false
    //--------------------------------------------------
    bool LoadRepositories(const std::string& piece_path, const std::string& synergy_path);

    //--------------------------------------------------
    // 駒定義リポジトリへの参照を取得
    //--------------------------------------------------
    const PieceRepository& GetPieceRepository() const { return piece_repo_; }

private:
    PieceRepository   piece_repo_;      //!< 駒定義リポジトリ
    SynergyRepository synergy_repo_;    //!< シナジー定義リポジトリ
};
