#ifndef __VORMAKER_EXCEPTION_H__
#define __VORMAKER_EXCEPTION_H__

//==============================================================================
// Nome        : MakerException.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão     : 2.0
// Descrição  : Tratamento de exceções para a biblioteca VoronoiMeshMaker.
//                Parte do grupo 'error' para gerenciamento robusto de erros.
//
// Este programa é software livre: você pode redistribuí-lo e/ou modificá-lo
// sob os termos da GNU General Public License como publicada pela
// Free Software Foundation, versão 3 da Licença.
//
// Este programa é distribuído na esperança de que seja útil,
// mas SEM QUALQUER GARANTIA; sem sequer a garantia implícita de
// COMERCIALIZAÇÃO ou ADEQUAÇÃO A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a
// GNU General Public License para mais detalhes.
//==============================================================================

/**
 * @file MakerException.h
 * @brief Define a classe MakerException, que fornece capacidades robustas de tratamento de erros para a biblioteca VoronoiMeshMaker.
 *
 * A classe MakerException captura informações detalhadas sobre erros, incluindo arquivo, linha e função
 * onde ocorreram, além de suportar mensagens de erro personalizadas e mecanismos de tratamento.
 * Esta classe faz parte do grupo 'error' na biblioteca VoronoiMeshMaker.
 *
 * @ingroup error
 *
 * @version 2.0
 * @date 2024
 * @author
 * Joao Flavio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <format>

//==============================================================================
// Includes da biblioteca VoronoiMeshMaker
//==============================================================================
#include <VoronoiMeshMaker/Error/MakerLogger.h>
#include <VoronoiMeshMaker/Misc/namespace.h>

//==============================================================================
// Declaração do namespace
//==============================================================================

VORMAKER_NAMESPACE_OPEN

/**
 * @ingroup error
 * @enum MakerErrorCode
 * @brief Enumeração dos possíveis códigos de erro na biblioteca VoronoiMeshMaker.
 *
 * Este enum representa os diversos tipos de erros que podem ocorrer na biblioteca VoronoiMeshMaker.
 */
enum class MakerErrorCode {
    FileNotFound = 0,
    InvalidPolygon,
    NullPointer,
    // Adicione outros códigos de erro conforme necessário
};

/**
 * @ingroup error
 * @class MakerException
 * @brief Classe de exceção para tratamento de erros na biblioteca VoronoiMeshMaker.
 *
 * Esta classe captura informações detalhadas sobre erros, incluindo a função, arquivo e linha
 * onde a exceção foi lançada. Suporta mensagens de erro personalizadas e se integra ao MakerLogger
 * para fornecer um caminho de execução do programa que levou ao erro.
 */
class MakerException : public std::exception {

public:
    /**
     * @brief Construtor para criar uma exceção com o código de erro.
     *
     * @param errorCode O tipo de erro (baseado em MakerErrorCode).
     * @param message Uma mensagem de erro personalizada para fornecer informações adicionais.
     * @param location Localização da origem do erro (padrão: localização atual).
     */
    
    MakerException  (   const MakerErrorCode& errorCode
                    ,   const std::string_view message = ""
                    ,   const std::source_location& location = std::source_location::current());

    /**
     * @brief Obtém uma descrição da exceção.
     *
     * @return Uma string no estilo C contendo a mensagem de erro.
     */
    virtual const char* what() const noexcept override;

    /**
     * @brief Obtém o código de erro.
     *
     * @return O MakerErrorCode associado a esta exceção.
     */
    MakerErrorCode errorCode() const noexcept;

    /**
     * @brief Obtém a localização de origem onde a exceção foi lançada.
     *
     * @return Uma referência para o objeto de localização de origem.
     */
    const std::source_location& location() const noexcept;

private:
    
    MakerErrorCode errorCode_;                 ///< Enum que representa o tipo de erro.
    std::string message_;                      ///< Mensagem de erro personalizada.
    std::source_location location_;            ///< Localização da origem do erro.
    std::string fullMessage_;                  ///< Mensagem de erro formatada completa.

    /**
     * @brief Formata a mensagem completa de erro.
     */
    void formatMessage();

    /**
     * @brief Registra a exceção usando o MakerLogger.
     */
    void logException() const;

    /**
     * @brief Retorna o mapeamento dos códigos de erro para suas representações em string.
     *
     * @return Uma referência para o unordered_map contendo códigos de erro e suas mensagens.
     */
    static const std::unordered_map<MakerErrorCode, std::string>& errorCodeMap();
};

VORMAKER_NAMESPACE_CLOSE

#endif // __VORMAKER_EXCEPTION_H__
