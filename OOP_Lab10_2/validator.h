#pragma once
#include <string>
#include "Domain.h"
#include <vector>
#include <ctime>
#include <chrono>
#include <cassert>

using std::vector;
using std::string;
using std::ostream;

class ValidateException {
	vector<string> msgs;
public:
	ValidateException(const vector<string>& errors) :msgs{ errors } {}
	string getMsg()const {
		string msg_str = "";
		for (auto& ms : msgs) {
			msg_str = msg_str + ms;
		}
		return "ValidateException: " + msg_str;
	}	
	//functie friend (vreau sa folosesc membru privat msg)
	friend ostream& operator<<(ostream& out, const ValidateException& ex);
};

ostream& operator<<(ostream& out, const ValidateException& ex);

class Validator {

public:
	/**
	Functia validate verifica daca obiectul de tip Movie dat ca parametru este valid sau nu, adaugand mesaje corespunzatoare in lista de mesaje de eroare in cazul in care nu este valid.
	Parametri:
	m: obiectul Movie de validat (const Movie&)
	Return: nu returneaza nimic, dar arunca o exceptie de tip ValidateException in cazul in care obiectul nu este valid
	Exceptie: ValidateException. Mesaje de eroare posibile:
		"Titlu prea scurt. "
		"Gen prea scurt. "
		"Actor principal prea scurt. "
		"Anul nu este valid. "
		"Genul nu este valid. (Trebuie sa fie: actiune, comedie, drama, horror, musical, science fiction, thriller sau western)"
	*/
	void validate(const Movie& m);
};