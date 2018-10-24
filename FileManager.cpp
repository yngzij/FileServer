//
// Created by Yang ziJ on 2018/10/22.
//

#include "FileManager.h"
#include <dirent.h>
#include <sys/stat.h>


bool FileManager::init(const char *basepath) {
    base_path=basepath;
    DIR *dir=opendir(basepath);
    if (dir== nullptr){
        fprintf(stderr,"open base dir error  \n");
        if(mkdir(basepath,S_IRWXU|S_IRWXG|S_IXOTH)==0){
            return true;
        }
        fprintf(stderr,"Create dir error \n");
        return false;
    }
    struct dirent *dirp;
    while ((dirp=readdir(dir))!= nullptr){
        if(strcmp(dirp->d_name,".")==0 ||strcmp(dirp->d_name,"..")==0){
            continue;
        }

        list_files.emplace_back(dirp->d_name);
    }
    closedir(dir);

    return true;
}

bool FileManager::isFileExsit(const char *filename) {
    std::lock_guard<std::mutex>guard(mux_file);
    for (const auto & iter:list_files){
        if(iter==filename){
            return true;
        }
    }
    std::string  file_path=base_path;
    file_path+=filename;
    FILE *fp=fopen(file_path.c_str(),"r");
    if(fp!= nullptr){
        fclose(fp);
        list_files.emplace_back(filename);
        return true;
    }
    return false;
}

void FileManager::addFile(const char *filename) {
    std::lock_guard<std::mutex>guard(mux_file);
    list_files.emplace_back(filename);
}
