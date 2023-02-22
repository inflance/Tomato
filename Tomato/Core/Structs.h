#pragma once

namespace Tomato
{
	template <typename T>
	struct Vector_t
	{
		T x_;

		Vector_t<T> operator*(const Vector_t<T>& other)
		{
			Vector_t<T> vec;
			vec.x_ = this->x_ * other.x_;
			return vec;
		}

		const Vector_t<T>& operator*=(const Vector_t<T>& other)
		{
			this->x_ *= other.x_;
			return *this;
		}

		Vector_t<T> operator/(const Vector_t<T>& other)
		{
			Vector_t<T> vec;
			vec.x_ = this->x_ * other.x_;
			return vec;
		}

		const Vector_t<T>& operator/=(const Vector_t<T>& other)
		{
			this->x_ *= other.x_;
			return *this;
		}

		Vector_t<T> operator+(const Vector_t<T>& other)
		{
			Vector_t<T> vec;
			vec.x_ = this->x_ + other.x_;
			return vec;
		}

		const Vector_t<T>& operator+=(const Vector_t<T>& other)
		{
			this->x_ += other.x_;
			return *this;
		}

		Vector_t<T> operator-(const Vector_t<T>& other)
		{
			Vector_t<T> vec;
			vec.x_ = this->x_ - other.x_;
			return vec;
		}

		const Vector_t<T>& operator-=(const Vector_t<T>& other)
		{
			this->x_ -= other.x_;
			return *this;
		}

		bool operator==(const Vector_t<T>& other)
		{
			return this->x_ == other.x_;
		}

		bool operator!=(const Vector_t<T>& other)
		{
			return !(*this == other);
		}

		Vector_t<T>& operator=(const Vector_t<T>& other)
		{
			this->x_ = other.x_;
			return *this;
		}
	};

	template <typename T>
	struct Vector2_t
	{
		T x_;
		T y_;

		Vector2_t<T> operator+(const Vector2_t<T>& other)
		{
			Vector2_t<T> vec;
			vec.x_ = this->x_ + other.x_;
			vec.y_ = this->y_ + other.y_;
			return vec;
		}

		const Vector2_t<T>& operator+=(const Vector2_t<T>& other)
		{
			this->x_ += other.x_;
			this->y_ += other.y_;
			return *this;
		}
	};

	template <typename T>
	struct Vector3_t
	{
		T x_;
		T y_;
		T z_;
	};

	template <typename T>
	struct Vector4_t
	{
		T x_;
		T y_;
		T z_;
		T w_;
	};

	using Vector = Vector_t<float>;
	using Vector2 = Vector2_t<float>;
	using Vector3 = Vector3_t<float>;
	using Vector4 = Vector4_t<float>;


	using Vec = Vector;
	using Vec2 = Vector2;
	using Vec3 = Vector3;
	using Vec4 = Vector4;

	struct Extend2
	{
		uint32_t width_;
		uint32_t height_;
	};

	struct Extend3
	{
		uint32_t width_;
		uint32_t height_;
		uint32_t depth_;
	};

	struct Extend_i : Vector_t<int>
	{
	};

	struct Extend2_i : Vector2_t<int>
	{
	};

	struct Extend3_i : Vector3_t<int>
	{
	};

	struct Extend_u : Vector_t<uint32_t>
	{
	};

	struct Extend2_u : Vector2_t<uint32_t>
	{
	};

	struct Extend3_u : Vector3_t<uint32_t>
	{
	};
}
