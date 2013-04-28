# -*- coding: cp936 -*-
""" create ChineseMenu and EnglishMenu to Simplify the code """

import wx

def initChineseMenu():
	menuBar = wx.MenuBar()

	fileMenu = wx.Menu()
	fileMenu.Append( wx.ID_EXIT, u"退出(&x)", "Exit this application")

	dialogMenu = wx.Menu()
	dialogMenu.Append( 201, u"模式对话框(&M)\tCtrl+A", "open mode dialog")
	dialogMenu.Append( 202, u"无模式对话框(&L\tCtrl+B", "open modeless dialog")
	dialogMenu.Append( 203, u"文件对话框(&F)\tCtrl+C", "open file dialog")

	cursorMenu = wx.Menu()
	cursorMenu.Append( 301, u"光标&1(箭头)\tCtrl+1", "use arrow cursor", wx.ITEM_RADIO )
	cursorMenu.Append( 302, u"光标&2(十字)\tCtrl+2", "use cross cursor", wx.ITEM_RADIO )
	cursorMenu.Append( 303, u"光标&3(自定义)\tCtrl+3", "use user defined cursor", wx.ITEM_RADIO )

	langMenu = wx.Menu()
	langMenu.Append( 401,u"中文(&C)\tCtrl+Shift+C", "use Chinese", wx.ITEM_RADIO )
	langMenu.Append( 402,"English(&E)\tCtrl+Shift+E", "use English", wx.ITEM_RADIO )

	aboutMenu = wx.Menu()
	aboutMenu.Append( 501, u"程序信息(&I)\tF1", "")

	menuBar.Append( fileMenu, u"文件(&F)")
	menuBar.Append( dialogMenu, u"对话框(&D)")
	menuBar.Append( cursorMenu, u"光标类型(&C)")
	menuBar.Append( langMenu, u"语言(&L)")
	menuBar.Append( aboutMenu, u"关于(&A)")
	menuBar.Check( 401, True )

	return menuBar


def initEnglishMenu():
	menuBar = wx.MenuBar()

	fileMenu = wx.Menu()
	fileMenu.Append( wx.ID_EXIT, "E&xit", "Exit this application")

	dialogMenu = wx.Menu()
	dialogMenu.Append( 201, "&ModalDialog...\tCtrl+A", "open mode dialog")
	dialogMenu.Append( 202, "Modal&LessDialog...\tCtrl+B", "open modeless dialog")
	dialogMenu.Append( 203, "&File Dialog...\tCtrl+C", "open file dialog")

	cursorMenu = wx.Menu()
	cursorMenu.Append( 301, "Cursor&1(ARROW)\tCtrl+1", "use arrow cursor", wx.ITEM_RADIO )
	cursorMenu.Append( 302, "Cursor&2(CROSS)\tCtrl+2", "use cross cursor", wx.ITEM_RADIO )
	cursorMenu.Append( 303, "Cursor&3(UserDefined)\tCtrl+3", "use user defined cursor", wx.ITEM_RADIO )

	langMenu = wx.Menu()
	langMenu.Append( 401,u"中文(&C)\tCtrl+Shift+C", "use Chinese", wx.ITEM_RADIO )
	langMenu.Append( 402,"English(&E)\tCtrl+Shift+E", "use English", wx.ITEM_RADIO )

	aboutMenu = wx.Menu()
	aboutMenu.Append( 501, "Program &Information\tF1", "")

	menuBar.Append( fileMenu, "&File")
	menuBar.Append( dialogMenu, "&Dialog")
	menuBar.Append( cursorMenu, "&Cursor")
	menuBar.Append( langMenu, "&Language")
	menuBar.Append( aboutMenu, "&About")
	menuBar.Check( 402, True )

	return menuBar
