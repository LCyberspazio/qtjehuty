/* QTJehuty, a simple text encryption program
 * by Giovanni Santostefano
 * http://twocentssecurity.wordpress.com
 *
 * Jehuty integrates the official C code
 * implementation of Anubis cipher
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
