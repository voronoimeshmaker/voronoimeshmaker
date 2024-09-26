//==============================================================================
// Name        : Shape.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Implementation of the Shape class methods.
//
// Copyright   : Copyright (C) 2024 Joao Flavio Vasconcellos
//               (jflavio at iprj.uerj.br)
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape.h>

VORMAKER_NAMESPACE_OPEN

Shape :: Shape(const GeometricDataHolder& _dataHolder) {
    ptrDataHolder_ = std::make_unique<GeometricDataHolder>(_dataHolder);
}

VORMAKER_NAMESPACE_CLOSE
