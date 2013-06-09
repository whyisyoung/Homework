# coding: utf-8
""" 
	tips:
	If you want to know more about this program,
	please read README.md.

	You can also see my code in 
	https://github.com/linus-young/ylm/blob/master/windows_homework/4
"""



import wx
import MyMenu
import os
from pytesser import *
import wx.lib.imagebrowser as imagebrowser
import Image


class MyFrame( wx.Frame ):
	def __init__( self ):
		wx.Frame.__init__( self, None, -1, "lab4", size=(800,600))

		icon = wx.Icon( name="icon1.ico", type=wx.BITMAP_TYPE_ICO )
		self.SetIcon( icon );

		self.menuBar = MyMenu.initMenu()
		self.SetMenuBar( self.menuBar )
		self.CreateStatusBar()
		self.menuBar.Enable( 102, False ) # make saveToFile menu disabled at first

		self.Bind( wx.EVT_MENU, self.OnOpenFile, id=101 )
		self.Bind( wx.EVT_MENU, self.OnSaveFile, id=102 )
		self.Bind( wx.EVT_MENU, self.OnClose, id=wx.ID_EXIT )
		self.Bind( wx.EVT_MENU_RANGE, self.OnOcrPicture, id=201, id2=202 )
		self.Bind( wx.EVT_MENU, self.OnPicEnhance, id=301 )
		self.Bind( wx.EVT_MENU, self.OnAbout, id=401 )
		self.Bind( wx.EVT_PAINT, self.OnPaint )

		self.image = None
		self.imageOfBitmap = None
		self.imageName = None
		self.text = None
		self.bPicShow = False
		self.showImageName = wx.StaticText( self, -1, u"图像文件：None", pos=(10,10) )
		self.enhanceStatus = wx.StaticText( self, -1, "", pos=(40,30))
		self.ocrStatus = wx.StaticText( self, -1, "", pos=(40,30) )
		# add special font style
		font = wx.Font( 23, wx.ROMAN, wx.NORMAL, wx.BOLD )
		self.ocrStatus.SetFont( font )
		self.enhanceStatus.SetFont( font )

	def OnPaint( self, evt ):
		dc = wx.PaintDC( self )
		if self.bPicShow == True:
			dc.DrawBitmap( self.imageOfBitmap, 10, 80 )


	def OnOpenFile( self, evt ):
		# open an image file to identify
		openImageDialog = imagebrowser.ImageDialog( None )
		if openImageDialog.ShowModal() == wx.ID_OK:
			self.imageName = openImageDialog.GetFile()
			self.showImageName.SetLabel( u"图像文件：".encode('gbk')+self.imageName )
			#self.showImageName.SetLabel( u"图像文件："+self.imageName ) #use this in linux
			# BITTYPE_TYPE_ANY will auto detect the picture's type
			self.image = Image.open( self.imageName )
			self.imageOfBitmap = wx.Image( name=self.imageName, type=wx.BITMAP_TYPE_ANY ).ConvertToBitmap()
			self.bPicShow = True
			self.Refresh()
		elif openImageDialog.ShowModal() == wx.ID_CANCEL:
			# keep last status
			print "you canceled to choose an image"
		else:
			wx.MessageBox( "ERROR",u"图像文件不正确", wx.ICON_INFORMATION, self )
			self.showImageName.SetLabel( u"图像文件：None")
			self.bPicShow = False
			self.Refresh()

		openImageDialog.Destroy()
 
	def OnSaveFile( self, evt ):
		""" You can save the outputs in a txt file"""
		wildcard = "Text Files (*.txt)|*.txt|All files (*.*)|*.*"
		# remember to use wx.OVERWRITE_PROMPT style when write to an exist file
		saveDlg = wx.FileDialog( None, "Save textfile as...", os.getcwd(), "", \
								wildcard, wx.SAVE | wx.OVERWRITE_PROMPT )
		if saveDlg.ShowModal() == wx.ID_OK:
			fullFileName = saveDlg.GetPath()
			fileName = saveDlg.GetFilename()
			savedFile = open( fullFileName, "w")

			savedFile.write( self.text.encode('gbk') )
			savedFile.close()
			self.menuBar.Enable( 102, False )
		else: # when click the cancel button, it should keep save menu still enable
			self.menuBar.Enable( 102, True )
		saveDlg.Destroy()
		


	def OnClose( self, evt ):
		# close this app
		self.Close()

	def OnOcrPicture( self, evt ):
		if self.image:
			langId = evt.GetId()
			self.ocrStatus.SetLabel( u"正在识别中，请等待..." )
			if langId == 201:
				self.text = unicode( image_to_string( self.image, lang="eng" ), "utf-8")
			else:
				self.text = unicode( image_to_string( self.image, lang="chi_sim"), "utf-8" )
				print(type(self.text))
			wx.MessageBox( self.text, "TEXT", wx.ICON_INFORMATION, self )
			self.ocrStatus.SetLabel("")
			self.menuBar.Enable( 102, True )
		else:
			wx.MessageBox( u"没有打开的图像文件", "ERROR", wx.OK, self )

	def OnPicEnhance( self, evt ):
		""" if the words on the picture is not clear enough, 
			you can use this method to enhance it to make it 
			more clear to identify easily """
		if self.image:
			wildcard = "PNG files (*.png)|*.png|All files (*.*)|*.*" # .tiff can not show in windows.
				   
			saveImageDlg = wx.FileDialog( None, "Save Image file as...", os.getcwd(), "", \
									 wildcard, wx.SAVE | wx.OVERWRITE_PROMPT ) 
			if saveImageDlg.ShowModal() == wx.ID_OK:
				fullImageName = saveImageDlg.GetPath()
				call_imagemagick( self.imageName, fullImageName )
				print fullImageName
				fullImageName = fullImageName
				#self.image.save( fullImageName )//this is wrong, you should not open a file and try to write it and save it.
				self.showImageName.SetLabel( fullImageName )
				self.enhanceStatus.SetLabel(  u"正在增强图像中，请等待..." )
				self.image = Image.open( fullImageName )
				self.imageOfBitmap = wx.Image( name=fullImageName, type=wx.BITMAP_TYPE_ANY ).ConvertToBitmap()
				self.Refresh()
				self.enhanceStatus.SetLabel("")
			saveImageDlg.Destroy()
		else:
			wx.MessageBox( u"尚未装入图像文件", "ERROR", wx.OK, self )


	def OnAbout( self, evt ):
		wx.MessageBox( u"第四次上机作业\n\n姓名:***\n学号:***",
					"lab4", wx.OK | wx.ICON_INFORMATION, self )



if __name__ == '__main__':
	app = wx.PySimpleApp()
	frame = MyFrame()
	frame.Show(True)
	app.MainLoop()