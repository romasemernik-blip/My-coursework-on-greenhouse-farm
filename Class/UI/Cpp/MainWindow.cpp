#include "Class/UI/Header/MainWindow.h"
#include "Class/Inheritor/Plant/Header/Tomato.h"
#include "Class/Manager/File_manager/Header/Storage_loader.h"
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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Greenhouse Simulator - Управление");
    resize(1200, 800);

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
}

// ... (остальные методы on_load_full_clicked, on_add_bed_clicked и т.д. остаются как были) ...

// НОВЫЙ МЕТОД: Сбор урожая на склад
void MainWindow::on_collect_to_storage_clicked() {
    if (!current_selected_bed_) {
        QMessageBox::warning(this, "Внимание", "Выберите грядку!");
        return;
    }
    
    try {
        int count = 0;
        for (std::size_t i = 0; i < current_selected_bed_->get_plant_count(); ) {
            Plant* p = current_selected_bed_->get_plant(i);
            
            // Собираем только созревшие (Fruiting)
            if (p->get_is_mature()) {
                // 1. Добавляем на склад (например, 1.5 кг за штуку)
                storage_.add(p->get_type(), 1.5); 
                
                // 2. Удаляем с грядки
                current_selected_bed_->remove_plant(i);
                count++;
            } else {
                i++;
            }
        }
        
        if (count > 0) {
            log(QString("Успешно собрано %1 растений на склад!").arg(count));
            refresh_bed_view(current_selected_bed_);
            refresh_storage_view(); // Обновляем вкладку склада
        } else {
            log("На грядке нет созревших растений.");
        }
    } catch (const Storage_overflow_exception& e) {
        QMessageBox::critical(this, "Склад переполнен", e.what());
    } catch (const Greenhouse_exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}

// НОВЫЙ МЕТОД: Отрисовка склада
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

// НОВЫЙ МЕТОД: Экспорт склада
void MainWindow::on_export_storage_clicked() {
    try {
        QString file = QFileDialog::getSaveFileName(this, "Экспорт склада", "storage_export.txt", "Text Files (*.txt)");
        if (file.isEmpty()) return;

        Storage_loader::save_to_file(file.toStdString(), storage_);
        log("Склад успешно экспортирован в файл: " + file);
    } catch (const Greenhouse_exception& e) {
        QMessageBox::critical(this, "Ошибка экспорта", e.what());
    }
}