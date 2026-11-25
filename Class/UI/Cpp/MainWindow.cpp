#include "Class/UI/Header/MainWindow.h"
#include "Class/Manager/File_manager/Header/Storage_loader.h"
#include "Class/Exception/Header/All_exception.h"

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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Greenhouse Simulator - Управление");
    resize(1200, 800);

    // Инициализация таймера игры
    game_timer_ = new QTimer(this);
    connect(game_timer_, &QTimer::timeout, this, &MainWindow::on_game_tick);
    game_timer_->start(1000); // Тик каждую секунду реального времени
    real_seconds_accumulator_ = 0.0;

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // ==========================================
    // ЛЕВАЯ ПАНЕЛЬ (Загрузка и Дерево)
    // ==========================================
    QWidget* left_panel = new QWidget(this);
    QVBoxLayout* left_layout = new QVBoxLayout(left_panel);

    QPushButton* btn_load_full = new QPushButton("Загрузить проект (полностью)", this);
    QPushButton* btn_load_partial = new QPushButton("Загрузить типы растений", this);
    QPushButton* btn_save_full = new QPushButton("Сохранить проект", this);
    
    connect(btn_load_full, &QPushButton::clicked, this, &MainWindow::on_load_full_clicked);
    connect(btn_load_partial, &QPushButton::clicked, this, &MainWindow::on_load_partial_clicked);
    connect(btn_save_full, &QPushButton::clicked, this, &MainWindow::on_save_full_clicked);

    left_layout->addWidget(btn_load_full);
    left_layout->addWidget(btn_load_partial);
    left_layout->addWidget(btn_save_full);

    tree_widget_ = new QTreeWidget(this);
    tree_widget_->setHeaderLabel("Структура теплицы");
    connect(tree_widget_, &QTreeWidget::itemClicked, this, &MainWindow::on_tree_item_clicked);
    left_layout->addWidget(tree_widget_);

    // ==========================================
    // ПРАВАЯ ПАНЕЛЬ (Вкладки)
    // ==========================================
    QTabWidget* tab_widget = new QTabWidget(this);

    // --- ВКЛАДКА 1: ГРЯДКИ ---
    QWidget* beds_tab = new QWidget();
    QVBoxLayout* beds_layout = new QVBoxLayout(beds_tab);

    QGroupBox* bed_group = new QGroupBox("Визуализация грядки (Вид сверху)", beds_tab);
    QVBoxLayout* bed_layout = new QVBoxLayout(bed_group);
    bed_view_widget_ = new QWidget(bed_group);
    bed_view_widget_->setStyleSheet("background-color: #e0f8e0; border: 2px solid #4caf50;");
    bed_view_widget_->setMinimumHeight(200);
    bed_layout->addWidget(bed_view_widget_);
    beds_layout->addWidget(bed_group);

    QHBoxLayout* action_layout = new QHBoxLayout();
    QPushButton* btn_add_bed = new QPushButton("Добавить грядку", this);
    QPushButton* btn_plant = new QPushButton("Посадить", this);
    QPushButton* btn_water = new QPushButton("Полить", this);
    QPushButton* btn_collect = new QPushButton("Собрать на склад", this);
    
    connect(btn_add_bed, &QPushButton::clicked, this, &MainWindow::on_add_bed_clicked);
    connect(btn_plant, &QPushButton::clicked, this, &MainWindow::on_plant_clicked);
    connect(btn_water, &QPushButton::clicked, this, &MainWindow::on_water_fertilize_clicked);
    connect(btn_collect, &QPushButton::clicked, this, &MainWindow::on_collect_to_storage_clicked);

    action_layout->addWidget(btn_add_bed);
    action_layout->addWidget(btn_plant);
    action_layout->addWidget(btn_water);
    action_layout->addWidget(btn_collect);
    beds_layout->addLayout(action_layout);
    tab_widget->addTab(beds_tab, "Грядки");

    // --- ВКЛАДКА 2: СКЛАД ---
    QWidget* storage_tab = new QWidget();
    QVBoxLayout* storage_layout = new QVBoxLayout(storage_tab);

    storage_capacity_label_ = new QLabel("Склад: 0 / 0 кг", this);
    storage_list_widget_ = new QListWidget(this);
    
    QPushButton* btn_export_storage = new QPushButton("Экспортировать склад в файл", this);
    connect(btn_export_storage, &QPushButton::clicked, this, &MainWindow::on_export_storage_clicked);

    storage_layout->addWidget(storage_capacity_label_);
    storage_layout->addWidget(storage_list_widget_);
    storage_layout->addWidget(btn_export_storage);
    tab_widget->addTab(storage_tab, "Склад урожая");

    // ==========================================
    // НИЖНЯЯ ПАНЕЛЬ (Время и Логи)
    // ==========================================
    QWidget* bottom_panel = new QWidget(this);
    QVBoxLayout* bottom_layout = new QVBoxLayout(bottom_panel);
    
    QHBoxLayout* time_layout = new QHBoxLayout();
    speed_label_ = new QLabel("Ускорение времени: x1", this);
    QComboBox* speed_combo = new QComboBox(this);
    speed_combo->addItem("x1", 1.0);
    speed_combo->addItem("x5", 5.0);
    speed_combo->addItem("x10", 10.0);
    connect(speed_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_speed_changed);
    time_layout->addWidget(speed_label_);
    time_layout->addWidget(speed_combo);
    time_layout->addStretch();
    bottom_layout->addLayout(time_layout);

    log_widget_ = new QTextEdit(this);
    log_widget_->setReadOnly(true);
    bottom_layout->addWidget(log_widget_);

    // --- СБОРКА ВСЕГО ВМЕСТЕ ---
    QSplitter* right_splitter = new QSplitter(Qt::Vertical, this);
    right_splitter->addWidget(tab_widget);
    right_splitter->addWidget(bottom_panel);

    splitter->addWidget(left_panel);
    splitter->addWidget(right_splitter);
    setCentralWidget(splitter);
    
    log("Программа запущена.");
}

MainWindow::~MainWindow() {
    delete game_timer_;
}

// ==========================================
// ИГРОВОЙ ЦИКЛ И ТАЙМЕР
// ==========================================

void MainWindow::on_game_tick() {
    // Здесь должна быть логика обновления времени и роста растений
    // Например: time_manager_.update(dt); 
    // Для простоты просто обновляем время на экране
    update_time_label();
}

void MainWindow::update_time_label() {
    // Если у Time_manager есть метод получения текущего времени, используйте его.
    // time_label_->setText(...);
}

void MainWindow::on_speed_changed(int index) {
    // Примерная логика смены скорости
    double speed = 1.0;
    if (index == 1) speed = 5.0;
    else if (index == 2) speed = 10.0;
    
    speed_label_->setText(QString("Ускорение времени: x%1").arg(speed));
    log(QString("Скорость времени изменена на x%1").arg(speed));
}

// ==========================================
// РАБОТА С ФАЙЛАМИ
// ==========================================

void MainWindow::on_load_full_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку с сохранением");
    if (dir.isEmpty()) return;

    try {
        file_manager_.load_all(dir.toStdString(), greenhouse_, storage_, time_manager_);
        log("Проект успешно загружен.");
        refresh_tree();
        refresh_storage_view();
    } catch (const File_open_exception& e) {
        QMessageBox::critical(this, "Ошибка файла", e.what());
        log(QString("Ошибка открытия файла: %1").arg(e.what()));
    } catch (const File_format_exception& e) {
        QMessageBox::critical(this, "Ошибка формата", e.what());
        log(QString("Ошибка формата данных: %1").arg(e.what()));
    } catch (const Greenhouse_exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
        log(QString("Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::on_save_full_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку для сохранения");
    if (dir.isEmpty()) return;

    try {
        file_manager_.save_all(dir.toStdString(), greenhouse_, storage_, time_manager_);
        log("Проект успешно сохранен.");
    } catch (const Greenhouse_exception& e) {
        QMessageBox::critical(this, "Ошибка сохранения", e.what());
        log(QString("Ошибка сохранения: %1").arg(e.what()));
    }
}

void MainWindow::on_load_partial_clicked() {
    QString file = QFileDialog::getOpenFileName(this, "Загрузить типы растений", "", "Text Files (*.txt)");
    if (file.isEmpty()) return;

    try {
        file_manager_.load_plant_types(file.toStdString());
        log("Типы растений успешно загружены.");
    } catch (const Greenhouse_exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
        log(QString("Ошибка загрузки типов: %1").arg(e.what()));
    }
}

// ==========================================
// РАБОТА С ГРЯДКАМИ И РАСТЕНИЯМИ
// ==========================================

void MainWindow::on_add_bed_clicked() {
    bool ok;
    int count = QInputDialog::getInt(this, "Добавить грядку", "Сколько грядок добавить?", 1, 1, 10, 1, &ok);
    if (!ok) return;

    try {
        for (int i = 0; i < count; ++i) {
            // Создаем грядку с параметрами по умолчанию или запрашиваем их
            auto new_bed = std::make_unique<Bed>(); // Используем конструктор по умолчанию
            greenhouse_.add_bed(std::move(new_bed));
        }
        log(QString("Добавлено %1 грядок.").arg(count));
        refresh_tree();
    } catch (const Greenhouse_full_exception& e) {
        QMessageBox::warning(this, "Теплица переполнена", e.what());
        log(QString("Ошибка: %1").arg(e.what()));
    } catch (const Greenhouse_exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
        log(QString("Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::on_water_fertilize_clicked() {
    if (!current_selected_bed_) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите грядку в дереве слева!");
        return;
    }

    try {
        // Пример: увеличиваем влажность на 10%
        double current_moisture = current_selected_bed_->get_moisture();
        current_selected_bed_->set_moisture(current_moisture + 10.0);
        
        log(QString("Грядка полита. Влажность: %1%").arg(current_selected_bed_->get_moisture()));
        refresh_bed_view(current_selected_bed_);
    } catch (const Invalid_argument_exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
        log(QString("Ошибка аргумента: %1").arg(e.what()));
    }
}

void MainWindow::on_remove_bed_clicked() {
    if (!current_selected_bed_) return;
    // Логика удаления грядки (требует поиска индекса и вызова greenhouse_.remove_bed())
    log("Функция удаления грядки в разработке.");
}

void MainWindow::on_plant_clicked() {
    if (!current_selected_bed_) {
        QMessageBox::warning(this, "Внимание", "Выберите грядку!");
        return;
    }
    
    const auto& types = file_manager_.get_plant_types();
    if (types.empty()) {
        QMessageBox::warning(this, "Нет данных", "Сначала загрузите типы растений!");
        return;
    }

    // Выбор типа растения
    QStringList items;
    for (const auto& t : types) items << QString::fromStdString(t.get_type_name());
    bool ok;
    QString selected = QInputDialog::getItem(this, "Посадка", "Выберите тип растения:", items, 0, false, &ok);
    if (!ok || selected.isEmpty()) return;

    // Находим нужный тип
    const Plant_type_data* selected_type = nullptr;
    for (const auto& t : types) {
        if (t.get_type_name() == selected.toStdString()) {
            selected_type = &t;
            break;
        }
    }

    if (selected_type) {
        try {
            // Создаем растение
            auto plant = std::make_unique<Plant>(
                "id_" + std::to_string(rand()),
                selected_type->get_type_name(),
                10.0, // height
                selected_type->get_water_requirement(),
                selected_type->get_optimal_temperature(),
                selected_type->get_optimal_humidity()
            );
            
            current_selected_bed_->add_plant(std::move(plant));
            log(QString("Посажено растение: %1").arg(selected));
            refresh_bed_view(current_selected_bed_);
        } catch (const Greenhouse_exception& e) {
            QMessageBox::warning(this, "Ошибка посадки", e.what());
        }
    }
}

void MainWindow::on_water_clicked() {
    // То же самое, что и on_water_fertilize_clicked
    on_water_fertilize_clicked();
}

void MainWindow::on_fertilize_clicked() {
    if (!current_selected_bed_) return;
    // Увеличиваем pH или добавляем питательные вещества
    log("Грядка удобрена.");
}

void MainWindow::on_collect_to_storage_clicked() {
    if (!current_selected_bed_) {
        QMessageBox::warning(this, "Внимание", "Выберите грядку!");
        return;
    }
    
    try {
        int count = 0;
        for (std::size_t i = 0; i < current_selected_bed_->get_plant_count(); ) {
            Plant* p = current_selected_bed_->get_plant(i);
            
            if (p->get_is_mature()) {
                storage_.add(p->get_type(), 1.5); // 1.5 кг за штуку
                current_selected_bed_->remove_plant(i);
                count++;
            } else {
                i++;
            }
        }
        
        if (count > 0) {
            log(QString("Успешно собрано %1 растений на склад!").arg(count));
            refresh_bed_view(current_selected_bed_);
            refresh_storage_view();
        } else {
            log("На грядке нет созревших растений.");
        }
    } catch (const Storage_overflow_exception& e) {
        QMessageBox::critical(this, "Склад переполнен", e.what());
        log(QString("Склад переполнен: %1").arg(e.what()));
    } catch (const Greenhouse_exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
        log(QString("Ошибка: %1").arg(e.what()));
    }
}

void MainWindow::on_remove_dead_clicked() {
    if (!current_selected_bed_) return;
    
    int removed = 0;
    for (std::size_t i = 0; i < current_selected_bed_->get_plant_count(); ) {
        Plant* p = current_selected_bed_->get_plant(i);
        if (p->get_is_dead()) {
            current_selected_bed_->remove_plant(i);
            removed++;
        } else {
            i++;
        }
    }
    if (removed > 0) {
        log(QString("Удалено %1 мертвых растений.").arg(removed));
        refresh_bed_view(current_selected_bed_);
    }
}

// ==========================================
// ОБОРУДОВАНИЕ И РАБОЧИЕ
// ==========================================

void MainWindow::on_toggle_light_clicked() {
    log("Переключение света (в разработке).");
}

void MainWindow::on_repair_equipment_clicked() {
    log("Ремонт оборудования (в разработке).");
}

void MainWindow::on_assign_task_clicked() {
    log("Назначение задачи (в разработке).");
}

// ==========================================
// ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ИНТЕРФЕЙСА
// ==========================================

void MainWindow::on_tree_item_clicked(QTreeWidgetItem *item, int column) {
    if (!item) return;
    
    // Предполагаем, что в item->data(0, Qt::UserRole) хранится индекс грядки
    QVariant data = item->data(0, Qt::UserRole);
    if (data.isValid()) {
        int bed_index = data.toInt();
        if (bed_index >= 0 && bed_index < greenhouse_.get_bed_count()) {
            current_selected_bed_ = greenhouse_.get_bed(bed_index);
            log(QString("Выбрана грядка №%1").arg(bed_index + 1));
            refresh_bed_view(current_selected_bed_);
        }
    }
}

void MainWindow::refresh_tree() {
    tree_widget_->clear();
    
    QTreeWidgetItem* root = new QTreeWidgetItem(tree_widget_);
    root->setText(0, QString::fromStdString(greenhouse_.get_name()));
    
    for (std::size_t i = 0; i < greenhouse_.get_bed_count(); ++i) {
        QTreeWidgetItem* bed_item = new QTreeWidgetItem(root);
        bed_item->setText(0, QString("Грядка %1").arg(i + 1));
        bed_item->setData(0, Qt::UserRole, static_cast<int>(i)); // Сохраняем индекс
        
        Bed* bed = greenhouse_.get_bed(i);
        if (bed) {
            for (std::size_t j = 0; j < bed->get_plant_count(); ++j) {
                Plant* p = bed->get_plant(j);
                if (p) {
                    QTreeWidgetItem* plant_item = new QTreeWidgetItem(bed_item);
                    plant_item->setText(0, QString::fromStdString(p->get_type()));
                }
            }
        }
    }
    tree_widget_->expandAll();
}

void MainWindow::refresh_bed_view(Bed* bed) {
    if (!bed || !bed_view_widget_) return;
    
    // Здесь можно нарисовать растения на bed_view_widget_ с помощью QPainter
    // Для простоты просто меняем цвет фона в зависимости от влажности
    if (bed->get_moisture() < 30.0) {
        bed_view_widget_->setStyleSheet("background-color: #ffcccc; border: 2px solid #f44336;"); // Красный - сухо
    } else {
        bed_view_widget_->setStyleSheet("background-color: #e0f8e0; border: 2px solid #4caf50;"); // Зеленый - норм
    }
}

void MainWindow::refresh_storage_view() {
    storage_list_widget_->clear();
    
    storage_capacity_label_->setText(QString("Склад: %1 / %2 кг")
        .arg(storage_.get_current_load())
        .arg(storage_.get_capacity()));

    const auto& contents = storage_.get_contents();
    for (const auto& pair : contents) {
        QString item_text = QString("%1: %2 кг")
            .arg(QString::fromStdString(pair.first))
            .arg(pair.second);
        storage_list_widget_->addItem(item_text);
    }
}

void MainWindow::refresh_equipment_view() {
    // if (equipment_list_) equipment_list_->clear();
}

void MainWindow::refresh_agronomist_view() {
    // if (agronomist_list_) agronomist_list_->clear();
}

void MainWindow::log(const QString& message) {
    if (log_widget_) {
        log_widget_->append(message);
    }
}
void MainWindow::on_export_storage_clicked() {
    try {
        QString file = QFileDialog::getSaveFileName(this, "Экспорт склада", "storage_export.txt", "Text Files (*.txt)");
        if (file.isEmpty()) return;

        Storage_loader::save_to_file(file.toStdString(), storage_);
        log("Склад успешно экспортирован в файл: " + file);
    } catch (const Greenhouse_exception& e) {
        QMessageBox::critical(this, "Ошибка экспорта", e.what());
        log(QString("Ошибка экспорта: %1").arg(e.what()));
    }
}