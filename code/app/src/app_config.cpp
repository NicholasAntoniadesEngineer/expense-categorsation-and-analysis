/**
 * @file app_config.cpp
 * @brief Implementation of application configuration
 * @author Nicholas Antoniades
 * @date 2024-01-24
 */

#include "app_config.hpp"

namespace FinanceManager {

AppConfig AppConfig::getDefaultConfig() {
    AppConfig config;
    
    // Set application metadata
    config.app_name = "Finance Manager";
    config.org_name = "Finance";
    config.org_domain = "finance.manager";
    config.app_version = "0.1.1";
    config.style_name = "fusion";
    config.font_family = "SF Pro Display";
    config.default_window_width = 660;
    config.default_window_height = 440;
    config.default_font_size = 12;
    
    // Set UI strings
    config.strings = {
        .INPUT_DIR_TITLE = "Input Directory",
        .OUTPUT_DIR_TITLE = "Output Directory",
        .KEYWORD_FILE_TITLE = "Keyword File",
        .EXPORT_OPTIONS_TITLE = "Export Options",
        .BROWSE_BUTTON_TEXT = "Browse",
        .PROCESS_BUTTON_TEXT = "Process Files",
        .MONTHLY_SUMMARY_TEXT = "Export Monthly Summary",
        .WEEKLY_SUMMARY_TEXT = "Export Weekly Summary",
        .FULL_DATASET_TEXT = "Export Full Dataset",
        .SELECT_INPUT_DIR_TEXT = "Select Input Directory",
        .SELECT_OUTPUT_DIR_TEXT = "Select Output Directory",
        .SELECT_KEYWORD_FILE_TEXT = "Select Keyword File",
        .CSV_FILE_FILTER = "CSV Files (*.csv);;All Files (*)",
        .ERROR_TITLE = "Error",
        .SUCCESS_TITLE = "Success",
        .FIELDS_REQUIRED_ERROR = "All fields must be filled",
        .PROCESS_SUCCESS = "Files processed successfully!",
        .PLOT_WEEKLY_TEXT = "Plot Weekly Summary",
        .PLOT_MONTHLY_TEXT = "Plot Monthly Summary"
    };
    
    return config;
}
} 