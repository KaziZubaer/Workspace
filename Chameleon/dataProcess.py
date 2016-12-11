inp = open("tabOld.txt","r")
farTest = 0
fa = 0
frrTest = 0
fr = 0
cnt = 0

while(True) :
	line = inp.readline()
	if(not line) :
		break
	if(len(line)>1) :
		line = line.split(" ")
		who = line[0]
		rej = False

		if (line[1] == "Accepted") :
			whom = line[3]
			if(who != whom[0:len(whom)-1]) :
				fa += 1
		else :
			rej = True
			
		line = inp.readline()
		line = line.split(" ")[2]
		line = line.split(",")
		if (len(line)>1) :
			farTest += 1
		line[len(line)-1] = line[len(line)-1][0:len(line[len(line)-1])-1]
		if (who in line) :
			frrTest += 1
			if (rej) :
				fr += 1		
				print(who,line)
		line = inp.readline()
		
print(fr,frrTest,fa,farTest);
