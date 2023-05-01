#pragma once

namespace UnityEngine
{
	enum class QueryTriggerInteraction
	{
		UseGlobal,
		Ignore,
		Collide
	};

	struct RaycastHit
	{
	public:
		Vector3 m_Point;
		Vector3 m_Normal;
		unsigned int m_FaceID;
		float m_Distance;
		Vector2 m_UV;
		int m_Collider;
	};
	
	class Physics
	{
	public:
		Physics() = delete;
		CLASS("UnityEngine.PhysicsModule", "UnityEngine", "Physics")
		
		static bool Linecast(Vector3 start, Vector3 end, int layerMask)
		{
			static auto func = (bool(*)(Vector3 start, Vector3 end, int layer, MethodInfo * method))(
								I2Hrame->GetMethod(ThisClass(), "System.Boolean Linecast(UnityEngine.Vector3, UnityEngine.Vector3, System.Int32)"));
			return func(start, end, layerMask, nullptr);
		}

				static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance)
		{
			static auto func = (bool(*)(Vector3 origin, Vector3 direction, float maxDistance, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Boolean Raycast(UnityEngine.Vector3, UnityEngine.Vector3, System.Single)"));
			return func(origin, direction, maxDistance, nullptr);
		}

		static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance, int layerMask)
		{
			static auto func = (bool(*)(Vector3 origin, Vector3 direction, float maxDistance, int layerMask, MethodInfo * method))(
								I2Hrame->GetMethod(ThisClass(), "System.Boolean Raycast(UnityEngine.Vector3, UnityEngine.Vector3, System.Single, System.Int32)"));
			return func(origin, direction, maxDistance, layerMask, nullptr);
		}

				static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit* hitInfo, float maxDistance, int layerMask, QueryTriggerInteraction queryTriggerInteraction)
		{
			static auto func = (bool(*)(Vector3 origin, Vector3 direction, RaycastHit * hitInfo, float maxDistance, int layerMask, QueryTriggerInteraction queryTriggerInteraction, MethodInfo * method))(
								I2Hrame->GetMethod(ThisClass(), "System.Boolean Raycast(UnityEngine.Vector3, UnityEngine.Vector3, out UnityEngine.RaycastHit, System.Single, System.Int32, UnityEngine.QueryTriggerInteraction)"));
			return func(origin, direction, hitInfo, maxDistance, layerMask, queryTriggerInteraction, nullptr);
		}

				static bool Linecast(Vector3 start, Vector3 end, RaycastHit* hitInfo, int layerMask, QueryTriggerInteraction queryTriggerInteraction)
		{
			Vector3 direction = end - start;
			return Raycast(start, direction, hitInfo, direction.magnitude(), layerMask, queryTriggerInteraction);
		}
	};

	class Collider : public Component
	{
	public:
		Collider() = delete;
		CLASS("UnityEngine.PhysicsModule", "UnityEngine", "Collider")

		bool get_enabled()
		{
			static auto func = (bool (*)(Collider * __this, MethodInfo * method))(
								I2Hrame->GetMethod(ThisClass(), "System.Boolean get_enabled()"));
			return func(this, nullptr);
		}

		void set_enabled(bool value)
		{
			static auto func = (void (*)(Collider * __this, bool value, MethodInfo * method))(
								I2Hrame->GetMethod(ThisClass(), "System.Void set_enabled(System.Boolean)"));
			func(this, value, nullptr);
		}
	};

	class CharacterController : public Collider
	{
	public:
		CharacterController() = delete;
		CLASS("UnityEngine.PhysicsModule", "UnityEngine", "CharacterController")

		Vector3 get_velocity()
		{
			static auto func = (Vector3(*)(CharacterController * __this, MethodInfo * method))(
								I2Hrame->GetMethod(ThisClass(), "UnityEngine.Vector3 get_velocity()"));
			return func(this, nullptr);
		}
	};

	class Rigidboby : public Component
	{
	public:
		Rigidboby() = delete;
		CLASS("UnityEngine.PhysicsModule", "UnityEngine", "Rigidbody")
		
		Vector3 get_velocity()
		{
			static auto func = (Vector3(*)(Rigidboby * __this, MethodInfo * method))(
								I2Hrame->GetMethod(ThisClass(), "UnityEngine.Vector3 get_velocity()"));
			return func(this, nullptr);
		}
	};
}