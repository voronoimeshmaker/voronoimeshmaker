#ifndef VORMAKER_FUNCTIONNULL_LOGGER_H
#define VORMAKER_FUNCTIONNULL_LOGGER_H

//==============================================================================
// Nome        : FunctionLoggerNull.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 2.1
// Descrição   : Implementação nula do logger de fluxo de função para a biblioteca VoronoiMeshMaker.
//               Parte do grupo 'debug' para rastreamento de entrada e saída de funções.
//
// Este programa é software livre: você pode redistribuí-lo e/ou modificá-lo
// sob os termos da Licença Pública Geral GNU como publicada pela
// Free Software Foundation, versão 3 da Licença, ou (a seu critério)
// qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil,
// mas SEM QUALQUER GARANTIA; sem mesmo a garantia implícita de
// COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO FIM. Consulte a
// Licença Pública Geral GNU para mais detalhes.
//==============================================================================

/**
 * @file FunctionLoggerNull.h
 * @brief Define a implementação nula da classe FunctionLogger para a biblioteca VoronoiMeshMaker.
 *
 * A classe FunctionLogger, nesta versão nula, é utilizada em builds de release onde o
 * rastreamento de entrada e saída de funções é desativado para evitar sobrecarga de desempenho.
 * Esta implementação assegura que o código que utiliza FunctionLogger possa ser compilado sem
 * alterações, mas não realizará nenhuma ação em tempo de execução.
 *
 * O comportamento é controlado pela macro `NDEBUG1`. Se `NDEBUG1` for definida,
 * esta implementação nula será utilizada. Caso contrário, a implementação completa
 * em `FunctionLoggerFull.h` será utilizada para fornecer informações detalhadas de log.
 *
 * @ingroup debug
 * @version 2.1
 * @date 2024
 * @author
 * João Flávio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

//==============================================================================
// Inclusões da Biblioteca VoronoiMeshMaker
//==============================================================================
#include <VoronoiMeshMaker/Misc/namespace.h>

VORMAKER_NAMESPACE_OPEN

#pragma message("FunctionLoggerNull.h has been included")

/**
 * @ingroup debug
 * @class FunctionLogger
 * @brief Implementação vazia para builds de release da biblioteca VoronoiMeshMaker.
 *
 * Em modo release, a classe FunctionLogger é definida, mas não realiza nenhuma ação,
 * garantindo que não haja sobrecarga em tempo de execução.
 */
class FunctionLogger {
public:
    /**
     * @brief Construtor padrão que não realiza nenhuma ação.
     *
     * @param location Parâmetro opcional para compatibilidade, não utilizado nesta implementação.
     */
    FunctionLogger(const std::source_location& = std::source_location::current()) {}

    /**
     * @brief Destrutor padrão que não realiza nenhuma ação.
     */
    ~FunctionLogger() {}
};

VORMAKER_NAMESPACE_CLOSE

#endif // VORMAKER_FUNCTIONNULL_LOGGER_H
