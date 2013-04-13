# -*- coding: cp936 -*-

# lab2 
# 图标，菜单，加速键，消息框
""" there is still a problem : 
	when checked 显示1 and 
    change icon from 1 to 3,显示1 will not show, 
    but when change icon back(from 3 to 1),
    显示1 still not show
    i will fix it later :( """

import wx
class MyFrame(wx.Frame):
	def __init__(self):
		wx.Frame.__init__( self, None, -1, "lab2", size = (800,500))
		wx.StaticText( self, -1, u"当前显示的是图标1", pos = (200,100))
		wx.StaticText( self, -1, "", pos = (500,100)) 
		# geez,this is to help to show last sentence in proper position

		# set the default icon 
		defaultIcon = wx.Icon ( name = "icon1.ico", type = wx.BITMAP_TYPE_ICO)
		self.SetIcon(defaultIcon)

		self.currentIconId = 201 # store the original icon menu id in case of wx.NO

		self.menuBar = wx.MenuBar()

		# add file menu
		fileMenu = wx.Menu()
		fileMenu.Append( wx.ID_EXIT, u"&Exit\tCtrl+Shift+Delete", "Exit this app")
		self.Bind( wx.EVT_MENU, self.OnClose, id=wx.ID_EXIT)
		self.menuBar.Append( fileMenu, "&File")

		# add icon change menu
		iconMenu = wx.Menu()
		iconMenu.Append( 201, u"图标1\tCtrl+1", "change icon to icon1", wx.ITEM_RADIO)
		iconMenu.Append( 202, u"图标2\tCtrl+2", "change icon to icon2", wx.ITEM_RADIO)
		iconMenu.Append( 203, u"图标3\tCtrl+3", "change icon to icon3", wx.ITEM_RADIO)
		self.Bind( wx.EVT_MENU_RANGE, self.OnIconChange, id = 201, id2 = 203)
		self.menuBar.Append( iconMenu, "图标(&I)")
		self.menuBar.Check( 201, True)

		# add show text menu
		showMenu = wx.Menu()
		showMenu.Append( 301, u"显示&1\tCtrl+Shift+1", "显示1", wx.ITEM_CHECK )
		showMenu.Append( 302, u"显示&2\tCtrl+Shift+2", "显示2", wx.ITEM_CHECK )
		showMenu.Append( 303, u"显示&3\tCtrl+Shift+3", "显示3", wx.ITEM_CHECK )
		showMenu.Append( 304, u"显示&4\tCtrl+Shift+4", "显示4", wx.ITEM_CHECK )
		self.Bind( wx.EVT_MENU_RANGE, self.OnShowText, id = 301, id2 = 304 )
		self.menuBar.Append( showMenu, "显示(&D)")

		# add about menu
		aboutMenu = wx.Menu()
		idAbout = aboutMenu.Append( -1, "程序信息(&I)\tF1","")
		self.Bind( wx.EVT_MENU, self.OnHelp, idAbout )
		self.menuBar.Append( aboutMenu, "关于(&A)")

		self.SetMenuBar( self.menuBar )
		self.CreateStatusBar()
		self.showString = [ wx.StaticText( self, -1, u"显示1",pos=(200,200)),
							wx.StaticText( self, -1, u"显示2",pos=(250,200)),
							wx.StaticText( self, -1, u"显示3",pos=(300,200)),
							wx.StaticText( self, -1, u"显示4",pos=(350,200)),]
		for eachShowString in self.showString:
			eachShowString.Hide()

	def OnIconChange( self, evt ):
		iconSet    = [ wx.Icon ( name = "icon1.ico", type = wx.BITMAP_TYPE_ICO),
					   wx.Icon ( name = "icon2.ICO", type = wx.BITMAP_TYPE_ICO),
					   wx.Icon ( name = "icon3.ico", type = wx.BITMAP_TYPE_ICO) ]
		iconString = [ u"当前显示的是图标1", 
					   u"当前显示的是图标2", 
					   u"当前显示的是图标3" ]
		menuBar    = self.GetMenuBar()
		showTextId = [ 301, 302, 303, 304 ]
		iconId = evt.GetId() # which returns int type
		answer = wx.MessageBox( "确定要修改吗？", "Confirmation",
			 wx.YES_NO | wx.ICON_EXCLAMATION , self )
		if answer == wx.YES:
			changeIcon = iconSet[ iconId - 201 ]
			self.SetIcon(changeIcon)
			wx.StaticText( self, -1, iconString[ iconId-201 ], pos = (200,100))
			self.currentIconId = iconId 
		else: # if answer == wx.NO, set the checked icon menu to the original one
			menuBar.Check( self.currentIconId, True )

		# add some rules for icon3 to not show 显示1,2,3,4
		if iconId == 203:
			for eachShowId in showTextId:
				self.showString[ eachShowId-301 ].Hide()
				menuBar.Enable( eachShowId, False )
		else:
			for eachShowId in showTextId:
				menuBar.Enable( eachShowId, True )
		self.Refresh()

	def OnShowText( self, evt ):
		menuBar    = self.GetMenuBar()
		showId 	   = evt.GetId()
		showItemId = menuBar.FindItemById(showId)

		# when an menuitem is checked, show it ,else hide it
		if showItemId.IsChecked() :
			self.showString[ showId-301 ].Show()
		else:
			self.showString[ showId-301 ].Hide()
		self.Refresh()

	def OnHelp( self, evt ):
		wx.MessageBox( u"第二次上机练习\n\n杨利民\n10112130261",
			 "lab2", wx.OK | wx.ICON_INFORMATION, self )

	def OnClose( self, evt ):
		self.Close()

if __name__ == '__main__':
	app = wx.PySimpleApp()
	frame = MyFrame()
	frame.Show( True )
	app.MainLoop()