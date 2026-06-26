#ifndef MIPSSIMUL_COMMON_H
#define MIPSSIMUL_COMMON_H

#include <definitions.h>

std::string getProjectRoot(int projectRootOffset);

std::string verifyPath(const std::string &path);

std::vector<std::pair<std::string, std::string> > getFilesListByExtension(
    const std::string &path, const std::string &file_extension);

#endif
