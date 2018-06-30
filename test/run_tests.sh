#!/usr/bin/bash
mkidr -p ./results/boscastle50m_72_u/
# Boscastle test 1
# 50m resolution, 72 hours rainfall
../bin/hailcaesar ./input_data/boscastle_input_data/ boscastle_test_72hr_50m_u.params
