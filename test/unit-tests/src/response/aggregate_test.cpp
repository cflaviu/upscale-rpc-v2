// Upscale RPC library
// Copyright (C) 2023 Flaviu C.
#include "types.hpp"
#include <upscale_rpc/response/aggregate.hpp>

namespace upscale_rpc::response::aggregate
{
    TEST_CASE("response/aggregate - small payloads")
    {
        raw_buffer buffer(0xFFFFu);
        auto buffer_span = to_span(buffer);

        random::device dev {};
        random::generator rng(dev());
        random::distribution no_of_contexts_generator(2, 10);
        random::distribution no_of_responses_per_context(1, 10);
        random::distribution context_generator(1, 30000);
        random::distribution param_size_generator(1, 1000);
        random::distribution char_generator(32, 126);
        random::distribution method_id_generator(1, 200);

        call_data_items_context_pair_vector expected_responses(no_of_contexts_generator(rng));
        for (auto& item: expected_responses)
        {
            auto& call_data_vector = item.first;
            call_data_vector.resize(no_of_responses_per_context(rng));
            for (auto& call_data: call_data_vector)
            {
                auto& response = call_data.params;
                response.resize(param_size_generator(rng));
                std::for_each(response.begin(), response.end(),
                              [&](std::uint8_t& c) { c = static_cast<std::uint8_t>(char_generator(rng)); });

                call_data.method = {0, method_id_generator(rng)};
            }

            item.second = context_generator(rng);
        }

        serialize(expected_responses, buffer_span);
        const auto* resulted_responses = deserialize(buffer_span);

        check_items(expected_responses, resulted_responses);
    }

    TEST_CASE("response/aggregate - min size")
    {
        call_data_items_context_pair_vector expected_data(1);
        auto& expected_responses = expected_data[0].first;
        expected_responses.resize(1);
        expected_responses[0].params.resize(1);

        raw_buffer buffer(128u);
        auto buffer_span = to_span(buffer);

        serialize(expected_data, buffer_span);
        constexpr auto expected_buffer_size = 97u;
        REQUIRE(buffer_span.size() == expected_buffer_size);
    }
}
