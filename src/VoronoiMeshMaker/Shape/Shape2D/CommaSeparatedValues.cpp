//==============================================================================
// includes da biblioteca C++
//==============================================================================

#include <filesystem>
#include <fstream>
#include <sstream> 
namespace fs = std::filesystem;

//==============================================================================
// includes da biblioteca VoronoiMeshMaker
//==============================================================================

#include <VoronoiMeshMaker/Shape/Shape2D/CommaSeparatedValues.h>
#include <VoronoiMeshMaker/Error/MakerException.h>
#include <VoronoiMeshMaker/Misc/misc.h>

VORMAKER_NAMESPACE_OPEN
 
//#undef  __FUNCT__
//#define __FUNCT__ "PolygonCSV::PolygonCSV  (const std::string&)"
//PolygonCSV::PolygonCSV  (const std::string& _filename) : filename(_filename)                            
//{
//
//        try {
//
//                if (!GenerateBoundary2D ()) {
//                    throw  
//                    VMMException    (   SourceInfo( __FILE__, __LINE__, __FUNCT__)
//                                    ,   vmm::NULLPOLYGON
//                                    ,   true
//                                    );
//                }        
//                
//        } catch (const VMMException& _adsException) {
//            std::cout << _adsException;
//            exit (EXIT_FAILURE);
//        };
//        
//        auto flag = TestBoundary2D();
//}
//
////==============================================================================
//// Gerando os pontos do retangulo
////==============================================================================
//
//#undef  __FUNCT__
//#define __FUNCT__ "bool Boundary2D :: GenerateBoundary2D ()"
//bool 
//PolygonCSV :: GenerateBoundary2D () 
//{
//    ReadCSV ();
//    
//    if (ptrBoundaryPolygon_->size() == 0) return false;
//    return true;
//    
//};
//
//void 
//PolygonCSV :: ReadCSV () {
//
//auto split = [](const std::string& s, char delimiter) -> std::vector<std::string> {
//        std::vector<std::string> tokens;
//        std::string token;
//        std::istringstream tokenStream(s);
//
//        while (std::getline(tokenStream, token, delimiter)) {
//            tokens.push_back(token);
//        }
//        
//        return tokens;
//    };     
//    
//std::string         line;    
//std::ifstream       input;
//LstPoint2D          lstPoint2D; 
//
//        try {
//                if (!fs::exists(filename)) {
//                    throw  
//                    VMMException    (   SourceInfo( __FILE__, __LINE__, __FUNCT__)
//                                    ,   vmm::FILENOTFOUND
//                                    ,   ("Nome do arquivo: " + filename)
//                                    ,   true
//                                    );
//                }
//                
//                input.open(filename);
//                
//                if (!input.is_open()) {
//                    throw  
//                    VMMException    (   SourceInfo( __FILE__, __LINE__, __FUNCT__)
//                                    ,   vmm::FILEREADPROBLEM
//                                    ,   ("Nome do arquivo: " + filename)
//                                    ,   true
//                                    );
//                }
//                
//                while (std::getline(input, line)) {
//                    
//                    if (!IsEmptyOrBlank(line)) {
//                    
//                        auto tokens = split(line, ',');
//                        if (tokens.size() != 2) {
//                            throw std::runtime_error("Formato de linha inválido: " + line);
//                        }
//                    
//                        lstPoint2D.emplace_back(std::stod(tokens[0]), std::stod(tokens[1]));
//                    }
//                }                
//                
//        } catch (const VMMException& _adsException) {
//            std::cout << _adsException;
//            exit (EXIT_FAILURE);
//        }    
//    
//        input.close();
//
//        ptrBoundaryPolygon_ = 
//                std::make_shared<Polygon2D> (   std::begin(lstPoint2D)
//                                            ,   std::end(lstPoint2D)
//                                            );
//        lstPoint2D.clear();        
// 
//        if (ptrBoundaryPolygon_->is_counterclockwise_oriented()) {
//            ptrBoundaryPolygon_->reverse_orientation();
//        }
//}
//

VORMAKER_NAMESPACE_CLOSE
