from pandas import DataFrame
import numpy
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.naive_bayes import MultinomialNB

MAX_TEST_PER_TOPIC = 100

topics = []
docCnt = 0

topicF = open("Data/topics.txt","r")

while (True) :
	tp = topicF.readline()
	if (not tp) :
		break
	tp = tp[:-1]
	topics.append(tp)

rows = []
index = []

for tp in topics:
	trainDoc = open("Data/Training/"+tp+".xml","r")
	while (True) :
		sentence = trainDoc.readline()
		if (not sentence) :
			break
		if("Body=\"" not in sentence) :
			continue
		sentence = sentence.split("Body=\"");
		sentence = sentence[1]
		
		content = ""
		
		ended = False
		while(True) :
			if("\" OwnerUserId" in sentence) :
				sentence = sentence.split("\" OwnerUserId")
				sentence = sentence[0]
				ended = True			
				
			content += sentence
			if(ended) :
				break
			sentence = trainDoc.readline()
		rows.append({'text': content, 'class': tp})
		index.append(docCnt)
		docCnt += 1

data = DataFrame(rows, index=index)
data = data.reindex(numpy.random.permutation(data.index))

countVectorizer = CountVectorizer()
counts = countVectorizer.fit_transform(data['text'].values)

classifier = MultinomialNB()
targets = data['class'].values
classifier.fit(counts, targets)

testContent = []
testTrueResult = []

for tp in topics:
	testDoc = open("Data/Test/"+tp+".xml","r")
	testCnt = 0
	while (testCnt < MAX_TEST_PER_TOPIC) :
		sentence = testDoc.readline()
		if (not sentence) :
			break
		if("Body=\"" not in sentence) :
			continue
		sentence = sentence.split("Body=\"");
		sentence = sentence[1]
		
		content = ""
		
		ended = False
		while(True) :
			if("\" OwnerUserId" in sentence) :
				sentence = sentence.split("\" OwnerUserId")
				sentence = sentence[0]
				ended = True			
				
			content += sentence
			if(ended) :
				break
			sentence = trainDoc.readline()
		testContent.append(content)
		testTrueResult.append(tp)
		testCnt += 1

testCounts = countVectorizer.transform(testContent)
predictions = classifier.predict(testCounts)

accuracy = 0
for i in range(0,len(testTrueResult)) :
	if(predictions[i] == testTrueResult[i]) :
		accuracy += 1

accuracy *= 100
accuracy /= len(testTrueResult)

print("Accuracy = "+str(accuracy)+"%")

