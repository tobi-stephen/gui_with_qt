#include <QtGui>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QStatusBar>
#include <QToolBar>
#include "mainwindow.h"
// #include "finddialog.h"
// #include "gotocelldialog.h"
// #include "sortdialog.h"
#include "spreadsheet.h"

MainWindow::MainWindow()
{
	spreadsheet = new Spreadsheet;
	setCentralWidget(spreadsheet);

	createActions();
	createMenus();
	createContextMenu();
	createToolBars();
	createStatusBar();

	readSettings();

	// findDialog = 0;

	// setWindowIcon(QIcon(":/images/icon.png"));
	setCurrentFile("");
	setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::createActions()
{
	newAction = new QAction(tr("&New"), this);
	// newAction->setIcon(QIcon(":/images/new.png"));
	newAction->setShortcut(tr("Ctrl+N"));
	newAction->setStatusTip(tr("Create a new spreadsheet file"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

	openAction = new QAction(tr("&Open"), this);
	// openAction->setIcon(QIcon(":/images/open.png"));
	openAction->setShortcut(tr("Ctrl+O"));
	openAction->setStatusTip(tr("Open a spreadsheet file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	saveAction = new QAction(tr("&Save"), this);
	// saveAction->setIcon(QIcon(":/images/save.png"));
	saveAction->setShortcut(tr("Ctrl+S"));
	saveAction->setStatusTip(tr("Save a spreadsheet file"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAction = new QAction(tr("Save As"), this);
	// saveAsAction->setIcon(QIcon(":/images/save.png"));
	saveAsAction->setStatusTip(tr("Save as"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	for (int i = 0; i < MaxRecentFiles; ++i){
		recentFileActions[i] = new QAction(this);
		recentFileActions[i]->setVisible(false);
		connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}

	cutAction = new QAction(tr("Cu&t"), this);
	cutAction->setShortcut(tr("Ctrl+X"));
	cutAction->setStatusTip(tr("Cut this cell portion"));
	connect(cutAction, SIGNAL(triggered()), spreadsheet, SLOT(cut()));

	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setShortcut(tr("Ctrl+C"));
	copyAction->setStatusTip(tr("Copy this cell portion"));
	connect(copyAction, SIGNAL(triggered()), spreadsheet, SLOT(copy()));

	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setShortcut(tr("Ctrl+V"));
	pasteAction->setStatusTip(tr("Paste into this cell(s)"));
	connect(pasteAction, SIGNAL(triggered()), spreadsheet, SLOT(paste()));
	
	deleteAction = new QAction(tr("&Delete"), this);
	deleteAction->setShortcut(tr("Del"));
	deleteAction->setStatusTip(tr("Delete this cell"));
	connect(pasteAction, SIGNAL(triggered()), spreadsheet, SLOT(del()));

	selectRowAction = new QAction(tr("&Row"), this);
	selectRowAction->setStatusTip(tr("Select the rows"));
	// connect(selectRowAction, SIGNAL(triggered()), spreadsheet, SLOT(selectRow()));

	selectColumnAction = new QAction(tr("&Column"), this);
	selectColumnAction->setStatusTip(tr("Select the column"));
	// connect(selectColumnAction, SIGNAL(triggered()), spreadsheet, SLOT(selectColumn()));

	selectAllAction = new QAction(tr("&All"), this);
	selectAllAction->setShortcut(tr("Ctrl+A"));
	selectAllAction->setStatusTip(tr("Select all cells"));
	connect(selectAllAction, SIGNAL(triggered()), spreadsheet, SLOT(selectAll()));

	findAction = new QAction(tr("&Find..."), this);
	findAction->setShortcut(tr("Ctrl+F"));
	findAction->setStatusTip(tr("Find "));
	// connect(findAction, SIGNAL(triggered()), spreadsheet, SLOT(find()));

	goToCellAction = new QAction(tr("&Go to Cell..."), this);
	goToCellAction->setShortcut(tr("F5"));
	goToCellAction->setStatusTip(tr("Go to cell"));
	// connect(goToCellAction, SIGNAL(triggered()), spreadsheet, SLOT(goToCell()));

	recalculateAction = new QAction(tr("&Recalculate"), this);
	recalculateAction->setShortcut(tr("F9"));
	recalculateAction->setStatusTip(tr("Recalculate"));
	connect(recalculateAction, SIGNAL(triggered()), spreadsheet, SLOT(recalculate()));

	sortAction = new QAction(tr("&Sort..."), this);
	sortAction->setStatusTip(tr("Sort"));
	// connect(sortAction, SIGNAL(triggered()), spreadsheet, SLOT(sort()));

	showGridAction = new QAction(tr("&Show Grid"), this);
	showGridAction->setCheckable(true);
	showGridAction->setChecked(spreadsheet->showGrid());
	showGridAction->setStatusTip(tr("Show or hide the grid"));
	connect(showGridAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setShowGrid(bool)));

	autoRecalcAction = new QAction(tr("&Auto-recalculate"), this);
	autoRecalcAction->setCheckable(true);
	autoRecalcAction->setChecked(spreadsheet->autoRecalculate());
	autoRecalcAction->setStatusTip(tr("Auto/Manual Recalculate"));
	// connect(autoRecalcAction, SIGNAL(toggled(bool)), spreadsheet, SLOT(setAutoRecalc()));

	aboutAction = new QAction(tr("About"), this);
	aboutAction->setStatusTip(tr("Shows info about the app"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(tr("About &Qt"), this);
	aboutQtAction->setStatusTip(tr("Show Qt lib About box"));
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	closeAction = new QAction(tr("&Close"), this);
	closeAction->setShortcut(tr("Ctrl+W"));
	closeAction->setStatusTip(tr("Close this window"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	separatorAction = fileMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i)
		fileMenu->addAction(recentFileActions[i]);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAction);
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(deleteAction);

	selectSubMenu = editMenu->addMenu(tr("&Select"));
	selectSubMenu->addAction(selectRowAction);
	selectSubMenu->addAction(selectColumnAction);
	selectSubMenu->addAction(selectAllAction);

	editMenu->addSeparator();
	editMenu->addAction(findAction);
	editMenu->addAction(goToCellAction);

	toolsMenu = menuBar()->addMenu(tr("&Tools"));
	toolsMenu->addAction(recalculateAction);
	toolsMenu->addAction(sortAction);

	optionsMenu = menuBar()->addMenu(tr("&Options"));
	optionsMenu->addAction(showGridAction);
	optionsMenu->addAction(autoRecalcAction);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

void MainWindow::createContextMenu()
{
	spreadsheet->addAction(cutAction);
	spreadsheet->addAction(copyAction);
	spreadsheet->addAction(pasteAction);
	spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("&File"));
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	editToolBar = addToolBar(tr("&Edit"));
	editToolBar->addAction(cutAction);
	editToolBar->addAction(copyAction);
	editToolBar->addAction(pasteAction);
	editToolBar->addSeparator();
	editToolBar->addAction(findAction);
	editToolBar->addAction(goToCellAction);
}

void MainWindow::createStatusBar()
{
	locationLabel = new QLabel(" W999 ");
	locationLabel->setAlignment(Qt::AlignHCenter);
	locationLabel->setMinimumSize(locationLabel->sizeHint());

	formulaLabel = new QLabel;
	formulaLabel->setIndent(3);

	statusBar()->addWidget(locationLabel);
	statusBar()->addWidget(formulaLabel, 1);

	connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
	connect(spreadsheet, SIGNAL(modified()), this, SLOT(spreadsheetModified()));
	updateStatusBar();
}

void MainWindow::updateStatusBar()
{
	locationLabel->setText(spreadsheet->currentLocation());
	formulaLabel->setText(spreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
	setWindowModified(true);
	updateStatusBar();
}

void MainWindow::newFile()
{
	MainWindow *mainWin = new MainWindow;
	mainWin->show();
	// if (okToContinue()){
	// 	spreadsheet->clear();
	// 	setCurrentFile("");
	// }
}

bool MainWindow::okToContinue()
{
	if (isWindowModified()){
		int r = QMessageBox::warning(this, tr("Spreadsheet"), tr("The Document has been modified.\nDo you want to save your changes?"), QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);
		if (r == QMessageBox::Yes){
			return save();
		} else if (r == QMessageBox::Cancel){
			return false;
		}
	}
	return true;
}

void MainWindow::open()
{
	if (okToContinue()){
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spreadsheet"), ".", tr("Spreadsheet files (*.sp)"));
		if (!fileName.isEmpty())
			loadFile(fileName);
	}
}

bool MainWindow::loadFile(const QString &fileName)
{
	if (!spreadsheet->readFile(fileName)){
		statusBar()->showMessage(tr("Loading cancelled"), 2000);
		return false;
	}

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
	return true;
}

bool MainWindow::save()
{
	if (curFile.isEmpty()){
		return saveAs();
	} else {
		return saveFile(curFile);
	}
}

bool MainWindow::saveFile(const QString &fileName)
{
	if (!spreadsheet->writeFile(fileName)){
		statusBar()->showMessage(tr("Saving canceled"), 2000);
		return false;
	}

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spreadsheet"), ".", tr("Spreadsheet files (*.sp)"));
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (okToContinue()){
		writeSettings();
		event->accept();
	} else {
		event->ignore();
	}
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	setWindowModified(false);
	QString shownName = "Untitled";
	if (!curFile.isEmpty()) {
		shownName = strippedName(curFile);
		recentFiles.removeAll(curFile);
		recentFiles.prepend(curFile);
		updateRecentFileActions();
	}

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Spreadsheet")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateRecentFileActions()
{
	QMutableStringListIterator i(recentFiles);
	while (i.hasNext()){
		if (!QFile::exists(i.next()))
		i.remove();
	}

	for (int j = 0; j < MaxRecentFiles; ++j){
		if (j < recentFiles.count()){
			QString text = tr("&%1 %2").arg(j + 1).arg(strippedName(recentFiles[j]));
			recentFileActions[j]->setText(text);
			recentFileActions[j]->setData(recentFiles[j]);
			recentFileActions[j]->setVisible(true);
		} else {
			recentFileActions[j]->setVisible(false);
		}
	}
	separatorAction->setVisible(!recentFiles.isEmpty());
}

void MainWindow::openRecentFile()
{
	if (okToContinue()){
		QAction *action = qobject_cast<QAction *>(sender());
		if (action)
			loadFile(action->data().toString());
	}
}

void MainWindow::find()
{
	// if (!findDialog){
	// 	findDialog = new FindDialog(this);
	// 	connect(findDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)), spreadsheet, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
    //             connect(findDialog, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)), spreadsheet, SLOT(findPrevious(const QString &, Qt::CaseSensitivity)));
	// }

	// if (findDialog->isHidden()){
	// 	findDialog->show();
	// } else {
	// 	findDialog->activateWindow();
	// }
}

void MainWindow::goToCell()
{
	// GoToCellDialog *dialog = new GoToCellDialog(this);
	// if (dialog->exec()){
	// 	QString str = dialog->lineEdit->text().toUpper();
	// 	spreadsheet->setCurrentCell(str.mid(1).toInt() - 1, str[0].unicode() - 'A');
	// }
	// delete dialog;
}

void MainWindow::sort()
{
	// SortDialog dialog(this);
	// QTableWidgetSelectionRange range = spreadsheet->selectedRange();
	// dialog.setColumnRange('A' + range.leftColumn(), 'A' + range.rightColumn());

	// if (dialog.exec()){
	// 	SpreadsheetCompare compare;
	// 	compare.keys[0] = dialog.primaryColumnCombo->currentIndex();
	// 	compare.keys[1] = dialog.secondaryColumnCombo->currentIndex() - 1;
	// 	compare.keys[2] = dialog.tertiaryColumnCombo->currentIndex() - 1;
	// 	compare.ascending[0] = (dialog.primaryOrderCombo->currentIndex() == 0);
	// 	compare.ascending[1] = (dialog.secondaryOrderCombo->currentIndex() == 0);
	// 	compare.ascending[2] = (dialog.tertiaryOrderCombo->currentIndex() == 0);
	// 	spreadsheet->sort(compare);
	// }
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About Spreadsheet"), tr("<h2>Spreadsheet 1.1</h2>"
		"<p>Copyright &copy; 2006 Software Inc."
		"<p>Spreadsheet is a small application that "
		"demonstrates QAction, QMainWindow, QMenuBar, "
		"QStatusBar, QTableWidget, QToolBar, and many other "
		"Qt classes."));
}

void MainWindow::writeSettings()
{
	QSettings settings("Software Inc.", "Spreadsheet");
	
	settings.setValue("geometry", geometry());
	settings.setValue("recentFiles", recentFiles);
	settings.setValue("showGrid", showGridAction->isChecked());
	settings.setValue("autoRecalc", autoRecalcAction->isChecked());

	// settings.beginGroup("findDialog");
	// settings.setValue("matchCase", caseCheckBox->isChecked());
	// settings.setValue("searchBackward", backwardCheckBox->isChecked());
	// settings.endGroup();
}

void MainWindow::readSettings()
{
	QSettings settings("Software Inc.", "Spreadsheet");
	QRect rect = settings.value("geometry", QRect(200, 200, 400, 400)).toRect();
	move(rect.topLeft());
	resize(rect.size());

	recentFiles = settings.value("recentFiles").toStringList();
	updateRecentFileActions();

	bool showGrid = settings.value("showGrid", true).toBool();
	showGridAction->setChecked(showGrid);

	bool autoRecalc = settings.value("autoRecalc", true).toBool();
	autoRecalcAction->setChecked(autoRecalc);
}
