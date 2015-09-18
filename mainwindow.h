#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_buttonPlay_clicked();
	
private:
	void ReloadScreenSizes(QSize const&);
	bool RunGame();
	void SaveSettings();

private:
	Ui::MainWindow *ui;
	
	QString m_executable;
	QString m_iniPath;
	QSize m_screenSize;
    QString ThisDirectory;
};

#endif // MAINWINDOW_H
