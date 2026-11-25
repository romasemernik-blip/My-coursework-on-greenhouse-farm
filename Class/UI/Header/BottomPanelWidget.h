#pragma once
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>

class BottomPanelWidget : public QWidget {
    Q_OBJECT
public:
    explicit BottomPanelWidget(QWidget *parent = nullptr);

    void set_time_text(const QString& text);
    void set_weather_text(const QString& text);
    void set_speed_text(const QString& text);
    void log_message(const QString& msg);

signals:
    void speed_changed(int index);

private:
    QLabel* time_label_;
    QLabel* weather_label_;
    QLabel* speed_label_;
    QTextEdit* log_widget_;
    QComboBox* speed_combo_;
};