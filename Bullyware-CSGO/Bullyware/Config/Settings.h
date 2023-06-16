#pragma once
#include <string>
#include <map>
#include <any>

class CConfig
{
protected:
	std::map<std::string, std::any> Config = {
		{"Player.DynamicBoxes", false},
		{"Player.Name", false},
		{"Player.Health", false},
		{"Filter.Dead", false},
		{"Filter.Dormant", false},
		{"Filter.Team", false},
	};
public:
	template <typename T>
	T& Get(std::string key)
	{
		std::any& val = Config[key];
		return std::any_cast<T&>(val);
	}

	template <typename T>
	void Set(std::string key, T value)
	{
		Config[key] = value;
	}
};

extern CConfig Config;