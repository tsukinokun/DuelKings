//---------------------------------------------------------------------------
//!	@file	PieceRepository.h
//! @brief	ピースのマスターデータ情報のリポジトリクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include "PieceData.h"
class PieceRepository
{
public:
    //--------------------------------------------------
    //! @brief JSONファイルから駒定義を読み込む
    //! @param path JSONファイルのパス
    //! @return 成功時 true、失敗時 false
    //--------------------------------------------------
    bool LoadFromJson(const std::string& path);

    //--------------------------------------------------
    //! @brief 駒の種類名（type_name）で定義を検索
    //! @param type_name 駒の内部識別子（例: "ShogiKnight"）
    //! @return 該当する PieceData（存在しない場合は nullptr）
    //--------------------------------------------------
    const PieceData* FindByTypeName(const std::string& type_name) const;

    //--------------------------------------------------
    //! @brief 全駒定義を取得（UI表示などに使用）
    //--------------------------------------------------
    const std::vector<PieceData>& GetAllPieces() const { return piece_datas_; }

private:
    std::vector<PieceData> piece_datas_;    //!< 駒定義の一覧
};
