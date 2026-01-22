//---------------------------------------------------------------------------
//!	@file	BootScene.cpp
//! @brief	ブート処理シーンの実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "BootScene.h"
#include <Game/AutoChess/system/SoundManager.h>
#include <Game/AutoChess/system/SoundData.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/Info/SynergyModifierData.h>
#include <Game/AutoChess/TitleScene.h>
#include <Game/AutoChess/system/GameRepository.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool BootScene::Init()
{
    __super::Init();
    static SynergyModifierData synergy_modifier_data_instance;    //シナジーモディファイアデータのシングルトン生成
    ImageBuffer::Init();                                          //画像バッファの初期化

    //JSONファイルを開く
    std::ifstream ifs("data/AutoChess/MasterData/SoundDatas.json");
    if(!ifs) {
        return false;
    }
    //JSON → SoundData に読み込む
    SoundData sound_data;
    {
        cereal::JSONInputArchive archive(ifs);
        archive(sound_data);
    }

    static SoundManager   sound_manager_instance;
    static GameRepository game_repository_instance;
    //SoundManager に渡してロード
    SoundManager::instance()->LoadFromData(sound_data);
    //必要なら起動時にBGMを再生
    //SoundManager::instance()->PlayBGM("setup");
    Scene::Change(Scene::GetScene<TitleScene>());    //シーンの変更を行う処理
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void BootScene::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void BootScene::Draw()
{
    __super::Draw();
}
//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void BootScene::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void BootScene::GUI()
{
    __super::GUI();
}
