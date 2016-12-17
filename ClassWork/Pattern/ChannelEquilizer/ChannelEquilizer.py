import math
import random
import scipy.stats

TRAIN_CNT = 20
TEST_CNT = 1000

#def getProbability (x, avg, var) :
#	pr = (-1)*(x-avg)*(x-avg)
#	pr /= 2*var
	

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
		for i in range(self.k, TRAIN_CNT) :
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
			dp[0].append(scipy.stats.norm(self.avg[i], self.nVar).pdf(given[0]))
			preState[0].append(-1)
		for i in range(0, TEST_CNT) :
			state = state>>1
			if given[i] :
				state = state|(1<<(self.k-1))
			x = self.calculateChannelOut(state, min(i+1,self.k))
			for j in range(0,1<<self.k) :
				dp[1][j] *= scipy.stats.norm(self.avg[j], self.nVar).pdf(x)
				zeroState = ((1<<(self.k))-1)&(j<<1)
				oneState = zeroState+1
				if dp[0][zeroState]>dp[0][oneState] :
					dp[1][j] *= dp[0][zeroState]
					preState[i].append(zeroState)
				else :
					dp[1][j] *= dp[0][oneState]
					preState[i].append(oneState)
			for j in range(0,1<<self.k) :
				dp[0][j] = dp[1][j]
		result = []
		state = 0
		maxi = dp[0][0]
		for i in range(1, 1<<self.k) :
			if  dp[0][i]>maxi :
				maxi = dp[0][i]
				state = i
		if state&(1<<(self.k-1)) :
			result.append(True)
		else :
			result.append(False)
		state = preState[TEST_CNT-1][state]
		for i in range(TEST_CNT-2, -1, -1) :
			if state&(1<<(self.k-1)) :
				result.append(True)
			else :
				result.append(False)
		result.reverse()
		return result

#take input
#nVar = float(input())
nVar = 1.0
#k = int(input())
k = 3
h = []
for i in range(0,k) :
#	x = float(input())
	h.append(0.5)
#	h.append(x)
	
#create channel and train
channel = Channel(h, nVar)
channel.train()

generate test and test
bits = []
for i in range(0, TEST_CNT) :
	if random.random()>0.5 :
		bits.append(True)
	else :
		bits.append(False)
state = 0
for i in range(0, k-1) :
	if bits[i] :
		state = state|(1<<i)
state = state<<1
for i in range(k, TEST_CNT) :
	state = state>>1
	if bits[i] :
		state = state|(1<<(k-1))
	

obtained = channel.test(given)

#summarize result
#error = 0
#for i in range(0, TEST_CNT) :
#	if obtained[i] != given[i] :
#		error += 1
		
#print(error*100/TEST_CNT)
