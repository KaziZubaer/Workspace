inp = open("id3Data.csv","r")
oup = open("wekaData.csv","w")

line = inp.readline()
oup.write(line)

while (True) :
	line = inp.readline()
	if (not line) :
		break
	if (int(line[len(line)-2]) == 0) :
		line = line[0:len(line)-2]
		line = line + "Benine\n"
	else :
		line = line[0:len(line)-2]
		line = line + "Malignant\n"
	oup.write(line)
