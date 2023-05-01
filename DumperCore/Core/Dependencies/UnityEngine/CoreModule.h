#pragma once
namespace UnityEngine
{
	typedef void* Type;
	typedef class Cursor;
	typedef class Resolution;
	typedef class Screen;
	typedef class Color;
	typedef class Quaternion;
	typedef class Object;
	typedef class GameObject;
	typedef class Component;
	typedef class Transform;
	typedef class Behaviour;
	typedef class MonoBehaviour;
	typedef class Bounds;
	typedef class Shader;
	typedef class Material;
	typedef class Renderer;
	typedef class Ray;
	typedef class Camera;
	typedef class Collider;
	typedef class Rigidbody;
	typedef class RaycastHit;
	typedef class Physics;

	struct Vector2
	{
		Vector2() : x(0.f), y(0.f) {}
		Vector2(float _x, float _y) : x(_x), y(_y) {}
		inline static float Distance(Vector2 a, Vector2 b)
		{
			float num = a.x - b.x;
			float num2 = a.y - b.y;
			return (float)sqrt((double)(num * num + num2 * num2));
		}
		inline std::string ToString()
		{
			std::string str = "(" + to_string(x) + ", " + to_string(y) + ")";
			return str;
		}
		inline ImVec2 ToImVec2()
		{
			return ImVec2(x, y);
		}

		float x;
		float y;

		Vector2 operator +(Vector2 a) { return Vector2(this->x + a.x, this->y + a.y); }
		Vector2 operator -(Vector2 a) { return Vector2(this->x - a.x, this->y - a.y); }
		Vector2 operator *(Vector2 d) { return Vector2(this->x * d.x, this->y * d.y); }
		Vector2 operator *(float d) { return Vector2(this->x * d, this->y * d); }
		Vector2 operator /(float d) { return Vector2(this->x / d, this->y / d); }
	private:
		inline static std::string to_string(float x)
		{
			const auto _Len = static_cast<size_t>(_CSTD _scprintf("%.2f", x));
			std::string _Str(_Len, '\0');
			_CSTD sprintf_s(&_Str[0], _Len + 1, "%.2f", x);
			return _Str;
		}
	};

	struct Vector3
	{
		Vector3() : x(0.f), y(0.f), z(0.f) {}
		Vector3(float _x, float _y, float _z = 0.f) : x(_x), y(_y), z(_z) {}
		inline static float Distance(Vector3 a, Vector3 b)
		{
			float x_diff = a.x - b.x;
			float y_diff = a.y - b.y;
			float z_diff = a.z - b.z;

			float difftotal = x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
			return sqrt(difftotal);
		}
		inline float magnitude()
		{
			return sqrtf(x * x + y * y + z * z);
		}
		inline Vector3 normalized()
		{
			float num = magnitude();
			bool flag = num > 1E-05f;
			Vector3 result;
			if (flag)
			{
				result = *this / num;
			}
			else
			{
				result = Vector3();
			}
			return result;
		}
		inline float Sum()
		{
			return x + y + z;
		}
		inline float SizeSquared()
		{
			return x * x + y * y + z * z;
		}
		inline std::string ToString()
		{
			std::string str = "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
			return str;
		}


		float x;
		float y;
		float z;

		Vector3 operator +(Vector3 a) { return Vector3(this->x + a.x, this->y + a.y, this->y + a.y); }
		Vector3 operator -(Vector3 a) { return Vector3(this->x - a.x, this->y - a.y, this->y - a.y); }
		Vector3 operator *(Vector3 d) { return Vector3(this->x * d.x, this->y * d.y, this->y * d.y); }
		Vector3 operator *(float d) { return Vector3(this->x * d, this->y * d, this->y * d); }
		Vector3 operator /(float d) { return Vector3(this->x / d, this->y / d, this->y / d); }
	private:
		inline static std::string to_string(float x)
		{
			const auto _Len = static_cast<size_t>(_CSTD _scprintf("%.2f", x));
			std::string _Str(_Len, '\0');
			_CSTD sprintf_s(&_Str[0], _Len + 1, "%.2f", x);
			return _Str;
		}
	};

	struct Color
	{
		Color() : r(0.f), g(0.f), b(0.f), a(0.f) { };
		Color(float* colors) : r(colors[0]), g(colors[1]), b(colors[2]), a(colors[3]) { };
		Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { };
		float r;
		float g;
		float b;
		float a;
	};

	struct Quaternion
	{
		Quaternion() : w(0.f), x(0.f), y(0.f), z(0.f) {};
		Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {};

		float w;
		float x;
		float y;
		float z;

		Quaternion operator *(Quaternion rhs) { return Quaternion(this->w * rhs.x + this->x * rhs.w + this->y * rhs.z - this->z * rhs.y, this->w * rhs.y + this->y * rhs.w + this->z * rhs.x - this->x * rhs.z, this->w * rhs.z + this->z * rhs.w + this->x * rhs.y - this->y * rhs.x, this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z); }
		Vector3 operator *(Vector3 point) { float num = this->x * 2.f; float num2 = this->y * 2.f; float num3 = this->z * 2.f; float num4 = this->x * num; float num5 = this->y * num2; float num6 = this->z * num3; float num7 = this->x * num2; float num8 = this->x * num3; float num9 = this->y * num3; float num10 = this->w * num; float num11 = this->w * num2; float num12 = this->w * num3; Vector3 result; result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z; result.y = (num7 + num12) * point.x + (1.f - (num4 + num6)) * point.y + (num9 - num10) * point.z; result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z; return result; }
	};

	class Object
	{
	private:
		CLASS("UnityEngine.CoreModule", "UnityEngine", "Object")
	public:

		int GetInstanceID()
		{
			static auto func = (int(*)(Object * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Int32 GetInstanceID()"));
			return func(this, nullptr);
		}

		std::string get_name()
		{
			static auto func = (WString * (*)(Object * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.String get_name()"));
			return func(this, nullptr)->ToStr();
		}

		void set_name(std::string name)
		{
			static auto func = (void(*)(Object * __this, WString * _name, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_name(System.String)"));
			func(this, new WString(name), nullptr);
		}

		static void DontDestroyOnLoad(Object* target)
		{
			static auto func = (void(*)(Object * target, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void DontDestroyOnLoad(System.Object)"));
			func(target, nullptr);
		}
	};

	class ScriptableObject : public Object
	{
	public:
		ScriptableObject() = delete;
	};

	class Component : public Object
	{
	private:
		CLASS("UnityEngine.CoreModule", "UnityEngine", "Component")
	public:

		Transform* get_transform()
		{
			if (!this)
				return nullptr;
			static auto func = (Transform * (*)(Component * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Transform get_transform()"));
			return func(this, nullptr);
		}

		GameObject* get_gameObject()
		{
			static auto func = (GameObject * (*)(Component * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.GameObject get_gameObject()"));
			return func(this, nullptr);
		}

		Component* GetComponent(const std::string& type)
		{
			static auto func = (Component * (*)(Component * __this, Il2CppType * _type, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Component GetComponent(System.Type)"));
			return func(this, I2Hrame->GetType(type), nullptr);
		}

		Component* GetComponentInChildren(const std::string& type, bool includeInactive = false)
		{
			static auto func = (Component * (*)(Component * __this, Il2CppType * _type, bool i, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Component GetComponentInChildren(System.Type, System.Boolean)"));
			return func(this, I2Hrame->GetType(type), includeInactive, nullptr);
		}

		Component* GetComponentInParent(const std::string& type)
		{
			static auto func = (Component * (*)(Component * __this, Il2CppType * _type, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Component GetComponentInParent(System.Type)"));
			return func(this, I2Hrame->GetType(type), nullptr);
		}

		template<typename T>
		Array<T>* GetComponents(const std::string& type, const bool& includeInactive = false)
		{
			return get_gameObject()->GetComponents<T>(type, includeInactive);
		}

		template<typename T>
		Array<T>* GetComponentsInChildren(const std::string& type, const bool& includeInactive = false)
		{
			return get_gameObject()->GetComponentsInChildren<T>(type, includeInactive);
		}

		template<typename T>
		Array<T>* GetComponentsInParents(const std::string& type, const bool& includeInactive = false)
		{
			return get_gameObject()->GetComponentsInParents<T>(type, includeInactive);
		}

		std::string get_tag()
		{
			static auto func = (WString * (*)(Component * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.String get_tag()"));
			return func(this, nullptr)->ToStr();
		}
	};

	class Transform : public Component
	{
	private:
		CLASS("UnityEngine.CoreModule", "UnityEngine", "Transform")
	public:
		Transform() = delete;

		Vector3 get_position()
		{
			static auto func = (Vector3(*)(Transform * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Vector3 get_position()"));
			return func(this, nullptr);
		}

		void set_position(const Vector3& position)
		{
			static auto func = (void(*)(Transform * __this, Vector3 a, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_position(UnityEngine.Vector3)"));
			return func(this, position, nullptr);
		}

		Vector3 get_localPosition()
		{
			static auto func = (Vector3(*)(Transform * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Vector3 get_localPosition()"));
			return func(this, nullptr);
		}

		void set_localPosition(const Vector3& localPosition)
		{
			static auto func = (void(*)(Transform * __this, Vector3 a, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_localPosition(UnityEngine.Vector3)"));
			return func(this, localPosition, nullptr);
		}

		Vector3 get_eulerAngles()
		{
			static auto func = (Vector3(*)(Transform * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Vector3 get_eulerAngles()"));
			return func(this, nullptr);
		}

		Vector3 get_localEulerAngles()
		{
			static auto func = (Vector3(*)(Transform * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Vector3 get_localEulerAngles()"));
			return func(this, nullptr);
		}

		void set_eulerAngles(const Vector3& eulerAngles)
		{
			static auto func = (void(*)(Transform * __this, Vector3 a, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_eulerAngles(UnityEngine.Vector3)"));
			return func(this, eulerAngles, nullptr);
		}

		void set_localEulerAngles(const Vector3& eulerAngles)
		{
			static auto func = (void(*)(Transform * __this, Vector3 a, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_localEulerAngles(UnityEngine.Vector3)"));
			return func(this, eulerAngles, nullptr);
		}

		Quaternion get_rotation()
		{
			static auto func = (Quaternion(*)(Transform * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Quaternion get_rotation()"));
			return func(this, nullptr);
		}

		void set_rotation(Quaternion rotation)
		{
			static auto func = (void(*)(Transform * __this, Quaternion a, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_rotation(UnityEngine.Quaternion)"));
			return func(this, rotation, nullptr);
		}

		Vector3 get_right()
		{
			return get_rotation() * Vector3(1.f, 0.f, 0.f);
		}

		Vector3 get_up()
		{
			return get_rotation() * Vector3(0.f, 1.f, 0.f);
		}

		Vector3 get_forward()
		{
			return get_rotation() * Vector3(0.f, 0.f, 1.f);
		}

		Transform* get_parent()
		{
			static auto func = (Transform * (*)(Transform * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Transform get_parent()"));
			return func(this, nullptr);
		}
	};

	class GameObject : public Object
	{
	private:
		CLASS("UnityEngine.CoreModule", "UnityEngine", "GameObject")
	public:
		GameObject()
		{
			static auto func = (GameObject * (*)(MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void GameObject()"));
			func(nullptr);
		}
		GameObject(std::string name)
		{
			static auto func = (GameObject * (*)(WString * name, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void GameObject(System.String)"));
			func(new WString(name), nullptr);
		}

		Component* GetComponent(const std::string& type)
		{
			static auto func = (Component * (*)(GameObject * __this, Il2CppType * _type, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Component GetComponent(System.Type)"));
			return func(this, I2Hrame->GetType(type), nullptr);
		}

		Component* GetComponentInChildren(const std::string& type, bool includeInactive)
		{
			static auto func = (Component * (*)(GameObject * __this, Il2CppType * _type, bool b, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Component GetComponentInChildren(System.Type, System.Boolean)"));
			return func(this, I2Hrame->GetType(type), includeInactive, nullptr);
		}

		Component* GetComponentInParent(const std::string& type)
		{
			static auto func = (Component * (*)(GameObject * __this, Il2CppType * _type, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Component GetComponentInParent(System.Type)"));
			return func(this, I2Hrame->GetType(type), nullptr);
		}

		Transform* get_transform()
		{
			static auto func = (Transform * (*)(GameObject * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Transform get_transform()"));
			return func(this, nullptr);
		}

		std::string get_tag()
		{
			static auto func = (WString * (*)(GameObject * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.String get_tag()"));
			return func(this, nullptr)->ToStr();
		}

		void set_Active(bool value)
		{
			static auto func = (void (*)(GameObject * __this, bool b, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_Active(System.Boolean)"));
			func(this, value, nullptr);
		}

		int get_layer()
		{
			static auto func = (int (*)(GameObject * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Int32 get_layer()"));
			return func(this, nullptr);
		}

		template <typename T>
		Array<T>* GetComponents(const std::string& type, const bool& includeInactive = true)
		{
			return UnityEngine::GameObject::GetComponentsInternal<T>(type, true, false, includeInactive, false, nullptr);
		}

		template <typename T>
		Array<T>* GetComponentsInChildren(const std::string& type, const bool& includeInactive = false)
		{
			return UnityEngine::GameObject::GetComponentsInternal<T>(type, true, true, includeInactive, false, nullptr);
		}

		template <typename T>
		Array<T>* GetComponentsInParents(const std::string& type, const bool& includeInactive = false)
		{
			return UnityEngine::GameObject::GetComponentsInternal<T>(type, true, true, includeInactive, true, nullptr);
		}
	private:
		template <typename T>
		Array<T>* GetComponentsInternal(const std::string& type, const bool& useSearchTypeAsArrayReturnType, const bool& recursive, const bool& includeInactive, const bool& reverse, void* resultList)
		{
			static auto func = (uintptr_t(*)(GameObject * __this, Il2CppType * _type, bool a, bool b, bool c, bool d, void* e, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Array GetComponentsInternal(System.Type, System.Boolean, System.Boolean, System.Boolean, System.Boolean, System.Object)"));
			return reinterpret_cast<Array<T>*>(func(this, I2Hrame->GetType(type), useSearchTypeAsArrayReturnType, recursive, includeInactive, reverse, resultList, nullptr));
		}
	};

	class Behaviour : public Component
	{
	public:
	};

	class MonoBehaviour : public Behaviour
	{
	public:
	};

	class Screen
	{
	private:
		CLASS("UnityEngine.CoreModule", "UnityEngine", "Screen")
	public:
		Screen() = delete;
		static int get_width()
		{
			static auto func = (int(*)(MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Int32 get_width()"));
			return func(nullptr);
		}

		static int get_height()
		{
			static auto func = (int(*)(MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Int32 get_height()"));
			return func(nullptr);
		}

		static Vector2 _Center()
		{
			return Vector2(get_width() * 0.5f, get_height() * 0.5f);
		}
	};

	class Camera : public Behaviour
	{
	private:
		typedef struct _D3DMATRIX {
			union {
				struct {
					float        _11, _12, _13, _14;
					float        _21, _22, _23, _24;
					float        _31, _32, _33, _34;
					float        _41, _42, _43, _44;

				};
				float m[4][4];
			};
		} D3DMATRIX;
	private:
		CLASS("UnityEngine.CoreModule", "UnityEngine", "Camera")
	public:
		Camera() = delete;
		static Camera* get_main()
		{
			static auto func = (Camera * (*)(MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Camera get_main()"));
			return func(nullptr);
		}

		static Camera* get_current()
		{
			static auto func = (Camera * (*)(MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "UnityEngine.Camera get_current()"));
			return func(nullptr);
		}

		float get_fieldOfView()
		{
			static auto func = (float (*)(Camera * __this, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Single get_fieldOfView()"));
			return func(this, nullptr);
		}

		void set_fieldOfView(const float& fieldOfView)
		{
			static auto func = (void (*)(Camera * __this, float a, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_fieldOfView(System.Single)"));
			return func(this, fieldOfView, nullptr);
		}

		bool WorldToScreenPoint(const Vector3& position, Vector2& screenPoint)
		{
			Vector3 to;
			float w = 0.0f;
			__int64 WorldToScreen = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x10);
			D3DMATRIX viewmatrix = *reinterpret_cast<D3DMATRIX*>(WorldToScreen + 0xDC);
			to.x = viewmatrix._11 * position.x + viewmatrix._21 * position.y + viewmatrix._31 * position.z + viewmatrix._41;
			to.y = viewmatrix._12 * position.x + viewmatrix._22 * position.y + viewmatrix._32 * position.z + viewmatrix._42;

			w = viewmatrix._13 * position.x + viewmatrix._23 * position.y + viewmatrix._33 * position.z + viewmatrix._43;

			if (w < 0.01f)
				return false;

			to.x *= (1.0f / w);
			to.y *= (1.0f / w);

			int width = Screen::get_width();
			int height = Screen::get_height();

			float x = width / 2;
			float y = height / 2;

			x += 0.5f * to.x * width + 0.5f;
			y -= 0.5f * to.y * height + 0.5f;

			to.x = x;
			to.y = y;

			screenPoint.x = to.x;
			screenPoint.y = to.y;
			return true;
		}
	};

	class Time
	{
	private:
		CLASS("UnityEngine.CoreModule", "UnityEngine", "Time")
	public:
		Time() = delete;
		static void set_timeScale(float value)
		{
			static auto func = (void(*)(float a, MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Void set_timeScale(System.Single)"));
			return func(value, nullptr);
		}
	};

	class Random
	{
	private:
		CLASS("UnityEngine.CoreModule", "UnityEngine", "Random")
	public:
		Random() = delete;

		static float Range(float min, float max)
		{
			static auto func = (float(*)(MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Single Range(System.Single, System.Single)"));
			return func(nullptr);
		}

		static int Range(int min, int max)
		{
			static auto func = (int(*)(MethodInfo * method))(
				I2Hrame->GetMethod(ThisClass(), "System.Int32 Range(System.Int32, System.Int32)"));
			return func(nullptr);
		}
	};
}