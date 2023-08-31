// Upscale RPC library
// Copyright (C) 2022-2023 Flaviu C.

#include <catch2/catch.hpp>
#include <random>
#include <vector>

namespace upscale_rpc
{
    using raw_buffer = std::vector<std::uint8_t>;

    void check_items(const auto& expected_items, const auto* const resulted_items)
    {
        REQUIRE(expected_items.size() == resulted_items->size());
        if (expected_items.size() == resulted_items->size())
        {
            auto resulted_item_iter = resulted_items->begin();
            for (auto& expected_item: expected_items)
            {
                REQUIRE(expected_item == *resulted_item_iter);
                ++resulted_item_iter;
            }
        }
    }
}

namespace upscale_rpc::random
{
    using device = std::random_device;
    using generator = std::mt19937;
    using distribution = std::uniform_int_distribution<std::mt19937::result_type>;
}
