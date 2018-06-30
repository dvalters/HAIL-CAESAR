import pytest
import json
import rasterio
import numpy.testing as ntest

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

@pytest.fixture
def params():
    """
    Pytest fixture that reads in paths containing results/KGAs
    and returns them as a list of params.
    """
    with open('known_good_answers/paths.json') as f:
        fixtures = json.loads(f.read())
    params = []

    for fix in fixtures:
        params.append(pytest.param(raster(fixtures[fix]['result']),
                                   raster(fixtures[fix]['expected']),
                                   id=fix))
    return params


class TestingHAILCAESAR():
    @pytest.mark.parametrize('result,expected', params())
    def test_basic_metrics(self, result, expected):
        ntest.assert_allclose(result, expected, rtol=1e-03)
