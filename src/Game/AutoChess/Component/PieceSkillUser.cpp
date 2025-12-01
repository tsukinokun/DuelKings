//---------------------------------------------------------------------------
//!	@file	PieceSkillUser.cpp
//! @brief	ピースのスキル使用機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/PieceSkillUser.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void PieceSkillUser::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // スキル使用処理
    //---------------------------------------------------------
    auto skill_use_proc = [this]() {
        //オーナーはピースであることが前提
        auto this_piece = dynamic_pointer_cast<Piece>(GetOwnerPtr());
        //アクティブスキルコンポーネントを取得
        if(auto active_skill = this_piece->GetComponent<ComponentActiveSkill>()) {
            //スキルが使用可能かチェック
            if(active_skill->CanActivate()) {
                //スキルを使用
                active_skill->Activate();
            }
        }
    };
    SetProc("piece_skill_use_proc", skill_use_proc, ProcTiming::Update, ProcPriority::NONE);
}
