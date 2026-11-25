#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "Class/Base/Header/Bed.h"

class BedsWidget : public QWidget {
    Q_OBJECT
public:
    explicit BedsWidget(QWidget *parent = nullptr);

    // Метод для обновления интерфейса при выборе грядки
    void set_bed(Bed* bed, bool is_light_on, double current_time_hours);
    void clear();

signals:
    // Сигналы для MainWindow
    void add_bed_clicked();
    void remove_bed_clicked();
    void plant_clicked();
    void collect_clicked();
    void water_clicked();
    void fertilize_clicked();
    void toggle_light_clicked();
    void remove_dead_clicked(); // Вернули сигнал

private:
    QLabel* info_label_;
    QWidget* grid_widget_;
    QGridLayout* grid_layout_;
};