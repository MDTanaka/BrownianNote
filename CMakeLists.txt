cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR $ENV{VST3SDK_SOURCE_DIR})
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(BrownianNote
    VERSION 1.0.0.0 
    DESCRIPTION "BrownianNote VST 3 Plug-in"
)


add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(BrownianNote
    source/version.h
    source/myplugincids.h
    source/mypluginprocessor.h
    source/mypluginprocessor.cpp
    source/myplugincontroller.h
    source/myplugincontroller.cpp
    source/mypluginentry.cpp
    source/brownian_note.h
    source/brownian_note.cpp
)

target_link_libraries(BrownianNote
    PRIVATE
        sdk
)

smtg_target_configure_version_file(BrownianNote)

if(SMTG_MAC)
    smtg_target_set_bundle(BrownianNote
        BUNDLE_IDENTIFIER com.brownnotes.browniannote
        COMPANY_NAME "My Plug-in Company"
    )
    smtg_target_set_debug_executable(BrownianNote
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT BrownianNote)

        smtg_target_set_debug_executable(BrownianNote
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
