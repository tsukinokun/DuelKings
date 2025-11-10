//---------------------------------------------------------------------------
//!	@file	SynergySystem.cpp
//! @brief	シナジーのシステムクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "SynergySystem.h"
//--------------------------------------------------
//! @brief デフォルトコンストラクタ
//--------------------------------------------------
SynergySystem::SynergySystem()
{
    synergy_repository_ = nullptr;
    piece_repository_   = nullptr;
}

//--------------------------------------------------
//! @brief リポジトリのポインタを設定
//! @param repository シナジーリポジトリへのポインタ
//--------------------------------------------------
void SynergySystem::SetRepository(const SynergyRepository* synergy_repository, const PieceRepository* piece_repository)
{
    synergy_repository_ = synergy_repository;
    piece_repository_   = piece_repository;
}

//--------------------------------------------------
//! @brief ボードをスキャンしてシナジー情報を更新
//! @param board_info ボード情報への参照
//--------------------------------------------------
void SynergySystem::UpdateSynergys(const BoardInfo& board_info)
{
    // ひとまずシナジー情報をリセット
    Reset();
    // ボード上の駒を取得
    const auto&                     pieces = board_info.GetPieceInfoArray();
    std::unordered_set<std::string> unique_types;    // 重複しない駒のタイプ名を格納するセット
    // 駒のタイプ名を収集
    for(int file = 0; file < 4; ++file) {
        for(int rank = 0; rank < 8; ++rank) {
            const auto& piece_info = pieces[file][rank];
            if(!piece_info.GetTypeName().empty()) {
                unique_types.insert(piece_info.GetTypeName());
            }
        }
    }
    // 各駒タイプごとにシナジー情報を更新
    for(const auto& type_name : unique_types) {
        // 駒のマスターデータを取得
        auto piece_data = piece_repository_->FindByTypeName(type_name);
        if(piece_data) {
            // 駒が持つシナジーIDごとに処理
            for(const auto& synergy_id : piece_data->synergies_) {
                // 既存のシナジー情報を検索
                auto it = std::find_if(synergys_.begin(), synergys_.end(), [&synergy_id](const Synergy& synergy) { return synergy.GetID() == synergy_id; });
                if(it != synergys_.end()) {
                    // 既存のシナジー情報があればユニット数をインクリメント
                    it->IncrementSynergyCount();
                }
                else {
                    // 新しいシナジー情報を作成して追加
                    const SynergyData* synergy_data = synergy_repository_->FindByID(synergy_id);
                    if(synergy_data) {
                        Synergy new_synergy = Synergy::Builder()
                                                  .SetID(synergy_data->id_)
                                                  .SetName(synergy_data->name_)
                                                  .SetDescription(synergy_data->descriptions_.empty() ? "" : synergy_data->descriptions_[0])
                                                  .SetSynergyCount(1)
                                                  .Build();
                        synergys_.push_back(new_synergy);
                    }
                }
            }
        }
    }
}

//--------------------------------------------------
//! @brief シナジーのベクターを返す関数
//--------------------------------------------------
std::vector<Synergy> SynergySystem::GetSynergys() const
{
    return synergys_;
}

//--------------------------------------------------
//! @brief 指定したシナジーIDのシナジー情報を取得する関数
//--------------------------------------------------
const SynergyData* SynergySystem::GetSynergyData(SynergyID synergy_id) const
{
    if(synergy_repository_) {
        return synergy_repository_->FindByID(synergy_id);
    }
    return nullptr;
}

//--------------------------------------------------
//! @brief 全シナジー情報をリセット
//--------------------------------------------------
void SynergySystem::Reset()
{
    synergys_.clear();
}
