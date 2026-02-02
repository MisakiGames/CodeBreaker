#pragma once

#include "DeadComponent.h"
#include "IComponent.hpp"
#include "RigidBodyComponent.hpp"

namespace mmt_gd
{
	class PlayerMoveComponent : public IComponent
	{
	public:
		using ptr = std::shared_ptr<PlayerMoveComponent>;

		PlayerMoveComponent(GameObject& gameObject, RigidBodyComponent& rigidBody, DeadComponent& deadComponent, int playerIndex);

		float getDashDuration() const
		{
			return m_dashDuration;
		}

		bool init() override;
		void update(float deltaTime) override;

	private:
		int                 m_playerIndex;
		DeadComponent& m_deadComponent;
		RigidBodyComponent& m_rigidBody;
		sf::Vector2f        m_lastMoveDirection;
		bool                m_isDashing = false;
		bool                m_canDash = true;
		float               m_dashCooldown = 0.f;
		float               m_dashDuration = 0.f;
	};
} // namespace mmt_gd