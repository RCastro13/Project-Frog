#include "MagicProjectileActor.h"
#include "ActorConstants.h"
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

    if (mRotate180)
    {
        SetScale(Vector2(-1.0f, 1.0f));
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

    float t = Math::Min(mTimer / mTravelTime, 1.0f);
    Vector2 currentPos = mStartPos + (mEndPos - mStartPos) * t;
    SetPosition(currentPos);

    if (mTimer >= mTravelTime)
    {
        mIsComplete = true;
    }
}

void MagicProjectileActor::LoadMagicAnimation(AttackType type)
{
    using namespace ActorConstants;

    struct MagicInfo {
        const char* pasta;
        const char* prefixo;
    };

    static const MagicInfo magicMap[] = {
        {"Fire", "firemagic0"},
        {"Water", "watermagic0"},
        {"Grass", "grassmagic0"},
        {"Neutral", "neutralmagic0"}
    };

    int variant = Random::GetIntRange(Magic::MIN_VARIANT, Magic::MAX_VARIANT);
    const MagicInfo& info = magicMap[static_cast<int>(type)];

    std::string variantStr = std::to_string(variant);
    std::string basePath = std::string("../Assets/Magics/") + info.pasta + "/" + variantStr + "/";
    std::string fullPath = basePath + info.prefixo + variantStr;

    mAnimator = new AnimatorComponent(this,
        (fullPath + ".png").c_str(),
        (fullPath + ".json").c_str(),
        Sprites::PROJECTILE_WIDTH,
        Sprites::PROJECTILE_HEIGHT,
        Sprites::DEFAULT_DRAW_ORDER);

    mAnimator->AddAnimation("magic", {0, 1, 2, 3, 4});
    mAnimator->SetAnimFPS(Animation::DEFAULT_FPS);
    mAnimator->SetAnimation("magic");
}

