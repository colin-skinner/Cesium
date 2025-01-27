import os
import shutil

def create_config_files():
    target_dir = "src/config"
    target_file = "src/config/all_configs.h"
    
    # Opens file
    header_file = open(target_file, "w")

    header_file.write(F"// Automatically generated document\n")
    header_file.write(F"// Generate this code with 'create_config_header.py'\n")
    header_file.write(F"#pragma once\n\n")
    header_file.write("namespace Cesium {\n")
    header_file.write("namespace Config {\n\n")

    # Copies data from each .ini file

    for filename in os.listdir(target_dir):
        if ".json" not in filename:
            continue
        
        full_file_name = os.path.join(target_dir, filename)

        file = open(full_file_name, "r")

        if ".json" not in filename:
            continue
        config_title = filename.split(".")[0]

        header_file.write(F"const char {config_title}_config[] = R\"(\n")
        header_file.write(f"{file.read()}\n")
        header_file.write(F")\";\n\n\n")
        # header_file.write(F"const int {config_title}_size = sizeof({config_title}_config);\n\n")
        

    header_file.write("\n} // namespace Cesium\n")
    header_file.write("} // namespace Config\n")


create_config_files()