//
// Created by Yang ziJ on 2018/10/22.
//

#ifndef FILESERVER_FILEMANAGER_H
#define FILESERVER_FILEMANAGER_H


#include <list>
#include <mutex>
#include <string>

class FileManager {
public:
    FileManager()= default;
    ~FileManager()= default;

    FileManager(const FileManager &r)= delete;
    FileManager &operator=(const FileManager &r)= delete;

    bool init(const char *basepath);
    bool isFileExsit(const char *filename);
    void addFile(const char *filename);

private:
    std::list<std::string>   list_files;
    std::mutex             mux_file;
    std::string      base_path;
};


#endif //FILESERVER_FILEMANAGER_H
