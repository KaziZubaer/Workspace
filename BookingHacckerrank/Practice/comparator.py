out = open("out.txt","r")
exp = open("exp.txt","r")

cnt = 0

while (True) :
	lineOut = out.readline()
	lineExp = exp.readline()
	
	if(not lineOut) :
		break
		
	print(cnt)
	print(lineExp)
	print("\n")
	print(lineOut)
	
	cnt += 1
	
	if(lineExp != lineOut) :
		break
