# -*- coding: cp936 -*-
''' this is a program to show the information 
  of all running process in a window  '''
import wx
import psutil

# 注意 统一使用tab或space，否则容易报错
''' do not create some file in the same folder, eg:if i create new.py then it
never works, which made module wx failed.''' 

class MyFrame(wx.Frame):
	def __init__(self):
		wx.Frame.__init__(self, None, -1, "Process Ecnumeration",size=(1000,630))
		panel = wx.Panel(self, -1)
		#print " 进程ID：        模块名：".decode("gb2312")
		        #print "进程ID：    模块名：" in window
		count = 0
		while count < 3:
			wx.StaticText(panel, -1, u'\u8fdb\u7a0bID    \u6a21\u5757\u540d', pos=(20+count*350, 10))
			count = count + 1
		xPos = 20
		yPos = 30
		for pid in sorted(psutil.get_pid_list()):
			p = psutil.Process(pid)		
			wx.StaticText(panel, -1, "%05d      %s"%(pid,p.name), pos=(xPos, yPos))
			if xPos < 600: # 每行3个进程信息
				xPos += 300
			else:
				xPos = 20
				yPos += 20
				
if __name__ == '__main__':
	app = wx.PySimpleApp()
	frame = MyFrame()
	frame.Show(True)
	app.MainLoop()
