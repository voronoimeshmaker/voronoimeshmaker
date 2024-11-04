//==============================================================================
// Includes C++ Standard Libraries
//==============================================================================

#include <algorithm>        // std::generate_n
#include <cctype>           // For std::isspace
#include <iostream>         // std::iostream
#include <iterator>         // std::ostream_iterator
#include <filesystem>       // create_directory, exists

//==============================================================================
// Includes VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Misc/misc.h>

VORMAKER_NAMESPACE_OPEN

/**
 * @brief Prints a line of ASCII characters to the specified output stream.
 * 
 * This function prints a line composed of ASCII characters ('=') to the output stream.
 * 
 * @param[out] _os The output stream to print to.
 * @param[in] _lsize The number of characters to print in the line.
 */
void PrintLine(std::ostream& _os, const unsigned& _lsize) 
{
    const char ASCIINUMBER(0x3D); // '=' character

    auto PrintAscII = []() { return ASCIINUMBER; };

    std::generate_n(
        std::ostream_iterator<char>(_os, ""),
        _lsize,
        PrintAscII
    );

    _os << "\n";
}

/**
 * @brief Converts a string to a boolean value.
 * 
 * This function converts a string representing a boolean value (e.g., "true" or "false")
 * to its corresponding boolean value. The string is case-insensitive.
 * 
 * @param[in] str The string to convert.
 * @return The boolean value corresponding to the string.
 */
[[nodiscard]] bool string2bool(std::string_view str) 
{
    std::string lowerStr(str);  // Converte para std::string apenas se for necessário
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    
    std::istringstream is(lowerStr);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

/**
 * @brief Gets the current working directory.
 * 
 * This function retrieves the current working directory of the process.
 * 
 * @return The current working directory as a path object.
 */
[[nodiscard]] std::filesystem::path getCurrentWorkingDirectory() 
{
    return std::filesystem::current_path();
}

/**
 * @brief Gets the full path of a given file or directory.
 * 
 * This function converts a relative or absolute path into a full path.
 * 
 * @param[in] path The relative or absolute path to be converted.
 * @return The full path as a path object.
 */
[[nodiscard]] std::filesystem::path getFullPath(std::string_view path) 
{
    return std::filesystem::absolute(path);
}

/**
 * @brief Renames a file by changing its extension.
 * 
 * This function renames a file by replacing its current extension with a new extension.
 * If the file already has the new extension, the original name is returned.
 * 
 * @param[in] filename The current name of the file.
 * @param[in] newFiletype The new file extension.
 * @return The new name of the file.
 */
[[nodiscard]] std::string RenameFile(std::string_view filename, std::string_view newFiletype) 
{
    size_t lastDot = filename.find_last_of(".");

    std::string newFilename;
    if (lastDot != std::string::npos) {
        newFilename = std::string(filename.substr(0, lastDot)) + "." + std::string(newFiletype);
    } else {
        newFilename = std::string(filename) + "." + std::string(newFiletype);
    }

    std::filesystem::rename(filename, newFilename);  // Renomeia o arquivo no sistema de arquivos
    return newFilename;
}

/**
 * @brief Creates a directory if it does not exist.
 * 
 * This function checks whether the specified directory exists, and if not, creates it.
 * 
 * @param path The path of the directory to create.
 * @return True if the directory was created or already exists, false otherwise.
 */
bool CreateDirectoryIfNotExists(std::string_view path) 
{
    return std::filesystem::exists(path) || std::filesystem::create_directory(path);
}




// Overload operator<< for CGAL types
std::ostream& operator<<(std::ostream& os, const vmm::gtp::Point2D& point) {
    os << "Point_2(" << point.x() << ", " << point.y() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const vmm::gtp::Vector2D& vector) {
    os << "Vector_2(" << vector.x() << ", " << vector.y() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const vmm::gtp::Point3D& point) {
    os << "Point_3(" << point.x() << ", " << point.y() << ", " << point.z() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const vmm::gtp::Vector3D& vector) {
    os << "Vector_3(" << vector.x() << ", " << vector.y() << ", " << vector.z() << ")";
    return os;
}

VORMAKER_NAMESPACE_CLOSE
