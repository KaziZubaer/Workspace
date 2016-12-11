from sklearn import tree

TRAIN_CNT = 530
TEST_CNT = 139

dataFile = open("Data/id3Data.csv","r")

line = dataFile.readline()

X = []
Y = []
i = 0

while (i < TRAIN_CNT) :
	line = dataFile.readline()
	line = line.split(",")
	temp = []
	for j in range (0,8) :
		temp.append(int(line[j]))
	X.append(temp)
	Y.append(int(line[8]))
	i += 1

classifier = tree.DecisionTreeClassifier()
classifier = classifier.fit(X, Y)

testData = []
trueResult = []
i = 0

while (i < TEST_CNT) :
	line = dataFile.readline()
	line = line.split(",")
	temp = []
	for j in range (0,8) :
		temp.append(int(line[j]))
	testData.append(temp)
	trueResult.append(int(line[8]))
	i += 1

predictions = classifier.predict(testData)

accuracy = 0
for i in range(0,len(trueResult)) :
	if(predictions[i] == trueResult[i]) :
		accuracy += 1

accuracy *= 100
accuracy /= len(trueResult)

print("Accuracy = "+str(accuracy)+"%")
