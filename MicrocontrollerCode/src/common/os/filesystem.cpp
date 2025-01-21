#include "filesystem.h"
#include "../globals.h"
#include <esp_littlefs.h>

using namespace std;

namespace Cesium {

FileSystem::FileSystem() {}

bool FileSystem::begin(bool format_filesystem)
{
    if (!LittleFS.begin(format_filesystem, "/littlefs", 10, partition_label)) {
        DEBUGLN("LittleFS Mount Failed");
        return false;
    }
    DEBUGLN("Assuming mounted LittleFS");
    if (!LittleFS.begin(false, "/littlefs", 10, partition_label)){
        DEBUGLN("Could not mount");
        return false;
    }
    return true;
}

bool FileSystem::end()
{
    return false;
}

vector<String> FileSystem::listDir(const char *dirname)
{
    vector<String> files;
    DEBUG("Listing directory: ");
    DEBUGLN(dirname);

    // Opens up root
    File root = LittleFS.open(dirname);
    if (!root) {
        DEBUGLN(" - failed to open directory");
        return vector<String>();
    }
    if (!root.isDirectory()) {
        DEBUGLN(" - not a directory");
        return vector<String>();
    }

    // Tries to open next file
    File file = root.openNextFile();
    while (file) {

        
        if (file.isDirectory()) {
            // If that file is a directory, just list it
            DEBUG("  DIR : ");
            DEBUGLN(file.name());
            files.push_back((String)file.name());

        } else { 
            // If that file is a directory, list it AND the size
            DEBUG("  FILE: ");
            DEBUG(file.name());
            DEBUG("\tSIZE: ");
            DEBUGLN(file.size());

            // "|" is a delimeter for size
            files.push_back((String)file.name() + "|" + (String)file.size());
        }
        file = root.openNextFile();
    }

    return files;
}

bool FileSystem::mkdir(const char *dirname)
{
    DEBUG("Creating directory: ");
    DEBUGLN(dirname);

    if (LittleFS.mkdir(dirname)) {
        DEBUGLN(" - Dir created");
        return true;
    } else {
        DEBUGLN(" - mkdir failed");
        return false;
    }
}

void FileSystem::fileTree(const char *dirname) {
    File root = LittleFS.open(dirname);

    fileTreeHelper(root);

}

void FileSystem::fileTreeHelper(File root, uint8_t tabs) {


    while (true) {
        File entry =  root.openNextFile();
        if (!entry) {
            break; // no more files
        }
        for (uint8_t i = 0; i < tabs; i++) {
            Serial.print('\t');
        }

        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            fileTreeHelper(entry, tabs + 1);
        } else {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size());
        }
        entry.close();
    }
}

bool FileSystem::rmdir(const char *dirname, bool recursive)
{
    DEBUG("Removing directory: ");
    DEBUGLN(dirname);

    // Returns if directory is not empty and not recursive
    if (listDir(dirname).size() > 0 && !recursive) {
        DEBUGLN("Directory \"" + (String)dirname + "\" is not empty, and function is non-recursive");
        return false;
    }

    // Sees if directory exists
    File current_dir = LittleFS.open(dirname);
    if (!current_dir) {
        DEBUGLN(" - failed to open directory");
        return false;
    }
    if (!current_dir.isDirectory()) {
        DEBUGLN(" - not a directory");
        return false;
    }
    

    // If recursive 
    if (recursive) {

        // Tries to open next file
        File file = current_dir.openNextFile();
        while (file) {
            
            String new_filename = (String)dirname + "/" + file.name();
            
            if (file.isDirectory()) {
                rmdir(new_filename.c_str(), true);

            } else { 
                file.close();
                deleteFile(new_filename.c_str());
            }
            file = current_dir.openNextFile();
        }

        
    }


    // Finally deletes the directory
    if (LittleFS.rmdir(dirname)) {
        DEBUGLN(" - Dir removed");
        return true;
    } else {
        DEBUGLN(" - rmdir failed");
        return false;
    }
}

bool FileSystem::readFile(const char *path, String& result)
{
    DEBUG("Reading file: ");
    DEBUGLN(path);

    result = "";

    File file = LittleFS.open(path);
    if (!file || file.isDirectory()) {
        DEBUGLN(" - failed to open file for reading");
        return false;
    }

    while (file.available()) {
        result += (char)file.read();
    }
    file.close();

    return true;
}

bool FileSystem::readFile(const char *path, Stream& stream)
{
    DEBUG("Reading file: ");
    DEBUGLN(path);

    String file_contents;

    File file = LittleFS.open(path);
    if (!file || file.isDirectory()) {
        DEBUGLN(" - failed to open file for reading");
        return false;
    }

    while (file.available()) {
        stream.write(file.read());
    }
    file.close();

    return true;
}

bool FileSystem::writeFile(const char *path, const char *message)
{
    DEBUG("Writing to file: ");
    DEBUGLN(path);

    File file;
    
    file = LittleFS.open(path, FILE_WRITE);
    
    if (!file) {
        DEBUGLN(" - failed to open file for writing");
        return false;
    }
    if (file.print(message)) {
        DEBUGLN(" - file written");
        file.close();
        return true;
    } else {
        DEBUGLN(" - write failed");
        file.close();
        return false;
    }
}

bool FileSystem::writeFile(const char *path, const uint8_t *buffer, size_t len)
{
    DEBUG("Writing to file: ");
    DEBUGLN(path);

    File file;
    
    file = LittleFS.open(path, FILE_WRITE);
    
    if (!file) {
        DEBUGLN(" - failed to open file for writing");
        return false;
    }
    if (file.write(buffer, len)) {
        DEBUGLN(" - file written");
        file.close();
        return true;
    } else {
        DEBUGLN(" - write failed");
        file.close();
        return false;
    }
}

bool FileSystem::appendFile(const char *path, const char *message)
{
    DEBUG("Appending to file: ");
    DEBUGLN(path);

    File file;
    
    file = LittleFS.open(path, FILE_APPEND);
    
    if (!file) {
        DEBUGLN(" - failed to open file for appending");
        return false;
    }
    if (file.print(message)) {
        DEBUGLN(" - file appended");
        file.close();
        return true;
    } else {
        DEBUGLN(" - append failed");
        file.close();
        return false;
    }
}

bool FileSystem::appendFile(const char *path, const uint8_t *buffer, size_t len)
{
    DEBUG("Appending to file: ");
    DEBUGLN(path);

    File file;
    
    file = LittleFS.open(path, FILE_APPEND);
    
    if (!file) {
        DEBUGLN(" - failed to open file for appending");
        return false;
    }
    if (file.write(buffer, len)) {
        DEBUGLN(" - file appended");
        file.close();
        return true;
    } else {
        DEBUGLN(" - append failed");
        file.close();
        return false;
    }
}

bool FileSystem::renameFile(const char *from_path, const char *to_path)
{
    DEBUG("Renaming file: ");
    DEBUGLN(from_path);

    if (LittleFS.rename(from_path, to_path)) {
        DEBUGLN(" - File renamed");
        return true;
    } else {
        DEBUGLN(" - rename failed");
        return false;
    }
}

bool FileSystem::deleteFile(const char *path)
{
    DEBUG("Deleting file: ");
    DEBUGLN(path);

    if (LittleFS.remove(path)) {
        DEBUGLN(" - File deleted");
        return true;
    } else {
        DEBUGLN(" - delete failed");
        return false;
    }
}

bool FileSystem::testFileIO(const char *path)
{
    DEBUG("Testing file Input with: ");
    DEBUGLN(path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file) {
        DEBUGLN(" - failed to open file for writing");
        return false;
    }

    size_t i;
    DEBUG(" - writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++) {
    if ((i & 0x001F) == 0x001F) {
        DEBUG(".");
    }
    file.write(buf, 512);
    }
    DEBUGLN("");
    uint32_t end = millis() - start;
    DEBUG("Testing file Output with: ");
    DEBUGLN(path);

    DEBUG(" - ");
    DEBUG(2048 * 512);
    DEBUG(" bytes written in ");
    DEBUG(end);
    DEBUGLN(" ms");

    file.close();

    file = LittleFS.open(path);
    start = millis();
    end = start;
    i = 0;
    if (file && !file.isDirectory()) {
        len = file.size();
        size_t flen = len;
        start = millis();
        DEBUG("- reading");
        while (len) {
            size_t toRead = len;
            if (toRead > 512) {
            toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F) {
                DEBUG(".");
            }
            len -= toRead;
        }
        DEBUGLN("");
        end = millis() - start;

        DEBUG(" - ");
        DEBUG(flen);
        DEBUG(" bytes read in ");
        DEBUG(end);
        DEBUGLN(" ms");

        file.close();
    } else {
        DEBUGLN("- failed to open file for reading");
        return false;
    }
    
    return true;
}

bool FileSystem::usageBytes(size_t& total, size_t& used) {

    Serial.println(LittleFS.totalBytes());
    Serial.println(LittleFS.usedBytes());
    if(esp_littlefs_info(partition_label, &total, &used) != ESP_OK){
        return false;
    }

    return true;
    
}

float FileSystem::usageFraction() const
{
    size_t total, used;

    if(esp_littlefs_info(partition_label, &total, &used) != ESP_OK){
        Serial.println("BAD");
        return false;
    }
    Serial.println(total);
    Serial.println(used);
    return float(used) / total;
    
}

}