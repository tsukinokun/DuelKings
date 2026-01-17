//---------------------------------------------------------------------------
//!	@file	MatchInfo.h
//! @brief	マッチ情報クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class Agent;    // 前方宣言
class MatchInfo
{
public:
    std::weak_ptr<Agent> agent1_;             // エージェント1
    std::weak_ptr<Agent> agent2_;             // エージェント2
    float                battle_duration_;    // バトル時間、軽量化した疑似バトルなので、実際のバトル時間とは異なる
    bool                 is_ghost_2_;         // エージェント2がゴーストかどうか
    bool                 is_judged_;          //すでに判定したかどうか
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    MatchInfo()
        : battle_duration_(0.0f)
        , is_ghost_2_(false)
        , is_judged_(false)
    {
    }

    //---------------------------------------------------------------------------
    //! @brief 引数付きコンストラクタ
    //! @param agent1 エージェント1
    //! @param agent2 エージェント2
    //! @param battle_duration バトル時間
    //! @param is_ghost_2 エージェント2がゴーストかどうか
    //---------------------------------------------------------------------------
    MatchInfo(std::weak_ptr<Agent> agent1, std::weak_ptr<Agent> agent2, float battle_duration, bool is_ghost_2)
        : agent1_(agent1)
        , agent2_(agent2)
        , battle_duration_(battle_duration)
        , is_ghost_2_(is_ghost_2)
        , is_judged_(false)
    {
    }
};
