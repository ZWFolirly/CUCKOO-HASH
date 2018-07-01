import numpy as np

np.random.seed(1)

mac = 48
port = 4
address = 4 * 10**7 #生成个数
repeated = 1 #重复次数
filename = "F:\\mac"+str(mac)+"_port"+str(port)+"_address"+str(address)+"_repeated"+str(repeated)+".txt" #生成的文件
if __name__ == "__main__":
    with open(filename, "w") as fo:
        fo.write(str(address*repeated)+"\n") #文件第一行 数据总数
        for t in range(repeated):
            np.random.seed(1)
            for i in range(address):
                ppp = np.random.rand()
                tmac = int(ppp * 2**mac)
                if ppp < 0.02: #2%的概率生成mac相同但是port不同的数据，用以测试update操作
                    ppp = np.random.rand()
                tport = int(ppp * 2 ** port)
                fo.write(str(tmac)+" "+str(tport)+"\n")
                if(i % address/10 == 0):
                    print(i)




