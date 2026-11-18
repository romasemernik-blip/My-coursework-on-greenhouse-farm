cmake_minimum_required(VERSION 3.16)

project(GreenhouseSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
# Если вы устанавливали Qt 6, замените строку выше на:
# find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(SOURCES
    main.cpp
    
    # === Base ===
    Class/Base/Cpp/Bed.cpp
    Class/Base/Cpp/Equipment.cpp
    Class/Base/Cpp/Greenhouse.cpp
    Class/Base/Cpp/Human.cpp
    Class/Base/Cpp/Operation.cpp
    Class/Base/Cpp/Plant.cpp
    Class/Base/Cpp/Schedule.cpp
    Class/Base/Cpp/Storage.cpp
    
    # === Inheritor / Equipment ===
    Class/Inheritor/Equipment/Cpp/Permanent_Equipment.cpp
    Class/Inheritor/Equipment/Cpp/Single_use_Equipment.cpp
    
    # === Inheritor / Human ===
    Class/Inheritor/Human/Cpp/Agronomist.cpp
    Class/Inheritor/Human/Cpp/Operator.cpp
    
    # === Inheritor / Inheritor2 / Equipment ===
    Class/Inheritor/Inheritor2/Permanent_Equipment/Cpp/Lighting.cpp
    Class/Inheritor/Inheritor2/Permanent_Equipment/Cpp/Sensor.cpp
    Class/Inheritor/Inheritor2/Single_use_Equipment/Cpp/Fertilizer_Equipment.cpp
    Class/Inheritor/Inheritor2/Single_use_Equipment/Cpp/Watering_Equipment.cpp
    
    # === Inheritor / Operation ===
    Class/Inheritor/Operation/Cpp/Auto_Operation.cpp
    Class/Inheritor/Operation/Cpp/Collect.cpp
    Class/Inheritor/Operation/Cpp/Planting_crop.cpp
    
    # === Manager / File_manager ===
    Class/Manager/File_manager/Cpp/File_manager.cpp
    Class/Manager/File_manager/Cpp/Human_loader.cpp
    Class/Manager/File_manager/Cpp/Plant_type_data.cpp
    Class/Manager/File_manager/Cpp/Plant_type_loader.cpp
    Class/Manager/File_manager/Cpp/Progress_saver.cpp
    Class/Manager/File_manager/Cpp/Storage_loader.cpp
    
    # === Manager / Input_manager ===
    Class/Manager/Input_manager/Cpp/Input_reader.cpp
    
    # === Manager / Time_manager ===
    Class/Manager/Time_manager/Cpp/Time_manager.cpp
    
    # === UI ===
    Class/UI/Cpp/MainWindow.cpp
)

# Файлы заголовков (нужны для работы Qt MOC - обработчика сигналов и слотов)
set(HEADERS
    Class/UI/Header/MainWindow.h
)

add_executable(GreenhouseSimulator ${SOURCES} ${HEADERS})

target_include_directories(GreenhouseSimulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Линковка с Qt
target_link_libraries(GreenhouseSimulator PRIVATE Qt5::Widgets)
# Если у вас Qt 6, замените на: target_link_libraries(GreenhouseSimulator PRIVATE Qt6::Widgets)