[![Build Status](https://travis-ci.org/dvalters/HAIL-CAESAR.svg?branch=master)](https://travis-ci.org/dvalters/HAIL-CAESAR) 
[![Documentation Status](https://readthedocs.org/projects/hail-caesar/badge/?version=latest)](http://hail-caesar.readthedocs.io/en/latest/?badge=latest)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/11f4e6d1de1e4c5a9e1de7acbd3b6246)](https://www.codacy.com/app/dvalters/HAIL-CAESAR?utm_source=github.com&utm_medium=referral&utm_content=dvalters/HAIL-CAESAR&utm_campaign=badger)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![DOI](https://zenodo.org/badge/87577150.svg)](https://zenodo.org/badge/latestdoi/87577150)


# HAIL-CAESAR

This is the development version of the HAIL-CAESAR model. 

_The **H**igh-performance **A**rchitecture **I**ndependent **L**ISFLOOD-**C**AESAR model_

HAIL-CAESAR is a hydrodynamic, landscape evolution, cellular automaton model. In other words, it simulates hydrological and erosional processes in a river catchment, for the purposes of investigating flood-inundation, sediment transport and catchment evolution over a range of timescales from hours to thousands of years. (And potentially longer if you are really patient enough...)

The model is derived from the [CAESAR-Lisflood model](https://sourceforge.net/projects/caesar-lisflood/), which is a C#/.NET Windows-based implementation of the model, with a very useful GUI. You may find much of the documentation and discussion for CAESAR-Lisflood relevant for using HAIL-CAESAR, but remember there are small differences at present, so treat the documentation [here](http://hail-caesar.readthedocs.io/en/latest) as the canonical source.

HAIL-CAESAR doesn't have a GUI - the model is run from scripts or from typing commands at the command line/terminal. The model runs and writes output directly to files on disk. You then have to process these output files yourself to view the model output subsequently. 

[LSDMappingTools](https://github.com/LSDtopotools/LSDMappingTools) is a good package for visualising the output and producing research-quality figures from HAIL-CAESAR. GIS programs, such as the excellent [QGIS](www.qgis.org) are also useful.

[Documentation can be found here](http://hail-caesar.readthedocs.io/en/latest).







