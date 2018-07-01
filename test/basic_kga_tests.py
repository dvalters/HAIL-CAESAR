import pytest
import json
import rasterio
import numpy.testing as ntest
from numpy import loadtxt

"""
Python testing framework for the HAIL-CAESAR model

Regression/functional tests for standard test cases
of HAIL-CAESAR, comparing to Known Good Answers (KGA's)

Based on Stuart Grieve's LSDTopoTools test framework.
"""


def raster(filename):
    """
    Returns a numpy array from a filename for later diffing
    """
    out_data = rasterio.open(filename)
    return out_data.read(1)


def timeseries(filename, col_number):
    """
    Returns a 1D array from the filename specifying the timeseries
    file (output.dat) i.e. hydrographs, sedigraphs.
    """
    out_data = loadtxt(filename, usecols=col_number)
    return out_data


@pytest.fixture
def rasters_params():
    """
    Pytest fixture that reads in paths containing results/KGAs
    and returns them as a list of params. (For rasters)
    """
    with open('known_good_answers/rasters.json') as f:
        fixtures = json.loads(f.read())
    params = []

    for fix in fixtures:
        params.append(pytest.param(raster(fixtures[fix]['result']),
                                   raster(fixtures[fix]['expected']),
                                   id=fix))
    return params


def timeseries_params():
    """
    Fixture for the timeseries files
    """
    with open('known_good_answers/timeseries.json') as f:
        fixtures = json.loads(f.read())
    params = []

    for ts in fixtures:
        params.append(pytest.param(timeseries(fixtures[ts]['result'], 1),
                                   timeseries(fixtures[ts]['expected'], 1),
                                   id=ts))
    return params


class TestingHAILCAESAR():
    @pytest.mark.parametrize('result,expected', rasters_params())
    def test_water_depths(self, result, expected):
        ntest.assert_allclose(result, expected, rtol=1e-03)

    @pytest.mark.parametrize('result,expected', timeseries_params())
    def test_hydrograph_lisflood(self, result, expected):
        ntest.assert_allclose(result, expected, rtol=1e-03)
