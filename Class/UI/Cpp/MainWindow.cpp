#include "Class/UI/Header/MainWindow.h"
#include "Class/Manager/File_manager/Header/Storage_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Inheritor/Inheritor2/Permanent_Equipment/Header/Lighting.h"
#include "Class/Inheritor/Inheritor2/Single_use_Equipment/Header/Watering_Equipment.h"
#include "Class/Inheritor/Inheritor2/Single_use_Equipment/Header/Fertilizer_Equipment.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QGroupBox>
#include <QTabWidget>
#include <QListWidget>
#include <QInputDialog>
#include <QLabel>
#include <QTimer>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), current_selected_bed_(nullptr) {
    setWindowTitle("Greenhouse Simulator - Управление");
    resize(1300, 850);

    game_timer_ = new QTimer(this);
    connect(game_timer_, &QTimer::timeout, this, &MainWindow::on_game_tick);
    game_timer_->start(1000); // 1 секунда реального времени

    setup_ui();
    QTimer::singleShot(100, this, &MainWindow::on_load_all_clicked);
}

MainWindow::~MainWindow() { delete game_timer_; }

void MainWindow::setup_ui() {
    QSplitter* main_splitter = new QSplitter(Qt::Horizontal, this);

    // ЛЕВАЯ ПАНЕЛЬ
    QWidget* left_panel = new QWidget(this);
    QVBoxLayout* left_layout = new QVBoxLayout(left_panel);
    QPushButton* btn_load_all = new QPushButton("Загрузить все данные", this);
    QPushButton* btn_save_all = new QPushButton("Сохранить все данные", this);
    connect(btn_load_all, &QPushButton::clicked, this, &MainWindow::on_load_all_clicked);
    connect(btn_save_all, &QPushButton::clicked, this, &MainWindow::on_save_all_clicked);
    left_layout->addWidget(btn_load_all);
    left_layout->addWidget(btn_save_all);

    tree_widget_ = new QTreeWidget(this);
    tree_widget_->setHeaderLabel("Структура");
    connect(tree_widget_, &QTreeWidget::itemClicked, this, &MainWindow::on_tree_item_clicked);
    left_layout->addWidget(tree_widget_);

    // ПРАВАЯ ПАНЕЛЬ
    QTabWidget* tab_widget = new QTabWidget(this);

    // Вкладка Грядки
    QWidget* beds_tab = new QWidget();
    QVBoxLayout* beds_layout = new QVBoxLayout(beds_tab);
    bed_info_label_ = new QLabel("Выберите грядку для управления", this);
    bed_info_label_->setStyleSheet("font-weight: bold; font-size: 14px;");
    beds_layout->addWidget(bed_info_label_);
    grid_widget_ = new QWidget(beds_tab);
    grid_layout_ = new QGridLayout(grid_widget_);
    grid_widget_->setStyleSheet("background-color: #f0f0f0; border: 1px solid #ccc;");
    beds_layout->addWidget(grid_widget_);

    QHBoxLayout* action_layout = new QHBoxLayout();
    QPushButton* btn_add_bed = new QPushButton("Добавить грядку", this);
    QPushButton* btn_remove_bed = new QPushButton("Удалить грядку", this);
    QPushButton* btn_plant = new QPushButton("Посадить", this);
    QPushButton* btn_collect = new QPushButton("Собрать урожай", this);
    QPushButton* btn_water = new QPushButton("Полить", this);
    QPushButton* btn_fert = new QPushButton("Удобрить", this);
    QPushButton* btn_remove_dead = new QPushButton("Убрать мертвые", this);
    
    connect(btn_add_bed, &QPushButton::clicked, this, &MainWindow::on_add_bed_clicked);
    connect(btn_remove_bed, &QPushButton::clicked, this, &MainWindow::on_remove_bed_clicked);
    connect(btn_plant, &QPushButton::clicked, this, &MainWindow::on_plant_clicked);
    connect(btn_collect, &QPushButton::clicked, this, &MainWindow::on_collect_to_storage_clicked);
    connect(btn_water, &QPushButton::clicked, this, &MainWindow::on_water_clicked);
    connect(btn_fert, &QPushButton::clicked, this, &MainWindow::on_fertilize_clicked);
    connect(btn_remove_dead, &QPushButton::clicked, this, &MainWindow::on_remove_dead_clicked);

    action_layout->addWidget(btn_add_bed);
    action_layout->addWidget(btn_remove_bed);
    action_layout->addWidget(btn_plant);
    action_layout->addWidget(btn_collect);
    action_layout->addWidget(btn_water);
    action_layout->addWidget(btn_fert);
    action_layout->addWidget(btn_remove_dead);
    beds_layout->addLayout(action_layout);
    tab_widget->addTab(beds_tab, "Грядки");

    // Вкладка Склад
    QWidget* storage_tab = new QWidget();
    QVBoxLayout* storage_layout = new QVBoxLayout(storage_tab);
    storage_capacity_label_ = new QLabel("Склад: 0 / 0 кг", this);
    storage_list_widget_ = new QListWidget(this);
    QPushButton* btn_export_storage = new QPushButton("Экспорт склада", this);
    connect(btn_export_storage, &QPushButton::clicked, this, &MainWindow::on_export_storage_clicked);
    storage_layout->addWidget(storage_capacity_label_);
    storage_layout->addWidget(storage_list_widget_);
    storage_layout->addWidget(btn_export_storage);
    tab_widget->addTab(storage_tab, "Склад");

    // Вкладка Оборудование
    QWidget* equip_tab = new QWidget();
    QVBoxLayout* equip_layout = new QVBoxLayout(equip_tab);
    equipment_list_widget_ = new QListWidget(this);
    QHBoxLayout* equip_btn_layout = new QHBoxLayout();
    QPushButton* btn_toggle_light = new QPushButton("Вкл/Выкл Свет", this);
    QPushButton* btn_repair = new QPushButton("Починить оборудование", this);
    connect(btn_toggle_light, &QPushButton::clicked, this, &MainWindow::on_toggle_light_clicked);
    connect(btn_repair, &QPushButton::clicked, this, &MainWindow::on_repair_equipment_clicked);
    equip_btn_layout->addWidget(btn_toggle_light);
    equip_btn_layout->addWidget(btn_repair);
    equip_layout->addWidget(equipment_list_widget_);
    equip_layout->addLayout(equip_btn_layout);
    tab_widget->addTab(equip_tab, "Оборудование");

    // НИЖНЯЯ ПАНЕЛЬ
    QWidget* bottom_panel = new QWidget(this);
    QVBoxLayout* bottom_layout = new QVBoxLayout(bottom_panel);
    QHBoxLayout* time_layout = new QHBoxLayout();
    speed_label_ = new QLabel("Скорость: x1", this);
    time_label_ = new QLabel("Время: 00:00", this);
    QComboBox* speed_combo = new QComboBox(this);
    speed_combo->addItem("x1", 1.0);
    speed_combo->addItem("x5", 5.0);
    speed_combo->addItem("x10", 10.0);
    connect(speed_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_speed_changed);
    time_layout->addWidget(time_label_);
    time_layout->addWidget(speed_label_);
    time_layout->addWidget(speed_combo);
    time_layout->addStretch();
    bottom_layout->addLayout(time_layout);

    log_widget_ = new QTextEdit(this);
    log_widget_->setReadOnly(true);
    bottom_layout->addWidget(log_widget_);

    QSplitter* right_splitter = new QSplitter(Qt::Vertical, this);
    right_splitter->addWidget(tab_widget);
    right_splitter->addWidget(bottom_panel);
    main_splitter->addWidget(left_panel);
    main_splitter->addWidget(right_splitter);
    setCentralWidget(main_splitter);
}

// --- ИГРОВОЙ ЦИКЛ И ВРЕМЯ ---
void MainWindow::on_game_tick() {
    time_manager_.tick(1.0); // Передаем 1 секунду
    time_label_->setText(QString::fromStdString(time_manager_.get_formatted_time()));
    
    check_harvest_time();

    if (current_selected_bed_) refresh_bed_view();
}

void MainWindow::on_speed_changed(int index) {
    double speeds[] = {1.0, 5.0, 10.0};
    if (index >= 0 && index < 3) {
        time_manager_.set_speed_multiplier(speeds[index]);
        speed_label_->setText(QString("Скорость: x%1").arg(speeds[index]));
    }
}

void MainWindow::check_harvest_time() {
    double current_time = time_manager_.get_total_hours();
    for (auto it = harvest_times_.begin(); it != harvest_times_.end(); ++it) {
        Plant* plant = it.key();
        if (current_time >= it.value() && plant->get_stage() != Growth_stage::Fruiting) {
            plant->set_stage(Growth_stage::Fruiting);
            log(QString("Растение %1 созрело!").arg(QString::fromStdString(plant->get_type())));
        }
    }
}

// --- ЗАГРУЗКА/СОХРАНЕНИЕ ---
void MainWindow::on_load_all_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку с сохранением");
    if (dir.isEmpty()) return;
    current_save_folder_ = dir;
    try {
        file_manager_.load_all(dir.toStdString(), greenhouse_, storage_, time_manager_);
        log("Данные загружены.");
        refresh_all_views();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка загрузки", e.what());
    }
}

void MainWindow::on_save_all_clicked() {
    if (current_save_folder_.isEmpty()) {
        current_save_folder_ = QFileDialog::getExistingDirectory(this, "Папка для сохранения");
        if (current_save_folder_.isEmpty()) return;
    }
    try {
        file_manager_.save_all(current_save_folder_.toStdString(), greenhouse_, storage_, time_manager_);
        log("Данные сохранены.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка сохранения", e.what());
    }
}

// --- ОБНОВЛЕНИЕ ИНТЕРФЕЙСА ---
void MainWindow::refresh_all_views() {
    refresh_tree();
    refresh_bed_view();
    refresh_storage_view();
    refresh_equipment_view();
}

void MainWindow::on_tree_item_clicked(QTreeWidgetItem *item, int column) {
    if (!item) return;
    QVariant data = item->data(0, Qt::UserRole);
    if (data.isValid()) {
        current_selected_bed_ = greenhouse_.get_bed(data.toInt());
        refresh_bed_view();
    }
}

void MainWindow::refresh_tree() {
    tree_widget_->clear();
    QTreeWidgetItem* root = new QTreeWidgetItem(tree_widget_);
    root->setText(0, "Теплица");
    for (std::size_t i = 0; i < greenhouse_.get_bed_count(); ++i) {
        QTreeWidgetItem* bed_item = new QTreeWidgetItem(root);
        bed_item->setText(0, QString("Грядка %1").arg(i + 1));
        bed_item->setData(0, Qt::UserRole, static_cast<int>(i));
        Bed* bed = greenhouse_.get_bed(i);
        for (std::size_t j = 0; j < bed->get_plant_count(); ++j) {
            QTreeWidgetItem* p_item = new QTreeWidgetItem(bed_item);
            p_item->setText(0, QString::fromStdString(bed->get_plant(j)->get_type()));
        }
    }
    tree_widget_->expandAll();
}

void MainWindow::refresh_bed_view() {
    QLayoutItem* child;
    while ((child = grid_layout_->takeAt(0)) != nullptr) {
        delete child->widget(); delete child;
    }
    if (!current_selected_bed_) {
        bed_info_label_->setText("Выберите грядку.");
        return;
    }
    bed_info_label_->setText(QString("Грядка | Влажность: %1% | pH: %2")
        .arg(current_selected_bed_->get_moisture()).arg(current_selected_bed_->get_soil_ph()));

    int row = 0, col = 0;
    for (std::size_t i = 0; i < current_selected_bed_->get_plant_count(); ++i) {
        Plant* p = current_selected_bed_->get_plant(i);
        QLabel* label = new QLabel(this);
        label->setFixedSize(40, 40);
        
        QString tooltip = QString::fromStdString(p->get_type());
        if (p->get_is_mature()) {
            label->setStyleSheet("background-color: #ffeb3b; border: 1px solid #fbc02d;");
            tooltip += "\nГОТОВО К СБОРУ!";
        } else if (p->get_is_dead()) {
            label->setStyleSheet("background-color: #555; border: 1px solid #000;");
        } else {
            label->setStyleSheet("background-color: #4caf50; border: 1px solid #388e3c;");
            if (harvest_times_.contains(p)) {
                double left = harvest_times_[p] - time_manager_.get_total_hours();
                tooltip += QString("\nДо сбора: %1 ч.").arg(QString::number(left, 'f', 1));
            }
        }
        label->setToolTip(tooltip);
        grid_layout_->addWidget(label, row, col);
        if (++col >= 8) { col = 0; row++; }
    }
}

void MainWindow::refresh_storage_view() {
    storage_list_widget_->clear();
    storage_capacity_label_->setText(QString("Склад: %1 / %2 кг")
        .arg(storage_.get_current_load()).arg(storage_.get_capacity()));
    for (const auto& pair : storage_.get_contents()) {
        storage_list_widget_->addItem(QString("%1: %2 кг")
            .arg(QString::fromStdString(pair.first)).arg(pair.second));
    }
}

void MainWindow::refresh_equipment_view() {
    equipment_list_widget_->clear();
    for (std::size_t i = 0; i < greenhouse_.get_equipment_count(); ++i) {
        Equipment* eq = greenhouse_.get_equipment(i);
        equipment_list_widget_->addItem(QString("%1 (%2) | Износ: %3% | %4")
            .arg(QString::fromStdString(eq->get_name()))
            .arg(QString::fromStdString(eq->get_type()))
            .arg(eq->get_wear_level())
            .arg(eq->get_is_broken() ? "СЛОМАНО" : "ОК"));
    }
}

// --- ДЕЙСТВИЯ С ГРЯДКАМИ ---
void MainWindow::on_add_bed_clicked() {
    try {
        greenhouse_.add_bed(std::make_unique<Bed>(1, 1, 5.0, 2.0));
        refresh_all_views();
    } catch (const Greenhouse_full_exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

void MainWindow::on_remove_bed_clicked() {
    if (!current_selected_bed_) return;
    log("Удаление грядки (требует индексации).");
    current_selected_bed_ = nullptr;
    refresh_all_views();
}

void MainWindow::on_plant_clicked() {
    if (!current_selected_bed_) return;
    const auto& types = file_manager_.get_plant_types();
    if (types.empty()) return;

    QStringList items;
    for (const auto& t : types) items << QString::fromStdString(t.get_type_name());
    bool ok;
    QString selected = QInputDialog::getItem(this, "Посадка", "Выберите:", items, 0, false, &ok);
    if (!ok) return;

    for (const auto& t : types) {
        if (t.get_type_name() == selected.toStdString()) {
            auto plant = std::make_unique<Plant>("id", t.get_type_name(), 1.0, t.get_water_requirement(), t.get_optimal_temperature(), t.get_optimal_humidity());
            Plant* raw = plant.get();
            current_selected_bed_->add_plant(std::move(plant));
            
            // Время сбора = текущее время + (24 часа / скорость роста)
            double growth_hours = 24.0 / t.get_growth_speed();
            harvest_times_[raw] = time_manager_.get_total_hours() + growth_hours;
            
            log(QString("Посажено: %1. Созреет через %2 ч.").arg(selected).arg(growth_hours));
            refresh_all_views();
            break;
        }
    }
}

void MainWindow::on_collect_to_storage_clicked() {
    if (!current_selected_bed_) return;
    try {
        int count = 0;
        for (std::size_t i = 0; i < current_selected_bed_->get_plant_count(); ) {
            Plant* p = current_selected_bed_->get_plant(i);
            if (p->get_is_mature()) {
                storage_.add(p->get_type(), 1.5);
                harvest_times_.remove(p); // Удаляем из карты времени
                current_selected_bed_->remove_plant(i);
                count++;
            } else i++;
        }
        if (count > 0) { log(QString("Собрано %1 растений.").arg(count)); refresh_all_views(); }
    } catch (const Storage_overflow_exception& e) {
        QMessageBox::critical(this, "Склад переполнен", e.what());
    }
}

void MainWindow::on_water_clicked() {
    if (current_selected_bed_) {
        current_selected_bed_->set_moisture(current_selected_bed_->get_moisture() + 10.0);
        refresh_bed_view();
    }
}

void MainWindow::on_fertilize_clicked() {
    if (current_selected_bed_) {
        current_selected_bed_->set_soil_ph(current_selected_bed_->get_soil_ph() + 0.1);
        refresh_bed_view();
    }
}

void MainWindow::on_remove_dead_clicked() {
    if (!current_selected_bed_) return;
    for (std::size_t i = 0; i < current_selected_bed_->get_plant_count(); ) {
        if (current_selected_bed_->get_plant(i)->get_is_dead()) {
            harvest_times_.remove(current_selected_bed_->get_plant(i));
            current_selected_bed_->remove_plant(i);
        } else i++;
    }
    refresh_all_views();
}

// --- ОБОРУДОВАНИЕ ---
void MainWindow::on_toggle_light_clicked() {
    for (std::size_t i = 0; i < greenhouse_.get_equipment_count(); ++i) {
        Equipment* eq = greenhouse_.get_equipment(i);
        if (eq->get_type() == "Lighting") {
            if (eq->get_is_on()) eq->turn_off(); else eq->turn_on();
            refresh_equipment_view();
            log("Свет переключен.");
            return;
        }
    }
}

void MainWindow::on_repair_equipment_clicked() {
    for (std::size_t i = 0; i < greenhouse_.get_equipment_count(); ++i) {
        greenhouse_.get_equipment(i)->repair();
    }
    refresh_equipment_view();
    log("Оборудование починено.");
}

void MainWindow::on_export_storage_clicked() {
    try {
        QString file = QFileDialog::getSaveFileName(this, "Экспорт", "storage.txt", "Text (*.txt)");
        if (!file.isEmpty()) {
            Storage_loader::save_to_file(file.toStdString(), storage_);
            log("Склад экспортирован.");
        }
    } catch (const Greenhouse_exception& e) { QMessageBox::critical(this, "Ошибка", e.what()); }
}

void MainWindow::log(const QString& message) { if (log_widget_) log_widget_->append(message); }