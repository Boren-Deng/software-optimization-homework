#pragma once
#ifndef MANAGEMENTSYSTEM_H
#include<map>
template<typename K,typename V>
class ManagementSystem
{
public:
	ManagementSystem() = default;
	ManagementSystem(const ManagementSystem&) = default;
	ManagementSystem& operator=(const ManagementSystem&) = default;
	ManagementSystem(ManagementSystem&&) = default;
	ManagementSystem& operator=(ManagementSystem&&) = default;
	~ManagementSystem() = default;
	void person_insert(std::pair<K, V>& entry)
	{
		idNumberToInformation.insert(std::move(entry));
	}
	V& operator[](K& key)
	{
		return idNumberToInformation[key];
	}
	V* person_search(const K& key)
	{
		auto iter = idNumberToInformation.find(key);
		if (iter != idNumberToInformation.end())
			return &(iter->second);
		else
			return nullptr;
	}
	void clearData()
	{
		idNumberToInformation.clear();
	}
private:
	std::map<K, V>idNumberToInformation;
};

#endif // !MANAGEMENTSYSTEM_H

