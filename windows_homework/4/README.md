## Introduction
this application is to show and save words in a picture with an 

open source OCR project:tesseract, you may also enhance the picture 

to make it easier to identify, enjoy it!

## Notice:
请不要在中文目录下运行（图像增强功能将无法使用）
    
In Ubuntu, you need make some preparations first:

    sudo apt-get install tesseract-ocr

    sudo apt-get install tesseract-ocr-chi-sim

    sudo apt-get install imagemagick

maybe you need some other dependencies which is not listed here.
then do not put your program under path with Chinese name, just 
comment line 66 and uncomment line 67 in lab4.py, and you need to change "convert_exe_name" to "usr/bin/convert" in pytesser.py, that's done.

## my little finding: 
1. you'd better not use DrawBitmap with ClientDC,
it may got u many unexpected error when showing pictures.

2. when comes to save file, u need to add wx.OVERWRITE_PROMPT 
style to your dialog.

3. when you want to write Chinese character to file, u need to use 
string.encode("gbk"), also when you want to print unicode and string
,change unicode.encode('gbk') first

4. when encoding with UTF-8 without BOM, u should use u"中文" everywhere

## About ImageMagick
In this program, i only used a few function of ImageMagick.

ImageMagick can be also downloaded in Windows here:

[download from sorceforge](http://sourceforge.net/projects/imagemagick/files/6.8.5-exes/)

after installed, you can run your command shell with:

    convert -density 300 -colorspace "GRAY" -monochrome originImagefile newImageFile.tiff

    -density 300 : 改变像素为300*300
    -colorspace "GRAY" : 灰度化
    -monochrome : 二值化

then it will creat newImageFile.tiff,   .tiff is the easiest identified by Tesseract.

but remember when you want to convert jpeg to tiff, do not use "-monochrome", it will throw an error.

## Problem
I find a few problems both in windows 8 and kubuntu13.04.
when you enhance a picture saved with bmp format, it will throw an error said "unsupported BMP header type(108)", it is caused by PIL(someone said it's buggy), not my problem.

In windows 8, .tiff file can not show properly under a wxpython frame, but you can open it with windows photo viewer.

In kubuntu13.04, the file filter doesn't work well but it has no
bad effect in the end. even if you saved it to a file without extension, it worked wel.


## Author
whyisyoung