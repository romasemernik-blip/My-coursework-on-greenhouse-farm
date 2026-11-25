#include "Class/UI/Header/MainWindow.h"
#include "Class/Manager/File_manager/Header/Storage_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Inheritor/Inheritor2/Permanent_Equipment/Header/Lighting.h"
#include "Class/Inheritor/Human/Header/Agronomist.h"

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
#include <QCloseEvent>
#include <cmath>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), current_selected_bed_(nullptr), current_operator_(nullptr) {
    setWindowTitle("Greenhouse Simulator - Управление");
    resize(1300, 850);

    // 1. Инициализация логгеров
    sys_log_ = std::make_unique<System_logger>("system_log.txt");
    act_log_ = std::make_unique<Action_logger>("actions_log.txt");
    sys_log_->log("=== ЗАПУСК ПРОГРАММЫ ===");

    // 2. Создание интерфейса
    setup_ui();

    // 3. Требование выбрать оператора при старте
    if (!select_operator_dialog(true)) {
        sys_log_->log("Оператор не выбран при старте. Выход.");
        QTimer::singleShot(0, this, &QWidget::close); // Закрываем окно
        return;
    }

    // 4. Запуск таймера игры
    game_timer_ = new QTimer(this);
    connect(game_timer_, &QTimer::timeout, this, &MainWindow::on_game_tick);
    game_timer_->start(1000);

    // 5. Автоматическая загрузка данных
    QTimer::singleShot(100, this, &MainWindow::on_load_all_clicked);
}

MainWindow::~MainWindow() {
    if (sys_log_) sys_log_->log("=== ЗАВЕРШЕНИЕ ПРОГРАММЫ ===");
    delete game_timer_;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (sys_log_) sys_log_->log("Пользователь закрыл окно программы.");
    event->accept();
}

// ==========================================
// ИНТЕРФЕЙС (SETUP UI)
// ==========================================
void MainWindow::setup_ui() {
    QSplitter* main_splitter = new QSplitter(Qt::Horizontal, this);

    // --- ЛЕВАЯ ПАНЕЛЬ ---
    QWidget* left_panel = new QWidget(this);
    QVBoxLayout* left_layout = new QVBoxLayout(left_panel);

    // Кнопки управления файлами
    QPushButton* btn_load_all = new QPushButton("Загрузить все данные", this);
    QPushButton* btn_save_all = new QPushButton("Сохранить все данные", this);
    connect(btn_load_all, &QPushButton::clicked, this, &MainWindow::on_load_all_clicked);
    connect(btn_save_all, &QPushButton::clicked, this, &MainWindow::on_save_all_clicked);
    left_layout->addWidget(btn_load_all);
    left_layout->addWidget(btn_save_all);

    // Информация о текущем операторе
    operator_label_ = new QLabel("Текущий оператор: Не выбран", this);
    operator_label_->setStyleSheet("font-weight: bold; color: #2c3e50;");
    QPushButton* btn_switch_operator = new QPushButton("Сменить оператора", this);
    connect(btn_switch_operator, &QPushButton::clicked, this, &MainWindow::on_switch_operator_clicked);
    left_layout->addWidget(operator_label_);
    left_layout->addWidget(btn_switch_operator);

    // Дерево структуры
    tree_widget_ = new QTreeWidget(this);
    tree_widget_->setHeaderLabel("Структура теплицы");
    connect(tree_widget_, &QTreeWidget::itemClicked, this, &MainWindow::on_tree_item_clicked);
    left_layout->addWidget(tree_widget_);

    // --- ПРАВАЯ ПАНЕЛЬ (ВКЛАДКИ) ---
    QTabWidget* tab_widget = new QTabWidget(this);

    // Вкладка 1: Грядки
    QWidget* beds_tab = new QWidget();
    QVBoxLayout* beds_layout = new QVBoxLayout(beds_tab);
    
    bed_info_label_ = new QLabel("Выберите грядку для управления", this);
    bed_info_label_->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 5px;");
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

    // Вкладка 2: Склад
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

    // Вкладка 3: Оборудование
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

    // --- НИЖНЯЯ ПАНЕЛЬ ---
    QWidget* bottom_panel = new QWidget(this);
    QVBoxLayout* bottom_layout = new QVBoxLayout(bottom_panel);
    QHBoxLayout* time_layout = new QHBoxLayout();
    
    speed_label_ = new QLabel("Скорость: x1", this);
    time_label_ = new QLabel("Время: День 1 | 00:00", this);
    time_label_->setStyleSheet("font-weight: bold;");
    
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

    // Сборка панелей
    QSplitter* right_splitter = new QSplitter(Qt::Vertical, this);
    right_splitter->addWidget(tab_widget);
    right_splitter->addWidget(bottom_panel);
    main_splitter->addWidget(left_panel);
    main_splitter->addWidget(right_splitter);
    setCentralWidget(main_splitter);
}

// ==========================================
// ЛОГИКА ОПЕРАТОРА И АГРОНОМА
// ==========================================

bool MainWindow::select_operator_dialog(bool is_startup) {
    QString title = is_startup ? "Регистрация на смену" : "Смена оператора";
    QString prompt = is_startup ? "Выберите оператора для начала работы:" : "Выберите нового оператора:";
    
    // Временный список (в идеале нужно брать из File_manager)
    QStringList operators = {"Иван (Оператор)", "Петр (Оператор)", "Анна (Оператор)"};
    
    bool ok;
    QString selected = QInputDialog::getItem(this, title, prompt, operators, 0, false, &ok);
    if (!ok || selected.isEmpty()) return false;

    current_operator_name_ = selected;
    operator_label_->setText("Текущий оператор: " + selected);
    
    if (sys_log_) sys_log_->log("Оператор установлен: " + selected.toStdString());
    return true;
}

void MainWindow::on_switch_operator_clicked() {
    if (select_operator_dialog(false)) {
        if (act_log_) act_log_->log_action("Система", "Все", "Смена оператора на: " + current_operator_name_.toStdString());
        log("Смена оператора. Вы вошли как: " + current_operator_name_);
    }
}

bool MainWindow::assign_agronomist_dialog() {
    QStringList agronomists = {"Иван Иванов (Агроном)", "Мария Сидорова (Агроном)"};
    bool ok;
    QString selected = QInputDialog::getItem(this, "Назначение агронома", 
        "Выберите агронома для выполнения действия:", agronomists, 0, false, &ok);
    
    if (!ok || selected.isEmpty()) return false;
    
    if (act_log_) act_log_->log_action("Агроном", selected.toStdString(), "Назначен ответственным за действие");
    log("Действие выполняет агроном: " + selected);
    return true;
}

// ==========================================
// ДЕЙСТВИЯ С ГРЯДКАМИ
// ==========================================

void MainWindow::on_add_bed_clicked() {
    try {
        auto new_bed = std::make_unique<Bed>(1, 1, 5.0, 2.0);
        greenhouse_.add_bed(std::move(new_bed));
        
        log("Грядка добавлена.");
        if (act_log_) act_log_->log_action("Оператор", current_operator_name_.toStdString(), "Добавил грядку");
        if (sys_log_) sys_log_->log("Успешно добавлена грядка.");
        
        refresh_all_views();
    } catch (const Greenhouse_full_exception& e) {
        if (sys_log_) sys_log_->log("ОШИБКА при добавлении грядки: " + std::string(e.what()));
        handle_exception(e, "Добавление грядки (нет места)");
    } catch (const std::exception& e) {
        if (sys_log_) sys_log_->log("ОШИБКА: " + std::string(e.what()));
        handle_exception(e, "Добавление грядки");
    }
}

void MainWindow::on_plant_clicked() {
    if (!current_selected_bed_) {
        QMessageBox::warning(this, "Внимание", "Выберите грядку!");
        return;
    }
    if (!assign_agronomist_dialog()) return;

    const auto& types = file_manager_.get_plant_types();
    if (types.empty()) return;

    QStringList items;
    for (const auto& t : types) items << QString::fromStdString(t.get_type_name());
    bool ok;
    QString selected_type = QInputDialog::getItem(this, "Посадка", "Выберите растение:", items, 0, false, &ok);
    if (!ok) return;

    int count = QInputDialog::getInt(this, "Посадка", "Сколько посадить?", 1, 1, 50, 1, &ok);
    if (!ok || count <= 0) return;

    for (const auto& t : types) {
        if (t.get_type_name() == selected_type.toStdString()) {
            try {
                for (int i = 0; i < count; ++i) {
                    auto plant = std::make_unique<Plant>("id_" + std::to_string(rand()), t.get_type_name(), 1.0, 
                        t.get_water_requirement(), t.get_optimal_temperature(), t.get_optimal_humidity());
                    
                    Plant* raw = plant.get();
                    current_selected_bed_->add_plant(std::move(plant)); 
                    
                    double growth_hours = 24.0 / t.get_growth_speed();
                    harvest_times_[raw] = time_manager_.get_total_hours() + growth_hours;
                }
                log(QString("Посажено %1 растений %2.").arg(count).arg(selected_type));
                if (act_log_) act_log_->log_action("Агроном", "Выбранный", "Посадил " + std::to_string(count) + " шт. " + selected_type.toStdString());
                refresh_all_views();
            } catch (const std::exception& e) {
                if (sys_log_) sys_log_->log("ОШИБКА при посадке: " + std::string(e.what()));
                handle_exception(e, "Посадка растений");
            }
            break;
        }
    }
}

void MainWindow::on_water_clicked() {
    if (!current_selected_bed_) return;
    try {
        current_selected_bed_->set_moisture(current_selected_bed_->get_moisture() + 10.0);
        log("Грядка полита.");
        if (act_log_) act_log_->log_action("Оператор", current_operator_name_.toStdString(), "Полил грядку");
        refresh_bed_view();
    } catch (const Invalid_argument_exception& e) {
        if (sys_log_) sys_log_->log("ОШИБКА при поливе: " + std::string(e.what()));
        handle_exception(e, "Полив (влажность > 100%)");
    }
}

void MainWindow::on_fertilize_clicked() {
    if (!current_selected_bed_) return;
    if (!assign_agronomist_dialog()) return;

    try {
        current_selected_bed_->set_soil_ph(current_selected_bed_->get_soil_ph() + 0.5);
        log("Грядка удобрена.");
        if (act_log_) act_log_->log_action("Агроном", "Выбранный", "Удобрил грядку");
        refresh_bed_view();
    } catch (const Invalid_argument_exception& e) {
        if (sys_log_) sys_log_->log("ОШИБКА при удобрении: " + std::string(e.what()));
        handle_exception(e, "Удобрение (pH вышел за пределы)");
    }
}

void MainWindow::on_collect_to_storage_clicked() {
    if (!current_selected_bed_) return;
    if (!assign_agronomist_dialog()) return;

    try {
        double total_amount = 0.0;
        std::vector<std::size_t> mature_indices;

        for (std::size_t i = 0; i < current_selected_bed_->get_plant_count(); ++i) {
            if (current_selected_bed_->get_plant(i)->get_is_mature()) {
                total_amount += 1.5; 
                mature_indices.push_back(i);
            }
        }

        if (mature_indices.empty()) {
            log("Нет созревших растений.");
            return;
        }

        storage_.add("Harvest", total_amount);

        for (auto it = mature_indices.rbegin(); it != mature_indices.rend(); ++it) {
            Plant* p = current_selected_bed_->get_plant(*it);
            harvest_times_.remove(p);
            current_selected_bed_->remove_plant(*it);
        }

        log(QString("Собрано %1 растений на склад.").arg(mature_indices.size()));
        if (act_log_) act_log_->log_action("Агроном", "Выбранный", "Собрал " + std::to_string(mature_indices.size()) + " растений");
        refresh_all_views();
    } catch (const Storage_overflow_exception& e) {
        if (sys_log_) sys_log_->log("ОШИБКА при сборе: Склад переполнен.");
        handle_exception(e, "Сбор урожая (склад переполнен). Данные сохранены.");
    } catch (const std::exception& e) {
        handle_exception(e, "Сбор урожая");
    }
}

void MainWindow::on_remove_dead_clicked() {
    if (!current_selected_bed_) return;
    try {
        int removed = 0;
        for (std::size_t i = 0; i < current_selected_bed_->get_plant_count(); ) {
            if (current_selected_bed_->get_plant(i)->get_is_dead()) {
                harvest_times_.remove(current_selected_bed_->get_plant(i));
                current_selected_bed_->remove_plant(i);
                removed++;
            } else i++;
        }
        if (removed > 0) {
            log(QString("Удалено %1 мертвых растений.").arg(removed));
            if (act_log_) act_log_->log_action("Оператор", current_operator_name_.toStdString(), "Убрал мертвые растения");
        }
        refresh_all_views();
    } catch (const std::exception& e) {
        handle_exception(e, "Удаление мертвых растений");
    }
}

void MainWindow::on_remove_bed_clicked() {
    log("Удаление грядки (в разработке).");
}

// ==========================================
// ОБОРУДОВАНИЕ И СИСТЕМА
// ==========================================

void MainWindow::on_toggle_light_clicked() {
    for (std::size_t i = 0; i < greenhouse_.get_equipment_count(); ++i) {
        Equipment* eq = greenhouse_.get_equipment(i);
        if (eq->get_type() == "Lighting") {
            if (eq->get_is_on()) eq->turn_off(); else eq->turn_on();
            refresh_equipment_view();
            log("Свет переключен.");
            if (act_log_) act_log_->log_action("Оператор", current_operator_name_.toStdString(), "Переключил свет");
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
    if (act_log_) act_log_->log_action("Оператор", current_operator_name_.toStdString(), "Починил оборудование");
}

void MainWindow::on_export_storage_clicked() {
    try {
        QString file = QFileDialog::getSaveFileName(this, "Экспорт", "storage.txt", "Text (*.txt)");
        if (!file.isEmpty()) {
            Storage_loader::save_to_file(file.toStdString(), storage_);
            log("Склад экспортирован.");
            if (sys_log_) sys_log_->log("Экспорт склада в " + file.toStdString());
        }
    } catch (const std::exception& e) {
        handle_exception(e, "Экспорт склада");
    }
}

void MainWindow::on_load_all_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Папка с сохранением");
    if (dir.isEmpty()) return;
    
    if (sys_log_) sys_log_->log("Попытка загрузки данных из " + dir.toStdString());
    
    try {
        file_manager_.load_all(dir.toStdString(), greenhouse_, storage_, time_manager_);
        log("Данные загружены.");
        if (sys_log_) sys_log_->log("Данные успешно загружены.");
        refresh_all_views();
    } catch (const std::exception& e) {
        if (sys_log_) sys_log_->log("ОШИБКА загрузки: " + std::string(e.what()));
        handle_exception(e, "Загрузка данных");
    }
}

void MainWindow::on_save_all_clicked() {
    if (current_save_folder_.isEmpty()) {
        current_save_folder_ = QFileDialog::getExistingDirectory(this, "Папка для сохранения");
    }
    if (current_save_folder_.isEmpty()) return;
    
    if (sys_log_) sys_log_->log("Сохранение данных в " + current_save_folder_.toStdString());
    
    try {
        file_manager_.save_all(current_save_folder_.toStdString(), greenhouse_, storage_, time_manager_);
        log("Данные сохранены.");
        if (sys_log_) sys_log_->log("Данные успешно сохранены.");
    } catch (const std::exception& e) {
        if (sys_log_) sys_log_->log("ОШИБКА сохранения: " + std::string(e.what()));
        handle_exception(e, "Сохранение данных");
    }
}

// ==========================================
// ОБНОВЛЕНИЕ ИНТЕРФЕЙСА (REFRESH)
// ==========================================

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

// ==========================================
// ИГРОВОЙ ЦИКЛ И ОШИБКИ
// ==========================================

void MainWindow::on_game_tick() {
    time_manager_.tick(1.0);
    time_label_->setText(QString::fromStdString(time_manager_.get_formatted_time()));
    check_harvest_time();
    if (current_selected_bed_) refresh_bed_view();
}

void MainWindow::check_harvest_time() {
    double current_time = time_manager_.get_total_hours();
    for (auto it = harvest_times_.begin(); it != harvest_times_.end(); ++it) {
        Plant* plant = it.key();
        if (current_time >= it.value() && plant->get_stage() != Growth_stage::Fruiting) {
            plant->set_stage(Growth_stage::Fruiting);
            log(QString("Растение %1 созрело!").arg(QString::fromStdString(plant->get_type())));
            if (sys_log_) sys_log_->log("Растение " + plant->get_type() + " созрело.");
        }
    }
}

void MainWindow::on_speed_changed(int index) {
    double speeds[] = {1.0, 5.0, 10.0};
    if (index >= 0 && index < 3) {
        time_manager_.set_speed_multiplier(speeds[index]);
        speed_label_->setText(QString("Скорость: x%1").arg(speeds[index]));
        if (sys_log_) sys_log_->log("Скорость времени изменена на x" + std::to_string(speeds[index]));
    }
}

void MainWindow::handle_exception(const std::exception& e, const QString& action_name) {
    QMessageBox::critical(this, "Ошибка", QString("Действие: %1\nПричина: %2")
        .arg(action_name).arg(e.what()));
    log(QString("ОШИБКА: %1").arg(e.what()));
    if (sys_log_) sys_log_->log("ИСКЛЮЧЕНИЕ: " + std::string(e.what()) + " (при " + action_name.toStdString() + ")");
    refresh_all_views();
}

void MainWindow::log(const QString& message) {
    if (log_widget_) log_widget_->append(message);
}