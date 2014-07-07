/* QTJehuty, a simple text encryption program
 * by Giovanni Santostefano
 * http://twocentssecurity.wordpress.com
 *
 * Jehuty integrates the official C code
 * implementation of Anubis cipher
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nessie.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_encryptbutton_clicked();

    void on_decryptbutton_clicked();

    void on_pushButton_clicked();

    void on_cleartextline_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
