# coding: utf-8
"""
	This application is just like snipppingTool in Windows,
	function:
		1.save a selected area( using rectangle ) to an image
		   (include this program window or not )
		2.resize an image
		3.snip the whole screen (include this program window or not )

	rework:
		todo:
			write a shared function for SnipPart and SnipAll function,
"""

import wx
import os,sys,platform
import wx.lib.imagebrowser as imagebrowser
import time

import util
import constant as const
import select_area_dialog

"""
	the following code maybe a solution, but it's not 
	so good for cross-platform
"""
#if os.name == 'nt': # for Windows, actually, you can also use ImageMagick
#	import ImageGrab
#else: #Assume Linux/GTK
#	from config import launchCommand

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
		self.Bind( wx.EVT_CLOSE, self.CloseApp )

		self.Bind( wx.EVT_MOTION, self.OnMouseMove )
		self.Bind( wx.EVT_LEFT_DOWN, self.OnMouseLeftDown )
		self.Bind( wx.EVT_LEFT_UP, self.OnMouseLeftUp )
		self.Bind( wx.EVT_PAINT, self.OnPaint )

		self.imageOfBitmap = None # # current image showed in window
		self.startPoint = self.endPoint = None
		self.bResize = False # whether select ResizeImage menu or not
		self.resizeEnd = None 
		self.bHideCurrent = False # whether select "hide current window menu " or not
		self.isSaved = None # whether the result has saved or not
		 


	def OpenImgFile( self, evt ):
		openImageDialog = imagebrowser.ImageDialog( None )
		if openImageDialog.ShowModal() == wx.ID_OK:
			fileName = openImageDialog.GetFile()
			self.imageOfBitmap = wx.Image( name=fileName, type=wx.BITMAP_TYPE_ANY ).ConvertToBitmap()
			size = self.imageOfBitmap.GetSize()
			self.SetClientSizeWH( size[0]+200, size[1]+100 )
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
			fileName = saveDlg.GetFilename()
			self.imageOfBitmap.SaveFile( name=fileName, type=wx.BITMAP_TYPE_BMP )# do not use ANY here
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
				self.Destroy()
			else:
				wildcard= "BMP Files (*.bmp)|*.bmp|All Files(*.*)|*.*"
				saveDlg = wx.FileDialog( None, "Save Image as...", os.getcwd(), "", \
								wildcard, wx.SAVE | wx.OVERWRITE_PROMPT )
				if saveDlg.ShowModal() == wx.ID_OK:
					fileName = saveDlg.GetFilename()
					self.imageOfBitmap.SaveFile( name=fileName, type=wx.BITMAP_TYPE_BMP )
					self.Destroy()
		
		else:
			self.Destroy()

	def SnipPart( self, evt ):
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
				self.imageOfBitmap = wx.EmptyBitmap( rect.size[0], rect.size[1] )
				mem = wx.MemoryDC( self.imageOfBitmap )
				mem.Blit( 0, 0, rect.size[0], rect.size[1], scrDC, rect.GetX(), rect.GetY() )
				del mem
				self.SetClientSizeWH( rect.size[0]+200, rect.size[1]+100)
				self.Refresh()
				self.Iconize( False ) # restore app window
				self.isSaved = False
				self.menuBar.Enable( const.ID_FILE_SAVE, True )
		else:
			self.Iconize( False )

	def SnipAll( self, evt ):
		if self.bHideCurrent:
			self.Show( False )
			time.sleep( 0.2 ) # 0.2 seconds
		else:
			self.Show( True )
		screen = wx.ScreenDC()
		size = screen.GetSize()
		self.imageOfBitmap = wx.EmptyBitmap(size[0], size[1])
		mem = wx.MemoryDC( self.imageOfBitmap )
		mem.Blit(0, 0, size[0], size[1], screen, 0, 0)
		del mem  # Release bitmap
		self.SetClientSize( size )
		self.Refresh()
		self.Show( True )
		self.bResize = False
		self.menuBar.Enable( const.ID_FILE_SAVE, True )
		self.isSaved = False

		"""
			the following code maybe also useful, but I do not know which Linux launchCommand suits, 
			at least in KUbuntu 13.04, I didn't find it. If you knows, please contact me :)
		"""
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
			wxImage = self.imageOfBitmap.ConvertToImage()
			wxImage = wxImage.Scale( width, height, wx.IMAGE_QUALITY_HIGH )
			self.imageOfBitmap = wxImage.ConvertToBitmap()
			self.Refresh()
			self.menuBar.Enable( const.ID_FILE_SAVE, True )
			self.isSaved = False

	def ResizeImg( self, evt ):
		if self.imageOfBitmap:
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
			wxImage = self.imageOfBitmap.ConvertToImage()
			wxImage = wxImage.Scale( width, height, wx.IMAGE_QUALITY_HIGH )

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
			wxImage = self.imageOfBitmap.ConvertToImage()
			"""change Image size while mouse moving, use wx.Image method Scale"""
			wxImage = wxImage.Scale( width, height, wx.IMAGE_QUALITY_HIGH )
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
