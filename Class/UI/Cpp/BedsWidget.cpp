#include "Class/UI/Header/BedsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <cmath>

BedsWidget::BedsWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    info_label_ = new QLabel("Выберите грядку", this);
    info_label_->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 5px;");
    main_layout->addWidget(info_label_);

    grid_widget_ = new QWidget(this);
    grid_layout_ = new QGridLayout(grid_widget_);
    grid_widget_->setStyleSheet("background-color: #f0f0f0; border: 1px solid #ccc;");
    main_layout->addWidget(grid_widget_);

    QHBoxLayout* action_layout = new QHBoxLayout();
    
    // Создаем все кнопки
    QPushButton* btn_add = new QPushButton("Добавить", this);
    QPushButton* btn_remove = new QPushButton("Удалить", this);
    QPushButton* btn_plant = new QPushButton("Посадить", this);
    QPushButton* btn_collect = new QPushButton("Собрать", this);
    QPushButton* btn_water = new QPushButton("Полить", this);
    QPushButton* btn_fert = new QPushButton("Удобрить", this);
    QPushButton* btn_light = new QPushButton("Свет (грядка)", this);
    QPushButton* btn_remove_dead = new QPushButton("Убрать мертвые", this);

    // Подключаем сигналы
    connect(btn_add, &QPushButton::clicked, this, &BedsWidget::add_bed_clicked);
    connect(btn_remove, &QPushButton::clicked, this, &BedsWidget::remove_bed_clicked);
    connect(btn_plant, &QPushButton::clicked, this, &BedsWidget::plant_clicked);
    connect(btn_collect, &QPushButton::clicked, this, &BedsWidget::collect_clicked);
    connect(btn_water, &QPushButton::clicked, this, &BedsWidget::water_clicked);
    connect(btn_fert, &QPushButton::clicked, this, &BedsWidget::fertilize_clicked);
    connect(btn_light, &QPushButton::clicked, this, &BedsWidget::toggle_light_clicked);
    connect(btn_remove_dead, &QPushButton::clicked, this, &BedsWidget::remove_dead_clicked);

    // Добавляем в layout
    action_layout->addWidget(btn_add);
    action_layout->addWidget(btn_remove);
    action_layout->addWidget(btn_plant);
    action_layout->addWidget(btn_collect);
    action_layout->addWidget(btn_water);
    action_layout->addWidget(btn_fert);
    action_layout->addWidget(btn_light);
    action_layout->addWidget(btn_remove_dead); // Кнопка на месте
    
    main_layout->addLayout(action_layout);
}

void BedsWidget::clear() {
    QLayoutItem* child;
    while ((child = grid_layout_->takeAt(0)) != nullptr) {
        delete child->widget(); delete child;
    }
}

void BedsWidget::set_bed(Bed* bed, bool is_light_on, double current_time_hours) {
    clear();
    if (!bed) {
        info_label_->setText("Выберите грядку.");
        return;
    }

    info_label_->setText(QString("Грядка | Влажность: %1% | pH: %2")
        .arg(bed->get_moisture()).arg(bed->get_soil_ph()));

    int row = 0, col = 0;
    for (std::size_t i = 0; i < bed->get_plant_count(); ++i) {
        Plant* p = bed->get_plant(i);
        QLabel* label = new QLabel(this);
        label->setFixedSize(40, 40);
        
        QString tooltip = QString("Тип: %1\nЗдоровье: %2%\nСтадия: %3")
            .arg(QString::fromStdString(p->get_type())).arg(p->get_current_health())
            .arg(QString::fromStdString(growth_stage_to_string(p->get_stage())));

        if (p->get_is_dead()) {
            label->setStyleSheet("background-color: #555; border: 1px solid #000;");
            tooltip += "\nРАСТЕНИЕ МЕРТВО";
        } else if (p->get_is_mature()) {
            label->setStyleSheet("background-color: #ffeb3b; border: 1px solid #fbc02d;");
            tooltip += "\nГОТОВО К СБОРУ!";
        } else {
            label->setStyleSheet(is_light_on ? "background-color: #8bc34a; border: 1px solid #558b2f;" : "background-color: #4caf50; border: 1px solid #388e3c;");
            
            double hours_left = p->get_hours_until_mature();
            if (is_light_on) hours_left /= 1.5;

            int days = static_cast<int>(hours_left / 24);
            int hours = static_cast<int>(std::fmod(hours_left, 24.0));
            
            if (days > 0) tooltip += QString("\nДо сбора: %1 д. %2 ч.").arg(days).arg(hours);
            else tooltip += QString("\nДо сбора: %1 ч.").arg(hours);
        }
        label->setToolTip(tooltip);
        grid_layout_->addWidget(label, row, col);
        if (++col >= 8) { col = 0; row++; }
    }
}