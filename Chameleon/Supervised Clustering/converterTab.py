cnt = {}
idx = {}
trainOutBuffer = []
testOutBuffer = []
names = ['Azmi', 'Farin', 'Mousumi', 'Shejuti', 'Punam']

inp = open("chameleon_data1_Tab.txt","r")
trainOut = open("tabTrain.txt","w")
testOut = open("tabTest.txt","w")
personCnt = 0

while (True) :
	line = inp.readline()
	if (not line) :
		break
		
	parts = line.split(',')
	name = parts[13][0:len(parts[13])-1]
	
	if (name in cnt and cnt[name] == 10) :
		testOutBuffer[idx[name]].append(line)
	else :
		if (name in cnt) :
			cnt[name] = cnt[name] + 1
		else :
			cnt[name] = 1
			idx[name] = personCnt
			testOutBuffer.append(list())
			trainOutBuffer.append(list())
			personCnt += 1
		trainOut.write(line)
		trainOutBuffer[idx[name]].append(line)

for i in range(0,personCnt) :
	for j in trainOutBuffer[i] :
		parts = j.split(',')
		line = ""
		for k in range(0,13) :
			line += parts[k]+","
		line += names[i]+"\n"
		trainOut.write(line)
		
for i in range(0,personCnt) :
	for j in testOutBuffer[i] :
		parts = j.split(',')
		line = ""
		for k in range(0,13) :
			line += parts[k]+","
		line += str(i+1)+"\n"
		testOut.write(line)

for i in range(0,personCnt) :
	for j in testOutBuffer[i] :
		parts = j.split(',')
		line = ""
		for k in range(0,13) :
			line += parts[k]+","
		line += str(i+6)+"\n"
		testOut.write(line)
