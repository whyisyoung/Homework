"""OCR in Python using the Tesseract engine from Google
http://code.google.com/p/pytesser/
by Michael J.T. O'Kelly
V 0.0.1, 3/10/07"""
#coding:utf-8

import Image
import subprocess

import util
import errors


tesseract_exe_name = 'tesseract' # Name of executable to be called at command line
scratch_image_name = "temp.bmp" # This file must be .bmp or other Tesseract-compatible format
scratch_text_name_root = "temp" # Leave out the .txt extension
cleanup_scratch_flag = True  # Temporary files cleaned up after OCR operation
convert_exe_name = 'C:\Program Files\ImageMagick-6.8.5-Q16\convert'
#convert_exe_name = '/usr/bin/convert' # use this in linux


def call_imagemagick( inputFilename, outFilename ):
	"""Calls external convert.exe on input file(actually, pdf is also ok as ImageMagick is so powerful),
	outfile will be .tiff file to be easily identified."""
	args = [ convert_exe_name, "-density", "300", "-colorspace", "GRAY", inputFilename, outFilename ]
	proc = subprocess.Popen( args )
	retcode = proc.wait()



def call_tesseract(input_filename, output_filename, lang ):
	"""Calls external tesseract.exe on input file (restrictions on types),
	outputting output_filename+'txt'"""
	# args = [tesseract_exe_name, input_filename, output_filename ]
	args = [tesseract_exe_name, input_filename, output_filename, "-l", lang ]
	proc = subprocess.Popen(args)
	retcode = proc.wait()
	if retcode!=0:
		errors.check_for_errors()


def image_to_string(im, cleanup = cleanup_scratch_flag, lang="eng" ):
	"""Converts im to file, applies tesseract, and fetches resulting text.
	If cleanup=True, delete scratch files after operation."""
	try:
		util.image_to_scratch(im, scratch_image_name)
		call_tesseract(scratch_image_name, scratch_text_name_root, lang )
		text = util.retrieve_text(scratch_text_name_root)
	finally:
		if cleanup:
			util.perform_cleanup(scratch_image_name, scratch_text_name_root)
	return text

def image_file_to_string(filename, cleanup = cleanup_scratch_flag, graceful_errors=True, lang="eng" ):
	"""Applies tesseract to filename; or, if image is incompatible and graceful_errors=True,
	converts to compatible format and then applies tesseract.  Fetches resulting text.
	If cleanup=True, delete scratch files after operation."""
	try:
		try:
			call_tesseract( filename, scratch_text_name_root, lang )
			text = util.retrieve_text(scratch_text_name_root)
		except errors.Tesser_General_Exception:
			if graceful_errors:
				im = Image.open(filename)
				text = image_to_string( im, cleanup, lang )
			else:
				raise
	finally:
		if cleanup:
			util.perform_cleanup(scratch_image_name, scratch_text_name_root)
	return text
	

if __name__=='__main__':
	im = Image.open('phototest.tif')
	text = image_to_string(im)
	print text
	try:
		text = image_file_to_string('fnord.tif', graceful_errors=False)
	except errors.Tesser_General_Exception, value:
		print "fnord.tif is incompatible filetype.  Try graceful_errors=True"
		print value
	text = image_file_to_string('fnord.tif', graceful_errors=True, lang="eng")
	print "fnord.tif contents:", text
	text = image_file_to_string('fonts_test.png', graceful_errors=True, lang="eng" )
	print text


