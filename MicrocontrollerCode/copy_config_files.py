import os
import shutil

def copy_config_files():
    source_dir = "src/config"
    target_dir = "data"
    
    
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)

    else:
        # Clear directory
        for filename in os.listdir(target_dir):
            file_path = os.path.join(target_dir, filename)

            # If file is an alias, unlink instead of delete
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            # remove file
            elif os.path.isdir(file_path): 
                shutil.rmtree(file_path)
        
    # copy .ini files
    for file_name in os.listdir(source_dir):
        if ".ini" not in file_name:
            continue

        full_file_name = os.path.join(source_dir, file_name)
        if os.path.isfile(full_file_name):
            shutil.copy(full_file_name, target_dir)

copy_config_files()