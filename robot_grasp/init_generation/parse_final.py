num_config = 710

def main():
    fh = open('final.txt','r')
    fh_out = open('final_config.csv','w')
    data = fh.readlines()
    line_num = 0
    for line in data:
        line_t = line_num % 4
        coords = line.split(' ')
        out_line = coords[0]+','+coords[1]+','+coords[2]+','
        if line_t == 3:
            out_line = coords[0]+','+coords[1]+','+coords[2]+'\n'
        fh_out.write(out_line)
        line_num += 1
    fh.close()
    fh_out.close()
if __name__ == '__main__':
    main()
    
