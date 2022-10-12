f = open("NTSCcolors.txt", "r")
for i in range(0, 16):
    os = "{"
    for i in range(0,8):
        n = f.readline()
        r = int(n[5:7], 16)
        g = int(n[7:9], 16)
        b = int(n[9:11], 16)
        s=f"{{{r}, {g}, {b}}}"
        os = os+s+", "
    os+="},\n"
    print(os)
        
