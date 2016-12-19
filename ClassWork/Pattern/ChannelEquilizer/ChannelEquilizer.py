import math
import random
import scipy.stats

TRAIN_CNT = 10000
TEST_CNT = 100
f = open("input.txt","r")

class Channel :
	
	def __init__ (self,_h,_nVar) :
		self.k = len(_h)
		self.h = []
		self.avg = []
		self.nVar = _nVar
		for i in _h :
			self.h.append(i)
    
	def calculateChannelOut (self, state) :
		result = random.gauss(0, self.nVar)
		for i in range(0, k) :
			if state & (1<<i) > 0 :
				result += self.h[i]
			else :
				result -= self.h[i]
		return result

	def train(self) :
		state = 0
		cnt = []
		for i in range(0, (1<<self.k)) :
			self.avg.append(0)
			cnt.append(0)
		for i in range(0, self.k-1) :
			if random.random()>0.5 :
				state = state|(1<<i)	
		state = state<<1
		for i in range(self.k-1, TRAIN_CNT) :
			state = state>>1
			if random.random()>0.5 :
				state = state|(1<<(self.k-1))
			self.avg[state] += self.calculateChannelOut(state)
			cnt[state] += 1
		for i in range(0, 1<<self.k) :
			if cnt[i]>0 :
				self.avg[i] /= cnt[i]

	def test(self, given) :
		state = 0
		preState = []
		dp = []
		dp.append([])
		dp.append([])
		for i in range(0, TEST_CNT-self.k+1) :
			preState.append([])
		for i in range(0, 1<<self.k) :
			dp[0].append(scipy.stats.norm(self.avg[i], 1).pdf(given[0]))
			dp[1].append(0)
			preState[0].append(-1)
		for i in range(1, TEST_CNT-self.k+1) :
			for j in range(0,1<<self.k) :
				preState[i].append(-1)
			for j in range(0,1<<self.k) :
				state = j>>1
				tempVal = scipy.stats.norm(self.avg[state], 1).pdf(given[i])*dp[0][j]
				if(preState[i][state] == -1 or tempVal > dp[1][state]) :
					dp[1][state] = tempVal
					preState[i][state] = j
				state = state|(1<<(self.k-1))
				tempVal = scipy.stats.norm(self.avg[state], 1).pdf(given[i])*dp[0][j]
				if(preState[i][state] == -1 or tempVal > dp[1][state]) :
					dp[1][state] = tempVal
					preState[i][state] = j
			for j in range(0,1<<self.k) :
				dp[0][j] = dp[1][j]
		result = []
		state = 0
		maxi = dp[0][0]
		for i in range(1, 1<<self.k) :
			if  dp[0][i] > maxi :
				maxi = dp[0][i]
				state = i
		if state&(1<<(self.k-1)) > 0 :
			result.append(True)
		else :
			result.append(False)
		for i in range(TEST_CNT-self.k, 0, -1) :
			state = preState[i][state]
			if state&(1<<(self.k-1)) > 0 :
				result.append(True)
			else :
				result.append(False)
		for i in range(k-2, -1, -1) :
			if state&(1<<i) > 0 :
				result.append(True)
			else :
				result.append(False)
		result.reverse()
		return result
		
#take input
nVar = float(f.readline())
k = int(f.readline())
h = f.readline().split(" ")
for i in range(0,k) :
	h[i] = float(h[i])
	
#create channel and train
channel = Channel(h, nVar)
channel.train()

#generate test and test
state = 0
bits = []
for i in range(0, k-1) :
	if random.random()>0.5 :
		state = state|(1<<i)
		bits.append(True)
	else :
		bits.append(False)
state = state<<1
given = []
for i in range(k-1, TEST_CNT) :
	state = state>>1
	if random.random()>0.5 :
		state = state|(1<<(k-1))
		bits.append(True)
	else :
		state = state&((1<<(k-1))-1)
		bits.append(False)
	given.append(channel.calculateChannelOut(state))
obtained = channel.test(given)

#summarize result
error = 0
for i in range(0, TEST_CNT) :
	if obtained[i] != bits[i] :
		error += 1
		
print("Error = " + str(error*100/TEST_CNT) + "%")
