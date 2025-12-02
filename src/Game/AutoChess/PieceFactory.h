//---------------------------------------------------------------------------
//!	@file	PieceFactory.h
//! @brief	駒生成クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/AutoChess/Piece/PieceData/PieceRepository.h>
#include <Game/AutoChess/Component/SkillComponent/SkillData/SkillRepository.h>
//前方宣言
class Piece;
class PieceFactory
{
public:
    //---------------------------------------------------------------------------
    // ピース名を受け取って対応する駒インスタンスを生成する
    //! @param [in] type 駒の種類名（文字列）
    //! @param [in] 駒のレベル
    //! @return 生成された駒のインスタンス（nullptr の場合は未定義）
    //---------------------------------------------------------------------------
    static std::shared_ptr<Piece> CreatePiece(const std::string_view& type, int piece_level = 1);

    //---------------------------------------------------------------------------
    // 駒定義リポジトリを設定する関数
    //! @param repo [in] 駒定義リポジトリへの参照
    //---------------------------------------------------------------------------
    static void SetPieceRepository(const PieceRepository* repo);

    //---------------------------------------------------------------------------
    // スキル定義リポジトリを取得する関数
    //! @param repo [in] スキル定義リポジトリへの参照
    //---------------------------------------------------------------------------
    static void SetSkillRepository(const SkillRepository* repo);

private:
    using PieceCreator = std::function<std::shared_ptr<Piece>()>;                       // 駒生成関数の型定義
    static const std::unordered_map<std::string_view, PieceCreator> piece_creators_;    // 駒生成関数のマップ
    const static PieceRepository*                                   piece_repo_;        //!< 駒定義リポジトリ
    const static SkillRepository*                                   skill_repo_;        //!< スキル定義リポジトリ
};
