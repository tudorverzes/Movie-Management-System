#pragma once
#include "domain.h"
#include "repository.h"
#include "validator.h"
#include "observer.h"
#include <string>
#include <vector>
#include <functional>
#include <map>

using std::vector;
using std::map;


class CosService : public Observable {
	Repo& repo;
	Repo& database;

public:
	CosService(Repo& repo, Repo& database) : repo{ repo }, database{ database } {}

	CosService(const CosService& ot) = delete;

	const vector<Movie> getAllFromCart() noexcept;

	void addMovieInCart(const string& title, int year);

	void deleteMovieFromCart(const string& title, int year);

	void deleteAllInCart();

	int addRandomInCart(int n);

	void printInFileAsCSV(string filePath);

	void printInFileAsHTML(string filePath);

	int sizeOfRepo();

	int sizeOfDatabase();

	int numberOfMoviesWithGenre(string genre) const;

	map<std::string, int> getGenreCounts() const;
};

