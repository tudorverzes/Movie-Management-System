#include "service.h"
#include <functional>
#include <algorithm>
#include <assert.h>
#include <string>
#include <utility>
#include <sstream>

using std::stringstream;
using std::getline;
using std::pair;

const vector<Movie> Service::getAll() noexcept {
	return repo.getAll();
}

void Service::addMovie(const string& title, const string& genre, int year, const string& main_actor) {
	Movie m{ title, genre, year, main_actor };
	val.validate(m);
	repo.store(m);

	undoActions.push_back(std::make_unique<UndoAdd>(repo, m));
}

const Movie& Service::deleteMovie(const string& title, int year) {
	Movie m = repo.find(title, year);
	repo.delete_element(title, year);
	undoActions.push_back(std::make_unique<UndoDelete>(repo, m));
	return m;
}

const Movie& Service::modifyMovie(const string& title, int year, const string& new_title, const string& new_genre, int new_year, const string& new_main_actor) {
	Movie m{ new_title, new_genre, new_year, new_main_actor };
	val.validate(m);
	Movie old_movie = repo.find(title, year);
	repo.modify(title, year, m);

	undoActions.push_back(std::make_unique<UndoModify>(repo, old_movie, m));

	return old_movie;
}

void Service::undo() {
	if (undoActions.empty()) {
		throw UndoException("Nu mai exista operatii.");
	}
	undoActions.back()->doUndo();
	undoActions.pop_back();
}

const Movie& Service::findMovie(string title, int year) const {
	return repo.find(title, year);
}

string Service::toLowercase(const string& str) {
	string str_lowercase;
	transform(str.begin(), str.end(), back_inserter(str_lowercase), [](char c) {
		return tolower(c);
		});
	return str_lowercase;
}

vector<Movie> Service::filterByTitle(const string& title) {
	vector<pair<Movie, int>> new_data;
	string title_lowercase = toLowercase(title);
	for (const auto& movie : repo.getAll()) {
		string movie_title_lowercase = toLowercase(movie.getTitle());
		size_t pos = movie_title_lowercase.find(title_lowercase);
		if (pos != string::npos) {
			new_data.emplace_back(movie, pos); // add the movie to the result vector with its score
		}
		else {
			int distance = levenshteinDistance(movie_title_lowercase, title_lowercase);
			if (distance <= 3) {
				new_data.emplace_back(movie, distance); // add the movie to the result vector with its score
			}
		}
	}
	sort(new_data.begin(), new_data.end(), [](const pair<Movie, int>& p1, const pair<Movie, int>& p2) {
		return p1.second < p2.second; // sort the movies based on their score
		});
	vector<Movie> result;
	for (const auto& p : new_data) {
		result.push_back(p.first); // extract the movies from the result vector
	}
	return result;
}


int Service::levenshteinDistance(const string& s1, const string& s2) {
	const int m = s1.size();
	const int n = s2.size();
	vector<vector<int>> d(m + 1, vector<int>(n + 1));

	for (int i = 0; i <= m; i++) {
		d[i][0] = i;
	}
	for (int j = 0; j <= n; j++) {
		d[0][j] = j;
	}

	for (int j = 1; j <= n; j++) {
		for (int i = 1; i <= m; i++) {
			int substitutionCost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
			d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + substitutionCost });
		}
	}

	return d[m][n];
}

vector<Movie> Service::filterByYears(int start_year, int end_year) {
	vector<Movie> new_data;
	for (const auto& m : repo.getAll()) {
		if (m.getYear() >= start_year && m.getYear() <= end_year) {
			new_data.push_back(m);
		}
	}
	return new_data;
}

vector<Movie> Service::filterByGenre(const string& genre) {
	vector<Movie> new_data(repo.getAll().size());
	auto it = std::copy_if(repo.getAll().begin(), repo.getAll().end(), new_data.begin(), [genre](const Movie& m) {
		return m.getGenre() == genre;
		});
	new_data.resize(std::distance(new_data.begin(), it));
	return new_data;
}

vector<Movie> Service::filterByMainActor(const string& actor) {
	vector<Movie> new_data;
	for (const auto& movie : repo.getAll()) {
		if (movie.getMainActor().find(actor) != string::npos) {
			new_data.push_back(movie);
		}
	}
	return new_data;
}

vector<Movie> Service::generalSort(bool(*maiMicF)(const Movie&, const Movie&)) {
	vector<Movie> v{ repo.getAll() };
	for (size_t i = 0; i < v.size(); i++) {
		for (size_t j = i + 1; j < v.size(); j++) {
			if (!maiMicF(v[i], v[j])) {
				Movie aux = v[i];
				v[i] = v[j];
				v[j] = aux;
			}
		}
	}
	return v;
}

vector<Movie> Service::sortByTitle() {
	return generalSort([](const Movie& m1, const Movie& m2) {
		return m1.getTitle() < m2.getTitle();
		});
}

vector<Movie> Service::sortByGenre() {
	return generalSort([](const Movie& m1, const Movie& m2) {
		return m1.getGenre() < m2.getGenre();
		});
}

vector<Movie> Service::sortByYear() {
	return generalSort([](const Movie& m1, const Movie& m2) {
		return m1.getYear() < m2.getYear();
		});
}

vector<Movie> Service::sortByMainActor() {
	return generalSort([](const Movie& m1, const Movie& m2) {
		return m1.getMainActor() < m2.getMainActor();
		});
}

int Service::loadFromFile(string filename) {
	int count = 0;
	ifstream file(filename);
	if (!file.is_open()) {
		throw FileException("Fisierul " + filename + " nu exista.");
	}

	string line;
	getline(file, line);
	while (getline(file, line)) {
		string title, genre, main_actor;
		int year;

		stringstream ss(line);

		if (!getline(ss, title, ',')) {
			throw FileException("Format invalid!");
		}

		if (!getline(ss, genre, ',')) {
			throw FileException("Format invalid!");
		}

		if (!(ss >> year)) {
			throw FileException("Format invalid!");
		}

		if (!getline(ss, main_actor)) {
			throw FileException("Format invalid!");
		}
		main_actor.erase(0, 1);
		Movie m = { title,genre,year,main_actor };
		try {
			if (repo.exist(m) == false) {
				val.validate(m);
				repo.store(m);
				undoActions.push_back(std::make_unique<UndoAdd>(repo, m));
				count++;
			}
		}
		catch (ValidateException) {
			continue;
		}
	}
	if (count == 0) {
		throw FileException("Niciun film valid nu a fost gasit in fisier.");
	}
	return count;
}

ostream& operator<<(ostream& out, const FileException& ex) {
	out << "\033[31m" << "FileException: " << ex.msg << "\033[0m";
	return out;
}
