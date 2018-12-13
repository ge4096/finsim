#!/bin/python3

import sys
import glob

class StyleValidator():
	def __init__(self, filename):
		self._style_warnings = []
		self._filename = filename
		self._line_number = 0
		self._indentation_level = 0
		with open(filename, 'r') as f:
			for line in f:
				self._line_number += 1
				self._validate_length(line)
				self._validate_indentation(line)
				self._validate_invalid_sequences(line)
		if self._line_number > 200:
			self._add_style_warning('File is ' + str(self._line_number) +
			                        ' lines long')
	
	def _add_style_warning(self, message):
		warning = self._filename + ':' + str(self._line_number)
		warning += '  ' + message
		self._style_warnings.append(warning)
	
	def _validate_length(self, line):
		fixed_length_line = line.replace('\t', '    ')
		if len(fixed_length_line) > 80:
			self._add_style_warning('Line is ' + str(len(fixed_length_line)) +
			                        ' characters long')
	
	def _validate_indentation(self, line):
		current_indentation_level = 0
		only_tabs_seen = True
		for character in line:
			if character == '\t':
				current_indentation_level += 1
				if not only_tabs_seen:
					self._add_style_warning('Inconsistent indent whitespace')
			elif character == ' ':
				only_tabs_seen = False
			else:
				break
		if ((current_indentation_level < (self._indentation_level - 1)) or
		    (current_indentation_level > (self._indentation_level + 1))):
			self._add_style_warning('Discontinuous indentation')
		self._indentation_level = current_indentation_level
	
	def _validate_invalid_sequences(self, line):
		if '\r' in line:
			self._add_style_warning('Carriage return found')
		if '){' in line:
			self._add_style_warning('Inconsistent brace style')
		if (len(line) > 1) and (line.strip('\n')[-1] == ' '):
			self._add_style_warning('Trailing space')
		if line[-1] != '\n':
			self._add_style_warning('No trailing newline')
	
	def get_warnings(self):
		return self._style_warnings

if __name__ == '__main__':
	cpp_files = glob.glob('**/*.cpp', recursive = True)
	h_files = glob.glob('**/*.h', recursive = True)
	all_files = (cpp_files + h_files)
	total_warnings = 0

	print('Validating style in ' + str(len(all_files)) + ' files...')
	for source_file in all_files:
		warnings = StyleValidator(source_file).get_warnings()
		if len(warnings) > 0:
			print()
			print('\n'.join(warnings))
			total_warnings += len(warnings)
	sys.exit(total_warnings)
