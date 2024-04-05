#include "QtGUI.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <random>
#include <string>

void QtGUI::initGUICmps() {
	setWindowTitle("Gestiune filme");
	setWindowIcon(QIcon("clipchamp.png"));


	QHBoxLayout* ly = new QHBoxLayout;
	setLayout(ly);

	//search + list + sort buttons
	QWidget* widDreapta = new QWidget;
	QVBoxLayout* vl = new QVBoxLayout;

	widDreapta->setLayout(vl);
	
	//search
	QWidget* widSearch = new QWidget;
	QHBoxLayout* lySearch = new QHBoxLayout;

	widSearch->setLayout(lySearch);
	txtSearch = new QLineEdit;
	txtSearch->setPlaceholderText("Caută...");
	btnSearch = new QPushButton("Caută");

	btnUndo = new QPushButton(QIcon("undo.svg"), "");
	lySearch->addWidget(btnUndo);
	btnCart = new QPushButton(QIcon("cart.svg"), "");
	lySearch->addWidget(btnCart);
	btnStatistics = new QPushButton(QIcon("ins.svg"), "");
	lySearch->addWidget(btnStatistics);

	lySearch->addWidget(txtSearch);
	lySearch->addWidget(btnSearch);
	
	vl->addWidget(widSearch);

	//list
	lst = new QListWidget;
	vl->addWidget(lst);

	QWidget* widBtnDreapta;
	widBtnDreapta = new QWidget;
	QHBoxLayout* lyBtnsDr = new QHBoxLayout;
	widBtnDreapta->setLayout(lyBtnsDr);

	//buttons
	btnAddToCart = new QPushButton(QIcon("add_cart.svg"), "");
	lyBtnsDr->addWidget(btnAddToCart);

	btnClearCart = new QPushButton(QIcon("clear_cart.svg"), "");
	lyBtnsDr->addWidget(btnClearCart);

	btnSortByTitle = new QPushButton("Sortează după titlu");
	lyBtnsDr->addWidget(btnSortByTitle);

	btnSortByGenre = new QPushButton("Sortează după gen");
	lyBtnsDr->addWidget(btnSortByGenre);

	btnSortByYears = new QPushButton("Sortează după an");
	lyBtnsDr->addWidget(btnSortByYears);

	btnResetSort = new QPushButton("Resetează");
	lyBtnsDr->addWidget(btnResetSort);


	vl->addWidget(widBtnDreapta);
	ly->addWidget(widDreapta);

	//fac un form pentru detalii
	QWidget* widDetalii = new QWidget;
	QFormLayout* formLDetalii = new QFormLayout;
	widDetalii->setLayout(formLDetalii);

	txtTitle = new QLineEdit;
	formLDetalii->addRow(new QLabel("Titlu:"), txtTitle);

	txtGenre = new QLineEdit;
	formLDetalii->addRow(new QLabel("Gen:"), txtGenre);

	txtYear = new QLineEdit;
	formLDetalii->addRow(new QLabel("Anul apariției:"), txtYear);
	QIntValidator* validator = new QIntValidator(txtYear);
	txtYear->setValidator(validator);


	txtMainActor = new QLineEdit;
	formLDetalii->addRow(new QLabel("Actorul pricipal:"), txtMainActor);

	btnAdd = new QPushButton("Adaugă");
	btnDelete = new QPushButton("Șterge");
	btnModify = new QPushButton("Modifică");

	formLDetalii->addWidget(btnAdd);
	formLDetalii->addWidget(btnDelete);
	formLDetalii->addWidget(btnModify);

	ly->addWidget(widDetalii);
}


void QtGUI::connectSignalsSlots() {
	QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
		try {
			ctr.undo();
		}
		catch (UndoException& ex) {
			QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
		}
		reloadList(ctr.getAll());
		});
	QObject::connect(btnResetSort, &QPushButton::clicked, [&]() {
		reloadList(ctr.getAll());
		txtSearch->setText("");
		});
	//cand se emite semnalul clicked de pe buton reincarc lista
	QObject::connect(btnSortByGenre, &QPushButton::clicked, [&]() {
		reloadList(ctr.sortByGenre(), "genre");
		});
	//cand se emite semnalul clicked de pe buton reincarc lista
	QObject::connect(btnSortByYears, &QPushButton::clicked, [&]() {
		reloadList(ctr.sortByYear(), "year");
		});
	QObject::connect(btnSortByTitle, &QPushButton::clicked, [&]() {
		reloadList(ctr.sortByTitle());
		});
	QObject::connect(txtSearch, &QLineEdit::returnPressed, this, &QtGUI::handleSearch);

	QObject::connect(txtSearch, &QLineEdit::textChanged, this, &QtGUI::handleSearch);

	QObject::connect(btnSearch, &QPushButton::clicked, this, &QtGUI::handleSearch);

	//cand se selecteaza elementul din lista incarc detaliile
	QObject::connect(lst, &QListWidget::itemSelectionChanged, [&]() {
		if (lst->selectedItems().isEmpty()) {
			//nu este nimic selectat (golesc detaliile)
			txtTitle->setText("");
			txtGenre->setText("");
			txtYear->setText("");
			txtMainActor->setText("");
			return;
		}
		QListWidgetItem* selItem = lst->selectedItems().at(0);

		QString titleQ = selItem->data(Qt::UserRole).toString();
		txtTitle->setText(titleQ);
		QString yearQ = selItem->data(Qt::UserRole + 1).toString();
		txtYear->setText(yearQ);

		string title = titleQ.toStdString();
		int year = yearQ.toInt();

		try {
			Movie m = ctr.findMovie(title, year);
			txtGenre->setText(QString::fromStdString(m.getGenre()));
			txtMainActor->setText(QString::fromStdString(m.getMainActor()));
		}
		catch (RepoException& ex) {
			QMessageBox::warning(nullptr, "Warning", QString::fromStdString(ex.getMsg()));
		}
		});
	QObject::connect(btnAdd, &QPushButton::clicked, this, &QtGUI::addMovieGUI);
	QObject::connect(btnDelete, &QPushButton::clicked, this, &QtGUI::deleteMovieGUI);
	QObject::connect(btnModify, &QPushButton::clicked, this, &QtGUI::modifyMovieGUI);
	QObject::connect(btnCart, &QPushButton::clicked, this, &QtGUI::openCartGUI);
	QObject::connect(btnAddToCart, &QPushButton::clicked, this, &QtGUI::addToCartGUI);
	QObject::connect(btnClearCart, &QPushButton::clicked, this, [&] {
		cosCtr.deleteAllInCart();
		});
	QObject::connect(btnStatistics, &QPushButton::clicked, this, [&] {
		QtGUICartPainter* pGUI = new QtGUICartPainter(cosCtr);
		pGUI->show();
		});

}

void QtGUI::addMovieGUI() {
	try {
		ctr.addMovie(txtTitle->text().toStdString(), txtGenre->text().toStdString(), txtYear->text().toInt(), txtMainActor->text().toStdString());
		reloadList(ctr.getAll());
		emit changedListSignal();
	}
	catch (RepoException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
	}
	catch (ValidateException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
	}
}

void QtGUI::deleteMovieGUI() {
	string title = txtTitle->text().toStdString();
	int year = txtYear->text().toInt();
	try {
		ctr.deleteMovie(title, year);
		emit changedListSignal();
		reloadList(ctr.getAll());
	}
	catch (RepoException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
	}
	catch (ValidateException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
	}


	try {
		cosCtr.deleteMovieFromCart(title, year);
	}
	catch (RepoException& ex) {

	}
}

void QtGUI::modifyMovieGUI() {
	try {
		Movie m = ctr.findMovie(txtTitle->text().toStdString(), txtYear->text().toInt());

		try {
			cosCtr.deleteMovieFromCart(m.getTitle(), m.getYear());
			QMessageBox::warning(this, "Warning", "Atenție: Filmul pe care doriți să îl modificați a existat în coș. Acesta a fost șters și trebuie readăugat manual.");
			emit changedCartSignal();
		}
		catch (RepoException& ex) {

		}

		QtGUIModify* mGUI = new QtGUIModify(ctr, m);
		mGUI->setAttribute(Qt::WA_DeleteOnClose);

		mGUI->show();
		QObject::connect(mGUI, &QtGUIModify::windowClosed, this, [=]() {
			reloadList(ctr.getAll());
			});
		emit changedListSignal();
	}
	catch (RepoException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
		return;
	}
}

void QtGUI::openCartGUI() {
	QtGUICart* cGUI = new QtGUICart(cosCtr);
	cGUI->setAttribute(Qt::WA_DeleteOnClose);
	QObject::connect(this, &QtGUI::changedCartSignal, cGUI, &QtGUICart::cartChanged);
	QObject::connect(this, &QtGUI::changedListSignal, cGUI, &QtGUICart::listChanged);

	cGUI->show();
}

void QtGUI::addToCartGUI() {
	try {
		cosCtr.addMovieInCart(txtTitle->text().toStdString(), txtYear->text().toInt());
		emit changedCartSignal();
	}
	catch (RepoException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
		return;
	}
}

void QtGUI::handleSearch() {
	auto titleMatch = ctr.filterByTitle(txtSearch->text().toStdString());
	auto yearsMatch = ctr.filterByYears(txtSearch->text().toInt(), txtSearch->text().toInt());
	auto genreMatch = ctr.filterByGenre(txtSearch->text().toStdString());
	auto actorMatch = ctr.filterByMainActor(txtSearch->text().toStdString());
	titleMatch.insert(titleMatch.end(), yearsMatch.begin(), yearsMatch.end());
	titleMatch.insert(titleMatch.end(), genreMatch.begin(), genreMatch.end());
	titleMatch.insert(titleMatch.end(), actorMatch.begin(), actorMatch.end());
	reloadList(titleMatch);
}


void QtGUI::reloadList(std::vector<Movie> database, string criteria) {
	lst->clear();
	for (auto& m : database) {
		QString itemTxt;
		if (criteria == "genre") {
			itemTxt = QString::fromStdString(m.getTitle()) + ", " + QString::fromStdString(m.getGenre());
		}
		else if (criteria == "year") {
			itemTxt = QString::fromStdString(m.getTitle()) + " (" + QString::number(m.getYear()) + ")";
		}
		else {
			itemTxt = QString::fromStdString(m.getTitle());

		}
		QListWidgetItem* item = new QListWidgetItem(itemTxt, lst);
		item->setData(Qt::UserRole, QString::fromStdString(m.getTitle()));
		item->setData(Qt::UserRole + 1, QString::number(m.getYear()));
		lst->addItem(item);
	}
}

/////////////

void QtGUIModify::initGUICmps() {
	setWindowTitle("Modifică film");
	setWindowIcon(QIcon("clipchamp.png"));
	resize(400, 0);

	QVBoxLayout* mainLayout = new QVBoxLayout{ this };
	QFormLayout* formLayout = new QFormLayout{};
	QHBoxLayout* buttonLayout = new QHBoxLayout{};

	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);


	txtNewTitle = new QLineEdit{};
	formLayout->addRow(new QLabel{ "Titlu:" }, txtNewTitle);

	txtNewGenre = new QLineEdit{};
	formLayout->addRow(new QLabel{ "Gen:" }, txtNewGenre);

	txtNewYear = new QLineEdit{};
	formLayout->addRow(new QLabel{ "Anul apariției:" }, txtNewYear);
	QIntValidator* validator = new QIntValidator(txtNewYear);
	txtNewYear->setValidator(validator);

	txtNewMainActor = new QLineEdit{};
	formLayout->addRow(new QLabel{ "Actor principal:" }, txtNewMainActor);

	txtNewTitle->setText(QString::fromStdString(movie.getTitle()));
	txtNewGenre->setText(QString::fromStdString(movie.getGenre()));
	txtNewYear->setText(QString::number(movie.getYear()));
	txtNewMainActor->setText(QString::fromStdString(movie.getMainActor()));

	QPushButton* modifyButton = new QPushButton{ "Modifică" };
	QPushButton* cancelButton = new QPushButton{ "Anulează" };
	buttonLayout->addWidget(cancelButton);
	buttonLayout->addWidget(modifyButton);

	QObject::connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
	QObject::connect(modifyButton, &QPushButton::clicked, this, &QtGUIModify::modifyMovie);
}

void QtGUIModify::modifyMovie() {
	string new_title = txtNewTitle->text().toStdString();
	string new_genre = txtNewGenre->text().toStdString();
	int new_year = txtNewYear->text().toInt();
	string new_main_actor = txtNewMainActor->text().toStdString();

	try {
		ctr.modifyMovie(movie.getTitle(), movie.getYear(), new_title, new_genre, new_year, new_main_actor);
		emit windowClosed();
		close();
	}
	catch (const RepoException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
	}
	catch (const ValidateException& ex) {
		QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
	}
}

void QtGUICart::initGUICmps() {
	setWindowTitle("Coș de cumparaturi");
	setWindowIcon(QIcon("clipchamp.png"));
	resize(400, 300);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QFormLayout* formLayout = new QFormLayout;
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	QHBoxLayout* randomLayout = new QHBoxLayout;

	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addLayout(randomLayout);

	moviesCount = new QLabel(QString("<html>Filme în coș: <b>%1</b></html>").arg(cosCtr.getAllFromCart().size()));

	lst = new QListWidget;
	formLayout->addWidget(moviesCount);
	formLayout->addWidget(lst);


	btnDelete = new QPushButton(QIcon("delete.svg"), "Șterge din coș");
	btnExport = new QPushButton(QIcon("download.svg"), "Exportă coș");
	buttonLayout->addWidget(btnDelete);
	buttonLayout->addWidget(btnExport);

	txtRandom = new QLineEdit;
	int numberOfMoviesAvalaible = cosCtr.sizeOfDatabase() - cosCtr.sizeOfRepo();
	if (numberOfMoviesAvalaible == 0) {
		txtRandom->setPlaceholderText("Nu mai poți adauga filme în coș");
	}
	else {
		txtRandom->setPlaceholderText(QString("Adaugă între 1 și %1 filme aleatorii").arg(numberOfMoviesAvalaible));
	}
	btnRandom = new QPushButton("Adaugă aleatoriu");

	QIntValidator* validator = new QIntValidator(txtRandom);
	txtRandom->setValidator(validator);

	randomLayout->addWidget(txtRandom);
	randomLayout->addWidget(btnRandom);
}

void QtGUICart::connectSignalsSlots() {
	cosCtr.addObserver(this);

	QObject::connect(btnDelete, &QPushButton::clicked, this, [&]() {
		string title;
		int year;

		if (lst->selectedItems().isEmpty()) {
			title = "";
			year = 0;
		}
		else {
			QListWidgetItem* selItem = lst->selectedItems().at(0);

			title = selItem->data(Qt::UserRole).toString().toStdString();
			year = selItem->data(Qt::UserRole + 1).toString().toInt();
		}

		try {
			cosCtr.deleteMovieFromCart(title, year);
		}
		catch (const RepoException& ex) {
			QMessageBox::warning(this, "Warning", QString::fromStdString(ex.getMsg()));
		}
		cartChanged();
		});
	QObject::connect(btnExport, &QPushButton::clicked, this, [&]() {
		QString filePath = QFileDialog::getSaveFileName(this, "Selectează locația", "", "HTML files (*.html);;CSV files (*.csv)");
		QString fileExtension = QFileInfo(filePath).suffix().toLower();

		if (filePath != "") {
			if (fileExtension == "csv") {
				cosCtr.printInFileAsCSV(filePath.toStdString());
				QMessageBox::information(this, "Succes", "Export realizat cu succes!");
			}
			else if (fileExtension == "html") {
				cosCtr.printInFileAsHTML(filePath.toStdString());
				QMessageBox::information(this, "Succes", "Export realizat cu succes!");
			}
			else {
				QMessageBox::warning(this, "Warning", "FileException: Format invalid introdus");
			}
		}
		});
	QObject::connect(btnRandom, &QPushButton::clicked, this, &QtGUICart::handleRandom);
	QObject::connect(txtRandom, &QLineEdit::returnPressed, this, &QtGUICart::handleRandom);
}

void QtGUICart::handleRandom() {
	int moviesToAdd = txtRandom->text().toInt();
	int addedMovie = cosCtr.addRandomInCart(moviesToAdd);
	reloadList(cosCtr.getAllFromCart());

	int numberOfMoviesAvalaible = cosCtr.sizeOfDatabase() - cosCtr.sizeOfRepo();
	txtRandom->clear();
	if (numberOfMoviesAvalaible == 0) {
		txtRandom->setPlaceholderText("Nu mai poți adauga filme in coș");
	}
	else {
		txtRandom->setPlaceholderText(QString("Adaugă între 1 și %1 filme aleatorii").arg(numberOfMoviesAvalaible));
	}
	cartChanged();
}

void QtGUICart::listChanged() {
	reloadList(cosCtr.getAllFromCart());
	int numberOfMoviesAvalaible = cosCtr.sizeOfDatabase() - cosCtr.sizeOfRepo();
	if (numberOfMoviesAvalaible == 0) {
		txtRandom->setPlaceholderText("Nu mai poți adauga filme in coș");
	}
	else {
		txtRandom->setPlaceholderText(QString("Adaugă între 1 și %1 filme aleatorii").arg(numberOfMoviesAvalaible));
	}
}

void QtGUICart::cartChanged() {
	reloadList(cosCtr.getAllFromCart());
	moviesCount->setText(QString("<html>Filme în coș: <b>%1</b></html>").arg(cosCtr.getAllFromCart().size()));
	int numberOfMoviesAvalaible = cosCtr.sizeOfDatabase() - cosCtr.sizeOfRepo();
	if (numberOfMoviesAvalaible == 0) {
		txtRandom->setPlaceholderText("Nu mai poți adauga filme in coș");
	}
	else {
		txtRandom->setPlaceholderText(QString("Adaugă între 1 și %1 filme aleatorii").arg(numberOfMoviesAvalaible));
	}
}

void QtGUICart::update() {
	cartChanged();
}

void QtGUICart::reloadList(std::vector<Movie> database) {
	lst->clear();
	for (auto& m : database) {
		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(m.getTitle()), lst);
		item->setData(Qt::UserRole, QString::fromStdString(m.getTitle()));
		item->setData(Qt::UserRole + 1, QString::number(m.getYear()));
		lst->addItem(item);
	}
}

void QtGUICartStatistics::initGUICmps() {

}

void QtGUICartStatistics::reloadChart(std::vector<Movie> database) {

}


void QtGUICartStatistics::update() {
	reloadChart(cosCtr.getAllFromCart());
}
 
void QtGUICartPainter::paintEvent(QPaintEvent* ev) {
	// Call the base class paintEvent function
	QWidget::paintEvent(ev);

	// Get the genres and their counts from the cart
	const std::map<std::string, int>& genreCounts = cosCtr.getGenreCounts();

	// Calculate the total number of movies in the cart
	int totalMovies = 0;
	for (const auto& entry : genreCounts) {
		totalMovies += entry.second;
	}

	// Find the maximum movie count
	int maxMovieCount = 0;
	for (const auto& entry : genreCounts) {
		if (entry.second > maxMovieCount) {
			maxMovieCount = entry.second;
		}
	}

	// Define the chart dimensions
	int chartWidth = width() - 180;   // Subtracting margins and sidebar width
	int chartHeight = height() - 40;  // Subtracting margins
	int chartX = 20;                  // Left margin
	int chartY = 20;                  // Top margin

	// Set up the painting
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	// Draw the chart title
	painter.drawText(chartX, chartY - 10, "");

	// Define the colors for the chart bars
	const QStringList barColors = {
		"#ff0000", // Red
		"#00ff00", // Green
		"#0000ff", // Blue
		"#ffff00", // Yellow
		"#ff00ff", // Magenta
		"#00ffff", // Cyan
		"#800080", // Purple
		"#ffa500", // Orange
		"#008000", // Dark Green
		"#ff69b4"  // Pink
	};

	// Check if genreCounts is empty
	if (genreCounts.empty()) {
		painter.drawText(chartX, chartY + chartHeight / 2, "Nu există filme în coș.");
		return;
	}

	// Draw the chart bars
	int barWidth = chartWidth / genreCounts.size();
	int barX = chartX;
	int barY = chartY + 20; // Leave space for the title
	int maxBarHeight = chartHeight - 20; // Leave space for the title and labels

	// Calculate the scaling factor based on the maximum movie count
	double scalingFactor = static_cast<double>(maxBarHeight) / maxMovieCount;

	int colorIndex = 0; // Index to retrieve the color from the list

	// Draw the sidebar
	int sidebarX = chartWidth + 40; // Right margin
	int sidebarY = chartY;
	int sidebarWidth = 140; // Width of the sidebar
	int sidebarHeight = chartHeight;

	painter.setBrush(Qt::white);
	painter.drawRect(sidebarX, sidebarY, sidebarWidth, sidebarHeight);

	// Draw the count label at the top of the sidebar
	painter.drawText(sidebarX, sidebarY - 10, "");

	// Draw the count for each genre in the sidebar
	int countX = sidebarX + 10;
	int countY = sidebarY + 20; // Leave space for the title
	int countSpacing = 20; // Spacing between genre count labels

	for (const auto& entry : genreCounts) {
		// Set the color for the bar
		painter.setBrush(QColor(barColors[colorIndex]));

		// Calculate the height of the current bar with the scaling factor
		int barHeight = static_cast<int>(entry.second * scalingFactor);

		// Draw the bar
		painter.drawRect(barX, barY + maxBarHeight - barHeight, barWidth, barHeight);

		// Draw the genre label below the bar
		painter.drawText(barX, barY + maxBarHeight + 15, QString::fromStdString(entry.first));

		// Draw the count label in the sidebar
		painter.drawText(countX, countY, QString::fromStdString(entry.first + ": " + std::to_string(entry.second)));

		// Move to the next bar and count label
		barX += barWidth;
		countY += countSpacing;
		colorIndex = (colorIndex + 1) % barColors.size();
	}
}


void QtGUICartPainter::update() {
	repaint();
}