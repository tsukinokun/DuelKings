//---------------------------------------------------------------------------
//!	@file	SoundManager.cpp
//! @brief	効果音とBGMを管理するクラスの実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "SoundManager.h"
//---------------------------------------------------------------------------
//! @brief サウンドデータから効果音とBGMをロードする関数
//---------------------------------------------------------------------------
void SoundManager::LoadFromData(const SoundData& data)
{
    // SE のロード
    for(auto& [name, path] : data.se) {
        int handle        = LoadSoundMem(path.c_str());
        se_handles_[name] = handle;
    }

    // BGM のロード
    for(auto& [name, path] : data.bgm) {
        int handle         = LoadSoundMem(path.c_str());
        bgm_handles_[name] = handle;
    }
}

//---------------------------------------------------------------------------
//! @brief 効果音を再生する関数
//---------------------------------------------------------------------------
void SoundManager::PlaySE(const std::string& name)
{
    if(se_handles_.count(name)) {
        PlaySoundMem(se_handles_[name], DX_PLAYTYPE_BACK);
    }
}

//---------------------------------------------------------------------------
//! @brief BGMを再生する関数
//---------------------------------------------------------------------------
void SoundManager::PlayBGM(const std::string& name)
{
    if(bgm_handles_.count(name)) {
        PlaySoundMem(bgm_handles_[name], DX_PLAYTYPE_LOOP);
    }
}
