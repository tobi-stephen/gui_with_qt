#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QLabel;
class QString;
class QToolBar;
// class QMenu;
// class QString;
// class FindDialog;
class Spreadsheet;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void find();
	void goToCell();
	void sort();
	void about();
	void openRecentFile();
	void updateStatusBar();
	void spreadsheetModified();

private:
	void createActions();
	void createMenus();
	void createContextMenu();
	void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	bool okToContinue();
	bool loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void updateRecentFileActions();
	QString strippedName(const QString &fullFileName);

	Spreadsheet *spreadsheet;
	// FindDialog *findDialog;
	QLabel *locationLabel;
	QLabel *formulaLabel;
	QStringList recentFiles;
	QString curFile;

	QAction *newAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *openAction;

	enum { MaxRecentFiles = 5 };
	QAction *recentFileActions[MaxRecentFiles];
	QAction *separatorAction;
	QAction *exitAction;
	QAction *closeAction;

	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *deleteAction;
	QAction *selectRowAction;
	QAction *selectColumnAction;
	QAction *selectAllAction;
	QAction *findAction;
	QAction *goToCellAction;
		
	QAction *showGridAction;
	QAction *autoRecalcAction;

	QAction *recalculateAction;
	QAction *sortAction;
	
	QAction *aboutAction;
	QAction *aboutQtAction;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *toolsMenu;
	QMenu *helpMenu;
	QMenu *optionsMenu;
	QMenu *selectSubMenu;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;
};

#endif
