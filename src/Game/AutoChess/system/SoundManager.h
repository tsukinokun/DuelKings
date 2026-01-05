//---------------------------------------------------------------------------
//!	@file	SoundManager.h
//! @brief	効果音とBGMを管理するクラスの定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/AutoChess/system/singleton.h>
#include <Game/AutoChess/system/SoundData.h>
//---------------------------------------------------------------------------
//! @class SoundManager
//! @brief 効果音とBGMを管理するクラス
//! @note  Singletonを継承し、シングルトンとして実装
//---------------------------------------------------------------------------
class SoundManager : public Singleton<SoundManager>
{
public:
    //---------------------------------------------------------------------------
    // サウンドデータから効果音とBGMをロードする関数
    //! @param data サウンドデータの参照
    //---------------------------------------------------------------------------
    void LoadFromData(const SoundData& data)
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
    // 効果音を再生する関数
    //! @param name 効果音の名前
    //---------------------------------------------------------------------------
    void PlaySE(const std::string& name)
    {
        if(se_handles_.count(name)) {
            PlaySoundMem(se_handles_[name], DX_PLAYTYPE_BACK);
        }
    }

    //---------------------------------------------------------------------------
    // BGMを再生する関数
    //! @param name BGMの名前
    //---------------------------------------------------------------------------
    void PlayBGM(const std::string& name)
    {
        if(bgm_handles_.count(name)) {
            PlaySoundMem(bgm_handles_[name], DX_PLAYTYPE_LOOP);
        }
    }

private:
    std::unordered_map<std::string, int> se_handles_;     // 効果音のハンドルマップ
    std::unordered_map<std::string, int> bgm_handles_;    // BGMのハンドルマップ
};
