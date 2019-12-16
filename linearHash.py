import sys

bucket_cnt = 2

#
b = 2

#New hash function, it get updated as the original hash function gets updated
bNext = 4 
total_block_cnt = 2
block_cnt = {}
p = 0

# Total number of records.
totalNumbers = 0

#hashTable stores buckets, that are collection of blocks, blocks having size B
hashTable = {}
block_cnt[0] = 1
block_cnt[1] = 1
outBuff = []
fo = open('output.txt','w')

# print(M,B)

inpBuff = []
outBuff = []

def getGlobalVars():
	global total_block_cnt
	global totalNumbers
	global B
	global totalNumbers
	global outBuff
	global b
	global bucket_cnt
	global p
	global bNext

	return [total_block_cnt,totalNumbers,B,totalNumbers,outBuff,bucket_cnt,p,bNext]

# Function to check whether bucket is filled.
def full():
	arr_global = getGlobalVars()
	global total_block_cnt
	global totalNumbers
	global B
	sizNumbers = totalNumbers*4.0
	blockNumbers = B*total_block_cnt
	ratio = (sizNumbers/blockNumbers)*100.0
	return (0 if ratio <= 75.0 else 1)

def mul2(a,b):
	a *= b
	return (a)

def equal(a,b):
	return (a==b)

# Function to insert number to hashTable
def inserting(number):
	rem = 0
	global total_block_cnt
	global totalNumbers
	global outBuff
	found = 0

	if (number % b) < p:
		rem = number%bNext
	else:
		rem = number % b
	if rem not in hashTable:
		hashTable[rem] = []
		hashTable[rem].append([])

	for i in range(block_cnt[rem]):
		found = (1 if number in hashTable[rem][i] else found)

	if(found == 0):
		tmp = block_cnt[rem] - 1
		if len(hashTable[rem][tmp]) >= (B/4.0):
			lst = []
			total_block_cnt += 1
			tmp += 1
			block_cnt[rem] += 1
			hashTable[rem].append(lst)
		totalNumbers += 1
		hashTable[rem][tmp].append(number)
		outBuff.append(number)

		if len(outBuff) >= (B/4.0):
			for i in outBuff:
				fo.write(str(i)+'\n')
			outBuff = []
	if full():
		newBuckets()		

# Function for adding new buckets and rehashing numbers.
def newBuckets():
	global b
	global bucket_cnt
	global p
	global total_block_cnt
	global bNext

	bucket_cnt += 1
	lst = []

	# Temporary array made to rehash those values that are going to in full bucket.
	tmp_arr = [num for i in range(block_cnt[p]) for num in hashTable[p][i]]
	total_block_cnt -= block_cnt[p]
	hashTable[p] = []
	hashTable[p].append(lst)
	block_cnt[p] = 1
	total_block_cnt += 1

	hashTable[bucket_cnt - 1] = []
	hashTable[bucket_cnt - 1].append(lst)
	block_cnt[bucket_cnt - 1] = 1
	total_block_cnt += 1
	found = 0

	for num in tmp_arr:
		found = 0
		rem = num % bNext
		if rem in hashTable:
			o = 1
		elif rem not in hashTable:
			total_block_cnt += 1
			lst = []
			hashTable[rem] = []
			hashTable[rem].append(lst)
			block_cnt[rem] = 1

		for j in range(block_cnt[rem]):
			found = (1 if num in hashTable[rem][j] else found)

		if(found == 0):
			tmp = block_cnt[rem] - 1
			if len(hashTable[rem][tmp]) >= (B/4.0):
				lst = []
				total_block_cnt += 1
				tmp += 1
				block_cnt[rem] += 1
				hashTable[rem].append(lst)
			hashTable[rem][tmp].append(num)
	p += 1
	if equal(bucket_cnt,bNext):
		b = mul2(b,2)
		bNext = mul2(b,2)
		p = 0
	return 1

if __name__ == "__main__":

	if len(sys.argv) != 4:
		print("Incomplete or parameters not passed correctly")
		print("Format is [File Name] M B")
		exit(-1)

	filename = sys.argv[1]

	# M is number of buffers, (M-1)input buffers and 1 output buffer
	M = int(sys.argv[2])
	# B is buffer size, or bucket size.
	B = int(sys.argv[3])
	B = 4 if B<=4 else B #Minimum block size is 4.

	M = 2 if M<=2 else M # already given M>=2.
	f = open(filename,'r')

	#Reading all lines in array and then processing it.
	arr = f.readlines()
	for i in arr:
		line = i.strip()
		number = int(line)
		inpBuff.append(number)
		# Processing only when input buffer gets filled
		if len(inpBuff) >= (((M-1)*B*0.25)):
			for num in inpBuff:
				inserting(num)
				# print(num)
			inpBuff = []

	# If last iteration doesn't fill inpBuffer with enough elements, then below loop to get that.
	for num in inpBuff:
		inserting(num)

	# Writing all the things from output buffer to file.
	for i in outBuff:
		fo.write(str(i)+'\n')
	fo.close()