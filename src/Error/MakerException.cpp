//==============================================================================
// Nome        : MakerException.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão     : 2.0
// Descrição  : Implementação da classe MakerException para tratamento de erros na biblioteca VoronoiMeshMaker.
//              Parte do grupo 'error' para gerenciamento robusto de erros.
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
 * @file MakerException.cpp
 * @brief Implementa a classe MakerException para tratamento robusto de erros na biblioteca VoronoiMeshMaker.
 *
 * A classe MakerException captura informações detalhadas sobre erros, incluindo arquivo, linha e função
 * onde ocorreram, além de suportar mensagens de erro personalizadas e mecanismos de tratamento.
 * Este arquivo fornece a implementação dos métodos para rastrear e recuperar informações de erro,
 * incluindo o caminho de execução do programa até o erro.
 *
 * @ingroup error
 * @version 2.0
 * @date 2024
 * @author
 * Joao Flavio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//==============================================================================
// Includes da biblioteca VoronoiMeshMaker
//==============================================================================
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Misc/Misc.h>

VORMAKER_NAMESPACE_OPEN

//==============================================================================
// Definições de membros estáticos
//==============================================================================

/**
 * @brief Fornece um mapa de códigos de erro para mensagens correspondentes.
 * 
 * Esta função retorna um mapa estático que associa códigos de erro
 * (`MakerErrorCode`) às suas respectivas mensagens de erro (`std::string`).
 * 
 * @return Uma referência ao mapa contendo códigos de erro e mensagens.
 */
const std::unordered_map<MakerErrorCode, std::string>& MakerException::errorCodeMap() {
    static const std::unordered_map<MakerErrorCode, std::string> errorMap = {
        {MakerErrorCode::FileNotFound, "Arquivo Não Encontrado"},
        {MakerErrorCode::InvalidPolygon, "Polígono Inválido"},
        {MakerErrorCode::NullPointer, "Ponteiro Nulo"},
        // Adicione outros códigos de erro e mensagens aqui
    };
    return errorMap;
}

//==============================================================================
// Construtor
//==============================================================================

/**
 * @brief Constrói um novo objeto MakerException com um código de erro, mensagem e localização de origem.
 * 
 * Este construtor inicializa uma instância de MakerException com um código de erro fornecido,
 * uma mensagem personalizada e a localização de origem onde a exceção foi lançada.
 * 
 * @param errorCode O tipo de erro (baseado em `MakerErrorCode`).
 * @param message Uma mensagem de erro personalizada fornecendo detalhes adicionais.
 * @param location A localização de origem onde a exceção foi criada (padrão: localização atual).
 */
MakerException::MakerException  (   const MakerErrorCode& errorCode
                                ,   const std::string_view message
                                ,   const std::source_location& location
                                )
                                :   errorCode_(errorCode)
                                ,   message_(message)
                                ,   location_(location) {
    formatMessage();
    logException();
}

//==============================================================================
// Funções públicas
//==============================================================================

/**
 * @brief Fornece uma descrição da exceção.
 * 
 * Esta função retorna uma string em estilo C contendo a mensagem completa de erro,
 * que inclui o código de erro, mensagem, arquivo, função e linha.
 * 
 * @return Uma string em estilo C representando a mensagem completa de erro.
 */
const char* MakerException::what() const noexcept {
    return fullMessage_.c_str();
}

/**
 * @brief Obtém o código de erro associado a esta exceção.
 * 
 * Esta função retorna o `MakerErrorCode` que representa o tipo de erro.
 * 
 * @return O código de erro como um `MakerErrorCode`.
 */
MakerErrorCode MakerException::errorCode() const noexcept {
    return errorCode_;
}

/**
 * @brief Obtém a localização de origem onde a exceção foi lançada.
 * 
 * Esta função retorna a localização de origem (`std::source_location`) onde a exceção
 * foi criada, fornecendo informações sobre o arquivo, função e número da linha.
 * 
 * @return Uma referência para o `std::source_location` onde a exceção ocorreu.
 */
const std::source_location& MakerException::location() const noexcept {
    return location_;
}

//==============================================================================
// Funções privadas
//==============================================================================

/**
 * @brief Formata a mensagem completa de erro, incluindo código de erro, mensagem e localização de origem.
 * 
 * Esta função cria a mensagem completa de erro que inclui detalhes como o código de erro,
 * mensagem personalizada, nome do arquivo, nome da função e número da linha.
 * A mensagem formatada é armazenada em `fullMessage_`.
 */
void MakerException::formatMessage() {
    std::string errorCodeStr;
    auto it = errorCodeMap().find(errorCode_);
    if (it != errorCodeMap().end()) {
        errorCodeStr = it->second;
    } else {
        errorCodeStr = "Erro Desconhecido";
    }

    fullMessage_ = std::format("Erro: {}\nMensagem: {}\nArquivo: {}\nFunção: {}\nLinha: {}\n",
                               errorCodeStr,
                               message_,
                               getFileName(location_.file_name()),
                               location_.function_name(),
                               location_.line());
}

/**
 * @brief Registra os detalhes da exceção usando o MakerLogger.
 * 
 * Esta função registra a mensagem completa de erro usando o `MakerLogger` e também
 * registra o caminho de execução do programa para fornecer contexto sobre o fluxo
 * do programa que levou à exceção.
 */
void MakerException::logException() const {
    // Registra a mensagem detalhada da exceção
    MakerLogger::error(fullMessage_, location_);

    // Recupera e registra o caminho de execução
    std::string breadcrumbTrail = MakerLogger::getBreadcrumbTrail();
    MakerLogger::error(breadcrumbTrail, location_);
}

VORMAKER_NAMESPACE_CLOSE
