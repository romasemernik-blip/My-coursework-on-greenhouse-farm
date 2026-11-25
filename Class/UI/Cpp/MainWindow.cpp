#include "Class/UI/Header/MainWindow.h"
#include "Class/Manager/File_manager/Header/Storage_loader.h"
#include "Class/Exception/Header/All_exception.h"
#include "Class/Inheritor/Weather/Header/Clear.h"
#include "Class/Inheritor/Weather/Header/Rain.h"
#include "Class/Inheritor/Weather/Header/Storm.h"
#include "Class/Inheritor/Weather/Header/Snow.h"
#include "Class/Inheritor/Inheritor2/Permanent_Equipment/Header/Lighting.h"
#include "Class/Inheritor/Inheritor2/Permanent_Equipment/Header/Sensor.h"
#include "Class/Inheritor/Operation/Header/Auto_Operation.h"
#include "Class/Inheritor/Human/Header/Agronomist.h"
#include "Class/Inheritor/Human/Header/Operator.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QInputDialog>
#include <QLabel>
#include <QTimer>
#include <QCloseEvent>
#include <cmath>
#include <QDateTime>
#include <random>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Greenhouse Simulator - Управление");
    resize(1300, 850);

    sys_log_ = std::make_unique<System_logger>("system_log.txt");
    act_log_ = std::make_unique<Action_logger>("actions_log.txt");
    sys_log_->log("=== ЗАПУСК ПРОГРАММЫ ===");

    current_weather_ = std::make_unique<Clear>();

    setup_ui();

    if (!select_operator_dialog(true)) {
        sys_log_->log("Оператор не выбран при старте. Выход.");
        QTimer::singleShot(0, this, &QWidget::close);
        return;
    }

    game_timer_ = new QTimer(this);
    connect(game_timer_, &QTimer::timeout, this, &MainWindow::on_game_tick);
    game_timer_->start(1000);

    QTimer::singleShot(100, this, &MainWindow::on_load_all_clicked);
}

MainWindow::~MainWindow() {
    if (sys_log_) sys_log_->log("=== ЗАВЕРШЕНИЕ ПРОГРАММЫ ===");
    // delete game_timer_; <-- УДАЛЕНО (Qt сам очистит память)
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (sys_log_) sys_log_->log("Пользователь закрыл окно программы.");
    event->accept();
}

void MainWindow::setup_ui() {
    QSplitter* main_splitter = new QSplitter(Qt::Horizontal, this);

    QWidget* left_panel = new QWidget(this);
    QVBoxLayout* left_layout = new QVBoxLayout(left_panel);

    auto* btn_load_all = new QPushButton("Загрузить все данные", this);
    auto* btn_save_all = new QPushButton("Сохранить все данные", this);
    connect(btn_load_all, &QPushButton::clicked, this, &MainWindow::on_load_all_clicked);
    connect(btn_save_all, &QPushButton::clicked, this, &MainWindow::on_save_all_clicked);
    left_layout->addWidget(btn_load_all);
    left_layout->addWidget(btn_save_all);

    operator_label_ = new QLabel("Текущий оператор: Не выбран", this);
    operator_label_->setStyleSheet("font-weight: bold; color: #2c3e50;");
    auto* btn_switch_operator = new QPushButton("Сменить оператора", this);
    connect(btn_switch_operator, &QPushButton::clicked, this, &MainWindow::on_switch_operator_clicked);
    left_layout->addWidget(operator_label_);
    left_layout->addWidget(btn_switch_operator);

    tree_widget_ = new QTreeWidget(this);
    tree_widget_->setHeaderLabel("Структура теплицы");
    connect(tree_widget_, &QTreeWidget::itemClicked, this, &MainWindow::on_tree_item_clicked);
    left_layout->addWidget(tree_widget_);

    tab_widget_ = new QTabWidget(this);

    beds_widget_ = new BedsWidget(this);
    connect(beds_widget_, &BedsWidget::add_bed_clicked, this, &MainWindow::on_add_bed_clicked);
    connect(beds_widget_, &BedsWidget::remove_bed_clicked, this, &MainWindow::on_remove_bed_clicked);
    connect(beds_widget_, &BedsWidget::plant_clicked, this, &MainWindow::on_plant_clicked);
    connect(beds_widget_, &BedsWidget::collect_clicked, this, &MainWindow::on_collect_to_storage_clicked);
    connect(beds_widget_, &BedsWidget::water_clicked, this, &MainWindow::on_water_clicked);
    connect(beds_widget_, &BedsWidget::fertilize_clicked, this, &MainWindow::on_fertilize_clicked);
    connect(beds_widget_, &BedsWidget::toggle_light_clicked, this, &MainWindow::on_toggle_bed_light_clicked);
    connect(beds_widget_, &BedsWidget::remove_dead_clicked, this, &MainWindow::on_remove_dead_clicked);
    tab_widget_->addTab(beds_widget_, "Грядки");

    auto* storage_tab = new QWidget();
    auto* storage_layout = new QVBoxLayout(storage_tab);
    storage_capacity_label_ = new QLabel("Склад: 0 / 0 кг", this);
    storage_list_widget_ = new QListWidget(this);
    auto* btn_export_storage = new QPushButton("Экспорт склада", this);
    connect(btn_export_storage, &QPushButton::clicked, this, &MainWindow::on_export_storage_clicked);
    storage_layout->addWidget(storage_capacity_label_);
    storage_layout->addWidget(storage_list_widget_);
    storage_layout->addWidget(btn_export_storage);
    tab_widget_->addTab(storage_tab, "Склад");

    auto* equip_tab = new QWidget();
    auto* equip_layout = new QVBoxLayout(equip_tab);
    equipment_list_widget_ = new QListWidget(this);
    auto* equip_btns = new QHBoxLayout();
    auto* btn_add_lamp = new QPushButton("Добавить лампу", this);
    auto* btn_repair_all = new QPushButton("Починить все", this);
    connect(btn_add_lamp, &QPushButton::clicked, this, &MainWindow::on_add_equipment_clicked);
    connect(btn_repair_all, &QPushButton::clicked, this, &MainWindow::on_repair_equipment_clicked);
    equip_btns->addWidget(btn_add_lamp);
    equip_btns->addWidget(btn_repair_all);
    equip_layout->addWidget(equipment_list_widget_);
    equip_layout->addLayout(equip_btns);
    tab_widget_->addTab(equip_tab, "Оборудование");

    auto* humans_tab = new QWidget();
    auto* humans_layout = new QVBoxLayout(humans_tab);
    humans_list_widget_ = new QListWidget(this);
    auto* btn_hire = new QPushButton("Нанять рабочего", this);
    connect(btn_hire, &QPushButton::clicked, this, &MainWindow::on_add_human_clicked);
    humans_layout->addWidget(humans_list_widget_);
    humans_layout->addWidget(btn_hire);
    tab_widget_->addTab(humans_tab, "Персонал");

    auto* schedule_tab = new QWidget();
    auto* sched_layout = new QVBoxLayout(schedule_tab);
    schedule_list_widget_ = new QListWidget(this);
    auto* btn_add_task = new QPushButton("Создать автозадачу (Автополив)", this);
    connect(btn_add_task, &QPushButton::clicked, this, &MainWindow::on_add_schedule_operation_clicked);
    sched_layout->addWidget(schedule_list_widget_);
    sched_layout->addWidget(btn_add_task);
    tab_widget_->addTab(schedule_tab, "Расписание");

    bottom_panel_ = new BottomPanelWidget(this);
    connect(bottom_panel_, &BottomPanelWidget::speed_changed, this, &MainWindow::on_speed_changed);

    QSplitter* right_splitter = new QSplitter(Qt::Vertical, this);
    right_splitter->addWidget(tab_widget_);
    right_splitter->addWidget(bottom_panel_);
    main_splitter->addWidget(left_panel);
    main_splitter->addWidget(right_splitter);
    setCentralWidget(main_splitter);
}

void MainWindow::on_game_tick() {
    time_manager_.tick(1.0);
    
    weather_timer_ += time_manager_.get_speed_multiplier();
    if (weather_timer_ >= 24.0) {
        weather_timer_ = 0.0;
        
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> distr(0, 3);
        int r = distr(gen);
        
        if (r == 0) current_weather_ = std::make_unique<Clear>();
        else if (r == 1) current_weather_ = std::make_unique<Rain>();
        else if (r == 2) current_weather_ = std::make_unique<Storm>();
        else current_weather_ = std::make_unique<Snow>();
        
        log("Погода изменилась на: " + QString::fromStdString(current_weather_->get_name()));
    }

    if (current_selected_bed_) {
        apply_weather_effects(time_manager_.get_speed_multiplier());
    }

    greenhouse_.update(time_manager_.get_speed_multiplier(), is_bed_light_on_);

    double dt = time_manager_.get_speed_multiplier();
    schedule_.update(dt);
    schedule_.execute_all();

    check_harvest_time();
    
    bottom_panel_->set_time_text(QString::fromStdString(time_manager_.get_formatted_time()));
    bottom_panel_->set_weather_text("Погода: " + QString::fromStdString(current_weather_->get_name()));
    
    if (current_selected_bed_) {
        beds_widget_->set_bed(current_selected_bed_, is_bed_light_on_, time_manager_.get_total_hours());
    }
    
    if (tab_widget_->currentIndex() == 4) {
        refresh_schedule_view();
    }
}

void MainWindow::apply_weather_effects(double dt) {
    try {
        double moist_change = current_weather_->get_moisture_effect() * dt;
        double new_moisture = current_selected_bed_->get_moisture() + moist_change;
        if (new_moisture > 100.0) new_moisture = 100.0;
        if (new_moisture < 0.0) new_moisture = 0.0;
        current_selected_bed_->set_moisture(new_moisture);

        double ph_change = current_weather_->get_ph_effect() * dt;
        double new_ph = current_selected_bed_->get_soil_ph() + ph_change;
        if (new_ph > 14.0) new_ph = 14.0;
        if (new_ph < 0.0) new_ph = 0.0;
        current_selected_bed_->set_soil_ph(new_ph);
    } catch (...) {}
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
    double speeds[] = {1.0, 10.0, 50.0, 100.0, 200.0};
    if (index >= 0 && index < 5) {
        time_manager_.set_speed_multiplier(speeds[index]);
        bottom_panel_->set_speed_text(QString("Скорость: x%1").arg(speeds[index]));
    }
}

bool MainWindow::select_operator_dialog(bool is_startup) {
    QString title = is_startup ? "Регистрация на смену" : "Смена оператора";
    QString prompt = is_startup ? "Выберите оператора для начала работы:" : "Выберите нового оператора:";
    
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

void MainWindow::on_add_bed_clicked() {
    try {
        auto new_bed = std::make_unique<Bed>(1, 1, 5.0, 2.0);
        greenhouse_.add_bed(std::move(new_bed));
        log("Грядка добавлена.");
        if (act_log_) act_log_->log_action("Оператор", current_operator_name_.toStdString(), "Добавил грядку");
        refresh_all_views();
    } catch (const Greenhouse_full_exception& e) {
        handle_exception(e, "Добавление грядки (нет места)");
    } catch (const std::exception& e) {
        handle_exception(e, "Добавление грядки");
    }
}

void MainWindow::on_remove_bed_clicked() {
    if (!current_selected_bed_) {
        QMessageBox::warning(this, "Внимание", "Выберите грядку для удаления!");
        return;
    }
    try {
        std::size_t index_to_remove = 0;
        bool found = false;
        for (std::size_t i = 0; i < greenhouse_.get_bed_count(); ++i) {
            if (greenhouse_.get_bed(i) == current_selected_bed_) {
                index_to_remove = i;
                found = true;
                break;
            }
        }
        if (found) {
            greenhouse_.remove_bed(index_to_remove);
            current_selected_bed_ = nullptr;
            log("Грядка удалена.");
            refresh_all_views();
        }
    } catch (const std::exception& e) {
        handle_exception(e, "Удаление грядки");
    }
}

void MainWindow::on_plant_clicked() {
    if (!current_selected_bed_) return;
    if (!assign_agronomist_dialog()) return;

    const auto& types = file_manager_.get_plant_types();
    if (types.empty()) {
        QMessageBox::warning(this, "Нет данных", "Загрузите типы растений!");
        return;
    }

    QStringList items;
    for (const auto& t : types) items << QString::fromStdString(t.get_type_name());
    bool ok;
    QString selected_type = QInputDialog::getItem(this, "Посадка", "Выберите растение:", items, 0, false, &ok);
    if (!ok) return;

    int count = QInputDialog::getInt(this, "Посадка", "Сколько посадить?", 1, 1, 50, 1, &ok);
    if (!ok || count <= 0) return;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distr(0, 100000);

    for (const auto& t : types) {
        if (t.get_type_name() == selected_type.toStdString()) {
            try {
                for (int i = 0; i < count; ++i) {
                    auto plant = std::make_unique<Plant>(
                        "id_" + std::to_string(distr(gen)), t.get_type_name(), 1.0, 
                        t.get_water_requirement(), t.get_optimal_temperature(), t.get_optimal_humidity()
                    );
                    Plant* raw = plant.get();
                    current_selected_bed_->add_plant(std::move(plant)); 
                    
                    double growth_hours = 24.0 / t.get_growth_speed();
                    if (is_bed_light_on_) growth_hours /= 1.5;
                    harvest_times_[raw] = time_manager_.get_total_hours() + growth_hours;
                }
                log(QString("Посажено %1 растений %2.").arg(count).arg(selected_type));
                refresh_all_views();
            } catch (const std::exception& e) {
                handle_exception(e, "Посадка растений");
            }
            break;
        }
    }
}

void MainWindow::on_water_clicked() {
    if (!current_selected_bed_) return;
    try {
        current_selected_bed_->set_moisture(current_selected_bed_->get_moisture() + 15.0);
        log("Грядка полита.");
        refresh_all_views();
    } catch (const Invalid_argument_exception& e) {
        handle_exception(e, "Полив (влажность > 100%)");
    }
}

void MainWindow::on_fertilize_clicked() {
    if (!current_selected_bed_) return;
    if (!assign_agronomist_dialog()) return;
    try {
        current_selected_bed_->set_soil_ph(current_selected_bed_->get_soil_ph() + 0.5);
        log("Грядка удобрена.");
        refresh_all_views();
    } catch (const Invalid_argument_exception& e) {
        handle_exception(e, "Удобрение (pH вышел за пределы)");
    }
}

void MainWindow::on_toggle_bed_light_clicked() {
    if (!current_selected_bed_) return;
    is_bed_light_on_ = !is_bed_light_on_;
    log(QString("Свет на грядке: %1").arg(is_bed_light_on_ ? "ВКЛ" : "ВЫКЛ"));
    refresh_all_views();
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
            harvest_times_.remove(current_selected_bed_->get_plant(*it));
            current_selected_bed_->remove_plant(*it);
        }
        log("Урожай собран.");
        refresh_all_views();
    } catch (const Storage_overflow_exception& e) {
        handle_exception(e, "Сбор урожая (склад переполнен)");
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
            refresh_all_views();
        }
    } catch (const std::exception& e) {
        handle_exception(e, "Удаление мертвых растений");
    }
}

void MainWindow::on_add_equipment_clicked() {
    try {
        auto lamp = std::make_unique<Lighting>("Новая лампа", 50.0, 80.0, 550.0);
        greenhouse_.add_equipment(std::move(lamp));
        log("Лампа добавлена в теплицу.");
        refresh_equipment_view();
    } catch (const std::exception& e) {
        handle_exception(e, "Добавление оборудования");
    }
}

void MainWindow::on_add_human_clicked() {
    auto agr = std::make_unique<Agronomist>("Иван", "Петров", "Сидорович", 1980, "M", 2010, 12);
    humans_.push_back(std::move(agr));
    log("Нанят новый Агроном.");
    refresh_humans_view();
}

void MainWindow::on_add_schedule_operation_clicked() {
    if (!current_selected_bed_) {
        QMessageBox::warning(this, "Ошибка", "Выберите грядку для создания автозадачи.");
        return;
    }

    try {
        Bed* target_bed = current_selected_bed_; 
        
        auto auto_op = std::make_unique<Auto_operation>(
            "Автополив", 
            5.0, nullptr, nullptr, 
            24.0, 1,
            [this, target_bed]() {
                if (target_bed) {
                    try {
                        double new_m = target_bed->get_moisture() + 20.0;
                        if (new_m > 100.0) new_m = 100.0;
                        target_bed->set_moisture(new_m);
                        log("АВТОПОЛИВ: Грядка полита на +20%");
                    } catch(...) {}
                }
            }
        );
        
        schedule_.add_operation(std::move(auto_op));
        log("Добавлена автозадача: автополив каждые 24 часа.");
        refresh_schedule_view();
    } catch (const std::exception& e) {
        handle_exception(e, "Создание автозадачи");
    }
}

void MainWindow::on_repair_equipment_clicked() {
    for (std::size_t i = 0; i < greenhouse_.get_equipment_count(); ++i) {
        greenhouse_.get_equipment(i)->repair();
    }
    refresh_equipment_view();
    log("Оборудование починено.");
}

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

void MainWindow::on_load_all_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Папка с сохранением");
    if (dir.isEmpty()) return;
    if (sys_log_) sys_log_->log("Загрузка данных из " + dir.toStdString());
    try {
        file_manager_.load_all(dir.toStdString(), greenhouse_, storage_, time_manager_);
        log("Данные загружены.");
        refresh_all_views();
    } catch (const std::exception& e) {
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
    } catch (const std::exception& e) {
        handle_exception(e, "Сохранение данных");
    }
}

void MainWindow::on_export_storage_clicked() {
    try {
        QString file = QFileDialog::getSaveFileName(this, "Экспорт", "storage.txt", "Text (*.txt)");
        if (!file.isEmpty()) {
            Storage_loader::save_to_file(file.toStdString(), storage_);
            log("Склад экспортирован.");
        }
    } catch (const std::exception& e) {
        handle_exception(e, "Экспорт склада");
    }
}

void MainWindow::refresh_all_views() {
    refresh_tree();
    refresh_storage_view();
    refresh_equipment_view();
    refresh_humans_view();
    refresh_schedule_view();
    if (current_selected_bed_) {
        beds_widget_->set_bed(current_selected_bed_, is_bed_light_on_, time_manager_.get_total_hours());
    }
}

void MainWindow::refresh_tree() {
    tree_widget_->clear();
    auto* root = new QTreeWidgetItem(tree_widget_);
    root->setText(0, "Теплица");
    for (std::size_t i = 0; i < greenhouse_.get_bed_count(); ++i) {
        auto* bed_item = new QTreeWidgetItem(root);
        bed_item->setText(0, QString("Грядка %1").arg(i + 1));
        bed_item->setData(0, Qt::UserRole, static_cast<int>(i));
        Bed* bed = greenhouse_.get_bed(i);
        for (std::size_t j = 0; j < bed->get_plant_count(); ++j) {
            auto* p_item = new QTreeWidgetItem(bed_item);
            p_item->setText(0, QString::fromStdString(bed->get_plant(j)->get_type()));
        }
    }
    tree_widget_->expandAll();
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
        QString status = eq->get_is_broken() ? "СЛОМАНО" : (eq->get_is_on() ? "ВКЛ" : "ВЫКЛ");
        equipment_list_widget_->addItem(QString("%1 (%2) | Износ: %3% | %4")
            .arg(QString::fromStdString(eq->get_name()))
            .arg(QString::fromStdString(eq->get_type()))
            .arg(eq->get_wear_level())
            .arg(status));
    }
}

void MainWindow::refresh_humans_view() {
    humans_list_widget_->clear();
    for (std::size_t i = 0; i < humans_.size(); ++i) {
        Human* h = humans_[i].get();
        humans_list_widget_->addItem(QString("%1 (%2) | Пол: %3")
            .arg(QString::fromStdString(h->get_full_name()))
            .arg(QString::fromStdString(h->get_role()))
            .arg(QString::fromStdString(h->get_gender())));
    }
}

void MainWindow::refresh_schedule_view() {
    schedule_list_widget_->clear();
    for (std::size_t i = 0; i < schedule_.get_operation_count(); ++i) {
        Operation* op = schedule_.get_operation(i);
        auto* auto_op = dynamic_cast<Auto_operation*>(op);
        
        QString text = QString("%1 (Приоритет: %2)").arg(QString::fromStdString(op->get_name())).arg(op->get_priority());
        if (auto_op) {
            text += QString(" | Интервал: %1 ч.").arg(auto_op->get_interval());
        }
        schedule_list_widget_->addItem(text);
    }
}

void MainWindow::on_tree_item_clicked(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column);
    if (!item) return;
    QVariant data = item->data(0, Qt::UserRole);
    if (data.isValid()) {
        current_selected_bed_ = greenhouse_.get_bed(data.toInt());
        beds_widget_->set_bed(current_selected_bed_, is_bed_light_on_, time_manager_.get_total_hours());
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
    if (bottom_panel_) bottom_panel_->log_message(message);
}