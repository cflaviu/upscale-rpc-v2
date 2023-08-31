// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#include "types.hpp"
#include <upscale_rpc/request/aggregate.hpp>

namespace upscale_rpc::request::aggregate
{
    TEST_CASE("request/aggregate - small payloads")
    {
        raw_buffer buffer(0xFFFFu);
        auto buffer_span = to_span(buffer);

        random::device dev {};
        random::generator rng(dev());
        random::distribution no_of_params_generator(5, 70);
        const auto no_of_params = no_of_params_generator(rng);
        const auto max_params_size = (buffer.size() - no_of_params * 10u) / no_of_params;
        random::distribution param_size_generator(1, max_params_size);
        random::distribution char_generator(32, 126);
        random::distribution method_id_generator(1, 200);

        call_data::vector expected_calls(no_of_params);
        for (auto& expected_call: expected_calls)
        {
            auto& item = expected_call.params;
            item.resize(param_size_generator(rng));
            std::for_each(item.begin(), item.end(), [&](std::uint8_t& c) { c = static_cast<std::uint8_t>(char_generator(rng)); });

            expected_call.method = {0, method_id_generator(rng)};
        }

        const context_id_t expected_context_id {1234u};
        serialize(expected_context_id, expected_calls, buffer_span);
        const auto [resulted_calls, resulted_context_id] = deserialize(buffer_span);

        check_items(expected_calls, resulted_calls);
    }

    TEST_CASE("request/aggregate - min size")
    {
        call_data::vector expected_calls(1);
        expected_calls[0].params.resize(1);

        raw_buffer buffer(128u);
        auto buffer_span = to_span(buffer);

        serialize({}, expected_calls, buffer_span);
        constexpr auto expected_buffer_size = 65u;
        REQUIRE(buffer_span.size() == expected_buffer_size);
    }
}
