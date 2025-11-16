#include "MagicProjectileActor.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Random.h"
#include <string>

MagicProjectileActor::MagicProjectileActor(Game* game, AttackType type, Vector2 startPos, Vector2 endPos, float travelTime, bool rotate180)
    : Actor(game)
    , mAnimator(nullptr)
    , mStartPos(startPos)
    , mEndPos(endPos)
    , mTravelTime(travelTime)
    , mTimer(0.0f)
    , mIsComplete(false)
    , mRotate180(rotate180)
{
    SetPosition(startPos);
    LoadMagicAnimation(type);

    // Aplicar flip horizontal se necessário (projétil do inimigo)
    if (mRotate180)
    {
        SetScale(Vector2(-1.0f, 1.0f)); // Flip horizontal
    }
}

MagicProjectileActor::~MagicProjectileActor()
{
}

void MagicProjectileActor::OnUpdate(float deltaTime)
{
    if (mIsComplete)
        return;

    mTimer += deltaTime;

    // Lerp position from start to end
    float t = mTimer / mTravelTime;
    if (t > 1.0f)
        t = 1.0f;

    Vector2 currentPos = mStartPos + (mEndPos - mStartPos) * t;
    SetPosition(currentPos);

    // Check if travel is complete
    if (mTimer >= mTravelTime)
    {
        mIsComplete = true;
    }
}

void MagicProjectileActor::LoadMagicAnimation(AttackType type)
{
    struct MagicInfo {
        const char* pasta;
        const char* prefixo;
    };

    static const MagicInfo magicMap[] = {
        {"Fire", "firemagic0"},     // AttackType::Fire
        {"Water", "watermagic0"},   // AttackType::Water
        {"Grass", "grassmagic0"},   // AttackType::Plant
        {"Neutral", "neutralmagic0"} // AttackType::Neutral
    };

    int variant = Random::GetIntRange(1, 3);
    const MagicInfo& info = magicMap[static_cast<int>(type)];

    std::string variantStr = std::to_string(variant);
    std::string basePath = std::string("../Assets/Magics/") + info.pasta + "/" + variantStr + "/";
    std::string fullPath = basePath + info.prefixo + variantStr;

    mAnimator = new AnimatorComponent(this,
        (fullPath + ".png").c_str(),
        (fullPath + ".json").c_str(),
        64, 64, 100);

    mAnimator->AddAnimation("magic", {0, 1, 2, 3, 4});
    mAnimator->SetAnimFPS(10.0f);
    mAnimator->SetAnimation("magic");

    SDL_Log("Animação mágica carregada: %s", fullPath.c_str());
}

