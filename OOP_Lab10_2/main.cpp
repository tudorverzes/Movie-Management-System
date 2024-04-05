#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qtablewidget.h>
#include <QtWidgets/qmessagebox.h>
#include <qlist.h>

#include "repository.h"
#include "service.h"
#include "validator.h"
#include "QtGUI.h"

#include <string>
#include <list>


int main(int argc, char* argv[])
{
	Repo rep, cos_rep;
	Validator val;
	Service ctr{ rep, val };
	CosService cosCtr{ cos_rep, rep };

	QApplication a(argc, argv);
	QtGUI gui{ ctr, cosCtr };
	gui.show();


	return a.exec();
}
