#pragma once

#include "header.h"

class iCard {
	static iCard* _trump;

	std::string _suit;
	std::string _rank;
	iCard(std::string, std::string);

public:
	friend class CardManager;

	std::string suit(void) const;
	std::string rank(void) const;
	static int compare(const iCard*, const iCard*);
	bool operator>(const iCard*&);
	bool operator<(const iCard*&);
	bool operator>=(const iCard*&);
	bool operator<=(const iCard*&);
	bool operator==(const iCard*&);
	friend std::ostream& operator<<(std::ostream&, const iCard&);
	bool is_trump() const; // Return if it's trump card
};

// Specialized hash support for pair<string, string>
namespace std {
	template<>
	class hash<std::pair<std::string, std::string>> {
	public:
		std::size_t operator()(const std::pair<std::string, std::string>& obj) const {
			std::hash<std::string> first;
			std::hash<std::string> second;
			return first(obj.first) + second(obj.second);
		}
	};
}

class CardManager {
	std::vector<iCard*> all;
	std::unordered_map<std::pair<std::string, std::string>, iCard*> library;

public:
	CardManager(void);
	~CardManager(void);
	std::vector<iCard*> getall(void);
	iCard* get(std::string&, std::string&);
    void set_trump(iCard* trump);
	iCard* trump();
	// translate card* to icard*
    iCard* icard(const Card* card);
};
