#pragma once
#include "domain.h"

#include <vector>
#include <string>
#include <ostream>
#include <algorithm>

using std::vector;
using std::string;
using std::ostream;

class Repo {

public:
	vector<Movie> database;
	/**
	Functia exist verifica daca un film exista deja in baza de date.
	Parametri:
	m: obiect de tip Movie care reprezinta filmul cautat (const Movie&)
	Return: true daca filmul exista in baza de date, false altfel (bool)
	*/
	bool exist(const Movie& m) const;

	Repo() = default;
	Repo(const Repo& ot) = delete;

	/**
	Functia store adauga un nou film in baza de date, verificand mai intai daca acesta exista deja.
	Parametri:
	m: obiect de tip Movie care reprezinta filmul de adaugat (const Movie&)
	Return: nu exista (void)
	Aruncare exceptie: RepoException cu mesajul "Elementul exista deja in lista." daca filmul exista deja in baza de date.
	*/
	void store(const Movie& m);

	/**
	Functia delete_element sterge un film din baza de date, identificand filmul dupa nume si anul de lansare.
	Parametri:
	title: numele filmului de sters (string)
	year: anul de lansare al filmului de sters (int)
	Return: obiect de tip Movie care reprezinta filmul sters (const Movie&)
	Aruncare exceptie: RepoException cu mesajul "Elementul cautat nu a fost gasit." daca filmul nu exista in baza de date.
	*/
	const Movie& delete_element(string title, int year);

	/**
	Functia modify modifica informatiile despre un film existent in baza de date, identificand filmul dupa nume si anul de lansare.
	Parametri:
	title: numele filmului de modificat (string)
	year: anul de lansare al filmului de modificat (int)
	m: obiect de tip Movie care contine noile informatii despre film (const Movie&)
	Return: obiect de tip Movie care reprezinta filmul modificat (const Movie&)
	Aruncare exceptie: RepoException cu mesajul "Elementul cautat nu a fost gasit." daca filmul nu exista in baza de date.
	*/
	const Movie& modify(string title, int year, const Movie& m);

	/**
	Functia find cauta un film in baza de date dupa nume si anul de lansare.
	Parametri:
	title: numele filmului cautat (string)
	year: anul de lansare al filmului cautat (int)
	Return: obiect de tip Movie care reprezinta filmul cautat (const Movie&)
	Aruncare exceptie: RepoException cu mesajul "Elementul cautat nu a fost gasit." daca filmul nu exista in baza de date.
	*/
	const Movie& find(string title, int year) const;

	/**
	Funcția deleteAll șterge toate obiectele din baza de date.
	*/
	void deleteAll();

	const Movie& get(int index) const;

	/**
	Functia getAll returneaza toate filmele din baza de date.
	Parametri: nu exista
	Return: vector de obiecte de tip Movie care reprezinta toate filmele din baza de date (const vector<Movie>&)
	Aruncare exceptie: nu exista
	*/
	const vector<Movie>& getAll() const noexcept;
};

class RepoException {
	string msg;
public:
	RepoException(string m) : msg{ m } {}
	string getMsg()const { return "RepositoryException: " + msg; }
	friend ostream& operator<<(ostream& out, const RepoException& ex);

};

ostream& operator<<(ostream& out, const RepoException& ex);


