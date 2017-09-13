import os, glob, re, pprint, linecache, sys
from linecache import getline

arrPaths = []			 #We will use this array and fill it with each of the paths that lead to each suites post_route_timing_summary.txt

def extract(comm_path):
	pather = comm_path + "/*.yml"	
	#comm_path = '/proj/fisdata/fisusr/no_backup/ABELite/Results/MT8_VIVADO_SSI_SPR_DBLO_SPD3/' #This is the directory which holds all the yml files
	dirList = os.chdir(comm_path) #open this path
	#ymlFiles = glob.glob('/proj/fisdata/fisusr/no_backup/ABELite/Results/MT8_VIVADO_SSI_SPR_DBLO_SPD3/*.yml') ##Extract and Save all yml files in MT8 as a list!
	ymlFiles = glob.glob(pather) ##Extract and Save all yml files in MT8 as a list!
	string = "work_dir_path" #represent the work_dir_paths we need to extract from each of the .yml files

	for each_file in ymlFiles:
		with open(each_file) as f:	#open each of the yml files in f
			for line in f:
				line = line.rstrip()
				if string in line: #if we see "work_dir_path" we will extract the following path from that line as that path will lead us to the summary.txt for each model
					line=re.sub('^[ \t]+', "", line)
					rpaths = re.split(" ", line)
					#print rpaths
					#arrPaths.extend(line[27:].split())	#store each of the paths into a list. Easily fed
					#arrPaths.extend(line.split("work_dir_path:"))
					arrPaths.append(rpaths[1])
	#print arrPaths
tknp1 = []
tknp2 = []

def filtered(token1, token2):	
	res1 = [k for k in arrPaths if token1 in k]
	#pprint.pprint(res1)
	#pprint.pprint(res1) #this will give you all paths that have the token are looking for
	for x in sorted(res1):
		#print x
		#tknp1.extend(x.split())
		tknp1.append(x)
		#print(tknp1)	#ordered paths

	res2 = [l for l in arrPaths if token2 in l]
	#pprint.pprint(res2)	#this will do the same thing for token 2
	for y in sorted(res2):
		#tknp2.extend(y.split()) #ordered paths
		tknp2.append(y) #ordered paths
		#print(tknp2)

	#print "filtered", len(tknp1)
	#print tknp1
	#print "filtered", len(tknp1)
	#print tknp2
compPath1 = []
cpSort1 = []
compPath2 = []
cpSort2 = []


def getPathsFull():
	for i in tknp1:
		path = i
		fpath = os.path.join(path, "post_route_timing_summary.txt") #compute the files pathath.join(path, "/post_route_timing_summary.txt")
		#print fpath
		if os.path.exists(fpath):
			#compPath1.extend(fpath.split())
			compPath1.append(fpath)
	#for x in sorted(compPath1):
		#print x
		#cpSort1.append(x.split("2017")[0])
	#pprint.pprint(cpSort1)

	#print "\n"

	for j in tknp2:
		path2 = j
		fpath2 = os.path.join(path2, "post_route_timing_summary.txt") #compute the files pathath.join(path, "/post_route_timing_summary.txt")
		if os.path.exists(fpath2):				
			#compPath2.extend(fpath2.split())
			compPath2.append(fpath2)

	#for y in sorted(compPath2):
		#cpSort2.append(y.split("2017")[0])
		#sliceComp2.extend(y[:95].split())
	#pprint.pprint(cpSort2)


def parseFile(fname):
	f = open(fname, "r");
	summary_section = 0
	read_next_line  = 0
	retVal = []
	for line in f:
		line = line.rstrip()
		if re.search("Design Timing Summary", line):
			summary_section = 1
			#print line
			continue
		if summary_section:
			if re.search("WNS", line):
				#print line
				read_next_line = 1;
				continue
		if read_next_line:
			if re.search("-----", line):
				continue;
			line = re.sub("[ \t]+", " ", line)
			line = re.sub("^ +", "", line)
			#print line
			l = re.split(" ", line);
			print l
			retVal = [float(l[8]), float(l[9])]
			summary_section = 0
			read_next_line = 0
			break;
	f.close()
	return retVal

arrM = []
def metFirst(path):
	fpath = os.path.join(path) #compute the files path
	#f = open(path) #open the file here
	#content = f.readlines() #read the contents of the file
	#print content
	#pprint.pprint(content)
	with open(fpath) as f:
		for ind, line in enumerate(f,1):
		    if line.rstrip() == "| Design Timing Summary":
		        print(getline(f.name, ind + 6))
		
	#Create a list with contents of line 130
	#arrMetrics = []
	#for i in range(129,130):
		#arrMetrics.append(content[i].splitlines())
		#arrMetrics.extend(content[i].split())
		#print content[i]
		#print(arrMetrics)
	#print "Your Metrics Are: "
	#print arrMetrics
	#print "..............................................\n"
'''
	global TPWS_Failing_Endpoints
	TPWS_Failing_Endpoints = arrMetrics[2]
	
	global TPWS_Total_Endpoints
	TPWS_Total_Endpoints = arrMetrics[3]
	
	global WPWS
	WPWS = arrMetrics[8]
	
	global TPWS
	TPWS = arrMetrics[9]
'''
'''
	print "TPWS_Failing_Endpoints value is : ", TPWS_Failing_Endpoints
	print ".............................................."
	print "TPWS_Total_Endpoints value is : ", TPWS_Total_Endpoints
	print ".............................................."
	print "WPWS value is : ", WPWS
	print ".............................................."
	print "TPWS value is : ", TPWS
	print ".............................................."
'''
def results(v1, v2):
	print "\n"
	print "RESULTS AND ANALYSIS:"
	print "\n"
	if v2[0] < v1[0]:
		print "Your WPWS value DECREASED :(\n"
	if v2[0] == v1[0]:
		print "No change in values\n"
	if v2[0] > v1[0]:
		print "Your WPWS value INCREASED :)\n"

	if v2[1] < v1[1]:
		print "Your TPWS value DECREASED :(\n"
	if v2[1] == v1[1]:
		print "No change in values\n"
	if v2[1] > v1[1]:
		print "Your TPWS value INCREASED :)\n"

	
feeder1 = []
feeder2 = []

def matcher():
	#print token1
	#print "in matcher"
	for i in compPath1:
		ii = i.split(token1)[0]
		#print i
		#print ii
		for j in compPath2:
			jj = j.split(token2)[0]
			#print j
			if ii in jj:
				feeder1.append(i)
				feeder2.append(j)

	for i in range(0, len(feeder1)):
		print feeder1[i]
		v1 = parseFile(feeder1[i])
		print v1
		print feeder2[i]
		v2 = parseFile(feeder2[i])
		print v2
		results(v1, v2)

comm_path = sys.argv[1]
token1 = sys.argv[2]
token2 = sys.argv[3]
extract(comm_path)
filtered(token1, token2)
getPathsFull()
matcher()


