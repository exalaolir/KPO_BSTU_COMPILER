#pragma once
#include "pch.h"
#include "Automat.h"

namespace Utils
{
	template<typename Item>
	void Concat(std::vector<Item>& a, std::vector<Item>& b)
	{
		a.reserve(a.size() + b.size());
		a.insert(a.end(), b.begin(), b.end());
	}

	template<typename Item>
	void Concat(std::list<Item>& a, std::list<Item>& b)
	{
		a.splice(a.end(), b);
	}


	template<typename T>
	T Pop(std::stack<T>& stack)
	{
		T result = stack.top();
		stack.pop();
		return result;
	}

	template<typename T>
	T Pop(std::list<T>& list)
	{
		T result = list.back();
		list.pop_back();
		return result;
	}

	template<typename T>
	T Pop(std::queue<T>& stack)
	{
		T result = stack.front();
		stack.pop();
		return result;
	}

	template<typename T> bool Contains(T value, std::stack<T> stack)
	{
		while (!stack.empty())
		{
			if (value == Pop(stack)) return true;
		}
		return false;
	}

	template<typename T> bool Contains(T value, std::queue<T> stack)
	{
		while (!stack.empty())
		{
			if (value == Pop(stack)) return true;
		}
		return false;
	}

	template<typename T> bool Contains(T value, std::vector<T> vector)
	{
		for (size_t i = 0; i < vector.size(); i++)
		{
			if (value == vector[i]) return true;
		}
		return false;
	}

	template<typename T> bool Contains(T value, std::list<T> list)
	{
		return std::find(list.begin(), list.end(), value) != list.end();
	}

	template<typename T> void UnionMap(std::unordered_map<T, std::vector<T>>& output, std::unordered_map<T, std::vector<T>>& input)
	{
		for (auto& [key, value] : input)
		{
			output[key] = value;
		}
	}

	template<typename T, typename K> bool TryGetValue(T& key, std::map<T, K>& map, K& result)
	{
		if (map.contains(key))
		{
			result = map[key];
			return true;
		}
		return false;
	}

	template<typename T, typename K> bool TryGetValue(T& key, const std::map<T, K>& map, K& result)
	{
		if (map.contains(key))
		{
			result = map.at(key);
			return true;
		}
		return false;
	}

	template<typename T> std::list<std::string> Split(const std::string& s, T delim) {
		std::list<std::string> result;
		std::stringstream ss(s);
		std::string item;
		Regex::Regex regex;
		while (getline(ss, item, delim))
		{
			if (item != "" && item[0] != '#') result.push_back(item);
		}

		return result;
	}
}
