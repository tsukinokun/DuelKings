//---------------------------------------------------------------------------
//!	@file	SynergyRepository.cpp
//! @brief	シナジーのマスターデータ情報のリポジトリクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "SynergyRepository.h"
//------------------------------------------------------
//! @brief JSONファイルから駒定義（SynergyData）を読み込む
//------------------------------------------------------
bool SynergyRepository::LoadFromJson(const std::string& path)
{
    std::ifstream file(path);
    if(!file.is_open())
        return false;
    try {
        cereal::JSONInputArchive archive(file);
        archive(CEREAL_NVP(synergies_));
        return true;
    }
    catch(const std::exception& e) {
        std::cerr << "Failed to load SynergyData: " << e.what() << std::endl;
        return false;
    }
}

const SynergyData* SynergyRepository::FindByID(const SynergyID& id) const
{
    for(const auto& synergy : synergies_) {
        if(synergy.id_ == id)
            return &synergy;
    }
    return nullptr;
}
