import numpy as np
def maxdownsample2d(data, factor1, factor2):
    trimrows = int(np.floor(data.shape[0] / float(factor1))) * factor1
    trimcols = int(np.floor(data.shape[1] / float(factor2))) * factor2
    #print trimrows,trimcols
    first = True
    for coff in range(factor2):
        for roff in range(factor1):
            hopped = data[roff:trimrows:factor1, coff:trimcols:factor2]
            if first:
                maxsofar = hopped
                first = False
            else:
                maxsofar = np.maximum(maxsofar, hopped)
    return maxsofar
