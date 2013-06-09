import wx

class SelectArea( wx.Dialog ):
	def __init__( self, parent ):
		wx.Dialog.__init__( self, parent, -1, style=wx.STAY_ON_TOP )
		self.bSnipPart = False

		self.Bind( wx.EVT_LEFT_DOWN, self.StartSnip )
		self.Bind( wx.EVT_LEFT_UP, self.EndSnip )
		self.Bind( wx.EVT_MOTION, self.OnMouseMove )
		self.Bind( wx.EVT_PAINT, self.OnPaint )

		self.SetTransparent( 128 )
		self.SetBackgroundColour( 'transparent' )
		self.SetCursor( wx.StockCursor(wx.CURSOR_CROSS) )
		self.ShowFullScreen( True )


	def StartSnip( self, evt ):
		self.startPoint = evt.GetPosition()
		self.bSnipPart = True

	def EndSnip( self, evt ):
		self.bSnipPart = False
		self.GetParent().endPoint = evt.GetPosition()
		self.GetParent().startPoint = self.startPoint
		self.EndModal( wx.OK )

	def OnMouseMove( self, evt ):
		dc = wx.WindowDC( self )
		self.endPoint = evt.GetPosition()
		self.Refresh()

	def OnPaint( self, evt ):
		if self.bSnipPart:
			dc = wx.PaintDC( self )
			dc.SetBrush( wx.Brush('transparent'))
			x,y=min( self.startPoint.x, self.endPoint.x ), min( self.startPoint.y, self.endPoint.y )
			width = abs( self.startPoint.x - self.endPoint.x )
			height = abs( self.startPoint.y - self.endPoint.y )
			dc.DrawRectangle( x, y, width, height )

