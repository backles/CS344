__author__='Braden Ackles'
__assignment__='#5'
__class__='CS344'

import random
import string

def main():
   	fileNumber=1
	while fileNumber <= 3:						#For three files
   		stringIs=randomString(10)				#make a random string
		print "This is the string generated: %s" % stringIs	#print out the random string generated
		fileMaker(stringIs, fileNumber)				#call the function to write the string to the file
		fileNumber=fileNumber + 1				#Go to the next file
	numOne = randomNum()						#make a random number
	numTwo =randomNum()						#make a random number
	added = numOne * numTwo						#add the two
	print "The when you multiply the two numbers you get %d" % added	#Print the added number

def randomNum():
	Num=random.randrange(1,42)					#get a random number
	print Num							#print the random number
	return Num							#return that number
	
def randomString(length):
	return ''.join(random.choice(string.lowercase) for i in range(length))	#make A random string

def fileMaker(string, fileNum):
   	f = open('File%s' %fileNum, 'w+')					#Open the string to the file
	f.write('%s' %string)							#Write the string to the file
	f.close()								#close the file
main()
