#ifndef RGBNIRD_MAINWINDOW_H
#define RGBNIRD_MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class RGBNIRD_MainWindow;
}

class RGBNIRD_MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit RGBNIRD_MainWindow(QWidget *parent = 0);
	~RGBNIRD_MainWindow();

private:
	Ui::RGBNIRD_MainWindow *ui;
};

#endif // RGBNIRD_MAINWINDOW_H
