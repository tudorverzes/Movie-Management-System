#pragma once
#include "domain.h"
#include "repository.h"
#include "validator.h"
#include "undo.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <algorithm>
#include <memory>


using std::vector;
using std::function;
using std::ifstream;
using std::unique_ptr;


class Service {
	Repo& repo;
	Validator& val;

	std::vector<unique_ptr<DoUndo>> undoActions;

private:

	/**
	Funcția toLowercase transformă un șir de caractere în litere mici.
	Parametri:
	str: șirul de caractere ce trebuie transformat (const string&)
	Return: șirul de caractere cu litere mici (string)
	*/
	string toLowercase(const string& str);

	/**
	Funcția levenshteinDistance calculează distanța Levenshtein între două șiruri de caractere.
	Parametri:
	s1: primul șir de caractere (const string&)
	s2: al doilea șir de caractere (const string&)
	Return: distanța Levenshtein dintre cele două șiruri de caractere (int)
	*/
	int levenshteinDistance(const string& s1, const string& s2);

public:
	Service(Repo& repo, Validator& val) : repo{ repo }, val{ val } {}

	Service(const Service& ot) = delete;

	/**
	Functia getAll returneaza toate filmele din repo.
	Return: un vector de obiecte de tipul Movie ce contine toate filmele din repository (const vector<Movie>&)
	*/
	const vector<Movie> getAll() noexcept;

	/**
	Functia addMovie adauga un nou film in repository dupa validarea sa.
	Parametri:
	title: titlul filmului (const string&)
	genre: genul filmului (const string&)
	year: anul de lansare a filmului (int)
	main_actor: actorul principal din film (const string&)
	Return: N/A
	*/
	void addMovie(const string& title, const string& genre, int year, const string& main_actor);

	/**
	Functia deleteMovie sterge un film din repository.
	Parametri:
	title: titlul filmului ce trebuie sters (const string&)
	year: anul de lansare al filmului ce trebuie sters (int)
	Return: obiectul Movie sters (const Movie&)
	*/
	const Movie& deleteMovie(const string& title, int year);

	/**
	Functia modifyMovie modifica un film existent din repository.
	Parametri:
	title: titlul filmului ce trebuie modificat (const string&)
	year: anul de lansare al filmului ce trebuie modificat (int)
	new_title: noul titlu al filmului (const string&)
	new_genre: noul gen al filmului (const string&)
	new_year: noul an de lansare al filmului (int)
	new_main_actor: noul actor principal din film (const string&)
	Return: obiectul Movie modificat (const Movie&)
	*/
	const Movie& modifyMovie(const string& title, int year, const string& new_title, const string& new_genre, int new_year, const string& new_main_actor);

	/**
	Functia filterByTitle returneaza toate filmele din repository cu un anumit titlu.
	Parametri:
	title: titlul dupa care se filtreaza filmele (const string&)
	Return: un vector de obiecte de tipul Movie ce contine toate filmele din repository cu titlul specificat (vector<Movie>)
	*/
	vector<Movie> filterByTitle(const string& title);

	/**
	Functia filterByYears filtreaza filmele din repository-ul curent pe baza anilor de productie, returnand o noua lista de filme.
	Parametri:
	start_year: anul de inceput pentru filtrare (int)
	end_year: anul de sfarsit pentru filtrare (int)
	Return: vectorul de obiecte de tipul Movie care respecta criteriul specificat (vector<Movie>)
	*/
	vector<Movie> filterByYears(int start_year, int end_year);

	/**
	Functia filterByGenre returneaza un vector cu toate filmele din repo cu genul specificat.
	Parametri:
	genre: genul dupa care se filtreaza (string)
	Return: un vector de obiecte Movie, reprezentand filmele filtrate dupa gen
	*/
	vector<Movie> filterByGenre(const string& genre);

	/**
	Functia filterByMainActor returneaza un vector cu toate filmele din repo cu actorul principal specificat.
	Parametri:
	actor: numele actorului principal dupa care se filtreaza (string)
	Return: un vector de obiecte Movie, reprezentand filmele filtrate dupa actorul principal
	*/
	vector<Movie> filterByMainActor(const string& actor);

	/**
	Functia generalSort sorteaza toate filmele din repo conform unui criteriu specificat.
	Parametri:
	maiMicF: functie pointer catre o functie care specifica criteriul de sortare (bool(*maiMicF)(const Movie&, const Movie&))
	Return: un vector de obiecte Movie, reprezentand lista de filme sortata conform criteriului specificat
	*/
	vector<Movie> generalSort(bool(*maiMicF)(const Movie&, const Movie&));

	/**
	Functia sortByTitle sorteaza toate filmele din repo in ordine alfabetica dupa titlu.
	Parametri: Niciunul
	Return: un vector de obiecte Movie, reprezentand lista de filme sortata dupa titlu
	*/
	vector<Movie> sortByTitle();

	/**
	Functia sortByGenre sorteaza toate filmele din repo in ordine alfabetica dupa gen.
	Parametri: Niciunul
	Return: un vector de obiecte Movie, reprezentand lista de filme sortata dupa gen
	*/
	vector<Movie> sortByGenre();

	/**
	Functia sortByYear sorteaza toate filmele din repo in ordine crescatoare dupa anul de lansare.
	Parametri: Niciunul
	Return: un vector de obiecte Movie, reprezentand lista de filme sortata dupa anul de lansare
	*/
	vector<Movie> sortByYear();

	/**
	Functia sortByMainActor sorteaza toate filmele din repo in ordine alfabetica dupa numele actorului principal.
	Parametri: Niciunul
	Return: un vector de obiecte Movie, reprezentand lista de filme sortata dupa numele actorului principal
	*/
	vector<Movie> sortByMainActor();

	/**
	Funcția loadFromFile încarcă filme dintr-un fișier dat în repository.
	Parametri:
	filename: numele fișierului de intrare (string)
	Return: numărul de filme adăugate în repository (int)
	Aruncă o excepție de tipul FileException în cazul în care fișierul dat nu poate fi deschis sau nu conține filme valide.
	*/
	int loadFromFile(string filename);

	/**
	Funcția undo efectuează operația inversă ultimei acțiuni efectuate asupra repository-ului.
	Aruncă o excepție de tipul UndoException în cazul în care nu mai există acțiuni de anulare disponibile.
	*/
	void undo();

	/**
	Functia find cauta un film in baza de date dupa nume si anul de lansare.
	Parametri:
	title: numele filmului cautat (string)
	year: anul de lansare al filmului cautat (int)
	Return: obiect de tip Movie care reprezinta filmul cautat (const Movie&)
	*/
	const Movie& findMovie(string title, int year) const;
};

class FileException {
	string msg;
public:
	FileException(string m) : msg{ m } {}
	string getMsg()const { return "FileException: " + msg; }
	friend ostream& operator<<(ostream& out, const FileException& ex);

};

ostream& operator<<(ostream& out, const FileException& ex);

