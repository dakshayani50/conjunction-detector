# Conjunction Detector

A C++ tool that screens the public satellite catalog for close approaches
and shows that a spatial index finds the same events as brute force in a
fraction of the time.

## Limits

This is a screening tool on public, TLE-grade data with kilometre-level
uncertainty. It is not an operational collision-avoidance system and does
not compute collision probability.

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

## Status

Phase 0 (setup) in progress.