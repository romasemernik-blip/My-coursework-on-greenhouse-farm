#pragma once
#include <QMainWindow>
#include <QTreeWidget>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QListWidget>
#include <QComboBox>
#include <QTabWidget>
#include <QGridLayout>
#include <QMap>
#include <vector>
#include <memory>

#include "Class/Manager/File_manager/Header/File_manager.h"
#include "Class/Base/Header/Greenhouse.h"
#include "Class/Base/Header/Storage.h"
#include "Class/Manager/Time_manager/Header/Time_manager.h"
#include "Class/Base/Header/Schedule.h"
#include "Class/Inheritor/Human/Header/Operator.h"
#include "Class/Logger/Header/System_logger.h"
#include "Class/Logger/Header/Action_logger.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override; // Для логирования закрытия

private slots:
    void on_game_tick();
    void on_speed_changed(int index);
    void on_load_all_clicked();
    void on_save_all_clicked();
    void on_switch_operator_clicked(); // Новый слот
    
    // Грядки и Растения
    void on_add_bed_clicked();
    void on_remove_bed_clicked();
    void on_plant_clicked();
    void on_collect_to_storage_clicked();
    void on_water_clicked();
    void on_fertilize_clicked();
    void on_remove_dead_clicked();
    void on_tree_item_clicked(QTreeWidgetItem *item, int column);

    // Оборудование
    void on_toggle_light_clicked();
    void on_repair_equipment_clicked();
    void on_export_storage_clicked();

private:
    void setup_ui();
    void refresh_all_views();
    void refresh_tree();
    void refresh_bed_view();
    void refresh_storage_view();
    void refresh_equipment_view();
    void check_harvest_time();
    void log(const QString& message);
    void handle_exception(const std::exception& e, const QString& action_name);

    bool assign_agronomist_dialog();
    bool select_operator_dialog(bool is_startup);

    // Логгеры
    std::unique_ptr<System_logger> sys_log_;
    std::unique_ptr<Action_logger> act_log_;
    
    // Текущий рабочий
    Operator* current_operator_;
    QString current_operator_name_;

    File_manager file_manager_;
    Greenhouse greenhouse_;
    Storage storage_;
    Time_manager time_manager_;
    
    Bed* current_selected_bed_;
    QString current_save_folder_;
    QMap<Plant*, double> harvest_times_;

    // UI
    QTimer* game_timer_;
    QTreeWidget* tree_widget_;
    QTextEdit* log_widget_;
    QLabel* speed_label_;
    QLabel* time_label_;
    QLabel* operator_label_; // Показывает текущего оператора
    QWidget* grid_widget_;
    QGridLayout* grid_layout_;
    QLabel* bed_info_label_;
    QListWidget* storage_list_widget_;
    QLabel* storage_capacity_label_;
    QListWidget* equipment_list_widget_;
};