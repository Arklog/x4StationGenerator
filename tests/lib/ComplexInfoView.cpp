//
// Created by pierre on 3/15/26.
//

#include "ComplexInfoView.hpp"
ComplexInfoView::ComplexInfoView (const t_x4_complex &complex): complex(complex) {
  genModuleSummary();
}

void ComplexInfoView::genModuleSummary ()
{
  for (const auto &module: complex) {
      module_summary[module]++;
  }
}
