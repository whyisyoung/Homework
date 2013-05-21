#  coding: utf-8

# create menubar for lab4 main frame

import wx

def initMenu():
	menuBar = wx.MenuBar()
	
	fileMenu = wx.Menu()
	fileMenu.Append( 101, u"打开(&O)\tCtrl+O", "open a picture")
	fileMenu.Append( 102, u"保存(&S)\tCtrl+S", "save the result to a txt file")
	fileMenu.Append( wx.ID_EXIT, u"退出(&X)\tCtrl+X", "exit this app")

	ocrMenu = wx.Menu()
	ocrMenu.Append( 201, u"识别英文(&E)\tCtrl+E", "identify a picture with English words")
	ocrMenu.Append( 202, u"识别中文(&Z)\tCtrl+Z", "identify a picture with Chinese words")

	picMenu = wx.Menu()
	picMenu.Append( 301, u"图像增强(&W)\tCtrl+W", "enhance the picture")

	aboutMenu = wx.Menu()
	aboutMenu.Append( 401, u"(程序信息&A)\tF1", "About the app")

	menuBar.Append( fileMenu,  u"文件(&F)" )
	menuBar.Append( ocrMenu,    "OCR(&T)"  )
	menuBar.Append( picMenu,   u"图像(&I)" )
	menuBar.Append( aboutMenu, u"关于(&A)" ) 

	return menuBar
