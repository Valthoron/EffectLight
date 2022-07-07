import socket
import sys

from enum import IntEnum, auto
from threading import Thread
from time import sleep

from PyQt5.QtCore import pyqtSlot
from PyQt5.QtGui import QCloseEvent, QColor
from PyQt5.QtWidgets import QApplication, QColorDialog, QFrame, QListWidget, QMainWindow
from PyQt5.uic import loadUi

HOST = "effectlight.local"
PORT = 8888


class MessageHeader(IntEnum):
    Clear = 0
    Show = auto()
    Set = auto()
    Fill = auto()
    ListPrograms = 100
    ProgramEntry = auto()
    RunProgram = auto()
    StopProgram = auto()


def split_qcolor(color: QColor) -> list[int]:
    return [color.red(), color.green(), color.blue()]


class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)

        loadUi("mainWindow.ui", self)
        self.setFixedSize(self.size())

        self.color_dialog: QColorDialog
        self.color_dialog = QColorDialog()
        self.color_dialog.currentColorChanged.connect(self.colorSelected)
        self.color_dialog.show()

        self.color_indicator: QFrame
        self.color_indicator = self.findChild(QFrame, "colorIndicator")

        self.program_list: QListWidget
        self.program_list = self.findChild(QListWidget, "programList")

        self.socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.socket.settimeout(0.1)

        self.receive_thread_alive = True
        self.receive_thread = Thread(target=self.receiveLoop)
        self.receive_thread.start()

    def closeEvent(self, event: QCloseEvent) -> None:
        self.color_dialog.close()
        self.receive_thread_alive = False
        self.receive_thread.join()
        return super().closeEvent(event)

    def showColorPicker(self):
        self.color_dialog.show()

    def setPanel(self):
        start = [0, 0]
        count = [0, 0]
        rgb = split_qcolor(self.color_dialog.currentColor())
        message = [MessageHeader.Fill] + start + count + rgb
        self.socket.sendto(bytearray(message), (HOST, PORT))
        self.socket.sendto(bytearray([MessageHeader.Show]), (HOST, PORT))

    def clearPanel(self):
        self.socket.sendto(bytearray([MessageHeader.Clear]), (HOST, PORT))
        self.socket.sendto(bytearray([MessageHeader.Show]), (HOST, PORT))

    def stopProgram(self):
        self.socket.sendto(bytearray([MessageHeader.StopProgram]), (HOST, PORT))

    def runProgram(self):
        indexes = self.program_list.selectedIndexes()

        if len(indexes) == 0:
            return

        index = indexes[0].row()
        self.socket.sendto(bytearray([MessageHeader.RunProgram, index]), (HOST, PORT))

    def listPrograms(self):
        self.program_list.clear()
        self.socket.sendto(bytearray([MessageHeader.ListPrograms]), (HOST, PORT))

    @ pyqtSlot()
    def colorSelected(self):
        color = self.color_dialog.currentColor()
        self.color_indicator.setStyleSheet(f"background-color: rgb({color.red()},{color.green()},{color.blue()}); border: 1px solid black;")

    def receiveLoop(self):
        while self.receive_thread_alive:
            try:
                data, addr = self.socket.recvfrom(1024)
                header = MessageHeader(data[0])

                if header == MessageHeader.ProgramEntry:
                    self.program_list.addItem(data[1:17].decode())

            except socket.timeout:
                pass


app = QApplication(sys.argv)
app.setStyle("Fusion")
win = MainWindow()
win.show()
app.exec()
