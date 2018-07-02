#!/usr/bin/env bash
mkdir -p ./results/boscastle50m_72_u/
mkdir -p ./results/boscastle50m_72_u_flipped_lr/
# Boscastle test 1
# 50m resolution, 72 hours rainfall
../bin/HAIL-CAESAR.exe ./input_data/boscastle_input_data/ boscastle_test_72hr_50m_u.params
../bin/HAIL-CAESAR.exe ./input_data/boscastle_input_data/ boscastle_test_72hr_50m_u_flipped_lr.params
