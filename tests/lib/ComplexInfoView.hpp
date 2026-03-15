//
// Created by pierre on 3/15/26.
//

#ifndef X4STATIONGENERATOR_COMPLEXINFOVIEW_HPP
#define X4STATIONGENERATOR_COMPLEXINFOVIEW_HPP

#include "StationBuilder/defines.hpp"

class ComplexInfoView
{
private:
  void genModuleSummary();

public:
  using t_complex_module_number = std::unordered_map<t_module_id, size_t >;

  const t_x4_complex &complex;
  t_complex_module_number module_summary;

  ComplexInfoView(const t_x4_complex &complex);
};

#endif // X4STATIONGENERATOR_COMPLEXINFOVIEW_HPP
