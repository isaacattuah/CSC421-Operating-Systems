#!/usr/bin/env python3
import time, math, argparse, sys


class PickSequence:

	def __init__(self):
		self.r = [ [3,1,0],[7,3,0],[11,7,0],[43,17,0]]
		self.k = [['ape',0], ['bob',0],['act',0],['cog',0],['dog',0],['cat',0],
				['fox',0],['gas',0],['fix',0],['dig',0],['egg',0],['rat',0],['rot',0],
				['gag',0],['gun',0],['hat',0],['jug',0],['cup',0],['run',0],['sun',0],
				['ton',0],['tap',0],['pup',0],['pet',0],['pop',0],['lap',0],['lip',0],
				['not',0],['nut',0],['nip',0],['off',0],['opt',0],['oar',0],['odd',0],
				['lug',0],['vap',0],['oil',0],['err',0],['ace',0],['are',0],['bun',0]
				]
		self.ri = 0
				
	def next_one(self):
		x = self.r[self.ri]
		self.ri = (self.ri+1) % len(self.r)
		x[2] = ((x[2]+x[1]) % x[0]) % len(self.k)
		self.k[x[2]][1] += 1
		return self.k[x[2]]
		
	def print_all(self):
		print('r: '+self.r)
		print('k: '+self.k)
		print('i: '+self.ri)
		
	def write_a_key(self,primary, stor_dir):
		s = '\techo '+primary[0]+"."+str(primary[1])+" > "+stor_dir+"/"+primary[0]
		return s
		
	def write_key_sequence(self, how_many, stor_dir):
		s = ''
		for i in range(how_many):
			s += self.write_a_key(self.next_one(),stor_dir) + '\n'
		return s
		
	def read_all_keys(self,stor_dir):
		s = ''
		for k in self.k:
			if k[1]!=0:
				s += '\t@echo "should say '+k[0]+"."+str(k[1])+'"\n'
				s += '\tcat '+stor_dir+"/"+k[0]+'\n'
		return s
		
	def test_recipe(self,n,stor_dir):
		s = """#
# generated makefile
#

test:
"""
		s += self.write_key_sequence(n,stor_dir)	
		s += self.read_all_keys(stor_dir)
		s +="""#
# END
#
"""
		return s	
		


def parse_args():
	global args_g
	parser = argparse.ArgumentParser(description="create makefile contest to test leveldb")
	parser.add_argument('number', help='number of keys to store')
	parser.add_argument('stor_dir', help="name of the storage directory")
	args_g = parser.parse_args()
	return args_g

		

def main(argv):
	global args_g
	args_g = parse_args()
	ps = PickSequence()
	print(ps.test_recipe(int(args_g.number),args_g.stor_dir))

main(sys.argv)
