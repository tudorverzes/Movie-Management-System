#include "domain.h"

string Movie::getTitle() const {
	return title;
}

string Movie::getGenre() const {
	return genre;
}

int Movie::getYear() const {
	return year;
}

string Movie::getMainActor() const {
	return main_actor;
}

void Movie::setTitle(const std::string& t) {
	title = t;
}

void Movie::setGenre(const std::string& g) {
	genre = g;
}

void Movie::setYear(int y) {
	year = y;
}

void Movie::setMainActor(const std::string& ma) {
	main_actor = ma;
}