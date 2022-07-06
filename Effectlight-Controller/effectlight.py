import socket
import sys

from enum import IntEnum, auto

from PyQt5.QtCore import pyqtSlot
from PyQt5.QtGui import QCloseEvent, QColor
from PyQt5.QtWidgets import QApplication, QColorDialog, QFrame, QMainWindow, QListWidget
from PyQt5.uic import loadUi


HOST = "effectlight.local"
PORT = 8888

UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)


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
        self.program_list.addItem("blink test")
        self.program_list.addItem("fade test")
        self.program_list.addItem("police")

        # UDPServerSocket.sendto(bytearray([MessageHeader.StopProgram]), (HOST, PORT))

    def closeEvent(self, event: QCloseEvent) -> None:
        self.color_dialog.close()
        return super().closeEvent(event)

    def showColorPicker(self):
        self.color_dialog.show()

    def setPanel(self):
        start = [0, 0]
        count = [0, 0]
        rgb = split_qcolor(self.color_dialog.currentColor())
        message = [MessageHeader.Fill] + start + count + rgb
        UDPServerSocket.sendto(bytearray(message), (HOST, PORT))
        UDPServerSocket.sendto(bytearray([MessageHeader.Show]), (HOST, PORT))

    def clearPanel(self):
        start = [0, 0]
        count = [0, 0]
        rgb = [0, 0, 0]
        message = [MessageHeader.Fill] + start + count + rgb
        UDPServerSocket.sendto(bytearray(message), (HOST, PORT))
        UDPServerSocket.sendto(bytearray([MessageHeader.Show]), (HOST, PORT))

    def stopListPrograms(self):
        UDPServerSocket.sendto(bytearray([MessageHeader.StopProgram]), (HOST, PORT))

    def runProgram(self):
        indexes = self.program_list.selectedIndexes()

        if len(indexes) == 0:
            return

        index = indexes[0].row()
        UDPServerSocket.sendto(bytearray([MessageHeader.RunProgram, index]), (HOST, PORT))

    @ pyqtSlot()
    def colorSelected(self):
        color = self.color_dialog.currentColor()
        self.color_indicator.setStyleSheet(f"background-color: rgb({color.red()},{color.green()},{color.blue()}); border: 1px solid black;")


app = QApplication(sys.argv)
app.setStyle("Fusion")
win = MainWindow()
win.show()
app.exec()
