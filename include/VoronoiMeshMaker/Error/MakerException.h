#ifndef __VORONOMESHVMAKER_MAKEREXCEPTION_H__
#define __VORONOMESHVMAKER_MAKEREXCEPTION_H__


//==============================================================================
//  C++ include
//==============================================================================

#include <string>
#include <exception>
#include <ostream>


//==============================================================================
//  VoronoiMeshMaker include
//==============================================================================

#include <VoronoiMeshMaker/Misc/id.h>
#include <VoronoiMeshMaker/Misc/sourceinfo.h>


VORMAKER_NAMESPACE_OPEN

/**
 * @defgroup ErrorAnalysis Error Handling and Analysis
 * @brief This group contains classes and utilities for error handling and exception management.
 *
 * The 'ErrorAnalysis' group focuses on managing errors in the VoronoiMeshMaker library.
 * It provides detailed exception handling mechanisms to ensure robustness in error-prone areas.
 */

/**
 * @ingroup ErrorAnalysis
 * @enum VMMExceptionIndex
 * @brief Enumeration of possible exception types in the VoronoiMeshMaker library.
 * 
 * This enum represents various types of exceptions that can be raised in the VoronoiMeshMaker library.
 */
enum class VMMExceptionIndex {
    FileNotFound = 0,
    FileReadProblem,
    FileSystemError,
    IsNotPolygon,
    IsNotSimplePolygon,
    NullNodes,
    NullPolygon,
    NullBoundary2D,
    WrongAlpha,
    WrongDelta,
    WrongNNodes,
    NEGATIVELENGHT,
    WrongNPoints,
    GridGenerationProblem,
    NullOffsetPolygon,
    IOToolsDuplicateKey,
    IOToolsFail,
    IOToolsFileMissing,
    IOToolsNotFound,
    DATANOTAVAILABLE,
    SHAPENOTFOUNDREGISTRY
};

/**
 * @ingroup ErrorAnalysis
 * @class VMMException
 * @brief Exception class for handling errors in VoronoiMeshMaker.
 *
 * This class captures detailed error information, including the function, file, and line
 * where the exception was thrown. It supports custom error messages and flags to indicate
 * whether execution should abort.
 * 
 * The class inherits from ID for identification purposes.
 */
class VMMException : public std::exception, public ID {

    friend std::ostream& operator<<(std::ostream&, const VMMException&);

public:
    /**
     * @brief Get the class name.
     *
     * @return A string view representing the name of this class.
     */
    virtual std::string_view className() const noexcept override {
        return "MakerException";
    }

    /**
     * @brief Get the class ID.
     *
     * @return A ClassID enum representing the unique ID of this class.
     */
    virtual ClassID classID() const noexcept override {
        return ClassID::MakerException;
    }

    /**
     * @brief Constructor to create an exception with the source info, error index, and abort flag.
     *
     * @param _sourceInfo The location in the code where the exception was thrown.
     * @param _errorIndex The type of error (based on VMMExceptionIndex).
     * @param _flagAbort Indicates whether the program should abort after the exception.
     */
    VMMException(const SourceInfo& _sourceInfo, const VMMExceptionIndex& _errorIndex, bool _flagAbort);

    /**
     * @brief Constructor to create an exception with the source info, error index, custom message, and abort flag.
     *
     * @param _sourceInfo The location in the code where the exception was thrown.
     * @param _errorIndex The type of error (based on VMMExceptionIndex).
     * @param _errorMsg A custom error message to provide additional information.
     * @param _flagAbort Indicates whether the program should abort after the exception.
     */
    VMMException(const SourceInfo& _sourceInfo, const VMMExceptionIndex& _errorIndex, const std::string& _errorMsg, bool _flagAbort);

    /**
     * @brief Default virtual destructor.
     */
    virtual ~VMMException() = default;

    /**
     * @brief Get a description of the exception.
     *
     * @return A C-style string containing the error message.
     */
    virtual const char* what() const noexcept override;

    /**
     * @brief Check if the exception should cause an abort.
     *
     * @return True if the program should abort, false otherwise.
     */
    [[nodiscard]] bool VORAbort() const;

    /**
     * @brief Check if the error message should be displayed.
     *
     * @return True if the error message should be displayed, false otherwise.
     */
    static bool shouldDisplayMessage();

    /**
     * @brief Set whether the error message should be displayed.
     *
     * @param value The new value for the `displayErrorMessage` flag.
     */
    static void setDisplayMessage(bool value);

private:
    static bool displayErrorMessage;  ///< Controls whether the error message should be displayed.

    SourceInfo sourceInfo;             ///< Stores the source information (file, line, function).
    VMMExceptionIndex exceptionIndex;  ///< Enum representing the type of exception.
    std::string errorMsg;              ///< Custom error message.
    bool flagAbort;                    ///< Indicates whether the program should abort.
};

/**
 * @brief Function to get the string representation of an error type.
 *
 * @param _index The error index corresponding to a specific error.
 * @return The string description of the error.
 */
std::string ErrorType(const VMMExceptionIndex& _index);


VORMAKER_NAMESPACE_CLOSE

#endif // __VORONOMESHVMAKER_MAKEREXCEPTION_H__
