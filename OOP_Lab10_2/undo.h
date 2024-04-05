#pragma once
#include "domain.h"
#include "repository.h"

class DoUndo {
public:
	virtual void doUndo() = 0;
	virtual ~DoUndo() {};
};

class UndoAdd : public DoUndo {
	Movie addedMovie;
	Repo& repo;
public:
	UndoAdd(Repo& repo, const Movie& m) : repo{ repo }, addedMovie{ m } {}
	void doUndo() override {
		repo.delete_element(addedMovie.getTitle(), addedMovie.getYear());
	}
};

class UndoDelete :public DoUndo {
	Movie deletedMovie;
	Repo& repo;
public:
	UndoDelete(Repo& repo, const Movie& m) : repo{ repo }, deletedMovie{ m } {}
	void doUndo() override {
		repo.store(deletedMovie);
	}
};

class UndoModify :public DoUndo {
	Movie oldMovie, newMovie;
	Repo& repo;
public:
	UndoModify(Repo& repo, const Movie& old_movie, const Movie& new_movie) : repo{ repo }, oldMovie{ old_movie }, newMovie{ new_movie } {}
	void doUndo() override {
		repo.modify(newMovie.getTitle(), newMovie.getYear(), oldMovie);
	}
};


class UndoException {
	string msg;
public:
	UndoException(string m) : msg{ m } {}
	string getMsg()const { return "UndoException: " + msg; }
	friend ostream& operator<<(ostream& out, const UndoException& ex);

};

ostream& operator<<(ostream& out, const UndoException& ex);


