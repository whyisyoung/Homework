# coding: utf-8

import wx
import constant as const
import Image

def BuildMenuBar():
	menuBar = wx.MenuBar()

	fileMenu = wx.Menu()
	fileMenu.Append( const.ID_FILE_OPEN, u"打开(&O)\tCtrl+O", "open an image file")
	fileMenu.Append( const.ID_FILE_SAVE, u"保存(&S)\tCtrl+S", "save the screen capture" )
	fileMenu.Append( const.ID_FILE_EXIT, u"退出(&X)\tCtrl+X", "exit the app")

	snipMenu = wx.Menu()
	snipMenu.Append( const.ID_SNIP_PART, u"截取选区(&P)\tCtrl+P", "snip a selected area" )
	snipMenu.Append( const.ID_SNIP_ALL, u"截取全屏(&A)\tCtrl+A", "snip the whole screen" )

	optionMenu = wx.Menu()
	optionMenu.Append( const.ID_IMAGE_BIG,    u"图像到窗口大小(&W)\tCtrl+W", "set image to window size")
	optionMenu.Append( const.ID_IMAGE_RESIZE, u"重设图像大小(&R)\tCtrl+R", "resize the image" )
	optionMenu.Append( const.ID_HIDE_CURRENT, u"隐藏当前窗口(&H)\tCtrl+H", "hide this window when snipping", wx.ITEM_CHECK )

	aboutMenu = wx.Menu()
	aboutMenu.Append( const.ID_APP_ABOUT, u"帮助信息(&I)\tF1","about this app" )

	menuBar.Append( fileMenu,  u"文件(&F)" )
	menuBar.Append( snipMenu,  u"截图(&J)" )
	menuBar.Append( optionMenu,  u"选项(&H)" )
	menuBar.Append( aboutMenu, u"关于(&A)" )

	return menuBar 	

def ImageToPil( myWxImage ):
	"""Convert wx.Image to PIL Image """
	myPilImage = Image.new( 'RGB', (myWxImage.GetWidth(), myWxImage.GetHeight()) )
	myPilImage.fromstring( myWxImage.GetData() )
	return myPilImage
