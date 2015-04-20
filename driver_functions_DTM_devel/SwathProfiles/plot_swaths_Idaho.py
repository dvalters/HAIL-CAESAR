## eco_plot_stats.py
##=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Plots a histogram of return heights above ground, and a 3D plof of the point
## cloud
##=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## DTM 08/03/2013
##=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#import modules
import numpy as np, matplotlib.pyplot as plt, matplotlib.ticker as tk
import matplotlib.collections as collections
from matplotlib import rcParams
from mpl_toolkits.axes_grid1.inset_locator import *

# Set up fonts for plots
rcParams['font.family'] = 'sans-serif'
rcParams['font.sans-serif'] = ['arial']
rcParams['font.size'] = 14
rcParams['legend.numpoints'] = 1
axis_size = 16

def read_histogram_file(hist_file):
    f = open(hist_file, 'r')
    lines = f.readlines()
    N = len(lines)-1
    Mids = np.zeros(N)
    PD = np.zeros(N)
        
    for i in range (0, N):               
       line = lines[i+1].strip().split(" ")
       Mids[i] = float(line[0])
       PD[i] = float(line[4])
       
    f.close()
    return Mids,PD
    

def read_chan_file(ChanFile):
    f = open(ChanFile, 'r')
    lines = f.readlines()
    N = len(lines)-1
    distance = np.zeros(N)
    elevation = np.zeros(N)
        
    for i in range (0, N):               
       line = lines[i+1].strip().split(",")
       distance[i] = float(line[9])
       elevation[i] = float(line[10])
       
    f.close()
    distance = distance - np.min(distance)
    distance = np.max(distance)-distance
    return distance, elevation    
    
    
def read_swath_file(SwathFile):
    
    f = open(SwathFile, 'r')
    lines = f.readlines()
    N_Segments = len(lines)-1

    # Initialise a bunch of vectors
    distance = np.zeros(N_Segments)
    mean = np.zeros(N_Segments)
    sd = np.zeros(N_Segments)
    minimum = np.zeros(N_Segments)
    LQ = np.zeros(N_Segments)
    median = np.zeros(N_Segments)
    UQ = np.zeros(N_Segments)
    maximum = np.zeros(N_Segments)
    for i in range (0, N_Segments):        
       
       line = lines[i+1].strip().split(" ")
       distance[i] = float(line[0])
       mean[i] = float(line[1])
       sd[i] = float(line[2])
       minimum[i] = float(line[3])
       LQ[i] = float(line[4])
       median[i] = float(line[5])
       UQ[i] = float(line[6])
       maximum[i] = float(line[7])

    f.close()
    
    return distance, mean, sd, minimum, LQ, median, UQ, maximum

def plot_swath_profiles(distance, channel_elev, swathdistance, slope_med, slope_lower, slope_upper, slope_min, slope_max, s3_med, s3_lower, s3_upper, s3_min, s3_max,
                        mids_slope_a,PD_slope_a,mids_slope_b,PD_slope_b,mids_s3_a,PD_s3_a,mids_s3_b,PD_s3_b):

    OutputFigureFormat = 'pdf'    
    # SET UP COLOURMAPS
    
    # PLOT 1 -> channel profile
    plt.figure(1, facecolor='White',figsize=[10,10])
    
    ax1 = plt.subplot(313)
    collection = collections.BrokenBarHCollection.span_where(distance, ymin=0, ymax=3000, where=distance>=4800, facecolor='0.8', edgecolor='0.8',alpha=0.5)
    ax1.add_collection(collection)
    ax1.plot(distance,channel_elev,'-',color="black",linewidth=2)
    ax1.set_ylabel('Elevation (m)', fontsize = axis_size)  
    majorLocator   = tk.MultipleLocator(200)
    ax1.yaxis.set_major_locator(majorLocator)
    ax1.get_yaxis().set_label_coords(-0.06,0.5)
    ax1.annotate('Major knickpoint', xy=(4800, 1920),  xycoords='data',
                xytext=(0.95, 0.90), textcoords='axes fraction',
                arrowprops=dict(arrowstyle="fancy",facecolor="gold",edgecolor="black",linewidth=0.25,shrinkB=4,
                            connectionstyle="arc3,rad=0.1",),
                horizontalalignment='right', verticalalignment='top',fontsize = axis_size)   
    ax1.annotate('c', xy=(0.02,0.9), xycoords='axes fraction',backgroundcolor='white',horizontalalignment='left', verticalalignment='bottom', fontsize=rcParams['font.size']+2) 
    ax1.set_ylim(1000,2500)

    ax1.xaxis.grid(True)  

    # PLOT 2 -> Slope
    ax2 = plt.subplot(312, sharex=ax1)    
    collection2 = collections.BrokenBarHCollection.span_where(distance, ymin=0, ymax=1000, where=distance>=4800, facecolor='0.8', edgecolor='0.8',alpha=0.5)
    ax2.add_collection(collection2)
        
    print swathdistance.size
    print slope_med.size
    ax2.plot(swathdistance,slope_med,'-',color="blue")
    ax2.fill_between(swathdistance, slope_min, slope_max, where=None, color="white", alpha = 1) 
    ax2.fill_between(swathdistance, slope_lower, slope_upper, where=None, color="blue", alpha = 0.2) 
    ax2.fill_between(swathdistance, slope_min, slope_max, where=None, color="blue", alpha = 0.2)
    majorLocator = tk.MultipleLocator(0.5)
    ax2.yaxis.set_major_locator(majorLocator)       
    ax2.set_ylabel('Gradient', fontsize = axis_size)
    ax2.yaxis.tick_right()
    ax2.yaxis.set_label_position("right")
    ax2.set_ylim(0,2)
    ax2.xaxis.grid(True)
    ax2.annotate('b', xy=(0.02,0.9), xycoords='axes fraction',backgroundcolor='None',horizontalalignment='left', verticalalignment='bottom', fontsize=rcParams['font.size']+2) 
    
    inset_ax2 = inset_axes(ax2,width="20%",height="40%",loc=2,bbox_to_anchor=(0.1, 0., 1, 1),bbox_transform=ax2.transAxes)
    inset_ax2.plot(mids_slope_a,PD_slope_a,color="black")
    inset_ax2.plot(mids_slope_b,PD_slope_b,color="0.5")
    inset_ax2.fill_between(mids_slope_b, 0, PD_slope_b, where=None, color="0.8", alpha = 0.5)
    inset_ax2.plot(mids_slope_a,PD_slope_a,color="black")
    inset_ax2.set_xlim(0,2)    
    inset_ax2.set_xlabel('Gradient', fontsize = axis_size-2)
    inset_ax2.set_ylabel('Probability \ndensity', fontsize = axis_size-2, horizontalalignment = "center")
    inset_ax2.get_yaxis().set_label_coords(-0.15,0.5)
    plt.setp(inset_ax2, yticks=[])
    print "modal slopes"
    print mids_slope_a[PD_slope_a == np.max(PD_slope_a)]

    print mids_slope_b[PD_slope_b == np.max(PD_slope_b)]    
    
    # PLOT 3 -> Roughness
    ax3 = plt.subplot(311, sharex=ax1)    
    collection3 = collections.BrokenBarHCollection.span_where(distance, ymin=0, ymax=1000, where=distance>=4800, facecolor='0.8',edgecolor='0.8', alpha=0.5)
    ax3.add_collection(collection3)    
    ax3.plot(swathdistance,s3_med,'-', color="magenta")
    ax3.fill_between(swathdistance, s3_min, s3_max, where=None, color="white", alpha = 1) 
    ax3.fill_between(swathdistance, s3_lower, s3_upper, where=None, color="magenta", alpha = 0.2) 
    ax3.fill_between(swathdistance, s3_min, s3_max, where=None, color="magenta", alpha = 0.2) 
    ax3.set_ylabel('S$_3$', fontsize = axis_size, fontstyle='italic')
    majorLocator = tk.MultipleLocator(0.01)
    ax3.yaxis.set_major_locator(majorLocator)
    ax3.get_yaxis().set_label_coords(-0.06,0.5)
    ax3.set_ylim(0,0.05)
    ax3.xaxis.grid(True) 
    ax3.annotate('a', xy=(0.02,0.9), xycoords='axes fraction',backgroundcolor='None',horizontalalignment='left', verticalalignment='bottom', fontsize=rcParams['font.size']+2) 
    
    inset_ax3 = inset_axes(ax3,width="20%",height="40%",loc=2,bbox_to_anchor=(0.1, 0., 1, 1),bbox_transform=ax3.transAxes)
#    inset_ax3.set_xscale('log')
    inset_ax3.plot(mids_s3_b,PD_s3_b,color="0.5")
    inset_ax3.fill_between(mids_s3_b, 0, PD_s3_b, where=None, color="0.8", alpha = 0.5)
    inset_ax3.plot(mids_s3_a,PD_s3_a,color="black")
    inset_ax3.set_xlim(0,0.015)
    majorLocator = tk.MultipleLocator(0.005)
    inset_ax3.xaxis.set_major_locator(majorLocator)
    inset_ax3.set_ylabel('Probability \ndensity', fontsize = axis_size-2, horizontalalignment = "center")
    inset_ax3.set_xlabel('S$_3$', fontsize = axis_size-2,fontstyle='italic')
    inset_ax3.get_yaxis().set_label_coords(-0.15,0.5)   
    inset_ax3.annotate('Upstream of knickpoint', xy=(0.0010,0.15),  xycoords='data',
                xytext=(1.1, 0.90), textcoords='axes fraction',
                arrowprops=dict(arrowstyle="fancy",facecolor="gold",edgecolor="black",linewidth=0.25,shrinkB=4,
                            connectionstyle="arc3,rad=0.1",),
                horizontalalignment='left', verticalalignment='top',fontsize = axis_size-2)       
                
    inset_ax3.annotate('Downstream of knickpoint', xy=(0.0048,0.04),  xycoords='data',
                xytext=(1.1, 0.60), textcoords='axes fraction',
                arrowprops=dict(arrowstyle="fancy",facecolor="gold",edgecolor="black",linewidth=0.25,shrinkB=4,
                            connectionstyle="arc3,rad=0.1",),
                horizontalalignment='left', verticalalignment='top',fontsize = axis_size-2)   
        
    # get some percentage stats below knickpoint
    percentage_greater_than_010 = np.sum(PD_s3_a[mids_s3_a>=0.010])
    print percentage_greater_than_010
    percentage_greater_than_015 = np.sum(PD_s3_a[mids_s3_a>=0.015])
    print percentage_greater_than_015    
    percentage_greater_than_010 = np.sum(PD_s3_b[mids_s3_b>=0.010])
    print percentage_greater_than_010
    percentage_greater_than_015 = np.sum(PD_s3_b[mids_s3_b>=0.015])
    print percentage_greater_than_015
    
    plt.setp(inset_ax3, yticks=[])
    
    # Configure final plot    
    xticklabels = ax3.get_xticklabels()+ax2.get_xticklabels()
    plt.setp(xticklabels, visible=False)
    ax1.set_xlabel('Distance downstream (m)', fontsize = axis_size)
     
    ax1.set_xlim(0,np.max(distance))
    plt.tight_layout()        
    plt.subplots_adjust(hspace=0.001)

    plt.savefig("K:/data/SurfaceRoughness/Idaho/id_swath_long_profiles.pdf",format = OutputFigureFormat)
    plt.show()
    
if __name__ == "__main__":
    ChanFile = "K:/data/SurfaceRoughness/Idaho/id_ChanNet_141_for_Arc.csv"    
    SlopeSwathFile = "K:/data/SurfaceRoughness/Idaho/id_slope_long_profile.txt"
    s3SwathFile = "K:/data/SurfaceRoughness/Idaho/id_s3_1_3_hillslope_long_profile.txt"
    SlopeHistFileAbove = "K:/data/SurfaceRoughness/Idaho/id_slopeaboveKP.txt"
    SlopeHistFileBelow = "K:/data/SurfaceRoughness/Idaho/id_slopebelowKP.txt"
    s3HistFileAbove = "K:/data/SurfaceRoughness/Idaho/id_s3_1_3_hillslopeaboveKP.txt"
    s3HistFileBelow = "K:/data/SurfaceRoughness/Idaho/id_s3_1_3_hillslopebelowKP.txt"
    
    distance, elevation = read_chan_file(ChanFile)
    mids_slope_a, PD_slope_a = read_histogram_file(SlopeHistFileAbove)
    mids_slope_b, PD_slope_b = read_histogram_file(SlopeHistFileBelow)
    mids_s3_a, PD_s3_a = read_histogram_file(s3HistFileAbove)
    mids_s3_b, PD_s3_b = read_histogram_file(s3HistFileBelow)
    
    swathdistance, slope_mean, slope_sd, slope_minimum, slope_LQ, slope_median, slope_UQ, slope_maximum = read_swath_file(SlopeSwathFile)
    swathdistance, s3_mean, s3_sd, s3_minimum, s3_LQ, s3_median, s3_UQ, s3_maximum = read_swath_file(s3SwathFile)
    plot_swath_profiles(distance, elevation, swathdistance, slope_median, slope_LQ, slope_UQ, slope_minimum, slope_maximum, 
                        s3_median, s3_LQ, s3_UQ, s3_minimum, s3_maximum,
                        mids_slope_a,PD_slope_a,mids_slope_b,PD_slope_b,mids_s3_a,PD_s3_a,mids_s3_b,PD_s3_b)
    