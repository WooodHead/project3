import sys, getopt

def main(argv):
    
    #default name of the input file
    inputfile = 'squares.txt'
    printsol= False
    opts, args = getopt.getopt(argv,"hi:p")
    for opt, arg in opts:
      if opt == '-h':
         print 'test.py -i <inputfile> [-p]'
         sys.exit()
      elif opt in ("-i"):
         inputfile = arg
      elif opt == "-p":
          printsol = True
      
#    print 'Input file is', inputfile

    with open(inputfile, 'r') as f:
        n = int(f.readline().lstrip("n: "))
        xLine = f.readline()
        yLine = f.readline()
        s = int(f.readline().lstrip("s: "))

    xStrings = xLine[4:len(xLine)-2].split(", ")
    x = [int(x) for x in xStrings]
    
    yStrings = yLine[4:len(yLine)-2].split(", ")
    y = [int(y) for y in yStrings]

    matrix = [[0] * s for i in range(s)]
    overlap = [[0] * s for i in range(s)]

    if len(x)!=len(y):
        print "FAIL : Size of vectors mismatch"
        sys.exit()

    opts=[0,1,3,5,7,9,11,13,15,18,21,24,27,30,33,36,39,43,47,50,54,58, 62, 66, 71, 75, 80, 84]

    if (s!=opts[n]):
        print "WARNING! Suboptimal solution"

    L = len(x)
    skip = n-L

#    if skip==1:
 #       print "Warning : To few squares. Ignore if skipping 1x1."

 #   elif skip==2:
  #      print "Warning : To few squares. Ignore if skipping more than 1x1 and 2x2."

    if skip>2:
        print "FAIL : To few squares."
        sys.exit()

    if L > n:
            print "FAIL : To many squares."
            sys.exit()
    
    for square in range(L):
        if x[square]+(n-square)>s or y[square]+(n-square)>s or x[square]<0 or y[square]<0:
            print "FAIL : Out of bounds"
            print "square: " + str(square)
            print "x[" +str(square) +"]: "  + str(x[square])
            print "y[" +str(square) +"]: "  + str(y[square])
            print "s: " + str(s)

        for xc in range(x[square], x[square]+(n-square)):
            for yc in range(y[square], y[square]+(n-square)):
                if matrix[xc][yc]>0:
                    overlap[xc][yc]+=1
                matrix[xc][yc]=n-square


    skipfail=False
    if skip==2:
        skipfail=True
    
        for xc in range(s-skip+1):
            for yc in range(s-skip+1):
                if matrix[xc][yc]==0:
                    foundspace=True
                    for i in range(skip):
                        for j in range(skip):
                            if matrix[xc+i][yc+j]!=0:
                                foundspace=False
                    if foundspace:
                        for i in range(skip):
                            for j in range(skip):
                                matrix[xc+i][yc+j]=skip
                        skipfail=False
                        break
            else:
                continue
            skip=1
            break

    if skipfail:
        print "FAIL : No room for skipped squares."
        sys.exit()


    if skip==1:
        skipfail=True
    
        for xc in range(s):
            for yc in range(s):
                if matrix[xc][yc]==0:
                    matrix[xc][yc]=1
                    skipfail=False
                    break
            else:
                continue
            break

    if skipfail:
        print "FAIL : No room for skipped squares."
        sys.exit()


    if printsol:
        for row in matrix:
            print row
        print
                
    for xc in range(s):
        for yc in range(s):
            if overlap[xc][yc]>0:
                print "FAIL : Overlap"
                for row in overlap:
                    print row
                print
                for row in matrix:
                    print row
                print
                sys.exit()
     
    print "Ok!"

if __name__ == "__main__":
    main(sys.argv[1:])
    
