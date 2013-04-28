# -*- coding: cp936 -*-
# lab3
# 对话框 光标 字符串 位图

import wx
import myMenu
import os

class MyFrame( wx.Frame ):
	def __init__(self):
		wx.Frame.__init__( self, None, -1, u"第三次上机练习", size = (800,650))

		icon = wx.Icon( name = "icon1.ico", type = wx.BITMAP_TYPE_ICO)
		self.SetIcon( icon );

		self.menuBar = myMenu.initChineseMenu()
		self.SetMenuBar( self.menuBar )
		self.CreateStatusBar()

		self.Bind( wx.EVT_MENU, self.OnClose, id=wx.ID_EXIT )
		self.Bind( wx.EVT_MENU, self.OnModeDialog, id=201 )
		self.Bind( wx.EVT_MENU, self.OnModelessDialog, id=202)
		self.Bind( wx.EVT_MENU, self.OnFileDialog, id=203)
		self.Bind( wx.EVT_MENU_RANGE, self.OnChangeCursor, id=301, id2=303 )
		self.Bind( wx.EVT_MENU_RANGE, self.OnSetLanguage, id=401, id2=402 )
		self.Bind( wx.EVT_MENU, self.OnHelp, id=501 )
		self.Bind( wx.EVT_PAINT, self.OnPaint )

		self.bCheckBox      = [ False, False, False ]  #设置模式对话框位图unchecked
		self.bRadioButton   = [ True, False, False ]   #设置无模式对话框位图选择情况
		self.bLanguage      = True 					   # True means Zh, False means En
		self.cursorSelector = 0						   # 0 -> ARROW, 1 -> CROSS, 2 -> USER_DEFINED
		#create three bitmaps to show
		image = [ wx.Image( name="bmp1.bmp", type=wx.BITMAP_TYPE_JPEG ),
				  wx.Image( name="bmp2.bmp", type=wx.BITMAP_TYPE_JPEG ),
				  wx.Image( name="bmp3.bmp", type=wx.BITMAP_TYPE_JPEG )]
		self.bmp = [ image[0].ConvertToBitmap(), 
					 image[1].ConvertToBitmap(), 
					 image[2].ConvertToBitmap() ]


		self.cursorZhStr = [ wx.StaticText(self, -1, u"当前使用的光标是：ARROW", pos=(50,30)),
					   	     wx.StaticText(self, -1, u"当前使用的光标是：CROSS", pos=(50,30)),
					         wx.StaticText(self, -1, u"当前使用的光标是：我画的光标", pos=(50,30)) ]
		self.cursorEnStr = [ wx.StaticText(self, -1, "The current cursor is: ARROW", pos=(50,30)),
						     wx.StaticText(self, -1, "The current cursor is: CROSS", pos=(50,30)),
						     wx.StaticText(self, -1, "The current cursor is: USER_DEFINED", pos=(50,30)) ]
		for eachStr in self.cursorZhStr:
			eachStr.Hide()
		for eachStr in self.cursorEnStr:
			eachStr.Hide()

	def OnModeDialog( self, evt ):
		mDialog = ModeDialog( self )
		val = mDialog.ShowModal()
		if val == wx.ID_OK :
			mDialog.GetParent().Refresh()
		elif val == wx.ID_CANCEL :
			mDialog.Destroy()


	def OnModelessDialog( self, evt ):
		mLessDlg = ModelessDialog( self )
		mLessDlg.Show() # use Show() when comes to modalLessDialog, it's a bool function

	def OnFileDialog( self, evt ):
		wildcard = "All fils(*.*)|*.*|" \
					"Python source(*.py)|.*py|" \
					"C++ source(*.cpp)|*.cpp"
		fileDlg = wx.FileDialog( None, "Choose a file", os.getcwd(), "", wildcard, wx.OPEN )
		if fileDlg.ShowModal() == wx.ID_OK:
			fileName = fileDlg.GetPath()
			wx.MessageBox( fileName ,u"选择的文件名",wx.OK, self )

	def OnChangeCursor( self, evt):
		cursorSet = [ wx.StockCursor( wx.CURSOR_ARROW ),
					  wx.StockCursor( wx.CURSOR_CROSS ), 
					  wx.Cursor( "myCursor.cur", type = wx.BITMAP_TYPE_CUR ) ] 

		self.cursorSelector = evt.GetId() - 301
		changeCursor = cursorSet[ self.cursorSelector ]
		self.SetCursor( changeCursor )
		self.Refresh()

	def OnSetLanguage( self, evt ):
		langId = evt.GetId()
		if langId == 401:
			self.SetLabel( u"第三次上机练习" )
			self.menuBar = myMenu.initChineseMenu()
			self.SetMenuBar( self.menuBar )
			self.menuBar.Check( 301+self.cursorSelector, True )
			self.bLanguage = True
		else:
			self.SetLabel( "lab3" )
			self.menuBar = myMenu.initEnglishMenu()
			self.SetMenuBar( self.menuBar )
			self.menuBar.Check( 301+self.cursorSelector, True )
			self.bLanguage = False
		self.Refresh()

	def OnHelp( self, evt ):
		wx.MessageBox( u"第三次上机练习\n\nname:***\nnumber:***",
			"lab3", wx.OK | wx.ICON_INFORMATION, self )

	def OnPaint( self, evt ):
		dc = wx.PaintDC( self )
		cursorIdSet = [ 301, 302, 303 ]
		for eachCursor in cursorIdSet:
			# to get the selected cursor
			showStrId = self.menuBar.FindItemById( eachCursor )
			if showStrId.IsChecked():
				if self.bLanguage == True:
					self.cursorZhStr[ eachCursor-301 ].Show()
					self.cursorEnStr[ eachCursor-301 ].Hide() # this is needed when change EnMenu to ZhMenu
				else:
					self.cursorEnStr[ eachCursor-301 ].Show()
			else:
				self.cursorZhStr[ eachCursor-301 ].Hide()
				self.cursorEnStr[ eachCursor-301 ].Hide()

		for i in range(3):
			if self.bCheckBox[i] == True:
				dc.DrawBitmap( self.bmp[i], 50+i*200, 70+i*150 )
			if self.bRadioButton[i] == True:
				dc.DrawBitmap( self.bmp[i], 550, 100 )
 		


	def OnClose( self, evt ):
		self.Close()


class ModeDialog( wx.Dialog ):
	def __init__( self, parent ):
		wx.Dialog.__init__( self, parent, -1, u"复选显示位图选择",size=(300,250),pos=(350,400))
		bmpCheck =[ wx.CheckBox( self, 1001, u"复选位图1", pos=(50,30) ),
					wx.CheckBox( self, 1002, u"复选位图2", pos=(50,80) ),
					wx.CheckBox( self, 1003, u"复选位图3", pos=(50,130) ) ]
		okButton  = wx.Button(self, wx.ID_OK, u"确定", pos=(150,60))
		cancelButton  = wx.Button(self, wx.ID_CANCEL, u"取消", pos=(150,110))
		#设置单选的状态
		self.bmpId = { 1001, 1002, 1003 }
		for eachBmp in self.bmpId:
			if self.GetParent().bCheckBox[ eachBmp-1001 ] == True:
				bmpCheck[ eachBmp-1001 ].SetValue( True )
			else:
				bmpCheck[ eachBmp-1001 ].SetValue( False )
			self.Bind( wx.EVT_CHECKBOX, self.OnModeShowBmp, id=eachBmp )
		#也许下一行也有效
		#self.Bind( wx.EVT_CHECKLISTBOX, self.onShowBmp, id=1001, id2=1003 ) 


	def OnModeShowBmp( self, evt ):
		checkId = evt.GetId()
		if self.GetParent().bCheckBox[ checkId-1001 ] == True:
			self.GetParent().bCheckBox[ checkId-1001 ] = False
		else:
			self.GetParent().bCheckBox[ checkId-1001 ] = True



class ModelessDialog( wx.Dialog ):
	def __init__( self, parent ):
		wx.Dialog.__init__( self, parent, -1, u"单选显示位图选择",size=(300,250), pos=(700,400))
		bmpRadio = [ wx.RadioButton( self, 2001, u"单选位图1", pos=(80,50) ),
					 wx.RadioButton( self, 2002, u"单选位图2", pos=(80,100) ),
					 wx.RadioButton( self, 2003, u"单选位图3", pos=(80,150) ) ]
		okButton  = wx.Button( self, wx.ID_OK, u"确定", pos=(180,60) )
		cancelButton  = wx.Button( self, wx.ID_CANCEL, u"取消", pos=(180,110) )
		self.bmpLessId = { 2001, 2002, 2003 }
		for eachBmp in self.bmpLessId:
			if self.GetParent().bRadioButton[ eachBmp-2001 ] ==True:
				bmpRadio[ eachBmp-2001 ].SetValue( True )
			else:
				bmpRadio[ eachBmp-2001 ].SetValue( False )
			self.Bind( wx.EVT_RADIOBUTTON, self.OnLessShowBmp, id=eachBmp )
		self.Bind( wx.EVT_BUTTON, self.OnOK, okButton )
		self.Bind( wx.EVT_BUTTON, self.OnCancel, cancelButton )

	def OnLessShowBmp( self, evt ):
		radioId = evt.GetId()
		if self.GetParent().bRadioButton[ radioId - 2001 ] == True:
			self.GetParent().bRadioButton[ radioId - 2001 ] = False
		else:
			self.GetParent().bRadioButton[ radioId - 2001 ] = True
		for eachBmp in self.bmpLessId:
			if not eachBmp == radioId:
				self.GetParent().bRadioButton[ eachBmp-2001 ] = False

	def OnOK( self, evt ):
		self.GetParent().Refresh()

	def OnCancel( self, evt ):
		self.Destroy()

if __name__ == '__main__':
	app = wx.PySimpleApp()
	frame = MyFrame()
	frame.Show(True)
	app.MainLoop()
