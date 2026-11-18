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
#include <vector>
#include <memory>

#include "Class/Manager/File_manager/Header/File_manager.h"
#include "Class/Base/Header/Greenhouse.h"
#include "Class/Base/Header/Storage.h"
#include "Class/Manager/Time_manager/Header/Time_manager.h"
#include "Class/Manager/Schedule/Header/Schedule.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_game_tick();
    
    void on_load_full_clicked();
    void on_save_full_clicked();
    void on_load_partial_clicked();
    
    void on_add_bed_clicked();
    void on_remove_bed_clicked();
    
    void on_plant_clicked();
    void on_water_clicked();
    void on_fertilize_clicked();
    void on_collect_to_storage_clicked();
    void on_remove_dead_clicked();
    
    void on_toggle_light_clicked();
    void on_repair_equipment_clicked();
    void on_assign_task_clicked();

    void on_speed_changed(int index);
    void on_tree_item_clicked(QTreeWidgetItem *item, int column);

    void on_export_storage_clicked();

private:
    void refresh_tree();
    void refresh_bed_view(Bed* bed);
    void refresh_storage_view();
    void refresh_equipment_view();
    void refresh_agronomist_view();
    void log(const QString& message);
    void update_time_label();

    File_manager file_manager_;
    Greenhouse greenhouse_;
    Storage storage_;
    Time_manager time_manager_;
    Schedule schedule_;
    
    QTimer* game_timer_;
    double real_seconds_accumulator_;

    QTreeWidget* tree_widget_;
    QTextEdit* log_widget_;
    QWidget* bed_view_widget_;
    QLabel* speed_label_;
    QLabel* time_label_;
    
    QListWidget* storage_list_widget_;
    QLabel* storage_capacity_label_;
    QListWidget* equipment_list_;
    QListWidget* agronomist_list_;

    Bed* current_selected_bed_;
};