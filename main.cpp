#include "Class/UI/Header/MainWindow.h"   // ← замените на имя вашего класса главного окна
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   // создаём объект приложения

    MainWindow w;                 // ← создаём объект главного окна
    w.show();                     // показываем окно

    return a.exec();              // запускаем цикл обработки событий
}