#include "Class/UI/Header/BottomPanelWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

BottomPanelWidget::BottomPanelWidget(QWidget *parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this); 
    auto* time_layout = new QHBoxLayout(); 

    weather_label_ = new QLabel("Погода: Ясно", this);
    weather_label_->setStyleSheet("color: #e67e22; font-weight: bold;");
    
    time_label_ = new QLabel("Время: День 1 | 00:00", this);
    time_label_->setStyleSheet("font-weight: bold;");
    
    speed_label_ = new QLabel("Скорость: x1", this);
    
    speed_combo_ = new QComboBox(this);
    speed_combo_->addItem("x1", 1.0);
    speed_combo_->addItem("x10", 10.0);
    speed_combo_->addItem("x50", 50.0);
    speed_combo_->addItem("x100", 100.0);
    speed_combo_->addItem("x200", 200.0);
    connect(speed_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BottomPanelWidget::speed_changed);

    time_layout->addWidget(weather_label_);
    time_layout->addWidget(time_label_);
    time_layout->addWidget(speed_label_);
    time_layout->addWidget(speed_combo_);
    time_layout->addStretch();
    layout->addLayout(time_layout);

    log_widget_ = new QTextEdit(this);
    log_widget_->setReadOnly(true);
    layout->addWidget(log_widget_);
}

void BottomPanelWidget::set_time_text(const QString& text) { time_label_->setText(text); }
void BottomPanelWidget::set_weather_text(const QString& text) { weather_label_->setText(text); }
void BottomPanelWidget::set_speed_text(const QString& text) { speed_label_->setText(text); }
void BottomPanelWidget::log_message(const QString& msg) { log_widget_->append(msg); }