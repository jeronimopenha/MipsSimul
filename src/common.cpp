#include <common.h>

using namespace std;

/**
* Return the full path from root to executable folder
 * @return
 */
string getProjectRoot() {
    filesystem::path path = filesystem::current_path();
    for (int i = 0; i < PROJECT_ROOT_OFFSET; ++i)
        path = path.parent_path();

    return path.string();
}

/**
 * Verifies if the path contains the "/" character in the end
 * @param path
 * @return
 */
string verifyPath(const string &path) {
    if (!path.empty() && path.back() != '/') {
        return path + '/';
    }

    return path;
}


/**
 *  Returns the list of files inside a folder that has the chose extension
 * @param path
 * @param file_extension
 * @return
 */
vector<pair<string, string> > getFilesListByExtension(
    const string &path, const string &file_extension) {
    vector<pair<string, string> > files_list_by_extension;

    for (const auto &entry: filesystem::recursive_directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == file_extension) {
            string file_path = entry.path().string();
            string file_name = entry.path().filename().string();
            files_list_by_extension.emplace_back(file_path, file_name);
        }
    }

    return files_list_by_extension;
}