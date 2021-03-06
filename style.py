#!/bin/python3

import sys
import re

class StyleValidation():
	def __init__(self, filename):
		self._style_warnings = []
		self._filename = filename
		self._line_number = 0
		
		extension = filename.split('.')[-1]
		self._set_validators(extension)
		self._validate_file()
	
	def _set_validators(self, extension):
		if extension in ['h', 'cpp']:
			self._validators = [LengthValidator(), IndentationValidator(False),
			                    WhitespaceValidator(), CppSequenceValidator()]
		elif extension == 'sh':
			self._validators = [LengthValidator(), IndentationValidator(False),
			                    WhitespaceValidator()]
		elif extension == 'py':
			self._validators = [LengthValidator(), IndentationValidator(True),
			                    WhitespaceValidator()]
		else:
			self._validators = []
	
	def _validate_file(self):
		if len(self._validators) == 0:
			return
		
		with open(self._filename, 'r', newline = '') as f:
			for line in f:
				self._line_number += 1
				for validator in self._validators:
					warnings = validator.validate(line)
					for warning in warnings:
						self._add_style_warning(warning)
		
		if self._line_number > 200:
			self._add_style_warning('File is ' + str(self._line_number) +
			                        ' lines long')
	
	def _add_style_warning(self, message):
		warning = self._filename + ':' + str(self._line_number)
		warning += '  ' + message
		self._style_warnings.append(warning)
	
	def get_warnings(self):
		return self._style_warnings

class LengthValidator():
	def __init___(self):
		pass
	
	def validate(self, line):
		fixed_length_line = line.replace('\t', '    ')
		if len(fixed_length_line) > 80:
			return ['Line is ' + str(len(fixed_length_line)) +
			        ' characters long']
		else:
			return []

class IndentationValidator():
	def __init__(self, allow_large_indent_decreases):
		self._indentation_level = 0
		self._allow_large_indent_decreases = allow_large_indent_decreases
	
	def validate(self, line):
		current_indentation_level = 0
		only_tabs_seen = True
		warnings = []
		
		for character in line:
			if character == '\t':
				current_indentation_level += 1
				if not only_tabs_seen:
					warnings.append('Inconsistent indent whitespace')
			elif character == ' ':
				only_tabs_seen = False
			else:
				break
		
		if ((current_indentation_level > (self._indentation_level + 1)) or
		    ((current_indentation_level < (self._indentation_level - 1)) and
		     not self._allow_large_indent_decreases)):
			warnings.append('Discontinuous indentation')
		
		self._indentation_level = current_indentation_level
		return warnings

class WhitespaceValidator():
	def __init__(self):
		pass
	
	def validate(self, line):
		warnings = []
		
		if '\r' in line:
			warnings.append('Carriage return found')
		if (len(line) > 1) and (line.strip('\n')[-1] == ' '):
			warnings.append('Trailing space')
		if line[-1] != '\n':
			warnings.append('No trailing newline')
		if '\t' in line.strip():
			warnings.append('Indentation after start of line')
		
		return warnings

class CppSequenceValidator():
	def __init__(self):
		self._parenthesis_spacing_pattern = re.compile(r'\w \(')
	
	def validate(self, line):
		warnings = []
		
		if '){' in line:
			warnings.append('Inconsistent brace spacing')
		if (self._parenthesis_spacing_pattern.search(line) and
		    'return' not in line):
			warnings.append('Inconsistent parenthesis spacing')
		
		return warnings

if __name__ == '__main__':
	files = [line.strip() for line in sys.stdin] + sys.argv[1:]
	total_warnings = 0
	
	print('Validating style in ' + str(len(files)) + ' files...')
	for source_file in files:
		warnings = StyleValidation(source_file).get_warnings()
		if len(warnings) > 0:
			print()
			print('\n'.join(warnings))
			total_warnings += len(warnings)
	sys.exit(total_warnings)
