#include "my_application.hxx"

my_namespace_start

namespace application {
namespace internal {
namespace { // unnamed

std::size_t get_calc_total( \
  const std::list<zer::application::contents::occbool_calc_projects> \
      &occbool_projects
  , const int type) {
  std::size_t total { 0 };
  for (const auto &project : occbool_projects) {
    for (const auto &bim_info : project.bim_models) {
      if ((bim_info.second.state & type) == type) {
        ++total;
      }
    }
  }

  return total;
}

} // end namespace unnamed
} // end namespace internal

  std::size_t contents::get_calc_total() const {
    return internal::get_calc_total(occbool_projects
    , bim_model_info::STATE::TODO);
  }

  std::size_t contents::get_calc_send_total() const {
    return internal::get_calc_total(occbool_projects
    , bim_model_info::STATE::SEND);
  }

  std::size_t contents::get_calc_success_total() const {
    return internal::get_calc_total(occbool_projects
    , bim_model_info::STATE::SUCCESS_CALC);
  }

} // end namespace application

my_namespace_end
