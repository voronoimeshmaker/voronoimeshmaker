#include <VoronoiMeshMaker/Error/MakerException.h>

VORMAKER_NAMESPACE_OPEN

// Static member initialization
bool VMMException::displayErrorMessage = true;

// Array of error messages
static const std::string VMMExceptionStr[] = {
    "File not found.",
    "Problem reading the file.",
    "Failed to create directory.",
    "Points do not form a polygon.",
    "Points do not form a simple polygon.",
    "Mesh nodes are not defined.",
    "Polygon is null.",
    "Boundary2D is not defined.",
    "Mesh eccentricity is inconsistent.",
    "Mesh spacing is inconsistent.",
    "Number of mesh nodes is null or negative.",
    "Length is null or negative.",
    "Incorrect number of points.",
    "Problem generating the mesh.",
    "Null offset polygon.",
    "IOTool duplicate parameter.",
    "Failed to open the file in IOTools::ReadParameters.",
    "File not found in IOTools::ReadParameters.",
    "Parameter not found in IOTools::Unpack.",
    "Missing data in GeometricDataHolder variable."
};

// Constructors
VMMException::VMMException(const SourceInfo& _sourceInfo, const VMMExceptionIndex& _errorIndex, bool _flagAbort)
    : sourceInfo(_sourceInfo), exceptionIndex(_errorIndex), flagAbort(_flagAbort) {
    errorMsg = VMMExceptionStr[static_cast<int>(_errorIndex)];
}

VMMException::VMMException(const SourceInfo& _sourceInfo, const VMMExceptionIndex& _errorIndex, const std::string& _errorMsg, bool _flagAbort)
    : sourceInfo(_sourceInfo), exceptionIndex(_errorIndex), errorMsg(_errorMsg.empty() ? VMMExceptionStr[static_cast<int>(_errorIndex)] : _errorMsg), flagAbort(_flagAbort) {}

/**
 * @brief Get a description of the exception.
 *
 * @return A C-style string containing the error message.
 */
const char* VMMException::what() const noexcept {
    return errorMsg.c_str();
}

/**
 * @brief Check if the exception should cause an abort.
 *
 * @return True if the program should abort, false otherwise.
 */
[[nodiscard]] bool VMMException::VORAbort() const {
    return flagAbort;
}

/**
 * @brief Check if the error message should be displayed.
 *
 * @return True if the error message should be displayed, false otherwise.
 */
bool VMMException::shouldDisplayMessage() {
    return displayErrorMessage;
}

/**
 * @brief Set whether the error message should be displayed.
 *
 * @param value The new value for the `displayErrorMessage` flag.
 */
void VMMException::setDisplayMessage(bool value) {
    displayErrorMessage = value;
}

/**
 * @brief Function to return a string representing the error type.
 *
 * @param _index The error index corresponding to the type of exception.
 * @return The string description of the error.
 */
std::string ErrorType(const VMMExceptionIndex& _index) {
    return VMMExceptionStr[static_cast<int>(_index)];
}


    std::ostream& operator << ( std::ostream& _os,
                                const VMMException& _isoException) {

        _os << "\n";
        PrintLine(_os);
        _os << "Erro:    " << VMMExceptionStr[static_cast<int>(_isoException.exceptionIndex)] << "\n";

//        if (!_isoException.errorMsg.empty()) {
//            _os << "Msg:     " << _isoException.errorMsg << "\n";
//        }

        _os << "Funcao:  " << _isoException.sourceInfo.FunctionName() << "\n"
            << "Arquivo: " << _isoException.sourceInfo.FileLocation() << "\n"
            << "Linha:   " << _isoException.sourceInfo.LineNumber() << "\n";


        if (_isoException.flagAbort) {
            _os << "Execucao cancelada\n";
        }

        PrintLine(_os);

        _os << std::flush;
        return _os;

    }
    
VORMAKER_NAMESPACE_CLOSE
