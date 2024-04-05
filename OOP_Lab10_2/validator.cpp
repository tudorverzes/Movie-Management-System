#include "validator.h"
#include <assert.h>
#include <sstream>



void Validator::validate(const Movie& m) {
	vector<string> msgs;

	//anul curent
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm = *std::localtime(&t);
	int curYear = tm.tm_year + 1900;

	if (m.getTitle().size() <= 1)
		msgs.push_back("Titlu prea scurt. ");
	if (m.getGenre().size() <= 1)
		msgs.push_back("Gen prea scurt. ");
	if (m.getMainActor().size() <= 1)
		msgs.push_back("Actor principal prea scurt. ");
	if (m.getYear() < 1850 || m.getYear() > curYear + 20)
		msgs.push_back("Anul nu este valid. ");
	if (m.getGenre() != "actiune" && m.getGenre() != "comedie" && m.getGenre() != "drama" && m.getGenre() != "horror" && m.getGenre() != "musical" && m.getGenre() != "science fiction" && m.getGenre() != "thriller" && m.getGenre() != "western" && m.getGenre() != "documentar" && m.getGenre() != "animatie") {
		msgs.push_back("Genul nu este valid. (Trebuie sa fie: actiune, comedie, drama, horror, musical, science fiction, thriller, western, documentar sau animatie)");
	}
	if (msgs.size() > 0) {
		throw ValidateException(msgs);
	}
}

ostream& operator<<(ostream& out, const ValidateException& ex) {
	out << "\033[31m" << "ValidateException: ";
	for (const auto& msg : ex.msgs) {
		out << msg;
	}
	out << "\033[0m";
	return out;
}
