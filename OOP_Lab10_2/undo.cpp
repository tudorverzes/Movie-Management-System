#include "undo.h"

ostream& operator<<(ostream& out, const UndoException& ex) {
	out << "\033[31m" << "UndoException: " << ex.msg << "\033[0m";
	return out;

}