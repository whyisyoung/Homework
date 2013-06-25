# coding: utf-8
""" 
	Tetris Game
	Designed by Jan Bodnar,
	Modified by Linus Young.

	Modified from : http://zetcode.com/wxpython/thetetrisgame/

	todo:
	start didn't work well
	restart not implement yet
"""

import wx
import random
import util
import constant as const

class Tetris(wx.Frame):
	def __init__(self, parent, id, title):
		wx.Frame.__init__(self, parent, id, title, size=(300, 380),style= wx.SYSTEM_MENU | wx.CAPTION | wx.CLOSE_BOX )
		self.menuBar = util.BuildMenuBar()
		self.SetMenuBar( self.menuBar )
		self.statusbar = self.CreateStatusBar()
		self.board = Board(self)
		self.board.SetFocus()
		self.Centre()
		self.Show(True)

		self.next = wx.StaticText( self.board, -1, "Next Square", pos=(175,20))
		self.score = wx.StaticText( self.board, -1, "Score: "+str(self.board.numLinesRemoved), pos=(185,180))
		font = wx.Font( 13, wx.ROMAN, wx.NORMAL, wx.BOLD )
		self.next.SetFont( font )
		self.score.SetFont( font )



		self.Bind( wx.EVT_MENU, self.board.start, id=const.ID_GAME_START )
		self.Bind( wx.EVT_MENU, self.board.restart, id=const.ID_GAME_RESTART )
		self.Bind( wx.EVT_MENU, self.OnExit, id=const.ID_APP_EXIT )
		self.Bind( wx.EVT_MENU_RANGE, self.board.setLevel, id=const.ID_LEVER_EASY, id2=const.ID_LEVER_HARD )
		self.Bind( wx.EVT_MENU, self.OnAbout, id=const.ID_APP_ABOUT )

	def OnExit( self, evt ):
		self.Close()

	def OnAbout( self, evt ):
		wx.MessageBox( "      Tetris Game\n\nDesigned by Jan Bodnar\nModified by Linus Young",
				"lab6", wx.OK, self )

class Board(wx.Panel):
	BoardWidth = 10
	BoardHeight = 22
	ID_TIMER = 1
	MainWidth = 164
	MainHeight = 319

	def __init__(self, parent):
		wx.Panel.__init__(self, parent,style=wx.WANTS_CHARS )

		self.timer = wx.Timer(self, Board.ID_TIMER)
		self.isWaitingAfterLine = False
		self.curPiece = Shape()
		self.nextPiece = Shape()
		self.curX = 0
		self.curY = 0
		self.numLinesRemoved = 0
		self.board = []

		self.Speed = 300
		self.pieceTemp = 1 # as a middle attribute, in order to show nextPiece properly

		self.isStarted = False
		self.isPaused = False

		self.Bind(wx.EVT_PAINT, self.OnPaint)
		self.Bind(wx.EVT_KEY_DOWN, self.OnKeyDown)
		self.Bind(wx.EVT_TIMER, self.OnTimer, id=Board.ID_TIMER)

		
		self.clearBoard()

	def shapeAt(self, x, y):
		return self.board[(y * Board.BoardWidth) + x]

	def setShapeAt(self, x, y, shape):
		self.board[(y * Board.BoardWidth) + x] = shape

	def squareWidth(self):
		return Board.MainWidth / Board.BoardWidth

	def squareHeight(self):
		return Board.MainHeight / Board.BoardHeight

	def start( self,evt ):
		if self.isPaused:
			return

		self.isStarted = True
		self.isWaitingAfterLine = False
		self.numLinesRemoved = 0
		self.clearBoard()

		self.newPiece()
		self.timer.Start(self.Speed)

	def pause(self):
		if not self.isStarted:
			return

		self.isPaused = not self.isPaused

		if self.isPaused:
			self.timer.Stop()
			self.GetParent().score.SetLabel("Paused !\n\n Score: " + str(self.numLinesRemoved) )
		else:
			self.timer.Start(self.Speed)
			self.GetParent().score.SetLabel("Score: " + str(self.numLinesRemoved) )

		self.Refresh()

	def restart( self, evt ):
		"""  
		todo: restart not implement yet
		"""
		self.isStarted = True
		self.isWaitingAfterLine = False
		self.numLinesRemoved = 0
		#self.clearBoard()
		

		self.newPiece()
		self.timer.Start(self.Speed)

	def setLevel( self, evt ):
		levelList = [ 300, 150, 100 ] # set different level
		level = evt.GetId()
		if level == const.ID_LEVER_EASY:
			self.Speed = levelList[0]
		elif level == const.ID_LEVER_NORMAL:
			self.Speed = levelList[1]
		else:
			self.Speed = levelList[2]
		self.Refresh()


	def clearBoard(self):
		for i in range(Board.BoardHeight * Board.BoardWidth):
			self.board.append(Tetrominoes.NoShape)

	def OnPaint(self, event):

		dc = wx.PaintDC(self)

		size = self.GetClientSize()
		boardTop = size.GetHeight() - Board.BoardHeight * self.squareHeight()


		for i in range(Board.BoardHeight):
			for j in range(Board.BoardWidth):
				shape = self.shapeAt(j, Board.BoardHeight - i - 1)
				if shape != Tetrominoes.NoShape:
					self.drawSquare(dc,
						0 + j * self.squareWidth(),
						boardTop + i * self.squareHeight(), shape)

		""" 
			the following block needs to be edited because start and restart
			menu doesn't work well.
		"""

		if self.curPiece.shape() != Tetrominoes.NoShape:
			for i in range(4):
				x = self.curX + self.curPiece.x(i)
				y = self.curY - self.curPiece.y(i)
				self.drawSquare(dc, 0 + x * self.squareWidth(),
					boardTop + (Board.BoardHeight - y - 1) * self.squareHeight(),
					self.curPiece.shape())

		if self.nextPiece.shape() != Tetrominoes.NoShape:
			for i in range(4):
				x = self.nextPiece.x(i)+4
				y = self.nextPiece.y(i)+4    
				self.drawSquare( dc, 150+x*self.squareWidth(), y*self.squareHeight(), self.nextPiece.shape() )

		dc.SetPen( wx.Pen("Gray", 6, wx.SOLID ))
		dc.DrawLine( Board.MainWidth, 0, Board.MainWidth, 380 )


	def OnKeyDown(self, event):
		if not self.isStarted or self.curPiece.shape() == Tetrominoes.NoShape:
			event.Skip()
			return

		keycode = event.GetKeyCode()

		if keycode == ord('P') or keycode == ord('p'):
			self.pause()
			return
		if self.isPaused:
			return
		elif keycode == wx.WXK_LEFT:
			self.tryMove(self.curPiece, self.curX - 1, self.curY)
		elif keycode == wx.WXK_RIGHT:
			self.tryMove(self.curPiece, self.curX + 1, self.curY)
		elif keycode == wx.WXK_DOWN:
			self.oneLineDown()
		elif keycode == wx.WXK_UP:
			self.tryMove(self.curPiece.rotatedLeft(), self.curX, self.curY)
		elif keycode == wx.WXK_SPACE:
			self.dropDown()
		else:
			event.Skip()


	def OnTimer(self, event):
		if event.GetId() == Board.ID_TIMER:
			if self.isWaitingAfterLine:
				self.isWaitingAfterLine = False
				self.newPiece()
			else:
				self.oneLineDown()
		else:
			event.Skip()


	def dropDown(self):
		newY = self.curY
		while newY > 0:
			if not self.tryMove(self.curPiece, self.curX, newY - 1):
				break
			newY -= 1

		self.pieceDropped()

	def oneLineDown(self):
		if not self.tryMove(self.curPiece, self.curX, self.curY - 1):
			self.pieceDropped()


	def pieceDropped(self):
		for i in range(4):
			x = self.curX + self.curPiece.x(i)
			y = self.curY - self.curPiece.y(i)
			self.setShapeAt(x, y, self.curPiece.shape())

		self.removeFullLines()

		if not self.isWaitingAfterLine:
			self.newPiece()


	def removeFullLines(self):
		numFullLines = 0

		rowsToRemove = []

		for i in range(Board.BoardHeight):
			n = 0
			for j in range(Board.BoardWidth):
				if not self.shapeAt(j, i) == Tetrominoes.NoShape:
					n = n + 1

			if n == 10:
				rowsToRemove.append(i)

		rowsToRemove.reverse()

		for m in rowsToRemove:
			for k in range(m, Board.BoardHeight):
				for l in range(Board.BoardWidth):
						self.setShapeAt(l, k, self.shapeAt(l, k + 1))

			numFullLines = numFullLines + len(rowsToRemove)

			if numFullLines > 0:
				self.numLinesRemoved = self.numLinesRemoved + numFullLines
				self.GetParent().score.SetLabel("Score: " +str(self.numLinesRemoved) )
				self.isWaitingAfterLine = True
				self.curPiece.setShape(Tetrominoes.NoShape)
				self.Refresh()


	def newPiece(self):
		self.curPiece.setShape( self.pieceTemp )
		self.nextPiece.setRandomShape()
		self.pieceTemp = self.nextPiece.shape()
		self.curX = Board.BoardWidth / 2 + 1
		self.curY = Board.BoardHeight - 1 + self.curPiece.minY()

		if not self.tryMove(self.curPiece, self.curX, self.curY):
			self.curPiece.setShape(Tetrominoes.NoShape)
			self.timer.Stop()
			self.isStarted = False
			self.GetParent().score.SetLabel("Game Over !\n\n  Score: " + str(self.numLinesRemoved) )

	def tryMove(self, newPiece, newX, newY):
		for i in range(4):
			x = newX + newPiece.x(i)
			y = newY - newPiece.y(i)
			if x < 0 or x >= Board.BoardWidth or y < 0 or y >= Board.BoardHeight:
				return False
			if self.shapeAt(x, y) != Tetrominoes.NoShape:
				return False

		self.curPiece = newPiece
		self.curX = newX
		self.curY = newY
		self.Refresh()
		return True


	def drawSquare(self, dc, x, y, shape):
		colors = ['#000000', '#CC6666', '#66CC66', '#6666CC',
				  '#CCCC66', '#CC66CC', '#66CCCC', '#DAAA00']

		light = ['#000000', '#F89FAB', '#79FC79', '#7979FC',
				 '#FCFC79', '#FC79FC', '#79FCFC', '#FCC600']

		dark = ['#000000', '#803C3B', '#3B803B', '#3B3B80',
				 '#80803B', '#803B80', '#3B8080', '#806200']

		pen = wx.Pen(light[shape])
		pen.SetCap(wx.CAP_PROJECTING)
		dc.SetPen(pen)

		dc.DrawLine(x, y + self.squareHeight() - 1, x, y)
		dc.DrawLine(x, y, x + self.squareWidth() - 1, y)

		darkpen = wx.Pen(dark[shape])
		darkpen.SetCap(wx.CAP_PROJECTING)
		dc.SetPen(darkpen)

		dc.DrawLine(x + 1, y + self.squareHeight() - 1,
			x + self.squareWidth() - 1, y + self.squareHeight() - 1)
		dc.DrawLine(x + self.squareWidth() - 1,
		y + self.squareHeight() - 1, x + self.squareWidth() - 1, y + 1)

		dc.SetPen(wx.TRANSPARENT_PEN)
		dc.SetBrush(wx.Brush(colors[shape]))
		dc.DrawRectangle(x + 1, y + 1, self.squareWidth() - 2,
		self.squareHeight() - 2)


class Tetrominoes(object):
	NoShape = 0
	ZShape = 1
	SShape = 2
	LineShape = 3
	TShape = 4
	SquareShape = 5
	LShape = 6
	MirroredLShape = 7


class Shape(object):
	coordsTable = (
		((0, 0),     (0, 0),     (0, 0),     (0, 0)),
		((0, -1),    (0, 0),     (-1, 0),    (-1, 1)),
		((0, -1),    (0, 0),     (1, 0),     (1, 1)),
		((0, -1),    (0, 0),     (0, 1),     (0, 2)),
		((-1, 0),    (0, 0),     (1, 0),     (0, 1)),
		((0, 0),     (1, 0),     (0, 1),     (1, 1)),
		((-1, -1),   (0, -1),    (0, 0),     (0, 1)),
		((1, -1),    (0, -1),    (0, 0),     (0, 1))
	)

	def __init__(self):
		self.coords = [[0,0] for i in range(4)]
		self.pieceShape = Tetrominoes.NoShape

		self.setShape(Tetrominoes.NoShape)

	def shape(self):
		return self.pieceShape

	def setShape(self, shape):
		table = Shape.coordsTable[shape]
		for i in range(4):
			for j in range(2):
				self.coords[i][j] = table[i][j]

		self.pieceShape = shape

	def setRandomShape(self):
		self.setShape(random.randint(1, 7))

	def x(self, index):
		return self.coords[index][0]

	def y(self, index):
		return self.coords[index][1]

	def setX(self, index, x):
		self.coords[index][0] = x

	def setY(self, index, y):
		self.coords[index][1] = y

	def minX(self):
		m = self.coords[0][0]
		for i in range(4):
			m = min(m, self.coords[i][0])

		return m

	def maxX(self):
		m = self.coords[0][0]
		for i in range(4):
			m = max(m, self.coords[i][0])

		return m

	def minY(self):
		m = self.coords[0][1]
		for i in range(4):
			m = min(m, self.coords[i][1])

		return m

	def maxY(self):
		m = self.coords[0][1]
		for i in range(4):
			m = max(m, self.coords[i][1])

		return m

	def rotatedLeft(self):
		if self.pieceShape == Tetrominoes.SquareShape:
			return self

		result = Shape()
		result.pieceShape = self.pieceShape
		for i in range(4):
			result.setX(i, self.y(i))
			result.setY(i, -self.x(i))

		return result


app = wx.App()
Tetris(None, -1, 'Tetris')
app.MainLoop()