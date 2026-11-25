#pragma once
#include <QMainWindow>
#include <QTreeWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QLabel>
#include <QMap>
#include <memory>
#include <vector>

#include "Class/UI/Header/BedsWidget.h"
#include "Class/UI/Header/BottomPanelWidget.h"
#include "Class/Manager/File_manager/Header/File_manager.h"
#include "Class/Base/Header/Greenhouse.h"
#include "Class/Base/Header/Storage.h"
#include "Class/Manager/Time_manager/Header/Time_manager.h"
#include "Class/Base/Header/Schedule.h"
#include "Class/Base/Header/Weather.h"
#include "Class/Base/Header/Human.h"
#include "Class/Logger/Header/System_logger.h"
#include "Class/Logger/Header/Action_logger.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_game_tick();
    void on_speed_changed(int index);
    
    void on_load_all_clicked();
    void on_save_all_clicked();
    void on_switch_operator_clicked();
    void on_export_storage_clicked();

    void on_add_bed_clicked();
    void on_remove_bed_clicked();
    void on_plant_clicked();
    void on_collect_to_storage_clicked();
    void on_water_clicked();
    void on_fertilize_clicked();
    void on_toggle_bed_light_clicked();
    void on_remove_dead_clicked();
    void on_tree_item_clicked(QTreeWidgetItem *item, int column);

    void on_toggle_light_clicked();
    void on_repair_equipment_clicked();
    void on_add_equipment_clicked();

    void on_add_human_clicked();

    void on_add_schedule_operation_clicked();

private:
    void setup_ui();
    void refresh_all_views();
    void refresh_tree();
    void refresh_storage_view();
    void refresh_equipment_view();
    void refresh_humans_view();
    void refresh_schedule_view();
    void apply_weather_effects(double dt);
    void check_harvest_time();
    void log(const QString& message);
    void handle_exception(const std::exception& e, const QString& action_name);
    
    bool assign_agronomist_dialog();
    bool select_operator_dialog(bool is_startup);

    File_manager file_manager_;
    Greenhouse greenhouse_;
    Storage storage_;
    Time_manager time_manager_;
    Schedule schedule_;
    
    Bed* current_selected_bed_;
    QString current_save_folder_;
    QString current_operator_name_;
    bool is_bed_light_on_;
    
    std::unique_ptr<Weather> current_weather_;
    double weather_timer_;
    QMap<Plant*, double> harvest_times_;
    
    std::vector<std::unique_ptr<Human>> humans_;

    std::unique_ptr<System_logger> sys_log_;
    std::unique_ptr<Action_logger> act_log_;

    QTimer* game_timer_;
    QTreeWidget* tree_widget_;
    QTabWidget* tab_widget_;
    BedsWidget* beds_widget_;
    BottomPanelWidget* bottom_panel_;
    
    QLabel* operator_label_;
    QLabel* storage_capacity_label_;
    QListWidget* storage_list_widget_;
    QListWidget* equipment_list_widget_;
    QListWidget* humans_list_widget_;
    QListWidget* schedule_list_widget_;
};