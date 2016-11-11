import numpy as np
import sys
def main():
    if len(sys.argv) < 3:
        print 'Usage: concaternate_data.py <filename_1> <filename_2> <filename_3> ...'
        quit()
    dataset = []
    for filename in sys.argv[1:]:
        data = np.load(filename)
        dataset.append(data) 
    dataset = np.vstack(dataset)
    np.save('MAX_img_s1s2s3',dataset)
    print dataset.shape
        
    
if __name__ == '__main__':
    main()
