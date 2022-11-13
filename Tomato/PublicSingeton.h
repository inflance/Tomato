#pragma once

template<typename T>
class PublicSingeton
{
public:
	static T& GetInstance()
	{
		static T s_instance;
		return s_instance;
	}

protected:
	PublicSingeton() {}
	~PublicSingeton() {}

private:
	PublicSingeton(const PublicSingeton& orther){};
	PublicSingeton operator=(const PublicSingeton& orther){};
};