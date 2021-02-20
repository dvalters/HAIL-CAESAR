#!/usr/bin/env bash
mkdir -p ./results/caersws_reach/
# Boscastle test 1
# 50m resolution, 72 hours rainfall
../bin/HAIL-CAESAR.exe ./input_data/caersws/ Caersws.params
