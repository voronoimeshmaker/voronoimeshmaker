#ifndef VORMAKER_FUNCTION_LOGGER_H
#define VORMAKER_FUNCTION_LOGGER_H

//==============================================================================
// Nome        : FunctionLogger.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 2.1
// Descrição   : Logger de fluxo de função para a biblioteca VoronoiMeshMaker.
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
 * @file FunctionLogger.h
 * @brief Define a classe FunctionLogger, que fornece rastreamento automático
 * de entrada e saída de funções para a biblioteca VoronoiMeshMaker.
 *
 * A classe FunctionLogger registra a entrada e saída de funções, capturando informações detalhadas
 * como arquivo, linha e nome da função. Suporta registro automatizado via RAII,
 * simplificando o rastreamento do fluxo do programa, especialmente em contextos de depuração
 * e tratamento de exceções.
 *
 * O comportamento do FunctionLogger pode ser controlado via a macro `NDEBUG1`. Se `NDEBUG1` for definida,
 * o FunctionLogger terá uma implementação vazia para evitar sobrecarga em builds de release.
 * Caso contrário, a implementação completa é utilizada para fornecer informações detalhadas de log.
 *
 * @ingroup debug
 * @version 2.1
 * @date 2024
 * @author
 * João Flávio Vieira de Vasconcellos
 * (jflavio at iprj.uerj.br)
 */

#undef NDEBUG1

#ifndef NDEBUG1  // Versão de depuração: função de log ativa
    #include <VoronoiMeshMaker/Error/FunctionLoggerFull.h>
#else  // Versão de release: implementação vazia para evitar sobrecarga
    #include <VoronoiMeshMaker/Error/FunctionLoggerNull.h>
#endif  

#endif // VORMAKER_FUNCTION_LOGGER_H
