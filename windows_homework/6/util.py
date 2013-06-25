# coding: utf-8

import wx
import constant as const

def BuildMenuBar():
	menuBar = wx.MenuBar()

	appMenu = wx.Menu()
	appMenu.Append( const.ID_GAME_START, u"开始游戏(&S)\tCtrl+S" )
	appMenu.Append( const.ID_GAME_RESTART, u"重新开始(&R)\tCtrl+R" )
	appMenu.Append( const.ID_APP_EXIT, u"退出(&X)\tCtrl+X" )

	levelMenu = wx.Menu()
	levelMenu.Append( const.ID_LEVER_EASY, u"简单(&E)\tCtrl+E", "", wx.ITEM_RADIO )
	levelMenu.Append( const.ID_LEVER_NORMAL, u"中等(&N)\tCtrl+N", "", wx.ITEM_RADIO )
	levelMenu.Append( const.ID_LEVER_HARD, u"偏难(&H)\tCtrl+H", "", wx.ITEM_RADIO )

	aboutMenu = wx.Menu()
	aboutMenu.Append( const.ID_APP_ABOUT, u"关于(&I)\tF1")

	menuBar.Append( appMenu, u"游戏(&G)" )
	menuBar.Append( levelMenu, u"难度(&L)" )
	menuBar.Append( aboutMenu, u"关于(&A)" )

	return menuBar
