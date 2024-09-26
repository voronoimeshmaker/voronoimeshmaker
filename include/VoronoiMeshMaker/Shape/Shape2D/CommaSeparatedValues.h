//==============================================================================
// Name        : PolygonCSV.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Classe para ler as coordenadas do poligono
//               no formato cvs
//
// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos
//                                      (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//==============================================================================

#ifndef __VORONOMESHVMAKER_POLYGONCSV_H__
#define __VORONOMESHVMAKER_POLYGONCSV_H__


//==============================================================================
// includes da VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/Shape2D.h>

VORMAKER_NAMESPACE_OPEN

class CommaSeparatedValues  //: public Boundary2D
{

//    //==============================================================================
//    // ID da classe
//    //==============================================================================
//
//public:
//    
//    DefineIdentity  (   "PolygonCSV"
//                    ,   ID::PolygonCSV
//                    );  
//
//
//    //==============================================================================
//    // construtores / destrutora
//    //==============================================================================
//
//public:
//    PolygonCSV() noexcept = default;
//    PolygonCSV(const PolygonCSV &) noexcept = default;
//    virtual ~PolygonCSV() noexcept = default;
//
//    explicit PolygonCSV(const std::string &);
//
//    //==============================================================================
//    // sobrecarga de operadores
//    //==============================================================================
//
//public:
//    PolygonCSV &operator=(const PolygonCSV &) noexcept = default;
//
//    //==============================================================================
//    // Funcoes puramente virtuais
//    //==============================================================================
//
//private:
//    [[nodiscard]]
//    virtual bool GenerateBoundary2D() override;
//
//    void ReadCSV();
//
//    //==============================================================================
//    // variaveis da classe
//    //==============================================================================
//
//private:
//    std::string filename;
};

//==============================================================================
// typedef
//==============================================================================

//using PtrPolygonCSV = std::shared_ptr<PolygonCSV>;
//using PtrConstPolygonCSV = std::shared_ptr<const PolygonCSV>;

VORMAKER_NAMESPACE_CLOSE

#endif /* __VORONOMESHVMAKER_POLYGONCSV_H__ */
