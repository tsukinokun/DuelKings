//---------------------------------------------------------------------------
//!	@file	SkillRepository.cpp
//! @brief	スキルのマスターデータ情報のリポジトリクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "SkillRepository.h"
//------------------------------------------------------
//! @brief JSONファイルからスキル定義（SkillData）を読み込む
//------------------------------------------------------
bool SkillRepository::LoadFromJson(const std::string& path)
{
    std::ifstream file(path);
    if(!file.is_open())
        return false;
    try {
        cereal::JSONInputArchive archive(file);
        archive(skills_);
        return true;
    }
    catch(const std::exception& e) {
        std::cerr << "Failed to load SkillData: " << e.what() << std::endl;
        return false;
    }
}

//--------------------------------------------------
//! @brief スキル名で定義を検索
//--------------------------------------------------
const SkillData* SkillRepository::FindBySkillName(const std::string_view& skill_name) const
{
    for(const auto& skill : skills_) {
        if(skill.skill_name_ == skill_name) {
            return &skill;
        }
    }
    return nullptr;
}

//--------------------------------------------------
//! @brief 全スキル定義を取得（UI表示などに使用）
//--------------------------------------------------
const std::vector<SkillData>& SkillRepository::GetAllSkills() const
{
    return skills_;
}
