#pragma once
#include <string>
#include <iostream>
#include <cassert>

using std::string;
using std::cout;

class Movie {

private:
	std::string title;
	std::string genre;
	int year;
	std::string main_actor;

public:
	Movie() = default;
	Movie(const string t, const string g, int y, const string ma) : title{ t }, genre{ g }, year{ y }, main_actor{ ma } {}

	/**
	Functia getTitle returneaza titlul filmului.
	Return: titlu (string)
	*/
	string getTitle() const;

	/**
	Functia getGenre returneaza genul filmului.
	Return: gen (string)
	*/
	string getGenre() const;

	/**
	Functia getYear returneaza anul de aparitie al filmului.
	Return: an (int)
	*/
	int getYear() const;

	/**
	Functia getMainActor returneaza numele actorului principal din film.
	Return: nume actor principal (string)
	*/
	string getMainActor() const;

	void setTitle(const std::string& t);
	void setGenre(const std::string& g);
	void setYear(int y);
	void setMainActor(const std::string& ma);

	~Movie() = default;

};