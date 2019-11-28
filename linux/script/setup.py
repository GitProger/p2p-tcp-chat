#!/usr/bin/env python3

def helpm():
    while True:
        print("Help? [y/n]", end="")
        c = input()
        if c == "y":
            os.system("bash man.sh")
            break
        elif c == "n":
            break

def getip():
    al = os.popen("ifconfig").read()
    beg = al.find("inet addr:192.168.")
    end = al.find("Bcast")
    if beg == -1 or end == -1:
        return ""
    beg += len("inet addr:")
    return al[beg:end]	

def main(args):
    helpm()
    os.system("clear")
    ip = getip()
    if ip == "":
        print("You are not connected to a local network.")
        exit(1)
    print("Port: ", end="")
    port = input()
    cmd = "sudo ../server/tcpserv " + ip + " " + port
    os.system(cmd)
    return 0

if __name__ == '__main__':
    import sys, os
    exit(main(sys.argv))
