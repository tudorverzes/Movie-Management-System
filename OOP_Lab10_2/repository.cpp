#include "repository.h"
#include <assert.h>
#include <iostream>
#include <sstream>

using std::ostream;
using std::stringstream;

bool Repo::exist(const Movie& m) const {
	try {
		find(m.getTitle(), m.getYear());
		return true;
	}
	catch (RepoException) {
		return false;
	}
}

void Repo::store(const Movie& m) {
	if (exist(m)) {
		throw RepoException("Elementul exista deja in lista.");
	}
	database.push_back(m);

}

const Movie& Repo::delete_element(string title, int year) {
	auto it = std::find_if(database.begin(), database.end(), [title, year](const Movie& m) {
		return m.getTitle() == title && m.getYear() == year;
		});

	if (it != database.end()) {
		const Movie& deleted_movie = *it;
		database.erase(it);
		return deleted_movie;
	}

	throw RepoException("Elementul cautat nu a fost gasit.");
}

const Movie& Repo::modify(string title, int year, const Movie& m) {
	auto it = std::find_if(database.begin(), database.end(), [title, year](const Movie& m) {
		return m.getTitle() == title && m.getYear() == year;
		});

	if (it != database.end()) {
		const Movie& modified_movie = *it;
		*it = m;
		return modified_movie;
	}
	throw RepoException("Elementul cautat nu a fost gasit.");
}

const Movie& Repo::find(string title, int year) const {
	for (const auto& m : database) {
		if (m.getTitle() == title && m.getYear() == year) {
			return m;
		}
	}
	throw RepoException("Elementul cautat nu a fost gasit.");
}

void Repo::deleteAll() {
	database.clear();
}

const Movie& Repo::get(int index) const {
	return database[index];
}

const vector<Movie>& Repo::getAll() const noexcept {
	return database;
}

ostream& operator<<(ostream& out, const RepoException& ex) {
	out << "\033[31m" << "RepositoryException: " << ex.msg << "\033[0m";
	return out;
}
