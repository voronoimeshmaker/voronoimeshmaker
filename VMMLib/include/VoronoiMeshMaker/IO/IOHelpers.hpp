// ----------------------------------------------------------------------------
// File: IOHelpers.hpp
// Author: VoronoiMeshMaker Team
// Version: 1.0.0
// Description: Helper classes for formatted output 
//              (PrintLine, HeaderPrinter, CommentPrinter)
// License: GNU GPL v3
// (c) 2025 VoronoiMeshMaker Project. All rights reserved.
// ----------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <iomanip>

#include <VoronoiMeshMaker/Core/namespace.h>

VORMAKER_NAMESPACE_OPEN
IO_NAMESPACE_OPEN

/**
 * @brief Manipulador personalizado para imprimir linhas no stream de saída
 */
class PrintLine {

public:
    explicit PrintLine(const unsigned& _lsize = 80) : m_lsize(_lsize) {}
    
    friend std::ostream& operator<<(std::ostream& _os, const PrintLine& _pl) {
        const char LINE_CHAR = '=';
        auto generateChar = [&]() { return LINE_CHAR; };
        
        _os << "// ";
        std::generate_n(std::ostream_iterator<char>(_os), _pl.m_lsize-3, generateChar);
        _os << "\n";
        
        return _os;
    }
    
    inline PrintLine& size(const unsigned& size) {
        m_lsize = size;
        return *this;
    }
    
private:
    unsigned m_lsize = 80;
};

/**
 * @brief Classe para impressão de cabeçalhos formatados
 */
class HeaderPrinter {
public:
    /**
     * @brief Construtor com texto de licença personalizado
     */
    HeaderPrinter   (   const std::string_view& _title
                    ,   const std::string_view& _author
                    ,   const std::string_view& _version
                    ,   const std::string_view& _description
                    ,   const std::string_view& _licenseText = ""
                    ,   const unsigned& _lineSize = 80
                    )
                    :   m_title(_title)
                    ,   m_author(_author)
                    ,   m_version(_version)
                    ,   m_description(_description)
                    ,   m_licenseText(_licenseText)
                    ,   m_lineSize(_lineSize)
                    ,   m_useDefaultLicense(_licenseText.empty()
                    )   {}
    
    friend std::ostream& operator<< (   std::ostream& _os
                                    ,   const HeaderPrinter& _hp
                                    ) {
        // Linha superior
        _os << "//";
        std::generate_n(std::ostream_iterator<char>(_os), _hp.m_lineSize - 2, 
                       []() { return '='; });
        _os << "\n";
        
        // Informações do cabeçalho
        _hp.printCentered(_os, "Nome        : " + std::string(_hp.m_title));
        _hp.printCentered(_os, "Autor       : " + std::string(_hp.m_author));
        _hp.printCentered(_os, "Versao      : " + std::string(_hp.m_version));
        _hp.printCentered(_os, "Descricao   : " + std::string(_hp.m_description));
        _os << "//\n";
        
        // Texto da licença (padrão ou personalizado)
        if (_hp.m_useDefaultLicense) {
            _hp.printDefaultLicense(_os);
        } else {
            _hp.printCustomLicense(_os);
        }
        
        // Linha inferior
        _os << "//";
        std::generate_n(std::ostream_iterator<char>(_os), _hp.m_lineSize - 2, 
                       []() { return '='; });
        _os << "\n";
        
        return _os;
    }
    
    HeaderPrinter& lineSize(const unsigned& size) {
        m_lineSize = size;
        return *this;
    }
    
    HeaderPrinter& licenseText  (   std::string_view licenseText) {
        m_licenseText = licenseText;
        m_useDefaultLicense = licenseText.empty();
        return *this;
    }
    
private:

    void printCentered  (   std::ostream& _os
                        ,   const std::string_view& _text
                        )   const {
        int totalSpaces = m_lineSize - 3 - _text.length();
        int leftSpaces = totalSpaces > 0 ? totalSpaces / 2 : 0;
        
        _os << "// " << std::setw(leftSpaces + _text.length()) 
            << std::setfill(' ') << std::left << _text;
        
        if (totalSpaces > 0 && totalSpaces % 2 != 0) {
            _os << " ";
        }
        
        _os << "\n";
    }
    

void printDefaultLicense(std::ostream& _os) const {
    _os << "//\n";
    printCentered(_os, "");
    printCentered(_os, "Este programa e software livre: voce pode redistribui-lo e/ou");
    printCentered(_os, "modifica-lo sob os termos da Licenca Publica Geral GNU, versao 3");
    printCentered(_os, "ou qualquer versao posterior.");
    printCentered(_os, "");
    printCentered(_os, "Esta biblioteca utiliza a CGAL (Computational Geometry Algorithms Library)");
    printCentered(_os, "sob os termos da LGPLv3+. Para mais informacoes sobre a CGAL, visite:");
    printCentered(_os, "https://www.cgal.org/license.html");
    printCentered(_os, "");
    printCentered(_os, "Este programa e distribuido na esperanca de que seja util,");
    printCentered(_os, "mas SEM QUALQUER GARANTIA; sem mesmo a garantia implicita de");
    printCentered(_os, "COMERCIABILIDADE ou ADEQUACAO A UM DETERMINADO PROPOSITO.");
    printCentered(_os, "Consulte a Licenca Publica Geral GNU para mais detalhes.");
    printCentered(_os, "");
    printCentered(_os, "Voce deve ter recebido uma copia da Licenca Publica Geral GNU");
    printCentered(_os, "junto com este programa. Se nao, veja <https://www.gnu.org/licenses/>.");
    _os << "//\n";
}

    
    void printCustomLicense(std::ostream& _os) const {
        std::istringstream stream{std::string(m_licenseText)};
        std::string line;
        
        _os << "//\n";
        while (std::getline(stream, line)) {
            printCentered(_os, line);
        }
        _os << "//\n";
    }
    
    std::string_view m_title;
    std::string_view m_author;
    std::string_view m_version;
    std::string_view m_description;
    std::string_view m_licenseText;
    unsigned m_lineSize = 0;
    bool m_useDefaultLicense = true;
};

/**
 * @brief Classe para impressão de textos formatados como comentários
 */
class CommentPrinter {
public:
    explicit CommentPrinter(std::string_view _text) : m_text(_text) {}
    
    friend std::ostream& operator<<(std::ostream& _os, const CommentPrinter& _cp) {
        std::istringstream stream(std::string(_cp.m_text));
        std::string line;
        
        while (std::getline(stream, line)) {
            _os << "// " << line << "\n";
        }
        
        return _os;
    }
    
    CommentPrinter& text(std::string_view text) {
        m_text = text;
        return *this;
    }
    
private:
    std::string_view m_text;
};

IO_NAMESPACE_CLOSE
VORMAKER_NAMESPACE_CLOSE

