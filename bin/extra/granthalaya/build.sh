#!/usr/bin/env bash
rustc src/main.rs -o granthalaya/granthalaya -C inline-threshold=275 -C target-cpu=native
strip granthalaya/granthalaya
