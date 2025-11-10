//---------------------------------------------------------------------------
//!	@file	SynergyRepository.h
//! @brief	シナジーのマスターデータ情報のリポジトリクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include "SynergyData.h"
#include <Game/AutoChess/Synergy/SynergyID.h>
class SynergyRepository
{
public:
    //--------------------------------------------------
    // JSONファイルからシナジー定義を読み込む
    //! @param path JSONファイルのパス
    //! @return 成功時 true、失敗時 false
    //--------------------------------------------------
    bool LoadFromJson(const std::string& path);

    //--------------------------------------------------
    // シナジーIDで定義を検索
    //! @param id シナジーの内部識別子（例: "Shogi"）
    //! @return 該当する SynergyData（存在しない場合は nullptr）
    //--------------------------------------------------
    const SynergyData* FindByID(const SynergyID& id) const;

    //--------------------------------------------------
    //! @brief 全シナジー定義を取得（UI表示などに使用）
    //--------------------------------------------------
    const std::vector<SynergyData>& GetAllSynergies() const { return synergies_; }

private:
    std::vector<SynergyData> synergies_;    //!< シナジー定義の一覧
};
