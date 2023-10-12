cmake_minimum_required(VERSION 3.5)

project(PushStock VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Charts) # Added 'Charts' here
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Charts) # Added 'Charts' here

set(TS_FILES PushStock_en_US.ts)

set(PROJECT_SOURCES
    main.cpp
    pushstock.cpp
    pushstock.h
    pushstock.ui
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PushStock
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        loginpage.h loginpage.cpp
        mainwindow.h mainwindow.cpp
        registrationpage.h registrationpage.cpp
        server.h server.cpp
        stockpage.h stockpage.cpp
        depositpage.h depositpage.cpp
        messagebuilder.h
        loginbuilder.h
        loginbuilder.cpp
        registerbuilder.h registerbuilder.cpp
        namesurnamevalidator.h namesurnamevalidator.cpp
        emailvalidator.h emailvalidator.cpp
        passwordvalidator.h passwordvalidator.cpp
        phonevalidator.h phonevalidator.cpp
        decrypt.h
        decrypt.cpp
        depositvalidator.h depositvalidator.cpp
        imagerecourses.qrc


    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(PushStock SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(PushStock
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
qt_add_resources(QT_RESOURCES resources.qrc)

target_link_libraries(PushStock PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts) # Added 'Charts' here

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PushStock)
endif()
set_target_properties(PushStock PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PushStock
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PushStock)
endif()
