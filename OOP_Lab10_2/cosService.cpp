#include "cosService.h"
#include <random>
#include <fstream>

using std::ofstream;


const vector<Movie> CosService::getAllFromCart() noexcept {
	return repo.getAll();
}

void CosService::addMovieInCart(const string& title, int year) {
	Movie m = database.find(title, year);
	repo.store(m);
    notify();
}

void CosService::deleteMovieFromCart(const string& title, int year) {
	repo.delete_element(title, year);
    notify();
}

void CosService::deleteAllInCart() {
	repo.deleteAll();
    notify();
}

int CosService::addRandomInCart(int n) {
    if (n <= 0) {
        return 0;
    }
	int availableMovies = (int)database.getAll().size();
	if (availableMovies == 0) return 0;

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> dist(0, availableMovies - 1);

	availableMovies = availableMovies - (int)repo.getAll().size();
	if (availableMovies < n) {
		n = availableMovies;
	}

	int count = 0;
	for (int i = 0; count < n; i++) {
		int rndNr = dist(mt);
		Movie m = database.get(rndNr);
		if (repo.exist(m) == false) {
			repo.store(m);
			count++;
		}
	}
    notify();
	return count;
}

void CosService::printInFileAsCSV(string filePath) {
	ofstream fout(filePath);
	fout << "Titlu,Gen,Anul aparitiei,Actor principal\n";
	for (Movie m : repo.getAll()) {
		fout << m.getTitle() << ',';
		fout << m.getGenre() << ',';
		fout << m.getYear() << ',';
		fout << m.getMainActor() << '\n';
	}
}

void CosService::printInFileAsHTML(string filePath) {
    ofstream fout(filePath);
    fout << "<!DOCTYPE html><html><head><style>"
        << "body { font-family: Arial, sans-serif; }"
        << "h2 { text-align: center; }"
        << "table { margin: 0 auto; border-collapse: collapse; box-shadow: 0 0 5px rgba(0, 0, 0, 0.1); background-color: #f9f9f9; }"
        << "th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }"
        << "th { background-color: #f2f2f2; font-weight: bold; cursor: pointer; position: relative; }"
        << "th::after { content: '\\2191'; position: absolute; top: 50%; right: 5px; transform: translateY(-50%); opacity: 0.5; }"
        << "th.asc::after { content: '\\2191'; opacity: 1; }"
        << "th.desc::after { content: '\\2193'; opacity: 1; }"
        << "tr:hover { background-color: #f5f5f5; }"
        << "</style>"
        << "<script>"
        << "function sortTable(columnIndex) {"
        << "  var table = document.getElementById('movie-table');"
        << "  var rows = Array.from(table.getElementsByTagName('tr'));"
        << "  var header = rows.shift();"
        << "  var sortIndicator = header.getElementsByTagName('th')[columnIndex];"
        << "  var isAscending = !sortIndicator.classList.contains('asc');"
        << "  Array.from(header.getElementsByTagName('th')).forEach(function(th) {"
        << "    th.classList.remove('asc', 'desc');"
        << "  });"
        << "  sortIndicator.classList.add(isAscending ? 'asc' : 'desc');"
        << "  var sortedRows = rows.sort(function(a, b) {"
        << "    var aValue = a.getElementsByTagName('td')[columnIndex].innerText;"
        << "    var bValue = b.getElementsByTagName('td')[columnIndex].innerText;"
        << "    if (columnIndex === 0) {"
        << "      return isAscending ? (parseInt(aValue) - parseInt(bValue)) : (parseInt(bValue) - parseInt(aValue));"
        << "    } else {"
        << "      return isAscending ? aValue.localeCompare(bValue) : bValue.localeCompare(aValue);"
        << "    }"
        << "  });"
        << "  table.tBodies[0].innerHTML = '';"
        << "  sortedRows.forEach(function(row) {"
        << "    table.tBodies[0].appendChild(row);"
        << "  });"
        << "}"
        << "</script>"
        << "</head><body><h2>Cos de cumparaturi: " << repo.getAll().size() << " filme </h2><table id='movie-table'>";
    fout << "<thead><tr>"
        << "<th onclick='sortTable(0)'>No.</th>"
        << "<th onclick='sortTable(1)'>Titlu</th>"
        << "<th onclick='sortTable(2)'>Gen</th>"
        << "<th onclick='sortTable(3)'>Anul aparitiei</th>"
        << "<th onclick='sortTable(4)'>Actor Principal</th>"
        << "</tr></thead><tbody>";
    int index = 1;
    for (Movie m : repo.getAll()) {
        fout << "<tr><td>" << index++ << "</td>";
        fout << "<td>" << m.getTitle() << "</td>";
        fout << "<td>" << m.getGenre() << "</td>";
        fout << "<td>" << m.getYear() << "</td>";
        fout << "<td>" << m.getMainActor() << "</td></tr>";
    }
    fout << "</tbody></table></body></html>";
}

int CosService::sizeOfRepo() {
	return repo.getAll().size();
}

int CosService::sizeOfDatabase() {
	return database.getAll().size();
}

int CosService::numberOfMoviesWithGenre(string genre) const {
    int count = 0;
    for (auto& it : repo.getAll()) {
        if (it.getGenre() == genre) {
            count++;
		}
	}
    return count;
}

std::map<std::string, int> CosService::getGenreCounts() const {
    std::map<std::string, int> genreCounts;

    for (const Movie& movie : repo.getAll()) {
        std::string genre = movie.getGenre();
        genreCounts[genre]++;
    }

    return genreCounts;

}