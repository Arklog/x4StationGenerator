//
// Created by pierre on 3/17/26.
//

#ifndef X4STATIONGENERATOR_FMT_VECTOR_FORMATTER_HPP
#define X4STATIONGENERATOR_FMT_VECTOR_FORMATTER_HPP

#include <cstddef>
#include <vector>

#include <spdlog/fmt/bundled/format.h>

template <typename T, typename Allocator>
struct fmt::formatter<std::vector<T, Allocator>>
{
    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const std::vector<T, Allocator>& values, FormatContext& ctx) const
        -> typename FormatContext::iterator
    {
        auto out = fmt::format_to(ctx.out(), "(");
        for (std::size_t i = 0; i < values.size(); ++i) {
            if (i > 0) {
                out = fmt::format_to(out, ", ");
            }
            out = fmt::format_to(out, "{}", values[i]);
        }
        return fmt::format_to(out, ")");
    }
};

template<>
struct fmt::formatter<WareAmount> {
  constexpr auto parse(fmt::format_parse_context & ctx) -> fmt::format_parse_context::iterator {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto format(const WareAmount& ware_amount, FormatContext& ctx) const -> typename FormatContext::iterator {
    return fmt::format_to(ctx.out(), "({}, {})", ware_amount.id.raw(), ware_amount.amount);
  }
};

#endif // X4STATIONGENERATOR_FMT_VECTOR_FORMATTER_HPP
