Background
==========

The model is essentially a stripped down translation of the CAESAR-Lisflood model, but without the GUI, and it runs on Linux unlike CAESAR-Lisflood, which is Windows only (as of 2017). If you are happy to run on Windows, and would prefer to use a model with a GUI-mode where you can view the output of your simulation in real-time, then CAESAR-Lisflood is a good choice for this. HAIL-CAESAR is designed to be faster and cross platform, will scale up to running across multi-core compute workstations and cluster PCs, and can be run to simulate multiple-member ensembles to explore model sensitivity. One of the drawbacks to the original model is that you need a dedicated Windows box to do your simulations, you can't send them off to a cluster computer-type facility, which typically run Unix. (Unless you have a bunch of spare Windows PCs...) 

The raster data handling is done using the libraries from the `LSDTopoTools <http://lsdtopotools.github.io/>`_ package, and so can be used in the same workflow as LSDTopoTools to do topographic analysis on your model output.


