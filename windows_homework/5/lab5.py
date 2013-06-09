# coding: utf-8
"""
	This application is just like snipppingTool in Windows,
	function:
		1.save a selected area to an image
		   (include this program window or not )
		2.resize an image
		3.snip the whole screen 
"""

import wx
import os,sys,platform
import Image
import wx.lib.imagebrowser as imagebrowser
import shutil

import util
import constant as const
import select_area_dialog

if os.name == 'nt': # for Windows, actually, you can also use ImageMagick
	import ImageGrab
else: #Assume Linux/GTK
	from config import launchCommand

class SnippingTool( wx.Frame ):
	def __init__( self ):
		wx.Frame.__init__( self, None, -1, "ScreenCapture", size=(600,400) )

		self.menuBar = util.BuildMenuBar()
		self.SetMenuBar( self.menuBar )
		self.CreateStatusBar()
		self.menuBar.Check( const.ID_HIDE_CURRENT, False )
		self.menuBar.Enable( const.ID_FILE_SAVE, False )

		self.Bind( wx.EVT_MENU, self.OpenImgFile, id =const.ID_FILE_OPEN )
		self.Bind( wx.EVT_MENU, self.SaveCapture, id =const.ID_FILE_SAVE )
		self.Bind( wx.EVT_MENU, self.CloseApp,    id =const.ID_FILE_EXIT )
		self.Bind( wx.EVT_MENU, self.SnipPart,    id =const.ID_SNIP_PART )
		self.Bind( wx.EVT_MENU, self.SnipAll,     id =const.ID_SNIP_ALL )
		self.Bind( wx.EVT_MENU, self.ImgToWindow, id =const.ID_IMAGE_BIG )
		self.Bind( wx.EVT_MENU, self.ResizeImg,   id =const.ID_IMAGE_RESIZE )
		self.Bind( wx.EVT_MENU, self.HideOption,  id =const.ID_HIDE_CURRENT )
		self.Bind( wx.EVT_MENU, self.AboutApp,    id =const.ID_APP_ABOUT )

		self.Bind( wx.EVT_MOTION, self.OnMouseMove )
		self.Bind( wx.EVT_LEFT_DOWN, self.OnMouseLeftDown )
		self.Bind( wx.EVT_LEFT_UP, self.OnMouseLeftUp )
		self.Bind( wx.EVT_PAINT, self.OnPaint )

		
		self.imageName = None
		self.image = None # PIL Image, it has size attribute
		self.imageOfBitmap = None # wx.Bitmap image, it can be painted by DrawBitmap function
		self.startPoint = self.endPoint = None
		self.bResize = False # whether select ResizeImage menu or not
		self.resizeEnd = None 
		self.bHideCurrent = False
		self.isSaved = None


	def OpenImgFile( self, evt ):
		openImageDialog = imagebrowser.ImageDialog( None )
		if openImageDialog.ShowModal() == wx.ID_OK:
			self.imageName = openImageDialog.GetFile()
			self.image = Image.open( self.imageName )
			self.imageOfBitmap = wx.Image( name=self.imageName, type=wx.BITMAP_TYPE_ANY ).ConvertToBitmap()
			self.SetClientSizeWH( self.image.size[0]+200, self.image.size[1]+100 )
			self.Refresh()
		elif openImageDialog.ShowModal() == wx.ID_CANCEL:
			print "you canceled to choose an image"
		else:
			wx.MessageBox( "ERROR", u"图像文件不正确", wx.ICON_EXCLAMATION, self )
			self.Refresh()
		openImageDialog.Destroy()


	def SaveCapture( self, evt ):
		self.bResize = False
		wildcard= "BMP Files (*.bmp)|*.bmp|All Files(*.*)|*.*"
		saveDlg = wx.FileDialog( None, "Save Image as...", os.getcwd(), "", \
								wildcard, wx.SAVE | wx.OVERWRITE_PROMPT )
		if saveDlg.ShowModal() == wx.ID_OK:
			fullFileName = saveDlg.GetPath()
			fileName = saveDlg.GetFilename()
			area = wx.ClientDC( self )
			size = self.image.size
			newImage = wx.EmptyBitmap( size[0], size[1] )
			mem = wx.MemoryDC( newImage )
			mem.Blit( 0, 0, size[0], size[1], area, 0, 0 )
			del mem
			newImage.SaveFile( name=fileName, type=wx.BITMAP_TYPE_BMP )# do not use ANY here
			self.menuBar.Enable( const.ID_FILE_SAVE, False )
			self.isSaved = True
		else:
			self.menuBar.Enable( const.ID_FILE_SAVE, True )
		saveDlg.Destroy()


	def CloseApp( self, evt ):
		if self.isSaved == False:
			dlg = wx.MessageDialog( self, u"结果尚未保存，确定要关闭？", "warning",
						 wx.YES_NO | wx.ICON_QUESTION  )
			if dlg.ShowModal() == wx.ID_YES:
				self.Close()
		else:
			self.Close()


	def SnipPart( self, evt ):
		"""
		todo:
			create a temp file to hold screencapture
		"""
		self.bResize = False
		if self.bHideCurrent:
			self.Iconize( True )# minimize current window
		else:
			self.Iconize( False ) # keep current window show
		dlg = select_area_dialog.SelectArea( self )
		result = dlg.ShowModal()
		dlg.Destroy()
		print (self.startPoint,self.endPoint)
		if result == wx.OK:
			rect = wx.RectPP( self.startPoint, self.endPoint )
			scrDC = wx.ScreenDC( )
			if rect:
				bmp = wx.EmptyBitmap( rect.size[0], rect.size[1] )
				mem = wx.MemoryDC( bmp )
				mem.Blit( 0, 0, rect.size[0], rect.size[1], scrDC, rect.GetX(), rect.GetY() )
				del mem
				bmp.SaveFile( 'bug.bmp', wx.BITMAP_TYPE_BMP )
				self.imageName = 'bug.bmp'
				self.image = Image.open('bug.bmp')
				self.imageOfBitmap = wx.Image( name="bug.bmp", type=wx.BITMAP_TYPE_ANY ).ConvertToBitmap()
				self.SetClientSizeWH( rect.size[0]+200, rect.size[1]+100)
				self.Refresh()
				self.Iconize( False ) # restore app window
				self.isSaved = False
		else:
			self.Iconize( False )

	def SnipAll( self, evt ):
		"""
		todo:
			create a temp file to hold screencapture
		"""
		screen = wx.ScreenDC()
		size = screen.GetSize()
		bmp = wx.EmptyBitmap(size[0], size[1])
		mem = wx.MemoryDC( bmp )
		mem.Blit(0, 0, size[0], size[1], screen, 0, 0)
		del mem  # Release bitmap
		bmp.SaveFile( 'temp.bmp', wx.BITMAP_TYPE_BMP )
		self.imageName = "temp.bmp"
		self.image = Image.open( 'temp.bmp' )
		self.imageOfBitmap = wx.Image( name="temp.bmp", type=wx.BITMAP_TYPE_ANY ).ConvertToBitmap()
		self.SetClientSize( self.image.size )
		self.Refresh()
		self.bResize = False
		self.menuBar.Enable( const.ID_FILE_SAVE, True )
		self.isSaved = False

		"""the following code is also useful"""
		#if platform.system() == 'Windows':
		#	self.image = ImageGrab.grab()
		#	self.image.save('temp.bmp')
		#	self.imageOfBitmap = wx.Image( name="temp.bmp", type=wx.BITMAP_TYPE_ANY ).ConvertToBitmap()
		#	self.SetClientSize( self.image.size )
		#	self.Refresh()
		#else:
		#	launchCommand( '/usr/bin/xwd -display  :0 -root -out %s' % 'test' )

	def ImgToWindow( self, evt ):
		windowSize = self.GetClientSize()
		width, height = windowSize[0], windowSize[1]
		if self.imageOfBitmap:
			wxImage = wx.Image( name=self.imageName, type=wx.BITMAP_TYPE_ANY )
			wxImage = wxImage.Scale( width, height, wx.IMAGE_QUALITY_HIGH )
			self.image = util.ImageToPil( wxImage )
			self.imageOfBitmap = wxImage.ConvertToBitmap()
			self.Refresh()
			self.menuBar.Enable( const.ID_FILE_SAVE, True )
			self.isSaved = False

	def ResizeImg( self, evt ):
		if self.image:
			self.bResize = True
		else:
			wx.MessageBox( u"没有载入图像文件", "ERROR", wx.OK, self )
			self.bResize = False
	

	def HideOption( self, evt ):
		self.bResize = False
		self.bHideCurrent = evt.IsChecked() # decide whether to hide current window or not

	def AboutApp( self, evt ):
		self.bResize = False
		wx.MessageBox( u"第五次上机作业\n\n姓名:***\n学号:***",
					"lab5", wx.OK, self )

	def OnMouseLeftDown( self, evt ):
		if self.bResize:
			self.SetCursor( wx.StockCursor(wx.CURSOR_CROSS) )
			self.resizeEnd = evt.GetPosition()
			width, height = self.resizeEnd.x, self.resizeEnd.y
			wxImage = wx.Image( name=self.imageName, type=wx.BITMAP_TYPE_ANY )
			wxImage = wxImage.Scale( width, height, wx.IMAGE_QUALITY_HIGH )
			self.image = util.ImageToPil( wxImage )
			self.imageOfBitmap = wxImage.ConvertToBitmap()
			self.Refresh()
			self.menuBar.Enable( const.ID_FILE_SAVE, True )
			self.isSaved = False


	def OnMouseLeftUp( self, evt ):
		if self.bResize:
			self.endPoint = evt.GetPosition()
			self.SetCursor( wx.StockCursor(wx.CURSOR_ARROW) )
			self.Refresh()
			if self.HasCapture():
				self.ReleaseMouse()

	def OnMouseMove( self, evt ):
		if evt.LeftIsDown() and self.bResize:
			self.resizeEnd = evt.GetPosition()
			width, height = self.resizeEnd.x, self.resizeEnd.y
			wxImage = wx.Image( name=self.imageName, type=wx.BITMAP_TYPE_ANY )
			"""change Image size while mouse moving, use wx.Image method Scale"""
			wxImage = wxImage.Scale( width, height, wx.IMAGE_QUALITY_HIGH )
			self.image = util.ImageToPil( wxImage )
			self.imageOfBitmap = wxImage.ConvertToBitmap()
			self.Refresh()
			self.menuBar.Enable( const.ID_FILE_SAVE, True )


	def OnPaint( self, evt ):
		dc = wx.PaintDC( self )
		if self.imageOfBitmap:
			dc.DrawBitmap( self.imageOfBitmap, 0, 0 )


if __name__ =='__main__':
	app = wx.PySimpleApp()
	frame = SnippingTool()
	frame.Show(True)
	app.MainLoop()
