//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AnimatorComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Json.h"
#include <fstream>

AnimatorComponent::AnimatorComponent(class Actor* owner, const std::string &texPath, const std::string &dataPath,
                                     int width, int height, int drawOrder)
        :DrawComponent(owner,  drawOrder)
        ,mAnimTimer(0.0f)
        ,mIsPaused(false)
        ,mIsLooping(true)
        ,mIsFinished(false)
        ,mWidth(width)
        ,mHeight(height)
        ,mTextureFactor(1.0f)
{
    mSpriteTexture = mOwner->GetGame()->GetRenderer()->GetTexture(texPath);
    LoadSpriteSheetData(dataPath);
}

AnimatorComponent::~AnimatorComponent()
{
    mAnimations.clear();
    mSpriteSheetData.clear();
}

bool AnimatorComponent::LoadSpriteSheetData(const std::string& dataPath)
{
    std::ifstream spriteSheetFile(dataPath);

    if (!spriteSheetFile.is_open()) {
        SDL_Log("ERROR: Failed to open sprite sheet data file: %s", dataPath.c_str());
        return false;
    }

    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    if (spriteSheetData.is_null()) {
        SDL_Log("ERROR: Failed to parse sprite sheet data file: %s", dataPath.c_str());
        return false;
    }

    auto textureWidth = static_cast<float>(spriteSheetData["meta"]["size"]["w"].get<int>());
    auto textureHeight = static_cast<float>(spriteSheetData["meta"]["size"]["h"].get<int>());

    for(const auto& frame : spriteSheetData["frames"]) {

        int x = frame["frame"]["x"].get<int>();
        int y = frame["frame"]["y"].get<int>();
        int w = frame["frame"]["w"].get<int>();
        int h = frame["frame"]["h"].get<int>();

        mSpriteSheetData.emplace_back(static_cast<float>(x)/textureWidth, static_cast<float>(y)/textureHeight,
                                      static_cast<float>(w)/textureWidth, static_cast<float>(h)/textureHeight);
    }

    return true;
}

void AnimatorComponent::Draw(Renderer* renderer)
{
    if (!mIsVisible || !mIsEnabled || !mSpriteTexture || mSpriteSheetData.empty()) {
        return;
    }

    if (mAnimations.empty() || mAnimName.empty()) {
        renderer->DrawTexture(
                mOwner->GetPosition(),
                Vector2(mWidth, mHeight),
                mOwner->GetRotation(),
                Color::White,
                mSpriteTexture,
            mSpriteSheetData[0],
                mOwner->GetGame()->GetCameraPos(),
                (mOwner->GetScale().x < 0),
                mTextureFactor
            );
        } else {
        // Verificar se a animação existe e tem frames
        auto it = mAnimations.find(mAnimName);
        if (it != mAnimations.end() && !it->second.empty()) {
            int frameIndex = static_cast<int>(mAnimTimer);
            if (frameIndex >= 0 && frameIndex < static_cast<int>(it->second.size())) {
                int spriteIndex = it->second[frameIndex];
                if (spriteIndex >= 0 && spriteIndex < static_cast<int>(mSpriteSheetData.size())) {
            renderer->DrawTexture(
                mOwner->GetPosition(),
                Vector2(mWidth, mHeight),
                mOwner->GetRotation(),
                Color::White,
                mSpriteTexture,
                        mSpriteSheetData[spriteIndex],
                mOwner->GetGame()->GetCameraPos(),
                (mOwner->GetScale().x < 0),
                mTextureFactor
            );
                }
            }
        }
    }
}

void AnimatorComponent::Update(float deltaTime)
{
    if (mIsPaused || mAnimations.empty() || mAnimName.empty()) {
        return;
    }

    auto it = mAnimations.find(mAnimName);
    if (it == mAnimations.end() || it->second.empty()) {
        return;
    }

    mAnimTimer += mAnimFPS * deltaTime;

    float duration = static_cast<float>(it->second.size());

    if (mIsLooping)
    { //se for uma animação que fica em looping
        while (mAnimTimer >= duration) {
            mAnimTimer -= duration;
        }
    }
    else
    {
        //se for uma animação que roda só uma vez
        if (mAnimTimer >= duration) {
            // trava o timer levemente antes do fim para exibir o último frame estático
            mAnimTimer = duration - 0.01f;
            mIsFinished = true;
        }
    }
}

void AnimatorComponent::SetFrameTime(float seconds)
{
    if (seconds > 0.0f) {
        mAnimFPS = 1.0f / seconds;
    } else {
        mAnimFPS = 0.0f; // Evita divisão por zero
    }
}

void AnimatorComponent::SetAnimation(const std::string& name)
{
    if (mAnimations.find(name) != mAnimations.end()) {
        mAnimName = name;
        mAnimTimer = 0.0f;
        mIsFinished = false;
    } else {
        SDL_Log("ERROR: Animation '%s' not found", name.c_str());
    }
}

void AnimatorComponent::AddAnimation(const std::string& name, const std::vector<int>& spriteNums)
{
    mAnimations.emplace(name, spriteNums);
}