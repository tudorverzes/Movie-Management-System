#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGUI.h"
#include <qwidget.h>
#include <qlistwidget.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <QPainter>
#include "service.h"
#include "cosService.h"
#include "domain.h"
#include "observer.h"
#include <vector>


class QtGUI : public QWidget {
	Q_OBJECT
private:
	Service& ctr;
	CosService& cosCtr;

	QListWidget* lst;

	QPushButton* btnUndo;
	QPushButton* btnCart;
	QPushButton* btnStatistics;
	QPushButton* btnSearch;
	QLineEdit* txtSearch;

	QLineEdit* txtTitle;
	QLineEdit* txtGenre;
	QLineEdit* txtYear;
	QLineEdit* txtMainActor;

	QPushButton* btnAdd;
	QPushButton* btnDelete;
	QPushButton* btnModify;
	QPushButton* btnAddToCart;
	QPushButton* btnClearCart;

	QPushButton* btnSortByTitle;
	QPushButton* btnSortByYears;
	QPushButton* btnSortByGenre;
	QPushButton* btnResetSort;

	void initGUICmps();
	void connectSignalsSlots();
	void reloadList(std::vector<Movie> database, string criteria = "");
	void addMovieGUI();
	void deleteMovieGUI();
	void handleSearch();
	void modifyMovieGUI();
	void addToCartGUI();
	void openCartGUI();

public:
	QtGUI(Service& ctr, CosService& cosCtr) :ctr{ ctr }, cosCtr { cosCtr } {
		initGUICmps();
		connectSignalsSlots();
		ctr.loadFromFile("database.csv");
		reloadList(ctr.getAll());
	}

	~QtGUI() {}
signals:
	void changedCartSignal();
	void changedListSignal();

};

class QtGUIModify : public QWidget {
	Q_OBJECT
private:
	Service& ctr;
	Movie movie;

	QLineEdit* txtNewTitle;
	QLineEdit* txtNewGenre;
	QLineEdit* txtNewYear;
	QLineEdit* txtNewMainActor;

	QPushButton* cancelButton;
	QPushButton* modifyButton;

	void initGUICmps();
	void modifyMovie();
public:
	QtGUIModify(Service& ctr, Movie movie) :ctr{ ctr }, movie{ movie } {
		initGUICmps();
	}

	~QtGUIModify() {}
signals:
	void windowClosed();
};

class QtGUICart : public QWidget, public Observer {
	Q_OBJECT
private:
	CosService& cosCtr;

	QListWidget* lst;

	QPushButton* btnDelete;
	QPushButton* btnRandom;
	QPushButton* btnExport;

	QLineEdit* txtRandom;
	QLabel* moviesCount;

	void initGUICmps();
	void connectSignalsSlots();
	void reloadList(std::vector<Movie> database);
	void handleRandom();
public slots:
	void cartChanged();
	void listChanged();
public:
	QtGUICart(CosService& cosCtr) :cosCtr{ cosCtr } {
		initGUICmps();
		connectSignalsSlots();
		reloadList(cosCtr.getAllFromCart());
	}

	~QtGUICart() {
		cosCtr.removeObserver(this);
	}

	void update() override;
};


class QtGUICartStatistics : public QWidget, public Observer {
private:
	CosService& cosCtr;
	
	//QChartView* chartView;
	//QChart* chart;
	//QBarSeries* series;


	void initGUICmps();
	void reloadChart(std::vector<Movie> database);

public:
	QtGUICartStatistics(CosService& cosCtr) : cosCtr{ cosCtr } {
		initGUICmps();
		cosCtr.addObserver(this);
	}

	~QtGUICartStatistics() {
		cosCtr.removeObserver(this);
	}

	void update() override;
};

class QtGUICartPainter : public QWidget, public Observer{
private:
	CosService& cosCtr;
public:

	QtGUICartPainter(CosService& cosCtr) : cosCtr{ cosCtr } {
		setWindowTitle("Statistici coș");
		setWindowIcon(QIcon("clipchamp.png"));
		cosCtr.addObserver(this);
	}

	~QtGUICartPainter() {
		cosCtr.removeObserver(this);
	}

	void paintEvent(QPaintEvent* ev) override;

	void update() override;
};