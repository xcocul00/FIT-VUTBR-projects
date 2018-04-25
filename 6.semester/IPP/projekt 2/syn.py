#!/usr/local/bin/env python3

#SYN:xcocul00

import argparse
import re
import sys
import os
import io
from enum import Enum

# enum for error states
class Error_state(Enum):
	err_ok = 0 
	err_param = 1
	err_input = 2
	err_output = 3
	err_format = 4
	err_unknown = 101

class Check_values(Enum):
	dot = ["."]
	right_parenth_or = [".","|","("]
	star_plus = ["+","*","|",".","("]

# function that chcek validity of arguments and parse arguments
# input -> argv
# output -> parsed arguments
def parse_argument(argv):
	# array for chcek validity
	array_regex = []
	array_regex.append((r'^syn.py$'))
	array_regex.append((r'^--input=(.*)$'))
	array_regex.append((r'^--output=(.*)$'))
	array_regex.append((r'^--format=(.*)$'))
	array_regex.append((r'^--br$'))
	array_regex.append((r'^--help$'))

	#argparser settings
	parser=argparse.ArgumentParser(add_help=False)
	parser.add_argument('--input', nargs='?')
	parser.add_argument('--output', nargs='?')
	parser.add_argument('--format', nargs='?')
	parser.add_argument('--br', action='store_true', default=False)
	parser.add_argument('--help', action='store_true', default=False)

	tmp=[]
	option=[]
	for argument in argv:
		is_match = False
		for regex in array_regex:
			if(re.match(regex,argument)):
				if regex in tmp:
					exit_program("ERR: duplicity of arguments\n",Error_state.err_param.value)		
				else:
					tmp.append(regex)
				is_match=True
		if(not is_match):
			exit_program("ERR: Unknown argument\n",Error_state.err_param.value)
	
	if "--help" in argv:
		if (len(argv)==2):
			Help()
		else:
			exit_program("ERR: wrong --help argument\n",Error_state.err_param.value)

	try:
		arguments=parser.parse_args()
	except:
		exit_program("ERR: Unknown argument\n",Error_state.err_param.value)
	return arguments

# function that chcek validity of format file
# input -> format file
# output -> none
def check_format(format_text):
	text=format_text.split('\n')
	validity_regex = (r'^.*\t+((((\w|:)+,\s*)+(\w|:)+\s*)|((\w|:)+\s*))$')
	for element in text:
		if (element == None or element==""):
			continue
		if(not re.match(validity_regex,element)):
			exit_program("ERR: bad format file content\n",Error_state.err_format.value)
	return text
# 
# input -> 
# output ->
def check_next_letter(regex,pos,chars):

	if(pos >=2):
		if(regex[pos-2]=="%"):
			return 0
	for element in chars:
		if(regex[pos-1] == element):
			return 1
	return 0

# 
# input -> 
# output ->
def check_regex(regex):
	negation=""
	checked=""
	jump=False
	"""
	! -> 
	( -> 
	****
	. -> "."
	| -> ". | ("
	*****
	) -> ". | (" 
	* -> "+ | . ( *"
	+ -> "+ | . ( *"
	"""
	for letter_pos in range(len(regex)):
		if(jump):
			jump=False
			continue
		if (regex[letter_pos]=="." or regex[letter_pos]=="|"):
			if(negation or letter_pos==len(regex)-1 or letter_pos==0 ):
				exit_program("ERR: bad format of regex\n",Error_state.err_format.value) #sem
			if(regex[letter_pos]=="|" and check_next_letter(regex,letter_pos,Check_values.right_parenth_or.value)):
				exit_program("ERR: bad format after |\n",Error_state.err_format.value)
			if(regex[letter_pos]=="." and check_next_letter(regex,letter_pos,Check_values.dot.value)):
				exit_program("ERR: bad format after .\n",Error_state.err_format.value)
				continue
			if(regex[letter_pos]=="|"):
				checked+=regex[letter_pos]
		
		elif (regex[letter_pos]=="+" or regex[letter_pos]=="*" or regex[letter_pos]==")"):
			if(letter_pos==0 or negation):
			   exit_program("ERR: bad format of regex\n",Error_state.err_format.value)
			if((regex[letter_pos]=="+" or regex[letter_pos]=="*" ) and check_next_letter(regex,letter_pos,Check_values.star_plus.value)):
				exit_program("ERR: bad format after * or +\n",Error_state.err_format.value)
			if(regex[letter_pos]==")" and check_next_letter(regex,letter_pos,Check_values.right_parenth_or.value)):
				exit_program("ERR: bad format after )\n",Error_state.err_format.value)
			checked+=regex[letter_pos]
		
		elif (regex[letter_pos]=="!" or regex[letter_pos]=="("):
			if(negation or letter_pos==len(regex)-1):
				exit_program("ERR: bad format of regex\n",Error_state.err_format.value)	
			if(regex[letter_pos]=="!"):
				negation="^"
			else:
				checked+=regex[letter_pos]
		
		elif (regex[letter_pos]=="%"):
			if(letter_pos==len(regex)-1):
				exit_program("ERR: bad format of regex\n",Error_state.err_format.value)
			if(regex[letter_pos+1]=="s"):
				checked+="["+negation+"\s]"
			elif(regex[letter_pos+1]=="a"):
				checked+="["+negation+"\x00-\x7F]"
			elif(regex[letter_pos+1]=="d"):
				checked+="["+negation+"0-9]"
			elif(regex[letter_pos+1]=="l"):
				checked+="["+negation+"a-z]"
			elif(regex[letter_pos+1]=="L"):
				checked+="["+negation+"A-Z]"
			elif(regex[letter_pos+1]=="w"):
				checked+="["+negation+"a-zA-Z]"
			elif(regex[letter_pos+1]=="W"):
				checked+="["+negation+"a-zA-Z0-9]"
			elif(regex[letter_pos+1]=="t"):
				checked+="["+negation+"\t]"
			elif(regex[letter_pos+1]=="n"):
				checked+="["+negation+"\n]"
			elif(regex[letter_pos+1] in ".|!*+()%"):
				checked+="["+negation+"\\"+regex[letter_pos+1]+"]"
			else:
				exit_program("ERR: unknown symbol in regex\n",Error_state.err_format.value)
			negation=""
			jump=True
		else:
			if(negation):
				checked=checked+"[^"+regex[letter_pos]+"]"
				negation=""
			elif(regex[letter_pos] in "\^[]{}$?"):
				checked=checked+"["+negation+"\\"+regex[letter_pos]+"]"
				negation=""
			else:
				checked=checked+regex[letter_pos]
	return checked

# function that parse regex from format file
# input -> format file
# output -> array with regex
def parse_regex(format_text):
	regex_array=[]
	for element in format_text:
		if(not element):
			continue
		tmp=re.sub("\t+.*","",element)
		tmp=check_regex(tmp)
		regex_array.append(tmp)
	return regex_array

# function that parse regex from format file
# input -> format file
# output -> array with regex
def parse_option(format_text):
	option_array=[]
	for element in format_text:
		if(not element):
			continue
		#find \t+
		tmp = re.search("\t+",element)
		#split from \t+ to end of line
		tmp=element[tmp.end() : len(element)]
		tmp=re.split(",\s*",tmp)
		option_array.append(tmp)
	return option_array

# 
# input -> 
# output ->
def proced_syntax(text,regex,option):
	start=""
	end=""
	line= [""]* (len(text)+1)
	output=""
	for position in range(len(regex)):
		start=""
		end=""
		for element in option[position]:
			if (re.match("bold",element)):
				start=start+"<b>"
				end="</b>"+end
			elif (re.match("italic",element)):
				start=start+"<i>"
				end="</i>"+end
			elif (re.match("underline",element)):
				start=start+"<u>"
				end="</u>"+end
			elif (re.match("teletype",element)):
				start=start+"<tt>"
				end="</tt>"+end
			elif (re.match("size:[1-7]",element)):
				element=element.replace(":","=")
				tmp=re.search("<font color=#(\w){6}>",start)
				if(not tmp):
					start=start+"<font "+element+">"
					end="</font>"+end
				else:
					start=start[0 : tmp.end()-1]+" "+element+start[tmp.end()-1 : len(start)]
			elif (re.match("color:(\w){6}",element)):
				element=element.replace(":","=#")
				tmp=re.search("<font size:[1-7]>",start)	
				if(not tmp):
					start=start+"<font "+element+">"
					end="</font>"+end
				else:
					start=start[0 : tmp.end()-1]+" "+element+start[tmp.end()-1 : len(start)]	
			else:
				exit_program("ERR: unknown parameter\n",Error_state.err_format.value)

		for find in re.finditer(regex[position], text, re.DOTALL):
			if(find.start() != find.end()):
				line[find.start()] += start
				line[find.end()] = end+line[find.end()] 
	for pos in range(len(text)):
		output+=line[pos]+text[pos]
	if (line[len(text)] != ""):
		output+=line[len(text)]
	return output

# function that read from file
# input -> input file
# output -> text from file in variable
def read_file(name,flag):
	if(name == None or name == ""):
		text=""
		if(flag):
			for line in sys.stdin:
				text=text+line
	else:
		try:
			file = open(name, "r")
			text = file.read()
			file.close()
		except:
			exit_program("ERR: Cannot open file: "+name+"\n", Error_state.err_input.value)
	return text

# 
# input -> 
# output -> 
def write_file(name,text):
	try:
		file = open(name,"w")
		file.seek(0)
		file.truncate()
		file.write(text)
		file.close()
	except:
		exit_program("ERR: Cannot open file: "+name+"\n", Error_state.err_output.value)
# function that print help on screen
# input -> none
# output -> printed help
def Help():
	print("----------------------------------------------------\n"+
		  "Help for SYN :\n\n"+
		  "--input=<filename>	-> input text  file\n"+
		  "--output=<filename>	-> output file with result\n"+
		  "--format=<filename>	-> file with regular expresions\n"+
		  "--br 			-> put <br> tag on end of all lines\n"+
		  "--help 			-> print help\n"+ 
		  "----------------------------------------------------"
		  )
	exit_program("", Error_state.err_ok.value)

# function that end program and write error status and message
# input -> output message and error code
# output -> end of program
def exit_program(message,code):
	if(message!=""):
		sys.stderr.write(message)
	#print (code)
	sys.exit(code)

# main function
# input -> none
# output -> completed SYN 
def main(argv):
	input_file=""
	format_file=""
	output_file=""
	regex_array=[]
	option_array=[]

	arguments = parse_argument(argv)
	input_file=read_file(arguments.input,True)
	format_file=read_file(arguments.format,False)

	if (format_file):
		format_file=check_format(format_file)
		regex_array=parse_regex(format_file)
		option_array=parse_option(format_file)
		output_file=proced_syntax(input_file,regex_array,option_array)
	else:
		output_file=input_file

	if (arguments.br):
		output_file=output_file.replace("\n", "<br />\n")

	if (arguments.output=="" or arguments.output==None):
		sys.stdout.write(output_file)
	else:
		write_file(arguments.output,output_file)

	exit_program("", Error_state.err_ok.value)
if __name__ == '__main__':
	main(sys.argv)
