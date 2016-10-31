TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES = ../../HAIL-CAESAR_driver.cpp \
../../../LSDCatchmentModel.cpp \
../../../LSDRaster.cpp \
../../../LSDIndexRaster.cpp \
../../../LSDStatsTools.cpp \
../../../LSDShapeTools.cpp \
../../../LSDGrainMatrix.cpp \
../../../LSDRainfallRunoff.cpp
