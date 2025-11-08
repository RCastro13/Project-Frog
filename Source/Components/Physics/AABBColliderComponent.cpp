//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

AABBColliderComponent::AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h,
        ColliderLayer layer, bool isStatic, int updateOrder)
        :Component(owner, updateOrder)
        ,mOffset(Vector2((float)dx, (float)dy))
        ,mIsStatic(isStatic)
        ,mWidth(w)
        ,mHeight(h)
        ,mLayer(layer)
{
    GetGame()->AddCollider(this);
}

AABBColliderComponent::~AABBColliderComponent()
{
    GetGame()->RemoveCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const
{
    return mOwner->GetPosition() + mOffset - Vector2(mWidth/2, mHeight/2);
}

Vector2 AABBColliderComponent::GetMax() const
{
    return mOwner->GetPosition() + mOffset + Vector2(mWidth/2, mHeight/2);
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent& b) const
{
    return !(
        GetMax().x <= b.GetMin().x ||
        b.GetMax().x <= GetMin().x ||
        GetMax().y <= b.GetMin().y ||
        b.GetMax().y <= GetMin().y
        );
}

float AABBColliderComponent::GetMinVerticalOverlap(AABBColliderComponent* b) const
{
    float aMinY = GetMin().y;
    float aMaxY = GetMax().y;
    float bMinY = b->GetMin().y;
    float bMaxY = b->GetMax().y;

    float overlapY = Math::Min(aMaxY, bMaxY) - Math::Max(aMinY, bMinY);
    if (overlapY <= 0.0f) return 0.0f;

    float d_t = aMinY - bMaxY;
    float d_b = aMaxY - bMinY;

    return (Math::Abs(d_t) < Math::Abs(d_b)) ? d_t : d_b;
}

float AABBColliderComponent::GetMinHorizontalOverlap(AABBColliderComponent* b) const
{
    float aMinX = GetMin().x;
    float aMaxX = GetMax().x;
    float bMinX = b->GetMin().x;
    float bMaxX = b->GetMax().x;

    float overlapX = Math::Min(aMaxX, bMaxX) - Math::Max(aMinX, bMinX);
    if (overlapX <= 0.0f) return 0.0f;

    float d_l = aMinX - bMaxX;
    float d_r = aMaxX - bMinX;

    return (Math::Abs(d_l) < Math::Abs(d_r)) ? d_l : d_r;
}

float AABBColliderComponent::DetectHorizontalCollision(RigidBodyComponent *rigidBody)
{
    if (mIsStatic) return false;

    auto colliders = GetGame()->GetColliders();

    float sobPos = 0.0f;

    for (AABBColliderComponent* collider : colliders) {
        if (collider->IsEnabled() && collider != this) {
            if (Intersect(*collider)) {
                sobPos = GetMinHorizontalOverlap(collider);

                if (sobPos != 0.0f) {
                    ResolveHorizontalCollisions(mOwner->GetComponent<RigidBodyComponent>(), sobPos);
                    mOwner->OnHorizontalCollision(sobPos, collider);
                    collider->GetOwner()->OnHorizontalCollision(sobPos, collider);
                    break;
                }
            }
        }
    }

    return sobPos;
}

float AABBColliderComponent::DetectVertialCollision(RigidBodyComponent *rigidBody)
{
    if (mIsStatic) return false;

    auto colliders = GetGame()->GetColliders();

    float sobPos = 0.0f;

    for (AABBColliderComponent* collider : colliders) {
        if (collider->IsEnabled() && collider != this) {
            if (Intersect(*collider)) {
                sobPos = GetMinVerticalOverlap(collider);

                if (sobPos != 0.0f) {
                    ResolveVerticalCollisions(mOwner->GetComponent<RigidBodyComponent>(), sobPos);
                    mOwner->OnVerticalCollision(sobPos, collider);
                    collider->GetOwner()->OnVerticalCollision(sobPos, collider);
                    break;
                }
            }
        }
    }

    return sobPos;
}

void AABBColliderComponent::ResolveHorizontalCollisions(RigidBodyComponent *rigidBody, const float minXOverlap)
{
    if (minXOverlap == 0.0f) return;

    Vector2 pos = mOwner->GetPosition();
    pos.x -= minXOverlap;
    mOwner->SetPosition(pos);

    if (rigidBody) {
        Vector2 vel = rigidBody->GetVelocity();
        vel.x = 0.0f;
        rigidBody->SetVelocity(vel);
    }
}

void AABBColliderComponent::ResolveVerticalCollisions(RigidBodyComponent *rigidBody, const float minYOverlap)
{
    if (minYOverlap == 0.0f) return;

    Vector2 pos = mOwner->GetPosition();
    pos.y -= minYOverlap;
    mOwner->SetPosition(pos);

    if (rigidBody) {
        Vector2 vel = rigidBody->GetVelocity();
        vel.y = 0.0f;
        rigidBody->SetVelocity(vel);
    }

    if (minYOverlap > 0.0f) {
        mOwner->SetOnGround();
    }
}

void AABBColliderComponent::DebugDraw(class Renderer *renderer)
{
    renderer->DrawRect(mOwner->GetPosition() + mOffset,Vector2((float)mWidth, (float)mHeight), mOwner->GetRotation(),
                       Color::Green, mOwner->GetGame()->GetCameraPos(), RendererMode::LINES);
}