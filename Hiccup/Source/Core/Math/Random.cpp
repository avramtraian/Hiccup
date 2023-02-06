// Copyright (c) 2022-2023 Avram Traian. All rights reserved.

#include "Random.h"

#include <random>

namespace HC
{

static_internal std::random_device s_RDFloat32;
static_internal std::mt19937 s_GenFloat32(s_RDFloat32());
static_internal std::uniform_real_distribution<float32_t> s_DistFloat32(0.0F, 1.0F);

static_internal std::random_device s_RDFloat64;
static_internal std::mt19937_64 s_GenFloat64(s_RDFloat64());
static_internal std::uniform_real_distribution<float64_t> s_DistFloat64(0.0, 1.0);

static_internal std::random_device s_RDInt32;
static_internal std::mt19937 s_GenInt32(s_RDInt32());
static_internal std::uniform_int_distribution<uint32_t> s_DistInt32;

static_internal std::random_device s_RDInt64;
static_internal std::mt19937_64 s_GenInt64(s_RDInt64());
static_internal std::uniform_int_distribution<uint64_t> s_DistInt64;

float32_t Random::float_32()
{
    return s_DistFloat32(s_GenFloat32);
}

float64_t Random::float_64()
{
    return s_DistFloat64(s_GenFloat64);
}

uint32_t Random::uint_32()
{
    return s_DistInt32(s_GenInt32);
}

uint64_t Random::uint_64()
{
    return s_DistInt64(s_GenInt64);
}

} // namespace HC